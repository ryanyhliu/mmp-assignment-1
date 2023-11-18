# make ci && ./ci.exe 9_coords.coord cout_9_output.dat 
# make ci && ./ci.exe 16_coords.coord cout_16_output.dat
# make ci && ./ci.exe 4096_coords.coord cout_4096_output.dat
ci: cInsertion.c coordReader.c
	gcc -g -std=c99 cInsertion.c coordReader.c -o ci.exe -lm
	

# make fi && ./fi.exe 9_coords.coord fout_9_output.dat
# make fi && ./fi.exe 16_coords.coord fout_16_output.dat
# make fi && ./fi.exe 4096_coords.coord fout_4096_output.dat
fi: fInsertion.c coordReader.c
	gcc -g -std=c99 fInsertion.c coordReader.c -o fi.exe -lm

# sbatch -c 1 OpenMP_batch.sh comp.exe
# sbatch -c 2 OpenMP_batch.sh comp.exe
# sbatch -c 4 OpenMP_batch.sh comp.exe
# sbatch -c 8 OpenMP_batch.sh comp.exe
# sbatch -c 16 OpenMP_batch.sh comp.exe
# sbatch -c 32 OpenMP_batch.sh comp.exe
comp: ompcInsertion.c coordReader.c
	gcc -fopenmp -std=c99 ompcInsertion.c coordReader.c -o comp.exe -lm

# sbatch -c 1 OpenMP_batch.sh fomp.exe
# sbatch -c 2 OpenMP_batch.sh fomp.exe
# sbatch -c 4 OpenMP_batch.sh fomp.exe
# sbatch -c 8 OpenMP_batch.sh fomp.exe
# sbatch -c 16 OpenMP_batch.sh fomp.exe
# sbatch -c 32 OpenMP_batch.sh fomp.exe
fomp: ompfInsertion.c coordReader.c
	gcc -fopenmp -std=c99 ompfInsertion.c coordReader.c -o fomp.exe -lm

# sbatch -c 1 OpenMP_batch.sh icomp.exe
# sbatch -c 2 OpenMP_batch.sh icomp.exe
# sbatch -c 4 OpenMP_batch.sh icomp.exe
# sbatch -c 8 OpenMP_batch.sh icomp.exe
# sbatch -c 16 OpenMP_batch.sh icomp.exe
# sbatch -c 32 OpenMP_batch.sh icomp.exe
icomp: ompcInsertion.c coordReader.c
	icc -qopenmp -std=c99 ompcInsertion.c coordReader.c -o icomp.exe -lm

# sbatch -c 1 OpenMP_batch.sh ifomp.exe
# sbatch -c 2 OpenMP_batch.sh ifomp.exe
# sbatch -c 4 OpenMP_batch.sh ifomp.exe
# sbatch -c 8 OpenMP_batch.sh ifomp.exe
# sbatch -c 16 OpenMP_batch.sh ifomp.exe
# sbatch -c 32 OpenMP_batch.sh ifomp.exe
ifomp: ompfInsertion.c coordReader.c
	icc -qopenmp -std=c99 ompfInsertion.c coordReader.c -o ifomp.exe -lm