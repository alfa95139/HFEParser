# HFEParser
Very crude HFE parser, trying to make sense of the specs

https://hxc2001.com/download/floppy_drive_emulator/SDCard_HxC_Floppy_Emulator_HFE_file_format.pdf

I tried on a HFE from an Ensoniq Mirage disk image

The reserved area in the HFE format is 1024 bytes: the first structure does not fill the 512 bytes, so there is some padding.
Also, the next structure ("up to 1024bytes") does not require additional memory, it falls withing the first 1024 bytes.
Once I understood this (which i did not think it was obvious), everything else fell nicely in place.
For instance, the offset (2 in my case) shows that my data starts after 1024 bytes - which is correct.
