# PowerPC Emulator Testsuite

The PowerPC Emulator Tester is a self-contained testsuite I wrote for the SheepShaver emulation engine. It was a very handy tool during the development of the SheepShaver PowerPC Emulator, and considerably reduced debugging time when AltiVec emulation was added. I hope this would be useful to other PowerPC Emulator authors. The code is distributed under the terms of the [GNU General Public License (GPL)](https://www.gnu.org/copyleft/gpl.html).

It's not terrific code and can largely be improved and cleaned up, but it provides reasonable information to chase down a bug.

See links for other testsuites. As of **2006-07-04**, the current SheepShaver CVS branch contains a more accurate FPU emulation engine. It passes VEX tests with more precision & exception bits. All the following glibc tests pass on PPC: `test-fenv`, `test-arith{,f}`, `test-{float,double}`. Almost all of them pass on x86-64 too (modulo a few errors in `test-float`), provided that GCC >= 4.0.1 is used. The more accurate engine is enabled with `PPC_ENABLE_FPU_EXCEPTIONS`.

## Features

- Self-contained. No guest OS nor any complicated run-time environment is required. Your CPU emulator simply needs to support a special opcode (`0x18000000`, `OP=6`) that terminates execution of the current basic block.
- Supports several CPU cores:
  - SheepShaver
  - [Microlib PPC 750 Simulator](https://web.archive.org/web/20070626045323/http://microlib.org/projects/ppc750sim/)
  - [Sega Model 3 Arcade Emulator](https://web.archive.org/web/20081007020217/http://model3emu.sourceforge.net/)
  - [QEMU](https://www.qemu.org/)
- Supports the AltiVec ISA.
- Generates more than 2 million tests with specific values to trigger condition codes changes, unspecified results (matches a PowerPC 7410, aka my Powerbook G4).

## Downloads

- The primary source is available in SheepShaver CVS.
- It is recommended that you first run the tester on a native PowerPC platform to generate a results file. For reference, here is the [[this>projects/ppctester/files/ppc-testresults.dat.bz2|PowerPC Emulator Tester results file]] I use for SheepShaver. This was generated with AltiVec tests enabled. This is for revision 1.30+ of the file. md5sum is: `3e29432abb6e21e625a2eef8cf2f0840`. **Caveat:** On 2019-03-02 the tests result file is not available in this GitHub repository. RK

## Links

- [Valgrind](http://www.valgrind.org/) contains an interesting testsuite for the VEX.
- [Qemu-PPC](https://web.archive.org/web/20080212093125/http://perso.magic.fr/l_indien/qemu-ppc/) contains part of the original ALU tests used in VEX.
