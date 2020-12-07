# Mac Emulation

[![Gitter](https://badges.gitter.im/emaculation/macemu.svg)](https://gitter.im/emaculation/macemu?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

This repository contains the [BasiliskII](BasiliskII/) and [SheepShaver](SheepShaver/) projects.  It is an attempt to centralize the individual development efforts that have gone on in the absence of the original [cebix/macemu](https://github.com/cebix/macemu) repository owner.

The [cxmon](https://github.com/emaculation/cxmon) project has been split from this repository.

### Goal of this fork

Many of the architectures on which this codebase originally ran are no longer in production, and several are not readily testable with modern distributed CI/CD services.  It is the opinion of this maintiner that such a situation is an existential threat to the future of the project as a whole:

* if testing requires the use of antique hardware or usage of bespoke 3rd-party testing services for old architectures, we can't provide continuous integration of pull requests
* if we can't provide continuous integration of pull requests, we can't make informed decisions about accepting patches
* if we can't make informed decisions about accepting patches, the project can't evolve from open-source contributions

> As such, be prepared to offer tests (or guidance on _how_ to perform automated testing) on any contributions.

To date, the following architectures are functioning with a bare minimum of continuous integration testing.  These are the only architectures that will be maintained, unless other CI services for other architectures can be added.

Target      | Options      | Windows MinGW | OSX  | Linux
------------|:-------------|:--------------|:-----|:------
BasiliskII  |32-bit, no JIT| - | - | -
BasiliskII  |32-bit, JIT   |[![Windows Build status](https://ci.appveyor.com/api/projects/status/k47o17u31v1xh175/branch/master?svg=true)](https://ci.appveyor.com/project/ianfixes/macemu) | - | [![Linux Build Status](http://badges.herokuapp.com/travis/emaculation/macemu?env=BADGE=linux-basiliskii&label=build&branch=master)](https://travis-ci.org/emaculation/macemu)
BasiliskII  |64-bit, no JIT| - | [![OSX Build Status](http://badges.herokuapp.com/travis/emaculation/macemu?env=BADGE=osx-basiliskii&label=build&branch=master)](https://travis-ci.org/emaculation/macemu) | -
BasiliskII  |64-bit, JIT   | - | - | -
SheepShaver |32-bit, no JIT|[![Windows Build status](https://ci.appveyor.com/api/projects/status/k47o17u31v1xh175/branch/master?svg=true)](https://ci.appveyor.com/project/ianfixes/macemu) | - | -
SheepShaver |32-bit, JIT   | - | - | [![Linux Build Status](http://badges.herokuapp.com/travis/emaculation/macemu?env=BADGE=linux-sheepshaver&label=build&branch=master)](https://travis-ci.org/emaculation/macemu)
SheepShaver |64-bit, no JIT| - | - | -
SheepShaver |64-bit, JIT   | - | [![OSX Build Status](http://badges.herokuapp.com/travis/emaculation/macemu?env=BADGE=osx-sheepshaver&label=build&branch=master)](https://travis-ci.org/emaculation/macemu) | -


#### Deprecated platforms

OS       | CI Status
---------|:---------
FreeBSD  | [💰 Costs Money](https://cirrus-ci.org/pricing/)
BeOS R4 (PowerPC and x86) | 📉 No longer available
AmigaOS 3.x | 📉 No longer available


## How To Build

These builds need the SDL2 framework/library to be installed.

`.travis.yml` and `appveyor.yml` contain the build scripts for CI.  More generally, here are the steps:

### BasiliskII
#### macOS
1. Open BasiliskII/src/MacOSX/BasiliskII.xcodeproj
1. Set Build Configuration to Release
1. Build

#### Linux(x86)
```
$ cd macemu/BasiliskII/src/Unix
$ ./autogen.sh
$ make
```
##### MinGW32/MSYS
```
$ cd macemu/BasiliskII/src/Windows
$ ../Unix/autogen.sh
$ make
```
### SheepShaver
#### macOS
1. Open SheepShaver/src/MacOSX/SheepShaver.xcodeproj
1. Set Build Configuration to Release
1. Build

#### Linux(x86)
```
$ cd macemu/SheepShaver
$ make links
$ cd src/Unix
$ ./autogen.sh
$ make
```
#### MinGW32/MSYS
```
$ cd macemu/SheepShaver
$ make links
$ cd src/Windows
$ ../Unix/autogen.sh
$ make
```


## Bug reports

You found a bug? Well, open an issue in this repo, supplying as much information as possible (operating system and versions of Basilisk II and MacOS being used, relevant hardware information, the exact steps to reproduce the bug, etc.)

I also strongly suggest reading this before posting a bug report:
http://www.chiark.greenend.org.uk/~sgtatham/bugs.html


## Support

There is no point in sending questions about ROM files and how/where to get them.


### These are almost certainly outdated

The official Basilisk II home page is at
http://www.uni-mainz.de/~bauec002/B2Main.html

The Basilisk II project page on SourceForge is at
http://sourceforge.net/projects/basilisk/

If you have problems, you may want to visit the Basilisk II forums:
http://sourceforge.net/forum/?group_id=2123

There is also a mailing list for Basilisk II users:
http://lists.sourceforge.net/lists/listinfo/basilisk-user

And another mailing list for Basilisk II developers:
http://lists.sourceforge.net/lists/listinfo/basilisk-devel

Some general advice about asking technical support questions can be found at
http://www.catb.org/~esr/faqs/smart-questions.html

Keeping this in mind will greatly increase your chances of getting a useful answer.
