default:
	clang++ ./*.cpp -o mkfatbin -lCMD -std=c++17

run: default # test run
	./mkfatbin -b bootsector.bin -o flop.bin -d fatdir

make install: default
	mv mkfatbin /usr/local/bin/mkfatbin
