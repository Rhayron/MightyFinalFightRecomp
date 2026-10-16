param(
    [string]$Rom,
    [string]$BuildDir,
    [string]$EngineRoot,
    [string]$RecompUi,
    [switch]$Headless
)
$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
if (-not $EngineRoot) { $EngineRoot=Join-Path $root 'nesrecomp' }
# Always name the recomp-ui checkout: package_cycle_windows.ps1 reads it back
# from CMakeCache.txt, and an empty NESRECOMP_RECOMP_UI there (CRLF line)
# comes back as "" instead of falling back to <root>\recomp-ui.
if (-not $RecompUi) { $RecompUi=Join-Path $root 'recomp-ui' }
if (-not $BuildDir) { $BuildDir = if ($Headless) { 'build_headless' } else { 'build_release' } }
if (-not $Rom) {
    $images=@(Get-ChildItem -LiteralPath (Join-Path $root 'rom'),$root -File -ErrorAction SilentlyContinue | Where-Object { $_.Extension -ieq '.nes' })
    if ($images.Count -eq 1) { $Rom=$images[0].FullName }
}
if (-not $Rom) { throw 'Supply -Rom with the original NTSC ROM path (or put one .nes in rom\).' }

$cmakeArgs=@()
if ($Headless) { $cmakeArgs += '-DNESRECOMP_HEADLESS=ON' }
# The cycle backend's code generation needs Python >= 3.11; hand CMake the
# launcher's newest interpreter so a Store alias or an old python.exe on PATH
# is never picked.
$py = Get-Command py.exe -CommandType Application -ErrorAction SilentlyContinue | Select-Object -First 1
if ($py) {
    $exe = & $py.Source -3 -I -c 'import sys; print(sys.executable if sys.version_info >= (3, 11) else "")'
    if ($exe) { $cmakeArgs += "-DPython3_EXECUTABLE=$exe" }
}

& (Join-Path $EngineRoot 'tools/build_cycle_windows.ps1') -ProjectRoot $root -Rom $Rom -BuildDir $BuildDir -EngineRoot $EngineRoot -RecompUi $RecompUi -CMakeArgs $cmakeArgs
