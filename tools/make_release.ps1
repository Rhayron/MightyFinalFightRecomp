param([string]$Rom, [string]$BuildDir='build_release', [string]$EngineRoot, [string]$RecompUi, [switch]$SkipBuild)
$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
if (-not $EngineRoot) { $EngineRoot=Join-Path $root 'nesrecomp' }
if (-not $Rom) {
    $images=@(Get-ChildItem -LiteralPath (Join-Path $root 'rom'),$root -File -ErrorAction SilentlyContinue | Where-Object { $_.Extension -ieq '.nes' })
    if ($images.Count -eq 1) { $Rom=$images[0].FullName }
}
if (-not $SkipBuild) {
    # Same configure as build_all.bat (Python pinned), then package without rebuilding.
    & (Join-Path $PSScriptRoot 'build.ps1') -Rom $Rom -BuildDir $BuildDir -EngineRoot $EngineRoot -RecompUi $RecompUi
}
& (Join-Path $EngineRoot 'tools/package_cycle_windows.ps1') -ProjectRoot $root -Target 'MightyFinalFightRecomp' -Title 'Mighty Final Fight' -BuildDir $BuildDir -Rom $Rom -EngineRoot $EngineRoot -RecompUi $RecompUi -SkipBuild -GameNotes 'Optional: --extra-scanlines 128 gives the MMC3 extra CPU time and reduces slowdown (an enhancement, off by default).'
