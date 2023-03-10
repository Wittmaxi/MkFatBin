# MkFatBin

Utility that I wrote as part of my OSDev-Endeavours. This is both a learning project and a utility that I will use to pack my kernel.

### usage

- create a folder with the files that should be mapped to the FS. Specify it with `-d <directory>`
- (optional) create a bootsector - if you want it to be flagged as bootable you need to set it's two upmost bytes to "0xAA 0x55". Specify the file with `-d <file.bin>`
- define a name for an output file, Defaults to "out.bin". Specify with "-o <file.bin>"

Run `mkfatbin -h` for extensive help.

### Installation

In order to install the tool, run

```
make install
```

Installation requires clang++ with support for c++17r.

### Some quirks

Not exhaustive - can also be seen as a ToDo list. 

- no support for long names, all names will be truncated to 8 + 3 length.
- no support for folders. All files will be dumped to the root.
- Makes some assumptions about the floppy config.
- only supports FAT16.

### On code quality

This project was something I needed to do in order to continue writing my kernel. With it being a roadblock, I tried to "get it over with".
Hence, the tool makes some assumptions that might not work for all contexts. Additionally, it is full of "TODO"-comments.

Please PR and contribute, I would be happy to collaborate!
My active e-mail is maximilian.wittmer@gmx.de
