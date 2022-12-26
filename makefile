default:
	clang++ ./*.cpp -o mkfatbin -lCMD -std=c++17

run: default
	./mkfatbin -b bootsector.bin -o flop.bin -d fatdir
