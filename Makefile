ci: cInsertion.c coordReader.c
	# make ci && ./ci.exe 9_coords.coord cout_9_output.dat 
	# make ci && ./ci.exe 16_coords.coord cout_16_output.dat
	# make ci && ./ci.exe 4096_coords.coord cout_4096_output.dat
	gcc -g -std=c99 cInsertion.c coordReader.c -o ci.exe -lm
	

fi: fInsertion.c coordReader.c
	gcc -g -std=c99 fInsertion.c coordReader.c -o fi.exe -lm

comp: ompcInsertion.c coordReader.c
	gcc -fopenmp -std=c99 ompcInsertion.c coordReader.c -o comp.exe -lm

fomp: ompfInsertion.c coordReader.c
	gcc -fopenmp -std=c99 ompfInsertion.c coordReader.c -o fomp.exe -lm

icomp: ompcInsertion.c coordReader.c
	icc -qopenmp -std=c99 ompcInsertion.c coordReader.c -o icomp.exe -lm

ifomp: ompfInsertion.c coordReader.c
	icc -qopenmp -std=c99 ompfInsertion.c coordReader.c -o ifomp.exe -lm