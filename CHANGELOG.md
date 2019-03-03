# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/) after v1.0.

## [Unreleased]
### Added
- Continuous Integration services

### Changed
- Documentation into Markdown format

### Deprecated

### Removed

### Fixed

### Security


# Pre-KeepAChangelog releases

## [v1.0.0] (V1.0 snapshot) – (date unknown)
### Added
- Unix: add large file support for 2+ GB hard disk images
- Unix: implemented `ignoresegv` feature on Linux/x86, Linux/ppc, Darwin/ppc
- Unix: new ether prefs option `tun` to use a TUN/TAP interface. The `configure` script used can be overriden with `etherconfig`
- Unix: sheep_net driver ported to kernel 2.6 (patches from Mac-On-Linux)
- Unix: add user-mode network emulation stack, enabled with `slirp`
- Unix: implement X11 to MacOS exchange of text through the clipboard
- Windows: a new port derived from Lauri's work is now available

### Changed
- backport `idlewait` feature from SheepShaver
- JIT: integrated dynamic translator for Linux/x86 and FreeBSD/x86
- JIT: ported backend to support Linux/x86\_64 (AMD64 and EM64T platforms)
- JIT: improve detection of newer CPUs based on the P4 core but fallback to a safe default, if necessary
- Unix: moved minimum `autoconf` support to 2.52+
- Unix: cleaned up `pthread` attributes [Brian Johnson]
- Unix: it is now possible to make the serial drivers pipe their input/output to programs by using a '|' followed by a command line as the modem or printer port setting (instead of a device name like `/dev/ttyS0`) [Brian Johnson]
- Unix: the option `--config FILE` tells B2 to use a different config file
- Unix: new prefs options `dsp` and `mixer` to set the OSS device names instead of the hardcoded `/dev/dsp` and `/dev/mixer`
- Unix: SDL audio and video output are now available in option
- Unix: improve overall responsiveness on NetBSD platforms

### Fixed
- fixed support for volumes larger than 4 GB
- fixed the problem with Ticks getting incremented on every interrupt, not just 60 Hz (e.g. moving the mouse made the caret blink faster)
- JIT: fixed translation through constant jumps (`jitinline` config option)
- Unix: fixed floppy problems under Linux
- Unix: serial port baud rates are now set correctly
- Unix: fix network code to work correctly on 64-bit platforms
- Unix: fix GUI to build with newer GTK+2
- Unix: fix crash when exitting the GUI early
- Unix: fix fullscreen DGA mode with the VidMode extension

## V1.0 snapshot) - 2002-01-15
### Added
- added support for on-the-fly video resolution and depth switching, and gamma tables
- added support for platform-independant mutexes, used by adb.cpp for thread-safe mouse handling

### Changed
- fsave/frestore emulation under AmigaOS and NetBSD/m68k always behaves like a 68882/68040 FPU, eliminating the need for 68060 FPU patches
- the TIME_OFFSET constant has been replaced by a (portable) function TimeToMacTime(); file dates in ExtFS should now be correct
- ADBInterrupt() is no longer called from the 60 Hz interrupt but has its own interrupt flag, potentially increasing the smoothness of mouse movement
- ether.cpp: implemented relatively platform-independant "AppleTalk over UDP" mode that doesn't require any special kernel modules or network drivers but can only interconnect instances of Basilisk II; this is enabled by setting `udptunnel` to `true`
- Unix: windowed display mode supports different resolutions and color depths, which can be switched on-the-fly
- Unix: `Ctrl-F5` grabs mouse in windowed mode (enhanced compatibility with games like flight simulators)
- Unix: X11 events are handled as soon as they arrive, outside of the 60 Hz video refresh raster
- Unix: audio sample rate, bit depth and channel count are adjustable in the MacOS `Sound` control panel

## V0.9 (release 0.9-1) - 2001-05-31
### Changed
- final adjustments for 0.9 release

## V0.9 (snapshot) - 2001-05-29
### Added
- added help for command line arguments, `PrefsInit()` now removes all processed options

### Changed
- Unix: some performance improvements to VOSF screen update code [Brian J. Johnson]
- Unix: renewed `SIGSEGV` support functions. Side effect: configure script cleanups and probable Direct Addressing/VOSF support for other platforms (NetBSD, AIX, OSF/1, Irix) [Gwenole Beauchesne]
- Unix: `-Ofast` option is supplied to `MIPSPro` compiler [Brian J. Johnson]
- Unix: FreeBSD `configure` script cleanups [Michael Alyn Miller]
- Unix: `ether_linux.cpp` moved and renamed to `ether_unix.cpp`, now also works with the tap driver under FreeBSD [Michael Alyn Miller]
- AmigaOS: SCSI buffer type can be selected

### Fixed
- `serial_unix.cpp`: `input/output_thread_cancel` weren't initialized [Kolja Waschk]
- Unix: workaround for IRIX `pthreads` bug in `Delay_usec()` [Brian J. Johnson]
- BeOS: fixed some bugs in the extfs file types handling

## V0.9 (snapshot) - 2001-02-17
### Added
- Unix: added (experimental) real addressing mode [Gwenole Beauchesne] (activated through the `--enable-addressing=real` configure option)
- Unix: added direct addressing mode [Gwenole Beauchesne] (default addressing mode, if possible)
- Unix: added screen updates on SEGV signals [Gwenole Beauchesne] (activated through the `--enable-vosf` configure option)
- Unix: added IRIX audio driver [Brian J. Johnson]
- AmigaOS: implemented XPRAM watchdog task
- BeOS: implemented the `powerrom_cpu` CPU engine for BeOS/PPC that uses the 680x0 emulator contained in an Apple PowerMac ROM to run Basilisk II (ca. 4x speed improvement); when running on a BeBox, a PCI PowerMac ROM image is needed to use this (the path name of the ROM file can be set with the preferences item `powerrom`, default is `PowerROM`)
- `cdrom.cpp`: implemented `Status(6)` call (return format list) [Landon Fuller]

### Changed
- adapted for cxmon V3.0 which is now the required minimum
- Unix: improved timing of periodic threads
- Unix: command line options now use `--`-prefix, e.g. `--rominfo`
- AmigaOS: Amiga mouse pointer is hidden inside windowed Mac displays
- AmigaOS: disabled 68060 Super Bypass mode because of CPU bug triggered by MacOS 8
- AmigaOS/Unix `extfs_*.cpp`: `.finf` helper file now stores complete `FInfo/FXInfo`, replaced `get/set_finder_*()` functions by `get/set_finfo()`
- AmigaOS/Unix: it's possible to specify preferences items on the command line
- `include/macos_util.h`: defines `FOURCC()` macro to make MacOS-like four-character-codes, replaced most instances of multi-character constants in the sources by this macro to avoid compiler warnings
- Imported some changes from the Windows port, most notably FPU fixes and the 1 Hz interrupt [Lauri Pesonen]
- Ported Lauri Pesonen's FPU core to GCC/i386 [Gwenole Beauchesne]

### Fixed
- UAE cpu: fixed a bug in the memory handlers preventing from correctly updating 15 and 16 bpp displays on big endian systems
- AmigaOS: enabled floppy support, fixed floppy bugs [Jürgen Lachmann]
- AmigaOS `sys_amiga.cpp`: workaround for `2060scsi.device` bug when when reading from CD-ROM [Jürgen Lachmann]
- AmigaOS `prefs_editor_amiga.cpp`: fixed bug when adding volumes [Jürgen Lachmann]
- AmigaOS: added MacsBug support (tested with MacsBug6.6.1), fixed `<move sr,(sp)>` bug [Jürgen Lachmann]

## V0.8 (snapshot) - 2000-07-13
### Added
- Unix `prefs_editor_unix.cpp`: new `Input` pane, containing keycode and mouse wheel options
- AmigaOS: added support for CyberGraphX [Jürgen Lachmann]
- `clip_*.cpp`: added prefs item `noclipconversion` for turning off charset conversion of text clips (only `CR->LF` gets translated) [Toshimitsu Tanaka]

### Changed
- Unix: runs natively on NetBSD/m68k
- Unix: compiles without `pthreads`, but without serial/ethernet/audio support
- Unix `video_x.cpp`: DGA and SHM are only tried on local X11 displays
- Unix `video_x.cpp`: re-integrated old window update method; `frameskip=0` ("Dynamic") selects new method, other values select old method

### Fixed
- Unix: `--without-mon` configure option now works
- `audio.cpp`: `audio_data` is cleared on reset [Lauri Pesonen]
- `prefs.cpp`: empty string prefs items could be written to prefs file but not read back correctly

## V0.8 (snapshot) - 2000-03-14
### Changed
- Unix `video_x.cpp`: new window refresh code [Samuel Lander]

### Fixed
- Unix `timer_unix.cpp`: fixed time zone handling under Linux and SVR4

## V0.8 (release 0.8-1) - 2000-01-30
### Changed
- `extfs`: extended size of finf helper files to 32 bytes to eventually allow storing the complete FInfo/FXInfo in it

## V0.8 (snapshot) - 1999-12-22
### Added
- new utility functions `Mac_memset`, `Mac2Host_memcpy`, `Host2Mac_memcpu` and `Mac2Mac_memcpy`

### Changed
- imported UAE CPU 0.8.10 changes
- some provisions for using UAE CPU compiler (doesn't work yet)
- video_x.cpp: XFree86 VidMode extension is used to switch modes in DGA mode

### Fixed
- `extfs.cpp`: fixed bug in `fs_rename()` and `fs_cat_move()` (auxiliary `IOParam` block was not in Mac address space and `FSItem` parent IDs didn't get swapped) [Lauri Pesonen]
- `extfs.cpp`: several fixes [Lauri Pesonen]

## V0.8 (snapshot) - 1999-11-02
### Added
- added replacement for `BlockMove()` trap
- added (incomplete) emulation of 68040 instructions (`CINV`, `CPUSH`, `MOVE16 (Ax)+,(Ay)+` and FPU stack frames), enough to boot MacOS
- Unix: added support for ESD audio output; merged with OSS audio and put in a new `audio_oss_esd.cpp` file which is also used under FreeBSD 3.x
- Unix: added `mkinstalldirs` to `make install` target
- Unix `video_x.cpp`: added mouse wheel support [Alexander R. Pruss]

### Changed
- a warning is issued if the ROM seems to require an FPU but FPU emulation is turned off
- medium removal is allowed in `CDROMExit()`
- UAE CPU: opcode routines no longer return the cycle count
- Unix: cleaned up the `configure` script
- Unix: ROM breakpoint can be specified with `-break` command line argument
- Unix: `-rominfo` command line argument to print information about ROM version and tables
- Unix: ExtFS supports resource forks and Finder info (these are kept in hidden `.finf` and `.rsrc` directories)
- Unix `audio_oss_esd.cpp`: AudioStatus is re-set after determining audio device capabilities (actual sample rate is also determined) [Alexander R. Pruss]
- AmigaOS `sys_amiga.cpp`: supports 64-bit device I/O (NSD)
- AmigaOS `sys_amiga.cpp`: device block size is respected on writes

### Removed
- removed Windows sources from the source archive; a version of these that actually compiles and works can be downloaded from Lauri Pesonen's site
- removed the `MemoryDispatch()` replacement routine; the routine in the ROM is now always used; this fixes the MacOS 8 Finder copying bug and also most "unimplemented trap" errors on MacOS bootup; yeah :-)
- AmigaOS: removed support for SAS/C

### Fixed
- fixed many UAE FPU bugs [Lauri Pesonen]
- `extfs.cpp`: fixed bug with `fsResolveWDCB` in `fs_get_wd_info()`
- Unix `audio_oss_esd.cpp`: `silence` in 8-bit mode used wrong fill value (`0` instead of `0x80`) [Alexander R. Pruss]
- Unix `sys_unix.cpp`: device size is read correctly under Linux

## V0.8 (snapshot) - 1999-10-21
### Added
- added external file system to access host OS files from the MacOS; root directory is specified by the `extfs` prefs item
- added patches for NetBSD [Bernd Sieker]
- Unix: added `fbdev` DGA support and SPARC assembly optimizations to UAE core [Gwenole Beauchesne]

### Changed
- `sony.cpp`, `disk.cpp` and `cdrom.cpp`: disk insertions are now checked for by an interrupt routine
- localizable strings are now split into a common and a platform-specific set
- moved most initialization/deinitialization code to `InitAll()` and `ExitAll()` in `main.cpp`
- AmigaOS: now compiles with GCC (Geek Gadgets)
- Unix: compilation of `cpuemu.cpp` is now split in 8 parts
- Unix: volume list in GTK prefs editor is reorderable

### Fixed
- corrected `TimerDateTime()` in `timer_unix.cpp` and `timer_beos.cpp` [Toshimitsu Tanaka]
- AmigaOS: fixed crash when `gtlayout.library` was not present
- AmigaOS `clip_amiga.cpp`: fixed small bug in `CR->LF` translation [Giacomo Magnini]

## V0.7 (release 0.7-2) - 1999-10-06
### Added
- Added `BasiliskII.spec` for making RPMs [with assistance from Roman Niewiarowski]
- Unix `sys_unix.cpp`: implemented `SysAddDiskPrefs()` for Linux (scans `/etc/fstab` to find HFS volumes)
- Unix `sys_unix.cpp`: implemented `Sys(Prevent/Allow)Removal()` [Ryan Cumming]
- Unix `main_unix.cpp`: added call to `gtk_set_locale()` before `gtk_init()` [Toshimitsu Tanaka]

### Changed
- Unix: `basilisk_ii_keycodes` renamed to `keycodes`, default location of keycodes file is now `/usr/local/share/BasiliskII/keycodes`
- Unix: improved the install target, added uninstall target
- Unix `sys_unix.cpp`: `Sys_open()` checks whether an HFS volume is currently mounted under Linux and tries to unmount it
- Unix `video_x.cpp`: `dga` screen preferences now takes width and height as parameters (e.g. `dga/640/480`), like `win` [Pablo Averbuj]
- Unix `prefs_editor_gtk.cpp`: made the window width/height widgets comboboxes providing some useful defaults

### Removed
- Unix/Linux/NetDriver: removed `MAKEDEV`, functionality now included in `Makefile` (`make dev`)

### Fixed
- Unix/Windows: fixed bugs in protocol removal [Lauri Pesonen]
- Unix `video_x.cpp`: Caps Lock behaves as expected [Pablo Averbuj]

## V0.7 (alpha) - 1999-07-25
### Added
- added new prefs item `nosound` to disable sound output
- sound output implemented for BeOS, Linux, Solaris and AmigaOS (requires Sound Manager 3.x)
- dummy `audio_dummy.cpp`: created
- dummy `prefs_dummy.cpp`: created
- dummy `xpram_dummy.cpp`: created
- `macos_util.cpp`: added `FileDiskLayout()`
- Unix: added 64-bit data types (needed by `timer_unix.cpp`)
- Unix: added keyboard translation method using raw keycodes instead of `keysyms` (controlled by `keycodes` and `keycodefile` prefs items) which doesn't depend on the selected keymap
- Unix `sysdeps.h`: added unaligned access functions for SGI MIPSPro compiler [Brian J. Johnson]
- Unix/Irix `unaligned.c`: created [Brian J. Johnson]

### Changed
- with 32-bit clean ROMs, the CPU type is now reported as a 68030 (for those OpenTransport fans :-)
- `uae_cpu`: updated to UAE 0.8.9
- Unix: moved Linux- and FreeBSD-specific files to their respective directories
- Unix: when running as root, Basilisk II tries to assign real-time priorities to some threads
- Unix: calls to `nanosleep()` protected by autoconf define
- Unix `main_unix.cpp`: tick thread replaced by POSIX.4 timer when possible
- Unix `timer_unix.cpp`: uses POSIX.4 timing facilities when possible
- Unix `sysdeps.h`: changed C++ comments to C comments as this file is included by some `*.c` files in `uae_cpu` [Brian J. Johnson]
- Windows: updated sources

### Removed
- `video.cpp`: removed useless `BlankMousePointer` flag
- Unix `video_x.cpp`: all X calls during emulation are now done from the redraw thread which is also active in DGA mode; as a result, `XLockServer()/XUnlockServer()` are no longer necessary

### Fixed
- `uae_cpu/gencpu.c`: fixed bug in CAS2, OpenTransport works now

## V0.6 (alpha) - 1999-07-12
### Added
- Unix `scsi_freebsd.cpp`: added support for FreeBSD 2.x [Orlando Bassotto]
- Unix/FreeBSD `scsidump.cpp`: added [Orlando Bassotto]

### Changed
- Unix: Basilisk II now accepts a `-display <display>` argument to specify the X display to use
- Unix `main_unix.cpp`: `XOpenDisplay()` is now called from `main()`
- Unix `posix_sem.cpp`: updated [Orlando Bassotto]
- Unix `prefs_editor_gtk.cpp`: serial pane recognizes FreeBSD serial/printer devices [Orlando Bassotto]
- Unix `serial_unix.cpp`: FreeBSD printer devices are recognized [Orlando Bassotto]

### Fixed
- Unix: mounting floppies with `Ctrl-F1` now works
- Unix `ether_linux.cpp`: now works when `ethertap` is configured for multicast support
- Unix `sys_unix.cpp`: FreeBSD CD-ROM/floppy support, correct default serial prefs for FreeBSD [Orlando Bassotto]
- Unix `sys_unix.cpp`: fixed `SysIsDiskInserted` for Linux floppy

## V0.6 (alpha) - 1999-07-05
### Added
- Unix `ether_linux.cpp`: added `ethertap` support (see README for details)
- dummy `clip_dummy.cpp`: created

### Fixed
- fixed mouse for Classic ROM and MacOS 6.x/7.x

### V0.6 (alpha) - 1999-07-02
### Added
- `rom_patches.cpp`: added `CheckROM()` which has to be called before `PatchROM()`
- `rom_patches.cpp`: started ROM patches for Classic/SE ROMs
- `uae_cpu`, `cpu_emulation.h`: added `FLAYOUT_NONE` to disable frame buffer mapping
- `uae_cpu`, `memory.cpp`: added support for Mac Plus/II memory maps and 24-bit addressing
- `main_*.cpp`: added TwentyFourBitAddressing flag
- Unix: support for FreeBSD [Orlando Bassotto]
- Unix `scsi_freebsd-cam.cpp`: created [Orlando Bassotto]
- Unix `posix_sem.cpp`: created [Orlando Bassotto]
- Unix `main_unix.cpp`: added call to `XF86DGAForkApp()`
- Unix `video_x.cpp`: support for Classic video mode and display on 1-bit screens
- Unix `video_x.cpp`: added check whether screen is really DGA capable
- Unix `video_x.cpp`: added check for image endianess
- Unix `sys_unix.cpp`: added replacement for Linux `llseek()` for platforms that don't have it

### Changed
- `adb.cpp`: ADB handlers get ADBBase passed in `a3`
- `adb.cp`p: moved temporary data area to `ADBBase+0x163` (Mac Classic mouse handler wants it there)
- `uae_cpu`, `basilisk_glue.cpp`: `RAMBaseMac`/`ROMBaseMac` are now set in `Init680x0()` (instead of `main_*.cpp`)
- `uae_cpu`, `newcpu.cpp`: re-implemented support for 68000/010 emulation
- `uae_cpu`, `cpuopti.c`: patched for egcs [Orlando Bassotto]
- `main_*.cpp`: CPU type and addressing mode depends on ROM version
- `main_*.cpp`: tick thread changed to use new `TimerDateTime()` function
- include `video.h`: `VideoInit()` takes `classic` parameter to select "Classic" video mode (512x342, 1-bit)
- include `timer.h`: new function `TimerDateTime()` to get local date/time in Mac format
- Unix `*.cpp`: replaced `sleep()` and `usleep()` by `nanosleep()`

### Fixed
- `emul_op.cpp`, `EMUL_OP_CLKNOMEM`: reading from the RTC time registers yields the correct time and date
- Amiga `prefs_editor_amiga.cpp`: ROM file selection via file requester didn't work

## V0.6 (alpha) - 1999-06-22
### Changed
- BeOS `prefs_editor_beos.cpp`: it is now possible to drop ROM files on the "ROM File" control
- BeOS `sys_beos.cpp`: Basilisk II will not try to unmount volumes (from BeOS) that are to be mounted read-only

### Fixed
- fixed small compilation problems on BeOS/x86
- `rsrc_patches.cpp`: fixed for MacOS 7.6.1
- `dummy/scsi_dummy.cpp`: `scsi_send_cmd()` was missing timeout parameter
- Unix `video_x.cpp`: fixed compilation problem on systems without DGA

## V0.6 (alpha) - 1999-06-20
### Added
- `scsi.cpp`: added `timeout` parameter to `scsi_send_cmd()`
- Unix `scsi_linux.cpp`: implemented
- Unix `video_x.cpp`: implemented `VideoQuitFullScreen()` (used by `ErrorAlert()`)
- BeOS `video_beos.cpp`: implemented `VideoQuitFullScreen()` (used by `ErrorAlert()`)

### Changed
- Unix `video_x.cpp`: new hotkeys:  
    Ctrl-Esc = emergency quit from DGA mode,
    Ctrl-Tab = suspend DGA mode

### Fixed
- `scsi_*.cpp`: fixed S/G table processing bug in `scsi_send_cmd()` (every entry was written to/read from the same buffer address)

## V0.6 (alpha) - 1999-06-19
### Added
- `user_strings.cpp`: added strings for GTK preferences editor
- Unix: implemented preferences editor (requires GTK)
- Unix: added `install` target to `Makefile`
- Unix: created minimal man page
- BeOS `prefs_editor_beos.cpp`: added "Create" button to volume list
- BeOS `prefs_editor_beos.cpp`: "About" dialog added

### Changed
- `sys_*.cpp`: 400K DiskCopy disk images are now recognized (84 byte header)
- Unix `main_unix.cpp`: `ErrorAlert()` and `WarningAlert()` pop-up dialog boxes when GTK is present

## V0.5 (alpha) - 1999-06-13
### Added
- `prefs.cpp`: added `ZapPRAM()`
- `user_strings.cpp`: added strings for preferences editor
- `prefs_editor_*.*`: created
- AmigaOS: implemented preferences editor
- BeOS: implemented preferences editor
- AmigaOS `sys_amiga.cpp`: added checks for NSD and 64-bit TD commands (not yet used)
- AmigaOS `sys_amiga.cpp`: implemented CD audio commands
- Unix `video_x.cpp`: added incremental refresh mode [David Lawrence]

### Changed
- `prefs.cpp`: `PrefsReplaceString()` can now take an index
- `sys.h`: `SysCDStop()` now takes the MSF address of the lead-out track as a parameter
- AmigaOS: start offset and size of devices is now given in blocks, not bytes
- Unix `sys_unix.cpp`: CD-ROM devices are opened with `O_NONBLOCK` flag
- Unix `sys_unix.cpp`: using `CDROM_DRIVE_STATUS` to check for inserted disks if drive supports it
- Unix `sys_unix.cpp`: CD-ROM devices are closed and reopened on eject to clear the driver's internal buffers
- Unix `video_x.cpp`: small changes to the DGA code

## V0.5 (alpha) - 1999-05-17
### Added
- new `fpu` prefs item to enable/disable the (buggy) UAE FPU emulation
- `macos_util.cpp`: new function `HasMacStarted()` to check Mac warm start flag
- `macos_util.cpp`: new function `MountVolume()` to aid implementation of manual volume mounting
- `cdrom.cpp`: added `CDROMMountVolume()`

### Changed
- Defaults for platform-specific prefs items can be set in the new `AddPlatformPrefsDefault()` function which gets called by `PrefsInit()`
- added dummy audio component, so MacOS versions >=7.5 should be more stable when running programs that try to do something with sound
- adb.cpp: relative mouse mode should work a bit better
- BeOS: replaced "Mount floppy" menu item by submenu with list of all removable volumes
- Win32: many fixes and improvements [Lauri Pesonen]

## V0.5 (alpha) - 1999-05-08
### Added
- added information about GPL to all source files
- created `dummy` platform directory containing generic "do-nothing" versions of certain files to aid in testing and porting
- Unix: now compiles under Solaris

### Changed
- replaced `off_t" by "loff_t` to allow 64-bit disk access under Linux
- Unix: improved autoconf stuff and `sysdeps.h`
- Unix `video_x.cpp`: automatic SHM detection, fallback to `XPutImage`

### Fixed
- `rom_patches.cpp`: fixed one unaligned access problem (Sparc)

## V0.5 (alpha) - 1999-04-26
### Added

### Changed
- `main_*.cpp`: ramsize is rounded down to 1 MB boundary
- `uae_cpu` `memory.h`: added `REGPARAM2` in more places [Lauri Pesonen]
- `uae_cpu` `memory.cpp`: changed "Diff" variables from `uae_8 *` to `uae_u32` [Lauri Pesonen]
- `uae_cpu` `memory.cpp`: added `REGPARAM2` in more places [Lauri Pesonen]
- `uae_cpu` `newcpu.h`: added `REGPARAM2` in more places [Lauri Pesonen]
- `uae_cpu` `newcpu.cpp`: added `REGPARAM2` in more places [Lauri Pesonen]
- Windows `*`: integrated updated Windows sources from Lauri Pesonen

## V0.5 (alpha) - 1999-04-25
### Added
- Unix `ether_unix.cpp`: implemented
- Unix/Linux/NetDriver: created

### Changed
- `uae_cpu/memory.cpp`: changed `uae_u8 REGPARAM2 *func()` to `uae_u8 *REGPARAM2 func()`
- Unix `video_x.cpp`: added support for 32-bit screens, cleaned up VisualInfo code
- BeOS `ether_beos.cpp`: cleaned up the code
- BeOS `video_beos.cpp`: full screen video mode and color depth can now be set via prefs(`scr/<mode>`)

### Fixed
- `prefs.cpp/PrefsRemoveItem()`: deleting the last item now works

## V0.4 (alpha) - 1999-04-15
### Added
- Added some literature references to the source files
- Unix: added `autoconf` stuff

### Changed
- Modularized CPU engine: the interface of the CPU engine is now defined in `cpu_emulation.h`, so different CPU engines can be substituted more easily [as requested by Bill Huey]

### Fixed
- `uae_cpu/gencpu.c`: `duplicate_carry()` was missing in SUB and NBCD routines
- Unix: modified the makefile so it won't fail if `cpuopti` didn't work [Daniel Thompson]

## V0.4 (alpha) - 1999-04-11
### Changed
- `cdrom.cpp`: `find_hfs_partition()` bailed out too soon when finding no `PN` signature [Lauri Pesonen]
- `prefs.cpp`: prefs file allows more whitespace between keyword and value [Lauri Pesonen]
- `prefs.cpp`: remodeled to allow platform-specific prefs items (list of keywords is kept in table)
- `video.cpp/cscSetEntries`: luminance mapping uses integer instead of floating point math
- `video_*.cpp`: on most systems, the frame buffer is allocated with two additional lines as some ROM versions seem to write behind the frame buffer proper
- BeOS/Unix `sysdeps.h`: defined `X86_ASSEMBLY` on i386/GCC systems
- BeOS/Unix `sysdeps.h`: redefined `get/put_mem()` functions to take and return an `uae_u32` value, added some more x86 optimizations
- Unix `serial_unix.cpp/kSERDBaudRate`: rates between 57600 and 65535 result in a rate of 57600 [Lauri Pesonen]

## V0.4 (alpha) - 1999-04-10
### Added
- new preferences item `rom` to specify the path of the ROM file
- new preferences item `ether` for Ethernet driver
- new `INTFLAG_ETHER` interrupt flag
- `main_*.cpp`: added `ChoiceAlert()`
- `serial_*.cpp`: implemented monitor (`#define MONITOR 1` to activate)
- `serial_defs.h`: added error codes
- AmigaOS `serial_amiga.cpp`: implemented
- AmigaOS `ether_amiga.cpp`: implemented
- BeOS `ether_beos.cpp` implemented
- BeOS `serial_beos.cpp`: implemented `kSERDBaudRate`
- Unix `serial_unix.cpp`: added support for parallel ports

### Changed
- upgraded to UAE 0.8.8 CPU, CPU is now built with `gencpu`, UAE sources moved to `uae` directory
- `serial_*.cpp`: `device_name` is now a `const char *`
- `*.h`: changed `#ifdef _XXX_H_` to `#ifdef XXX_H` as symbols starting with an underscore are reserved
- `rom_patches.cpp`: icon data moved to `sony/disk/cdrom.cpp`
- AmigaOS `sys_amiga.cpp`: `SysAddSerialPrefs()` now returns real defaults
- AmigaOS `main_amiga.cpp`: interrupts are disabled during execution of an `EMUL_OP` routine
- AmigaOS `asm_support.asm`: the `CTRL-C` handler checks for pending AmigaDOS operations
- AmigaOS `asm_support.asm`: the interrupt signal exception is rethrown when the interrupt level in `EmulatedSR` gets lowered while interrupts are pending
- BeOS: switched from project files to Be supplied makefile system
- BeOS `serial_beos.cpp`: modem status of parallel device is returned as `(dsrEvent | dcdEvent | ctsEvent)`
- BeOS `serial_beos.cpp`: `serdtDCE` is set in `PrimeIn()/PrimeOut()`
- Unix `serial_unix.cpp`: `serdtDCE` is set in `PrimeIn()/PrimeOut()`
- Unix `video_x.cpp`: pressing `F12` and q will quit Basilisk II in DGA mode (useful when it crashed)

### Removed
- `main_*.cpp`: disabled UAE FPU (as it is buggy)
- `rom_patches.cpp`: removed unnecessary `a0/a1` save/restore from drivers
- `slot_rom.cpp`: removed unnecessary `a0/a1` save/restore from drivers

### Fixed
- AmigaOS: the Kickstart ROM is no longer used to redirect NIL pointer accesses as this crashes some machines (especially 68040-based); a scratch memory block is used instead
- AmigaOS `main_amiga.cpp`: when the memory for the Mac RAM and ROM cannot be allocated, an error message is displayed (instead of crashing)

## V0.3 (alpha) - 1999-03-14
### Added
- Unix `video_x.cpp`: added 15, 16 and 32-bit window modes [Bill Huey]
- added documentation of preferences file format to README

### Fixed
- BeOS `video_beos.cpp`: `screen` prefs item is now either `win/<width>/<height>` for window mode or `scr` for full-screen mode

## V0.2 (alpha) - 1999-03-11
### Added
- `uae\memory.cpp`: added support for frame buffer pixel type conversion in virtual addressing mode, the Mac frame buffer layout now has to be specified in `MacFrameLayout` (`video.h`)

### Changed
- tested with ROM versions 19 and 26
- `prefs.cpp`: renamed `TYPE_BOOL` to `TYPE_BOOLEAN` beacuse of name clash on BeOS R4.1
- Unix `video_x.cpp`: the `screen` prefs item is now read to select window mode (with a specified window size) or DGA mode
- Unix `video_x.cpp`: DGA mode supports 15, 16 and 32-bit modes (packed 24-bit pixels don't work)
- Unix `Makefile.linux`: added `-D_REENTRANT` to compiler flags

## V0.2 (alpha) - 1999-01-23
### Added
- first alpha release

[Unreleased]: https://github.com/emaculation/BasiliskII/compare/v1.0...HEAD
[v1.0.0]: https://github.com/emaculation/BasiliskII/compare/v0.0.0...v1.0.0
