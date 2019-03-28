Platform | BasiliskII Build | SheepShaver Build
---------|:-----------------|:------------------
AmigaOS  | [Deprecate 💀](https://github.com/emaculation/macemu/issues/81) | [Deprecate 💀](https://github.com/emaculation/macemu/issues/81)
BeOS     | [Deprecate 💀](https://github.com/emaculation/macemu/issues/82) | [Deprecate 💀](https://github.com/emaculation/macemu/issues/82)
FreeBSD  | [Costs 💰](https://cirrus-ci.org/pricing/) | [Costs 💰](https://cirrus-ci.org/pricing/)
Linux    | [![Linux Build Status](http://badges.herokuapp.com/travis/emaculation/macemu?env=BADGE=linux&label=build&branch=master)](https://travis-ci.org/emaculation/macemu) | Soon
OSX      | [![OSX Build Status](http://badges.herokuapp.com/travis/emaculation/macemu?env=BADGE=osx&label=build&branch=master)](https://travis-ci.org/emaculation/macemu) [Null ⚠️](https://github.com/emaculation/macemu/issues/84) | Soon
Windows  | [![Windows Build status](https://ci.appveyor.com/api/projects/status/k47o17u31v1xh175/branch/master?svg=true)](https://ci.appveyor.com/project/ianfixes/macemu) | Soon


### Supported Builds

#### BasiliskII
```
macOS 64-bit ---
Linux 32-bit JIT
MinGW 32-bit JIT
```
#### SheepShaver
```
macOS 64-bit JIT
Linux 32-bit JIT
MinGW 32-bit ---
```
### How To Build
These builds need to be installed SDL2 framework/library.
#### BasiliskII
##### macOS
1. Open BasiliskII/src/MacOSX/BasiliskII.xcodeproj
1. Set Build Configuration to Release
1. Build

##### Linux(x86)
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
#### SheepShaver
##### macOS
1. Open SheepShaver/src/MacOSX/SheepShaver_Xcode8.xcodeproj
1. Set Build Configuration to Release
1. Build

##### Linux(x86)
```
$ cd macemu/SheepShaver
$ make links
$ cd src/Unix
$ ./autogen.sh
$ make
```
##### MinGW32/MSYS
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
