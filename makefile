default:
	clang++ ./*.cpp -o mkfatbin -lCMD

run: default
	./mkfatbin
