param(
    [string]$SolExe = '.\p2895_fixed.exe',
    [string]$BruExe = '.\p2895_brute.exe',
    [int]$From = 1,
    [int]$To = 2000,
    [int]$MaxShow = 3
)

$env:Path += ';C:\w64devkit\bin'
Set-Location 'D:\lenovo\chat_with_deepseek_harness\test_'

$bad = 0
for ($s = $From; $s -le $To; $s++) {
    .\gen2895.exe $s | Out-File -Encoding ascii stress_in.txt
    $a = (Get-Content stress_in.txt | & $SolExe) -join ''
    $b = (Get-Content stress_in.txt | & $BruExe) -join ''
    if ($a -ne $b) {
        $bad++
        if ($bad -le $MaxShow) {
            Write-Output "[seed $s] sol=[$a]  brute=[$b]"
            Get-Content stress_in.txt | ForEach-Object { Write-Output "        $_" }
        }
    }
}
Write-Output ("本轮结束: seeds {0}..{1}, 不一致 {2} 例 / 共 {3} 轮" -f $From, $To, $bad, ($To - $From + 1))
