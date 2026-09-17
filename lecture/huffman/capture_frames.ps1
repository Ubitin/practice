# ============================================================
#  Huffman demo: capture each animation state as a PNG frame
#  [ASCII-only on purpose: Windows PowerShell 5.1 reads
#   BOM-less scripts as ANSI]
#
#  Usage:
#    powershell -ExecutionPolicy Bypass -File capture_frames.ps1
#    powershell ... -File capture_frames.ps1 -Preset 1 -Scale 2 -OutDir frames2x
#
#  Renders huffman-demo.html?static=1&preset=P&step=N with headless
#  Edge/Chrome. The page hides its header/controls in static mode, so
#  each screenshot is exactly the 1000x620 scene.
# ============================================================
param(
  [int]$Preset = 0,
  [int]$Scale  = 1,
  [string]$OutDir = 'frames',
  [int]$Steps  = -1
)

$ErrorActionPreference = 'Stop'
$here = $PSScriptRoot

$edge = @(
  (Join-Path ${env:ProgramFiles(x86)} 'Microsoft\Edge\Application\msedge.exe'),
  (Join-Path $env:ProgramFiles 'Microsoft\Edge\Application\msedge.exe'),
  (Join-Path $env:ProgramFiles 'Google\Chrome\Application\chrome.exe')
) | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
if (-not $edge) { throw 'Edge / Chrome not found' }

$html = Join-Path $here 'huffman-demo.html'
if (-not (Test-Path -LiteralPath $html)) { throw 'huffman-demo.html missing: run make_huffman_demo.py first' }

if ($Steps -lt 0) {
  # step count comes from the JSON the generator wrote
  $json = Get-Content -LiteralPath (Join-Path $here 'huffman-steps.json') -Raw -Encoding UTF8 | ConvertFrom-Json
  $Steps = $json.presets[$Preset].steps.Count
}

$out = Join-Path $here $OutDir
New-Item -ItemType Directory -Force -Path $out | Out-Null
$profile = Join-Path $here '_edgeprofile'
Remove-Item -LiteralPath $profile -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $profile | Out-Null

$base = ([uri]$html).AbsoluteUri
Write-Host "render preset=$Preset scale=${Scale}x steps=$Steps -> $OutDir"

for ($i = 0; $i -lt $Steps; $i++) {
  $png = Join-Path $out ('step-{0}.png' -f $i)
  $url = "$base`?static=1&preset=$Preset&step=$i"
  $argList = @(
    '--headless=new', '--disable-gpu', '--hide-scrollbars', '--no-first-run',
    '--disable-crash-reporter', '--disable-breakpad',
    "--force-device-scale-factor=$Scale",
    "--user-data-dir=$profile",
    '--window-size=1000,620',
    "--screenshot=$png",
    $url
  )
  Remove-Item -LiteralPath $png -Force -ErrorAction SilentlyContinue
  $eap = $ErrorActionPreference
  $ErrorActionPreference = 'Continue'
  & $edge @argList 2>$null | Out-Null
  $ErrorActionPreference = $eap
  if (-not (Test-Path -LiteralPath $png)) {
    Remove-Item -LiteralPath $profile -Recurse -Force -ErrorAction SilentlyContinue
    New-Item -ItemType Directory -Force -Path $profile | Out-Null
    $ErrorActionPreference = 'Continue'
    & $edge @argList 2>$null | Out-Null
    $ErrorActionPreference = $eap
  }
  if (-not (Test-Path -LiteralPath $png)) { throw "render failed at step $i" }
  Write-Host ('  step-{0}.png  {1,7:N0} bytes' -f $i, (Get-Item -LiteralPath $png).Length)
}

Remove-Item -LiteralPath $profile -Recurse -Force -ErrorAction SilentlyContinue
Write-Host 'Done.'
