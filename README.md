# **SPAM**-factory

a clicker-game for the GameBoy.

[Play it online]() (powered by the ___ javascript emulator) or [download]() the rom from the releases.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development.

### Installing / Usage

#### Prerequisites

To build the project from source you will need the following:

* Small Devices C Compiler [SDCC](http://sdcc.sourceforge.net/)
* GB-Library [GBDK-n](https://github.com/andreasjhkarlsson/gbdk-n])

#### Build Instructions

GBDK-n comes with a few very helpful batch-files to ease the buildprocess.
The Make.bat file in the project directory will need to be adjusted to your environment.

Or you can do it the hard way and build the rom manually:

You will need to first compile all the modules seperately (SDCC can't compile all at once)
```
sdcc -mgbz80 --no-std-crt0 -I "%GBDK_DIR%\include" -I "%GBDK_DIR%\include\asm" -c main.c -o main.rel
```

Then you can link them all together
```
sdcc -mgbz80 --no-std-crt0 --data-loc 0xc0a0 -L "%GBDK_DIR%\lib" "%GBDK_DIR%\lib\crt0.rel" gb.lib -o a.ihx gamestate.rel main.rel ... -o spam.ihx
```

The result can finally be made into a Gameboy compatible rom using
```
makebin -Z spam.ihx spam.gb
```

This file can be played in an emulator or transferred to a cartridge to play it on the real hardware.

## Built With

Compiler: [SDCC](http://sdcc.sourceforge.net/) - GPL/zlib
Library: [GBDK-n](https://github.com/andreasjhkarlsson/gbdk-n]) - OSI
Code Editor: [Notepad++](https://notepad-plus-plus.org) - GPL
Graphics: [GBSpriteStudio](pending release as foss) - 
Music: [OpenMPT](https://openmpt.org/) - GPL
Converter: [pewMusicConverter](pending release as foss) - 
Sound: [gbsound.gb](http://gbdev.gg8.se/files/musictools/Unknown-Other/)
Emulator: [yet undecided]()

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on contributing to this project.

## Authors

* **Mathias Pain** - *Initial work* - [madpew](https://github.com/madpew)

Also take a look at the list of [contributors](https://github.com/madpew/spam-factory/contributors) who participated in this project.

## License

This project is licensed under the BSD 3-Clause License - see [LICENSE](LICENSE) for details

## Acknowledgments

* Thanks to everyone on the gbdev discord for helping with ideas and issues.
* This game was inspired by [paperclips](http://www.decisionproblem.com/paperclips/index.html) an clicker-browsergame. Give it a try!
* [OpenJam](http://openjam.io/) - a gamejam focused on encouraging use of open source.