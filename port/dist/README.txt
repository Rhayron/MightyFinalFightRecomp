Mighty Final Fight - the port
=============================

This is Capcom's Mighty Final Fight (NES, 1993) rebuilt as a native program:
the game's code in C, drawing and playing sound by itself, with no emulator
inside. It comes WITHOUT the game's graphics, music data or anything else
from the cartridge. You need your own dump of the game, once.


On Linux and macOS the program uses SDL2 from the system (2.0.4 or later):
Debian/Ubuntu apt install libsdl2-2.0-0, Fedora dnf install SDL2, macOS
brew install sdl2. On Windows SDL2.dll comes with it.


First run: your dump
--------------------

You need Mighty Final Fight (USA) as an iNES .nes file, CRC32 15EB0BEE
(the whole file, header included). The program checks it and refuses any
other file.

Give it to the game in one of these ways:

  - drop the .nes file on the program, MightyFinalFight.exe on Windows (or
    on the window that asks for it when you start the game without it);
  - put the .nes file in the same folder as the program and start it;
  - from a terminal: MightyFinalFight "path/to/Mighty Final Fight (USA).nes"
    (or MightyFinalFight --extract "path/to/the dump.nes" to only do this).

The game then extracts what it needs into an "assets" folder (about 220 KB)
and never reads the dump again; you can put the dump away.

Where the files go: next to the program if that folder can be written (a
portable install), otherwise in your user folder:
  Windows  %APPDATA%\MightyFinalFight
  macOS    ~/Library/Application Support/MightyFinalFight
  Linux    ~/.local/share/MightyFinalFight  (or $XDG_DATA_HOME)
There you find assets/, saves/ and MightyFinalFight.ini.


Controls
--------

  NES       keyboard        game controller
  A         X               B or Y
  B         Z               A or X
  Start     Enter           Start
  Select    Right Shift     Back
  D-pad     arrow keys      d-pad or left stick

  Tab         hold to run fast (no sound)
  F11         full screen (Alt+Enter too)
  F5          save a state to the current slot
  F7          load the current slot
  F6          next slot (1 to 4; the window's title shows the slot)
  Esc         quit

Save states are taken during play: if you press F5 on a title, map or story
screen, the game saves as soon as play resumes (the window's title says
"saved"); in the ending there is no saving.


Settings: MightyFinalFight.ini
------------------------------

Written on the first run; edit it with any text editor and start the game
again. Window size (scale), full screen, the picture (wide, no_flicker),
sound on/off, every key and
controller button (SDL names: https://wiki.libsdl.org/SDL2/SDL_Keycode),
and the slowdown option.


Command line
------------

  MightyFinalFight --help        all options
  --scale N                      window size, N x 256x240
  --wide                         16:9: the camera shows more of each stage
  --no-flicker                   every sprite every frame (no NES flicker)
  --load FILE                    start from a save state
  --slowdown off                 never slow down (by default the game slows
                                 down where the console did)
  --headless --smoke N           run N frames without a window (testing)


Legal
-----

Mighty Final Fight is (c) Capcom. This program contains no part of the
cartridge; it runs only with a dump you provide. It uses SDL2 (zlib
license, SDL2-LICENSE.txt).
