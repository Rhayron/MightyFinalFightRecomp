<#
  make_port_release.ps1 - the port's package for players (PLAN_DECOMP.md P8)

  .\tools\make_port_release.ps1 [-BuildDir build_port_release] [-Sdl2Dir DIR] [-Rom DUMP] [-SkipBuild]

  Builds port/ with the window (SDL2) in a clean directory, configured
  WITHOUT a dump (no assets are made), and zips
      MightyFinalFight/MightyFinalFight.exe
      MightyFinalFight/SDL2.dll
      MightyFinalFight/README.txt        (port/dist/README.txt: how to give it the dump)
      MightyFinalFight/SDL2-LICENSE.txt  (SDL2's COPYING.txt, zlib)
  into release\MightyFinalFight-port-windows-x64.zip. Before zipping it runs
  the executable from the package folder (--help, and a start without data,
  which must say the data is missing and leave no assets behind); after, it
  lists the zip and refuses anything else (port/tools/release_check.py),
  and with -Rom it also searches every file for runs of the dump's bytes.
  Nothing in the zip comes from the ROM; the player's own dump makes the
  data on the first run.

  PLAN_DECOMP.md section 6: port/ is not published until the owner decides,
  and the executable is compiled from code derived from the game's.
#>
param([string]$BuildDir = 'build_port_release', [string]$Sdl2Dir, [string]$Rom, [switch]$SkipBuild)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root $BuildDir
if (-not $Sdl2Dir) { $Sdl2Dir = Join-Path $root 'nesrecomp/runner/external/SDL2' }
if (-not (Test-Path (Join-Path $Sdl2Dir 'lib/x64/SDL2.dll'))) { throw "no SDL2 in $Sdl2Dir (setup.bat, or -Sdl2Dir)" }

if (-not $SkipBuild) {
    if (Test-Path $build) { Remove-Item -Recurse -Force $build }
    & cmake -S (Join-Path $root 'port') -B $build -A x64 -DMFF_SDL=ON "-DMFF_SDL2_DIR=$Sdl2Dir" '-DMFF_ROM=' | Out-Host
    if ($LASTEXITCODE) { throw 'cmake configure failed' }
    & cmake --build $build --config Release | Out-Host
    if ($LASTEXITCODE) { throw 'cmake build failed' }
}
$bin = Join-Path $build 'Release'
if (Test-Path (Join-Path $build 'assets')) { throw "$build has game data (assets/): configure without MFF_ROM" }

$stage = Join-Path $build 'package/MightyFinalFight'
if (Test-Path (Split-Path $stage)) { Remove-Item -Recurse -Force (Split-Path $stage) }
New-Item -ItemType Directory -Force $stage | Out-Null
Copy-Item (Join-Path $bin 'MightyFinalFight.exe') $stage
Copy-Item (Join-Path $bin 'SDL2.dll') $stage
Copy-Item (Join-Path $root 'port/dist/README.txt') $stage
Copy-Item (Join-Path $Sdl2Dir 'COPYING.txt') (Join-Path $stage 'SDL2-LICENSE.txt')

# smoke test from the package folder, hidden, before anything is zipped
Push-Location $stage
try {
    $help = & .\MightyFinalFight.exe --help
    if ($LASTEXITCODE -ne 0 -or -not ($help -match 'usage')) { throw '--help failed' }
    $env:SDL_VIDEODRIVER = 'dummy'; $env:SDL_AUDIODRIVER = 'dummy'
    $out = & .\MightyFinalFight.exe --headless --smoke 1 2>&1 | Out-String
    $code = $LASTEXITCODE
    if ($code -ne 1 -or $out -notmatch 'data is missing') { throw "a start without data should ask for the dump (exit $code): $out" }
    if (Test-Path assets) { throw 'the smoke test left assets/ in the package' }
} finally {
    Remove-Item Env:SDL_VIDEODRIVER, Env:SDL_AUDIODRIVER -ErrorAction SilentlyContinue
    Pop-Location
}

$release = Join-Path $root 'release'
New-Item -ItemType Directory -Force $release | Out-Null
$zip = Join-Path $release 'MightyFinalFight-port-windows-x64.zip'
if (Test-Path $zip) { Remove-Item -Force $zip }
Compress-Archive -Path $stage -DestinationPath $zip

$py = (Get-Command py -ErrorAction SilentlyContinue)
$check = Join-Path $root 'port/tools/release_check.py'
if ($py) {
    $pyArgs = @('-3', '-I', $check, $zip)
    if ($Rom) { $pyArgs += $Rom }
    & py @pyArgs
    if ($LASTEXITCODE) { throw 'the package check failed' }
} else {
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    $z = [IO.Compression.ZipFile]::OpenRead($zip)
    $z.Entries | ForEach-Object { '{0,10}  {1}' -f $_.Length, $_.FullName }
    $z.Dispose()
    Write-Warning 'no Python: listed, not checked'
}
"Package: $zip"
