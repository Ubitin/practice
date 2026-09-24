# Screenshot the graph-DFS animation at a few frames with Edge headless.
# ASCII-only on purpose (Windows PowerShell 5.1 reads BOM-less scripts as ANSI).
# NOTE: use INTERPOLATED strings for arguments that contain variables --
# inside an array literal,  'a' + $b  becomes TWO elements (comma binds tighter than +).
param([int[]]$Frames = @(0, 2, 13, 17, 23, 29))
$ErrorActionPreference = 'Stop'
$here = $PSScriptRoot

$edge = @(
  (Join-Path ${env:ProgramFiles(x86)} 'Microsoft\Edge\Application\msedge.exe'),
  (Join-Path $env:ProgramFiles 'Microsoft\Edge\Application\msedge.exe')
) | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
if (-not $edge) { throw 'msedge.exe not found' }
Write-Host "edge = $edge"

$src = Join-Path $here 'graph_dfs_anim.html'
if (-not (Test-Path -LiteralPath $src)) { throw "missing $src" }
$out = Join-Path $here '_shots'
New-Item -ItemType Directory -Force -Path $out | Out-Null
$profile = Join-Path $here '_edgeprofile_shot'

foreach ($f in $Frames) {
  Remove-Item -LiteralPath $profile -Recurse -Force -ErrorAction SilentlyContinue
  New-Item -ItemType Directory -Force -Path $profile | Out-Null
  $name = 'f{0:d3}' -f $f
  $png = Join-Path $out ($name + '.png')
  Remove-Item -LiteralPath $png -Force -ErrorAction SilentlyContinue
  $url = 'file:///' + ($src -replace '\\', '/') + "#f=$f"

  $argList = @(
    '--headless=new', '--disable-gpu', '--hide-scrollbars', '--no-first-run',
    '--disable-crash-reporter', '--disable-breakpad', '--virtual-time-budget=5000',
    "--user-data-dir=$profile",
    '--window-size=1180,920',
    "--screenshot=$png",
    $url
  )
  $eap = $ErrorActionPreference
  $ErrorActionPreference = 'Continue'
  & $edge @argList 2>$null | Out-Null
  $ErrorActionPreference = $eap
  if (Test-Path -LiteralPath $png) {
    Write-Host ('  frame {0,-4} -> {1}  {2:N0} bytes' -f $f, $name, (Get-Item -LiteralPath $png).Length)
  } else {
    Write-Host ('  frame {0,-4} -> FAILED' -f $f)
  }
}
Remove-Item -LiteralPath $profile -Recurse -Force -ErrorAction SilentlyContinue
Write-Host 'Done.'
