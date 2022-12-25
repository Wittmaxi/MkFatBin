default:
	clang++ ./*.cpp -o mkfatbin -lCMD

run: default
	./mkfatbin -b bootsector.bin -o flop.bin
