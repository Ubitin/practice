# ============================================================
#  机试代码模板速查手册（打印版）DOCX 构建脚本
#
#  用法： pwsh print_build\build_print_docx.ps1
#  流程： Markdown →(pandoc)→ docx →(排版：A4/页码/字体/底纹)
#  产出： 机试代码模板速查手册（打印版）.docx（与源 .md 同目录）
#
#  修改手册后重跑本脚本即可重新生成。
# ============================================================
$ErrorActionPreference = 'Stop'

$root  = Split-Path -Parent $PSScriptRoot
$srcMd = Join-Path $root '机试代码模板速查手册.md'
if (-not (Test-Path -LiteralPath $srcMd)) { throw "找不到源文件: $srcMd" }

$outDocx = Join-Path $root '机试代码模板速查手册（打印版）.docx'
$tmp     = Join-Path $PSScriptRoot '_build'
New-Item -ItemType Directory -Force -Path $tmp | Out-Null
$workMd = Join-Path $tmp 'work.md'
$midDocx = Join-Path $tmp 'manual_raw.docx'
$pyStyle = Join-Path $PSScriptRoot 'style_print_docx.py'
$pyCheck = Join-Path $PSScriptRoot 'check_docx.py'

try {
  Write-Host '[1/3] 准备 Markdown（标题 → 打印版）…'
  $content = Get-Content -LiteralPath $srcMd -Raw -Encoding UTF8
  $content = $content -replace '(?m)^#\s+机试代码模板速查手册.*$', '# 机试代码模板速查手册（打印版）'
  Set-Content -LiteralPath $workMd -Value $content -Encoding UTF8

  Write-Host '[2/3] pandoc → docx …'
  & pandoc $workMd -f gfm -t docx -o $midDocx
  if ($LASTEXITCODE -ne 0) { throw 'pandoc docx 转换失败' }

  Write-Host '[3/3] 排版收尾（A4 / 页脚页码 / 字体 / 代码底纹）…'
  & python $pyStyle $midDocx $outDocx
  if ($LASTEXITCODE -ne 0) { throw 'docx 排版失败' }

  if (Test-Path -LiteralPath $pyCheck) {
    & python $pyCheck $outDocx
  }
  Write-Host ''
  Write-Host '完成 ✔'
  Write-Host ("  DOCX: " + $outDocx)
} finally {
  Remove-Item -LiteralPath $tmp -Recurse -Force -ErrorAction SilentlyContinue
}
