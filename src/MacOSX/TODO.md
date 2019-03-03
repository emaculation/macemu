# To Do
## Bugs
- In window mode, if the framerate is low (e.g. if you leave it at the default of 10fps) or if the emulated screen is too large, really fast mouse clicks are sometimes not picked up by the Emulator. For now, click more slowly.
- In full screen mode after a restart, when the mouse is first moved, the emulated pointer jumps to the location that it was before the restart. Disturbing, but not damaging.
- Ejecting a CD only works in 10.2 or higher, and it freezes the emulation for about 5 seconds.
- Status of `dd` command is not always correct. (If it runs out of space, an error about file not found is printed?)
- The `Snapshot` function is currently broken in some situations (if the emulation changes its own screen settings, and the program is compiled with the default window drawing mode of `CGIMAGEREF`). Setting the depth to millions (in the emulator) is a workaround for now.
- Cut and paste between emulator and OS X only half works.

## Untested
- Mac Classic emulation. I don't have a ROM, but I suspect it will crash.
- Serial port code.

## Unimplemented
- CD audio stuff. I am still trying to get this to work.
- CD insert detection. At the moment, if a CD isn't there when the emulator boots, or if you change CDs, it will never know. I don't know how to register with the OS for disk insertion and mount events. (Gwenolé rewrote the CD code, and it should poll for new disks, but it don't work for me. I must be doing something wrong!)
- Floppy stuff. If a floppy is mounted by the OS X Finder, it is busy and cannot be opened by the emulator
- Interrupt function for emulator.
- 'nogui' to disable GUI alerts (and maybe preferences, but I need to split `MainMenu.nib` to do that).

## Possible Enhancements
- Use NSFileManager's movePath:toPath:handler to rename all a file's forks in `extfs_macosx.mm`.
- Emulator snapshot - save the current emulator state (memory + registers) to a file for fast startup next time.
- Multiple emulators. The window stuff is mostly there, but the uae_cpu code has lots of globals, and is not re-entrant
- Real addressing mode for the emulator. Mike Sliczniak had done most of the work, but with real addressing the emulator just crashes for me.
- Improve Objective-C object allocation (e.g. http://www.mulle-kybernetik.com/artikel/Optimisation/opti-5.html).
- Use `automake` instead of the current `1_prepare_files.sh`?
- Add JIT options to preferences?
- Use internal windows to display Help doco?
- Provide feedback during external commands (e.g. ejecting a CD or creating a disk volume).
- Widescreen window layout (suggestion by Michael Franz), so that users with widescreen displays can squeeze a bigger emulated screen in. I have a mock up of this that can be pasted in `MainMenu.nib`. Ideally, this would be via a generalised "theme" facility, but who has time for that :-)
