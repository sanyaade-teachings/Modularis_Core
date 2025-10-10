<!--
(C) 2022-2025 Серый MLGamer. All freedoms preserved.
Дзен: <https://dzen.ru/seriy_mlgamer>
SoundCloud: <https://soundcloud.com/seriy_mlgamer>
YouTube: <https://www.youtube.com/@Seriy_MLGamer>
GitVerse: <https://gitverse.ru/Seriy_MLGamer>
E-mail: <Seriy-MLGamer@yandex.ru>

This file is free documentation: you can redistribute it and/or modify it under the terms of the Creative Commons Attribution-ShareAlike 4.0 International license: <https://creativecommons.org/licenses/by-sa/4.0/>.
This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
-->

# Modularis Core

![Modularis Core full logo](share/logo%20en_US.svg)

This is a free (as in freedom) modular synthesis framework for creation of free digital audio workstations (DAW) and other free programs. [Modularis](https://gitverse.ru/Seriy_MLGamer/Modularis) free modular DAW is based on it. Framework is ***fully* crossplatform** between mobile and desktop devices, supports various programming languages.

## Framework essence

### Freedom

Aim of this framework development is to make music art accessible for users of very different devices. Framework architecture is designed with flexibility and extensibility in mind to make possible developing wide range of programs based on this framework. Free license allows users to have benefits from natural information properties: permission to use the framework for any purpose, study and change its work, share the framework.

### Modular synthesis

Synthesizers, effects and control tools are modules that can be connected to other modules at input and output ports. It is more efficient and flexible architecture for music writing than multitrack architecture. Modular synthesis architecture has more potential of performance, multithreading and memory economy.

## Features

### Use in your programs!

*Modularis Core* framework can be used as **player** of music compositions made in programs based on this framework. You can **interact** with them as you programmed it.

### Make your DAW!

*Modularis Core* framework architecture makes it possible to create ***very different* music editors**, be it sequencer DAW, modular synthesizer, tracker or even drum machine!

### Program music!

It is possible to make music using *Modularis Core* framework **without any DAW**. Choose the programming language supported by the framework, create an instance of `Modularis` class, instances of modules, connect them with each other, configure them and **make sound**!

### Connect plugins!

*Modularis Core* framework supports its own flexible and extensible plugin system. **VST3** and **LV2** plugin support will be added in the future.

### Save your compositions to files! (not implemented)

*Modularis Core* framework implements **extensible Modularis project file format** based on JSON for possibility of manual editing (just in case). A file contains information about settings and connections of modules. It also can contain information for the DAW the project was created in (or something else). You can create either light file with external dependencies on plugins, samples or something else or heavy, but portable file with embedded dependencies.

# License

Modularis Core is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Modularis Core is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Modularis Core. If not, see <https://www.gnu.org/licenses/>.

![GNU General Public License version 3](share/gplv3-with-text-136x68.png)

***Only* free** (*as in freedom*) software under the terms of the GNU General Public License can be based on the *Modularis Core* framework.

# The framework is very close to the full release!

## Modularis Core 0.0.0pre

The core of **reverse compatibility** has been created and some improvements have been added throughout the framework.

Moreover, the framework's proprietary **extension** system has been implemented! Create your own modules and even ports aside the framework. *VST3 and LV2 plugin support hasn't been implemented yet. :)*

## Framework contents

  * Modularis

It features singlethreaded sound synthesis. It has "lazy update" mode &ndash; modules that are not connected, directly or indirectly, to the `Output` module are not updated. System sound frame format is 32-bit floating point number.

"Safe memory" system has been implemented in order to detect memory leaks.

### Modules

#### Control tools

  * Sequencer

It features playing notes in patterns with different tone, velocity and phase change behavior. 5 tone and velocity interpolation modes available: `INTERPOLATION_NONE`, `INTERPOLATION_LINEAR`, `INTERPOLATION_FAST`, `INTERPOLATION_SLOW`, `INTERPOLATION_SMOOTH`. Infinite polyphony. Compact data.

`Any_pattern` and `Pattern_none` pattern types have been removed due to their redundancy. `Pattern_none` behavior can be reproduced by an instance of class `Pattern` with track count equal to zero.

#### Synthesizers

  * Oscillator

It features polyphonic sound playing with ADSR envelope of the following 4 waveform types: sine `0`, triangle `1`, saw `2`, square `3`.

#### Effects

  * Note_chorus

The note processing effect for supersaws creation.

  * Transpose

Note transposition.

  * Amplifier

The simple increaser/reducer/phase invertor for sound.

  * Delay

Sound delay. If you make a feedback chain with these modules, you can get an echo effect.

  * Modulator

The amplitude modulation sound effect.

#### Extension interface

  * IModule

The base class for third-party modules creation. Inherit it and override virtual method `on_update()` to create your module.

### Ports

  * Note

The port to convey notes.

The system of unique note "scancodes" has been implemented to allow free connection of `Note` ports.

  * Sound

The port to convey one channel of sound wave.

#### Controllers

  * Integer_controller

The integer controller.

  * Real_controller

The real number controller.

  * ADSR

The real number controller group: `attack`, `decay`, `sustain`, `release`. It has a method to calculate envelope level.

#### Miscellaneous

  * Port_group

The group of ports that can be connected to other ports and groups or disconnected from them as one.

#### Extension interface

  * IPort

The base class for third-party ports creation. Inherit it and override virtual method `on_update()` to create your port.

Now you know about the framework features at this moment. It is time to test them!

## The Guide for Building, Packaging and Testing

I've migrated this project from CMake build system to my own enhanced, advanced, innovative build system [MakePy](https://gitverse.ru/Seriy_MLGamer/MakePy/tag/0.0.0).

### Dependencies

Before starting the following procedures it is required to install these build dependencies:

  * Python 3

#### GNU/Linux

  * GCC
  * `debhelper` package (if you want to pack ".deb" packages)

#### Windows

  * mingw-w64 GCC (including tools `dlltool` and `gendef`)

### Configuring

Before starting the following procedures you can configure them in "configuration.py" file. Configuring tips are in the file.

### Building

It is done by one command:

#### Bash

	$ ./make.py

#### CMD

	>python make.py

### Installation

This command will install the framework:

#### Bash

	$ sudo ./make.py install

#### CMD

As administrator:

	>python make.py install

### Cleanup

This command will clean working folder from built files:

#### Bash

	$ ./make.py clean

#### CMD

	>python make.py clean

### Removal

This command will remove the framework from the installation folder:

#### Bash

	$ sudo ./make.py remove

#### CMD

As administrator:

	>python make.py remove

### Packaging

Packaging of archives or packages is also done by this single command:

#### Bash

	$ ./make.py pack

#### CMD

	>python make.py pack

Simple!

### Testing

After the procedures above you can test the framework by building and running the test program which uses this framework and is written in C. It contains the player with graphical interface where my new little track plays.

![The player](share/player.png)

First of all, install the framework (see Installation and also [Релизы](https://gitverse.ru/Seriy_MLGamer/Modularis_Core/releases) section at GitVerse). Then install SDL2, SDL2_image and SDL2_ttf libraries. Your PC must support OpenGL no earlier than version 2.0. After that run this command in "test" folder if you want to test the player with GUI:

#### Bash

	$ ./test

#### CMD

	>test

Or run this command if you want to test the player in console mode. It doesn't require neither SDL2_image and SDL2_ttf libraries nor OpenGL support:

#### Bash

	$ ./test-nogui

#### CMD

	>test-nogui

These build scripts expect working with GCC compiler. But, I think, manual test compilation using other compiler with similar compilation arguments will not be a big deal.

If you test in Windows, make sure that headers, static and dynamic library files of SDL2, SDL2_image and SDL2_ttf are in "test" folder or the libraries are added to `PATH` (\<SDL2/SDL2_image/SDL2_ttf root folder>\\bin), `CPATH` (\<SDL2/SDL2_image/SDL2_ttf root folder>\\include) and `LIBRARY_PATH` (\<SDL2/SDL2_image/SDL2_ttf root folder>\\lib) environment variables.

# Enjoy using this framework!