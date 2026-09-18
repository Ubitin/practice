# -*- coding: utf-8 -*-
"""
把 OI-wiki 构建站点里的某一页抽成 Markdown（便于与讲义合并）。

用法:
    python extract_oiwiki.py ds/huffman-tree --out _oiwiki-huffman.md
    python extract_oiwiki.py ds/huffman-tree --raw        # 打印正文 HTML 片段

注意：本地这份 OI-wiki 是构建产物，HTML 被压过——属性**没有引号**
（如 <img src=data:... title=n>），数学公式是 MathJax 渲染的 CHTML，
真正的 LaTeX 存在同级 <img> 的 title 属性里。所以这里用 HTMLParser
逐个标签处理，而不是写正则。
"""
import html
import io
import os
import re
import subprocess
import sys
from html.parser import HTMLParser

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                     '..', '..', 'OI-wiki'))
VOID = {'br', 'hr', 'img', 'input', 'meta', 'link'}


class Cleaner(HTMLParser):
    """保留结构、丢掉导航/锚点/行号列，并把 MathJax 的 CHTML 还原成 LaTeX。

    公式占位符（@@MATH0@@）在 pandoc 之后才替换回 LaTeX：直接塞 "$…$"
    会被 HTML reader 当成普通文本、再被转义掉。
    """

    def __init__(self):
        HTMLParser.__init__(self, convert_charrefs=True)
        self.out = []
        self.skip = []            # [(tag, depth)] 正在跳过的子树
        self.math = 0             # >0 表示在 mjx-container 里
        self.math_title = None
        self.highlight = 0        # >0 表示在代码高亮块里
        self.math_store = []      # 公式原文，正文里先放占位符，pandoc 之后再替换

    def _store(self, latex, display):
        self.math_store.append((latex, display))
        return len(self.math_store) - 1

    # -- 工具 ---------------------------------------------------------
    def emit(self, s):
        if not self.skip and not self.math:
            self.out.append(s)

    @staticmethod
    def attrs_to_str(attrs):
        out = []
        for k, v in attrs:
            if v is None:
                out.append(k)
            else:
                out.append('%s="%s"' % (k, html.escape(v, quote=True)))
        return (' ' + ' '.join(out)) if out else ''

    @staticmethod
    def cls(attrs):
        return (dict(attrs).get('class') or '')

    # -- 标签 ---------------------------------------------------------
    def handle_starttag(self, tag, attrs):
        if self.skip:
            if tag == self.skip[-1][0]:
                self.skip[-1][1] += 1
            return
        a = dict(attrs)
        cl = self.cls(attrs)
        if tag in ('script', 'style', 'svg'):
            self.skip.append([tag, 1])
            return
        if tag == 'mjx-container':
            self.math = 1
            self.math_title = None
            return
        if tag == 'a' and ('headerlink' in cl
                           or 'edit-landing' in (a.get('href') or '')
                           or 'md-content__button' in cl):
            self.skip.append(['a', 1])
            return
        if 'linenos' in cl or 'linenodiv' in cl:
            self.skip.append([tag, 1])
            return
        if tag == 'div' and 'highlight' in cl:
            self.highlight = 1
            return
        if self.highlight:
            if tag == 'div':
                self.highlight += 1
            if tag in ('table', 'thead', 'tbody', 'tr', 'td', 'colgroup', 'col',
                       'div', 'span'):
                return
        if tag == 'img':
            if self.math:
                if a.get('title'):
                    self.math_title = a['title']
                return
            self.emit('<img src="%s" alt="%s">' % (html.escape(a.get('src') or '', quote=True),
                                                   html.escape(a.get('alt') or '', quote=True)))
            return
        self.emit('<%s%s>' % (tag, self.attrs_to_str(attrs)))

    def handle_startendtag(self, tag, attrs):
        if self.skip or self.math or self.highlight:
            return
        self.emit('<%s%s>' % (tag, self.attrs_to_str(attrs)))

    def handle_endtag(self, tag):
        if self.skip:
            if tag == self.skip[-1][0]:
                self.skip[-1][1] -= 1
                if self.skip[-1][1] <= 0:
                    self.skip.pop()
            return
        if tag == 'mjx-container':
            self.math = 0
            raw = self.math_title or ''
            title = raw.strip()
            if title:
                if '\n' in raw or title.startswith('\\begin'):
                    self.out.append('\n\n@@MATH%s@@\n\n' % self._store(title, True))
                else:
                    self.out.append('@@MATH%s@@' % self._store(title, False))
            return
        if tag in VOID:
            return
        if self.highlight:
            if tag == 'div':
                self.highlight -= 1
            if tag in ('table', 'thead', 'tbody', 'tr', 'td', 'colgroup', 'col',
                       'div', 'span'):
                return
        self.emit('</%s>' % tag)

    def handle_data(self, data):
        self.emit(html.escape(data, quote=False))

    def handle_comment(self, data):
        pass


def article_body(doc):
    m = re.search(r'<article[^>]*class="[^"]*md-content__inner[^"]*"[^>]*>', doc)
    if not m:
        raise SystemExit('article container not found')
    start = m.end()
    end = doc.index('</article>', start)
    c = Cleaner()
    c.feed(doc[start:end])
    c.close()
    body = ''.join(c.out)
    body = re.sub(r'</p>\s*<p>', '</p>\n<p>', body)
    return body, c.math_store


def to_markdown(body, math_store):
    tmp = '_tmp_body.html'
    with io.open(tmp, 'w', encoding='utf-8') as f:
        f.write('<html><body>' + body + '</body></html>')
    out = subprocess.run(['pandoc', '-f', 'html', '-t', 'gfm', '--wrap=none', tmp],
                         capture_output=True)
    os.remove(tmp)
    if out.returncode != 0:
        raise SystemExit(out.stderr.decode('utf-8', 'replace'))
    md = out.stdout.decode('utf-8', 'replace')

    # 占位符换回 $…$ / $$…$$，并对 LaTeX 里的 markdown 转义做还原
    def unesc(s):
        return re.sub(r'\\([_*\[\]`|~^$\\])', r'\1', s)

    def sub_math(mo):
        idx = int(mo.group(1))
        latex, disp = math_store[idx]
        latex = unesc(latex)
        return ('\n\n$$%s$$\n\n' % latex) if disp else ('$%s$' % latex)
    md = re.sub(r'@@MATH(\d+)@@', sub_math, md)
    md = re.sub(r'\n{3,}', '\n\n', md)
    md = re.sub(r'[ \t]+\n', '\n', md)
    cut = md.find('本页面最近更新')
    if cut > 0:
        md = md[:md.rfind('\n', 0, cut)]
    return md.strip() + '\n'


def main():
    page = sys.argv[1] if len(sys.argv) > 1 else 'ds/huffman-tree'
    doc = open(os.path.join(ROOT, page.replace('/', os.sep), 'index.html'),
               encoding='utf-8').read()
    body, math_store = article_body(doc)
    if '--raw' in sys.argv:
        print(body[:6000])
        return
    md = to_markdown(body, math_store)
    if '--out' in sys.argv:
        out = sys.argv[sys.argv.index('--out') + 1]
        with open(out, 'w', encoding='utf-8') as f:
            f.write(md)
        print('wrote %s (%d chars, %d lines)' % (out, len(md), md.count('\n')))
        return
    print(md)


if __name__ == '__main__':
    main()
