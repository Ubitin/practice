# ============================================================
#  Catalan figures: SVG -> PNG  [ASCII-only on purpose:
#  Windows PowerShell 5.1 reads BOM-less scripts as ANSI,
#  so keep this file free of non-ASCII characters.]
#
#  Usage:  powershell -File render_figures.ps1 [-Preview1x]
#    (no switch)  : 2x PNG next to the SVG  (final artwork)
#    -Preview1x   : additionally writes 1x PNGs into _preview\
#                   (small enough for quick visual review)
#
#  Steps:  python make_figures.py  ->  figN-*.svg + _render_figN.html
#          Edge headless screenshot -> figN-*.png
#
#  PNG : for Word / pandoc(docx) / older editors
#  SVG : vector source, lossless zoom, for browser / VS Code / LaTeX
# ============================================================
param([switch]$Preview1x)

$ErrorActionPreference = 'Stop'
$here = $PSScriptRoot

$edge = @(
  (Join-Path ${env:ProgramFiles(x86)} 'Microsoft\Edge\Application\msedge.exe'),
  (Join-Path $env:ProgramFiles 'Microsoft\Edge\Application\msedge.exe'),
  (Join-Path $env:ProgramFiles 'Google\Chrome\Application\chrome.exe')
) | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
if (-not $edge) { throw 'Edge / Chrome not found: cannot rasterize SVG to PNG' }

Write-Host '[1/2] Generating SVG ...'
$env:PYTHONUTF8 = '1'
& python (Join-Path $here 'make_figures.py') | Out-Null
if ($LASTEXITCODE -ne 0) { throw 'make_figures.py failed' }

# figure index -> (width, height) in CSS px; names come from the SVG files
$sizes = @{
  1 = @{ w = 940;  h = 566 }
  2 = @{ w = 1010; h = 672 }
  3 = @{ w = 900;  h = 620 }
  4 = @{ w = 1140; h = 400 }
}
$profile = Join-Path $here '_edgeprofile'

# A stale profile makes Edge forward the command line to the old instance and
# bail out with "Multiple targets are not supported in headless mode", so wipe
# it before every shot.
function Reset-Profile {
  Remove-Item -LiteralPath $profile -Recurse -Force -ErrorAction SilentlyContinue
  New-Item -ItemType Directory -Force -Path $profile | Out-Null
}

function Invoke-Shot {
  param([int]$Index, [int]$Scale, [string]$OutDir)
  $svg = Get-ChildItem -LiteralPath $here -Filter ('fig{0}-*.svg' -f $Index) | Select-Object -First 1
  if (-not $svg) { throw ('fig{0}-*.svg not found' -f $Index) }
  $base = [System.IO.Path]::GetFileNameWithoutExtension($svg.Name)
  $html = Join-Path $here ('_render_fig{0}.html' -f $Index)
  if (-not (Test-Path -LiteralPath $html)) { throw "missing $html" }
  $png = Join-Path $OutDir ($base + '.png')

  # NOTE: use interpolated strings below. Inside an array literal,
  # "a" + $b would become TWO elements and Chromium then complains
  # "Multiple targets are not supported in headless mode".
  $argList = @(
    '--headless=new', '--disable-gpu', '--hide-scrollbars', '--no-first-run',
    '--disable-crash-reporter', '--disable-breakpad',
    "--force-device-scale-factor=$Scale",
    "--user-data-dir=$profile",
    "--window-size=$($sizes[$Index].w),$($sizes[$Index].h)",
    "--screenshot=$png",
    ([uri]$html).AbsoluteUri
  )
  Remove-Item -LiteralPath $png -Force -ErrorAction SilentlyContinue
  $eap = $ErrorActionPreference
  $ErrorActionPreference = 'Continue'      # Edge writes harmless noise to stderr
  & $edge @argList 2>$null | Out-Null
  $ErrorActionPreference = $eap
  if (-not (Test-Path -LiteralPath $png)) {
    Write-Host '  retry with a fresh profile ...'
    Reset-Profile
    $ErrorActionPreference = 'Continue'
    & $edge @argList 2>$null | Out-Null
    $ErrorActionPreference = $eap
  }
  if (-not (Test-Path -LiteralPath $png)) { throw ('render failed: ' + $base) }
  Write-Host ('  {0,-24} {1,5} x {2,-5} {3,8:N0} bytes   -> {4}' -f $base,
              ($sizes[$Index].w * $Scale), ($sizes[$Index].h * $Scale),
              (Get-Item -LiteralPath $png).Length, (Split-Path -Leaf $OutDir))
}

Write-Host '[2/2] Edge headless render (2x) ...'
Reset-Profile
foreach ($i in 1..4) { Invoke-Shot -Index $i -Scale 2 -OutDir $here }

if ($Preview1x) {
  Write-Host 'Preview render (1x) ...'
  $pv = Join-Path $here '_preview'
  New-Item -ItemType Directory -Force -Path $pv | Out-Null
  Reset-Profile
  foreach ($i in 1..4) { Invoke-Shot -Index $i -Scale 1 -OutDir $pv }
}

Remove-Item -LiteralPath $profile -Recurse -Force -ErrorAction SilentlyContinue
Write-Host 'Done.'
