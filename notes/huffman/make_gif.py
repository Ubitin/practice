# -*- coding: utf-8 -*-
"""
把 PNG 帧序列合成循环动画 GIF —— 纯标准库实现（不需要 PIL / ffmpeg）

用法:
    python make_gif.py                     # frames/step-*.png -> huffman-demo.gif
    python make_gif.py --frames frames --out huffman-demo.gif --hold-last 300

内部包含三段：
    1. PNG 解码（8 位、非隔行，支持灰度/RGB/调色板/RGBA 与 5 种行滤波）；
    2. 调色板量化（按通道分桶做频率统计，取前 255 色，其余映射到最近色）；
    3. GIF89a 编码（全局调色板 + NETSCAPE2.0 循环 + LZW，含回读自检）。

自检：编码后立刻用独立写的 LZW 解码器解回来，逐像素比对，确保码流正确。
"""
import argparse
import os
import struct
import zlib
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))


# ------------------------------------------------------------------ PNG 解码
def png_decode(data):
    """返回 (width, height, rgb_bytes)，RGB 每像素 3 字节。"""
    if data[:8] != b'\x89PNG\r\n\x1a\n':
        raise ValueError('not a PNG')
    pos, idat, pal, trns = 8, bytearray(), None, None
    width = height = depth = ctype = interlace = 0
    while pos < len(data):
        (length,) = struct.unpack('>I', data[pos:pos + 4])
        ctag = data[pos + 4:pos + 8]
        chunk = data[pos + 8:pos + 8 + length]
        pos += 12 + length
        if ctag == b'IHDR':
            width, height, depth, ctype, _comp, _filt, interlace = struct.unpack('>IIBBBBB', chunk)
        elif ctag == b'PLTE':
            pal = chunk
        elif ctag == b'tRNS':
            trns = chunk
        elif ctag == b'IDAT':
            idat += chunk
        elif ctag == b'IEND':
            break
    if depth != 8:
        raise ValueError('only 8-bit PNG supported, got %d' % depth)
    if interlace != 0:
        raise ValueError('interlaced PNG not supported')

    chan = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[ctype]
    raw = zlib.decompress(bytes(idat))
    stride = width * chan
    out = bytearray(height * stride)
    prev = bytearray(stride)
    p = 0
    for y in range(height):
        ft = raw[p]
        p += 1
        line = bytearray(raw[p:p + stride])
        p += stride
        if ft == 1:
            for i in range(chan, stride):
                line[i] = (line[i] + line[i - chan]) & 0xFF
        elif ft == 2:
            for i in range(stride):
                line[i] = (line[i] + prev[i]) & 0xFF
        elif ft == 3:
            for i in range(stride):
                a = line[i - chan] if i >= chan else 0
                line[i] = (line[i] + ((a + prev[i]) >> 1)) & 0xFF
        elif ft == 4:
            for i in range(stride):
                a = line[i - chan] if i >= chan else 0
                b = prev[i]
                c = prev[i - chan] if i >= chan else 0
                pa, pb, pc = abs(b - c), abs(a - c), abs(a + b - 2 * c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                line[i] = (line[i] + pr) & 0xFF
        elif ft != 0:
            raise ValueError('bad filter %d' % ft)
        out[y * stride:(y + 1) * stride] = line
        prev = line

    rgb = bytearray(width * height * 3)
    if ctype == 2:
        rgb[:] = out
    elif ctype == 6:
        for i in range(width * height):
            rgb[3 * i:3 * i + 3] = out[4 * i:4 * i + 3]
    elif ctype == 0:
        for i in range(width * height):
            v = out[i]
            rgb[3 * i:3 * i + 3] = bytes((v, v, v))
    elif ctype == 4:
        for i in range(width * height):
            v = out[2 * i]
            rgb[3 * i:3 * i + 3] = bytes((v, v, v))
    elif ctype == 3:
        for i in range(width * height):
            idx = out[i]
            rgb[3 * i:3 * i + 3] = pal[3 * idx:3 * idx + 3]
    return width, height, bytes(rgb)


# ------------------------------------------------------------------ 量化
def build_palette(frames, max_colors=256):
    """按 (r>>3,g>>3,b>>3) 分桶统计频率，保留最常用的 max_colors 桶。"""
    hist = Counter()
    for _w, _h, rgb in frames:
        for r, g, b in zip(rgb[0::3], rgb[1::3], rgb[2::3]):
            hist[(r >> 3, g >> 3, b >> 3)] += 1
    top = hist.most_common(max_colors)
    palette, index_of_bin = [], {}
    for k, (key, _cnt) in enumerate(top):
        palette.append((key[0] << 3 | 4, key[1] << 3 | 4, key[2] << 3 | 4))
        index_of_bin[key] = k
    # 剩下的桶映射到最近的保留色
    rest = [(key, cnt) for key, cnt in hist.items() if key not in index_of_bin]
    for key, _cnt in rest:
        c = (key[0] << 3 | 4, key[1] << 3 | 4, key[2] << 3 | 4)
        best, bd = 0, 1 << 30
        for j, p in enumerate(palette):
            d = (p[0] - c[0]) ** 2 + (p[1] - c[1]) ** 2 + (p[2] - c[2]) ** 2
            if d < bd:
                best, bd = j, d
        index_of_bin[key] = best
    while len(palette) < max_colors:
        palette.append((255, 255, 255))
    return palette, index_of_bin


def quantize(rgb, index_of_bin):
    return bytes(index_of_bin[(r >> 3, g >> 3, b >> 3)]
                 for r, g, b in zip(rgb[0::3], rgb[1::3], rgb[2::3]))


# ------------------------------------------------------------------ LZW
class BitWriter(object):
    def __init__(self):
        self.buf = bytearray()
        self.cur = 0
        self.nbits = 0

    def write(self, code, size):
        self.cur |= code << self.nbits
        self.nbits += size
        while self.nbits >= 8:
            self.buf.append(self.cur & 0xFF)
            self.cur >>= 8
            self.nbits -= 8

    def flush(self):
        if self.nbits:
            self.buf.append(self.cur & 0xFF)
            self.cur = 0
            self.nbits = 0
        return bytes(self.buf)


MAX_CODE = 4096


def lzw_encode(indices, min_code_size):
    clear, eoi = 1 << min_code_size, (1 << min_code_size) + 1
    table = {bytes((i,)): i for i in range(clear)}
    next_code, code_size = eoi + 1, min_code_size + 1
    w = BitWriter()
    w.write(clear, code_size)
    buf = b''
    for px in indices:
        c = bytes((px,))
        cand = buf + c
        if cand in table:
            buf = cand
            continue
        w.write(table[buf], code_size)
        # GIF 的经典规则：先按"当前还能放下多少码"决定是否加宽，再加新表项。
        # 顺序反了就会比规范早一步加宽，解码器读到宽度不符的码流。
        if next_code > (1 << code_size) - 1 and code_size < 12:
            code_size += 1
        if next_code < MAX_CODE:
            table[cand] = next_code
            next_code += 1
        else:
            w.write(clear, code_size)
            table = {bytes((i,)): i for i in range(clear)}
            next_code, code_size = eoi + 1, min_code_size + 1
        buf = c
    if buf:
        w.write(table[buf], code_size)
    w.write(eoi, code_size)
    return w.flush()


class BitReader(object):
    def __init__(self, data):
        self.data, self.pos, self.cur, self.nbits = data, 0, 0, 0

    def read(self, size):
        while self.nbits < size:
            if self.pos >= len(self.data):
                return None
            self.cur |= self.data[self.pos] << self.nbits
            self.pos += 1
            self.nbits += 8
        code = self.cur & ((1 << size) - 1)
        self.cur >>= size
        self.nbits -= size
        return code


def lzw_decode(data, min_code_size):
    """独立实现，用来回读自检。"""
    clear, eoi = 1 << min_code_size, (1 << min_code_size) + 1
    table = {i: bytes((i,)) for i in range(clear)}
    next_code, code_size = eoi + 1, min_code_size + 1
    r = BitReader(data)
    out, prev = bytearray(), None
    while True:
        code = r.read(code_size)
        if code is None or code == eoi:
            break
        if code == clear:
            table = {i: bytes((i,)) for i in range(clear)}
            next_code, code_size = eoi + 1, min_code_size + 1
            prev = None
            continue
        if code in table:
            entry = table[code]
        elif prev is not None and code == next_code:
            entry = prev + prev[:1]
        else:
            raise ValueError('corrupt LZW stream (code %d)' % code)
        out += entry
        if prev is not None and next_code < MAX_CODE:
            table[next_code] = prev + entry[:1]
            next_code += 1
            if next_code >= (1 << code_size) and code_size < 12:
                code_size += 1
        prev = entry
    return bytes(out)


def sub_blocks(data):
    out = bytearray()
    for i in range(0, len(data), 255):
        chunk = data[i:i + 255]
        out.append(len(chunk))
        out += chunk
    out.append(0)
    return bytes(out)


# ------------------------------------------------------------------ GIF
def write_gif(path, frames, delays, palette, loop=0):
    w, h = frames[0][0], frames[0][1]
    buf = bytearray(b'GIF89a')
    buf += struct.pack('<HH', w, h)
    buf += bytes((0xF7, 0, 0))            # 全局调色板 256 色
    for r, g, b in palette[:256]:
        buf += bytes((r, g, b))
    buf += b'\x21\xFF\x0BNETSCAPE2.0\x03\x01' + struct.pack('<H', loop) + b'\x00'
    for (fw, fh, _rgb, idx), delay in zip(frames, delays):
        buf += b'\x21\xF9\x04' + bytes((0x04,)) + struct.pack('<H', delay) + b'\x00\x00'
        buf += b'\x2C' + struct.pack('<HHHH', 0, 0, fw, fh) + b'\x00'
        buf += bytes((8,))
        buf += sub_blocks(lzw_encode(idx, 8))
    buf += b'\x3B'
    with open(path, 'wb') as f:
        f.write(bytes(buf))
    return len(buf)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--frames', default='frames')
    ap.add_argument('--out', default='huffman-demo.gif')
    ap.add_argument('--delay', type=int, default=110, help='每帧停留时间 (1/100 秒)')
    ap.add_argument('--hold-last', type=int, default=320)
    ap.add_argument('--pick', default='', help='只取指定帧序号，如 0,5（用于校验）')
    ap.add_argument('--verify', action='store_true', default=True)
    args = ap.parse_args()

    fdir = os.path.join(HERE, args.frames)
    names = sorted([f for f in os.listdir(fdir) if f.endswith('.png')],
                   key=lambda s: int(''.join(ch for ch in s if ch.isdigit())))
    if args.pick:
        want = [int(x) for x in args.pick.split(',')]
        names = [names[i] for i in want if 0 <= i < len(names)]
    if not names:
        raise SystemExit('no frames found in %s' % fdir)

    frames = []
    for n in names:
        with open(os.path.join(fdir, n), 'rb') as f:
            w, h, rgb = png_decode(f.read())
        frames.append((w, h, rgb))
        print('  loaded %-12s %dx%d' % (n, w, h))

    palette, bin_of = build_palette(frames, 256)
    print('palette: %d colors (bins: %d)' % (len(palette), len(bin_of)))

    packed = []
    for (w, h, rgb) in frames:
        idx = quantize(rgb, bin_of)
        packed.append((w, h, rgb, idx))

    delays = [args.delay] * (len(packed) - 1) + [args.hold_last]
    size = write_gif(os.path.join(HERE, args.out), packed, delays, palette)
    print('wrote %s (%d bytes, %d frames)' % (args.out, size, len(packed)))

    if args.verify:
        ok = True
        for i, (w, h, rgb, idx) in enumerate(packed):
            back = lzw_decode(lzw_encode(idx, 8), 8)
            if back != idx:
                ok = False
                print('  ✗ LZW round-trip mismatch on frame %d' % i)
        print('LZW round-trip self-check: %s' % ('✓ all frames identical' if ok else '✗ FAILED'))


if __name__ == '__main__':
    main()
