#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""docx 打印排版（单步收尾）：A4 几何、页脚页码域、updateFields、
   标题/代码块/引用的直接字体与底纹格式化。

用法: python style_print_docx.py <pandoc成品.docx> <输出.docx>
"""
import sys

from docx import Document
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Mm, Pt, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH

BODY_EA = '微软雅黑'
BODY_LATIN = 'Microsoft YaHei'
ACCENT = RGBColor(0x14, 0x3A, 0x5C)
ACCENT2 = RGBColor(0x1E, 0x4F, 0x79)
ACCENT3 = RGBColor(0x2E, 0x74, 0xB5)
GRAY_Q = RGBColor(0x2E, 0x3A, 0x47)
GRAY_C = RGBColor(0x5A, 0x66, 0x72)

_SHD_SUCC = (
    'w:tabs', 'w:suppressAutoHyphens', 'w:kinsoku', 'w:wordWrap',
    'w:overflowPunct', 'w:topLinePunct', 'w:autoSpaceDE', 'w:autoSpaceDN',
    'w:bidi', 'w:adjustRightInd', 'w:snapToGrid', 'w:spacing', 'w:ind',
    'w:contextualSpacing', 'w:mirrorIndents', 'w:suppressOverlap', 'w:jc',
    'w:textDirection', 'w:textAlignment', 'w:textboxTightWrap', 'w:outlineLvl',
    'w:divId', 'w:cnfStyle', 'w:rPr', 'w:sectPr', 'w:pPrChange',
)


def setup_page(sec):
    sec.page_width = Mm(210)
    sec.page_height = Mm(297)
    sec.top_margin = Mm(13)
    sec.bottom_margin = Mm(16)
    sec.left_margin = Mm(14)
    sec.right_margin = Mm(14)
    sec.header_distance = Mm(8)
    sec.footer_distance = Mm(10)


def style_run_font(run, latin, ea, size_pt=None, bold=None, color=None):
    run.font.name = latin  # ascii + hAnsi
    rpr = run._element.get_or_add_rPr()
    rf = rpr.find(qn('w:rFonts'))
    if rf is None:
        rf = OxmlElement('w:rFonts')
        rpr.insert(0, rf)
    rf.set(qn('w:ascii'), latin)
    rf.set(qn('w:hAnsi'), latin)
    rf.set(qn('w:eastAsia'), ea)
    rf.set(qn('w:cs'), latin)
    if size_pt is not None:
        run.font.size = Pt(size_pt)
    if bold is not None:
        run.font.bold = bold
    if color is not None:
        run.font.color.rgb = color


def shade_paragraph(p, fill):
    pPr = p._p.get_or_add_pPr()
    for e in pPr.findall(qn('w:shd')):
        pPr.remove(e)
    shd = OxmlElement('w:shd')
    shd.set(qn('w:val'), 'clear')
    shd.set(qn('w:color'), 'auto')
    shd.set(qn('w:fill'), fill)
    pPr.insert_element_before(shd, *_SHD_SUCC)


def append_field(p, instr):
    def mk_r():
        r = OxmlElement('w:r')
        rpr = OxmlElement('w:rPr')
        sz = OxmlElement('w:sz')
        sz.set(qn('w:val'), '16')          # 8pt
        szcs = OxmlElement('w:szCs')
        szcs.set(qn('w:val'), '16')
        rpr.append(sz)
        rpr.append(szcs)
        rf = OxmlElement('w:rFonts')
        rf.set(qn('w:ascii'), BODY_LATIN)
        rf.set(qn('w:hAnsi'), BODY_LATIN)
        rf.set(qn('w:eastAsia'), BODY_EA)
        rpr.insert(0, rf)
        r.append(rpr)
        return r

    for t, txt in (('begin', None), ('instr', instr), ('separate', None),
                   ('text', '1'), ('end', None)):
        r = mk_r()
        if t == 'text':
            tt = OxmlElement('w:t')
            tt.text = txt
            r.append(tt)
        else:
            f = OxmlElement('w:fldChar')
            f.set(qn('w:fldCharType'), t)
            if t == 'instr':
                it = OxmlElement('w:instrText')
                it.set(qn('xml:space'), 'preserve')
                it.text = instr
                r.append(it)
            r.append(f)
        p._p.append(r)


def add_footer(doc):
    for sec in doc.sections:
        footer = sec.footer
        footer.is_linked_to_previous = False
        para = footer.paragraphs[0]
        for r in list(para.runs):
            r._element.getparent().remove(r._element)
        para.alignment = WD_ALIGN_PARAGRAPH.CENTER
        para.paragraph_format.space_before = Pt(0)
        para.paragraph_format.space_after = Pt(0)
        para.add_run('机试代码模板速查手册（打印版） · 第 ')
        append_field(para, ' PAGE ')
        para.add_run(' 页 / 共 ')
        append_field(para, ' NUMPAGES ')
        para.add_run(' 页')
        for r in para.runs:
            style_run_font(r, BODY_LATIN, BODY_EA, size_pt=8,
                           color=RGBColor(0x59, 0x5F, 0x6B))


def set_doc_defaults(doc):
    """docDefaults 回退字体：微软雅黑 9.5pt，保证任何未直接格式化的文字统一。"""
    st = doc.styles.element
    dd = st.find(qn('w:docDefaults'))
    if dd is None:
        return
    rpd = dd.find(qn('w:rPrDefault'))
    if rpd is None:
        rpd = OxmlElement('w:rPrDefault')
        dd.insert(0, rpd)
    rpr = rpd.find(qn('w:rPr'))
    if rpr is None:
        rpr = OxmlElement('w:rPr')
        rpd.append(rpr)
    for tag in ('w:rFonts', 'w:sz', 'w:szCs'):
        for e in rpr.findall(qn(tag)):
            rpr.remove(e)
    rf = OxmlElement('w:rFonts')
    rf.set(qn('w:ascii'), BODY_LATIN)
    rf.set(qn('w:hAnsi'), BODY_LATIN)
    rf.set(qn('w:eastAsia'), BODY_EA)
    rf.set(qn('w:cs'), BODY_LATIN)
    rpr.insert(0, rf)
    for tag, val in (('w:sz', '19'), ('w:szCs', '19')):  # 9.5pt
        e = OxmlElement(tag)
        e.set(qn('w:val'), val)
        rpr.append(e)
    # Normal 同步
    try:
        n = doc.styles['Normal']
        n.font.size = Pt(9.5)
        n.font.name = BODY_LATIN
        n.font.color.rgb = RGBColor(0x17, 0x18, 0x1C)
    except Exception:
        pass


def fmt_paragraphs(doc):
    n_h1 = n_h2 = n_h3 = n_code = n_bq = 0
    for p in doc.paragraphs:
        try:
            nm = p.style.name
        except Exception:
            nm = ''
        pf = p.paragraph_format
        if nm == 'Heading 1':
            pf.alignment = WD_ALIGN_PARAGRAPH.CENTER
            pf.space_before = Pt(0)
            pf.space_after = Pt(8)
            pf.keep_with_next = True
            for r in p.runs:
                style_run_font(r, BODY_LATIN, BODY_EA, 17, True, ACCENT)
            n_h1 += 1
        elif nm == 'Heading 2':
            pf.space_before = Pt(10)
            pf.space_after = Pt(4)
            pf.keep_with_next = True
            for r in p.runs:
                style_run_font(r, BODY_LATIN, BODY_EA, 13, True, ACCENT2)
            n_h2 += 1
        elif nm == 'Heading 3':
            pf.space_before = Pt(6)
            pf.space_after = Pt(3)
            pf.keep_with_next = True
            for r in p.runs:
                style_run_font(r, BODY_LATIN, BODY_EA, 11, True, ACCENT3)
            n_h3 += 1
        elif nm == 'Source Code':
            shade_paragraph(p, 'F4F6F9')
            pf.space_before = Pt(2)
            pf.space_after = Pt(6)
            pf.line_spacing = 1.0
            pf.keep_together = True
            for r in p.runs:
                style_run_font(r, 'Consolas', BODY_EA, 8.2)
            n_code += 1
        elif nm in ('Block Text', 'Quote'):
            shade_paragraph(p, 'EEF2F7')
            pf.left_indent = Mm(4)
            pf.space_before = Pt(3)
            pf.space_after = Pt(6)
            for r in p.runs:
                style_run_font(r, BODY_LATIN, BODY_EA, 9, False, GRAY_Q)
            n_bq += 1
    print(f'[fmt] H1={n_h1} H2={n_h2} H3={n_h3} code={n_code} quote={n_bq}')


def main():
    src, dst = sys.argv[1], sys.argv[2]
    doc = Document(src)
    for sec in doc.sections:
        setup_page(sec)
    set_doc_defaults(doc)
    fmt_paragraphs(doc)
    add_footer(doc)
    doc.save(dst)
    print(f'[style] {src} -> {dst}')


if __name__ == '__main__':
    main()
