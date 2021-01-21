# Archive2-CPP
C++ version of my Archive2 tool; Archive2-CPP, supports creating GNMF BA2's

**Supported Platforms:**
---------------------------------------------------------------------
* Windows
* Mac OS X(beta)
* Linux(Soon*:tm:)

**Usage:**
---------------------------------------------------------------------
create a folder called "data", put textures and run Archive2-CPP.exe, either do it via CMD or double click it and let it do its magic, if should output a file called "COutput - Textures.ba2" if none is given, rename it to <espname - Textures.ba2>, and now you have a PS4 GNMF BA2

**Valid Arguments**:

NONE ATM, ill add these later:

"-nc" && "-nst" && "-h" && "--help"

"-nc" enables no compression mode

"-nst" enables No String Table mode

**Credits:**
---------------------------------------------------------------------
- Kernel Panic | Backporter

- [zlibnet](https://www.zlib.net/)

https://web.mit.edu/freebsd/head/sys/libkern/crc32.c - CRC32 table

**Notes:**
---------------------------------------------------------------------
* Make sure the dds are converted to GNF, this tool will not detect the texture format and it assume its already GNF

**Building in Visual Studio:**
---------------------------------------------------------------------
1: install zlib via vcpkg (vcpkg install zlib:x64-windows)

2: download src and load it in visual studio(2017)

3: hit build

4: prophet!?!?
