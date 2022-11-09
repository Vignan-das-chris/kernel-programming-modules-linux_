ALL := usr_cpu_aff usr_cpu_aff_dbg
all: ${ALL}
usr_cpu_aff: usr_cpu_aff.c  
	gcc -Wall -Os usr_cpu_aff.c -o usr_cpu_aff
usr_cpu_aff_dbg: usr_cpu_aff.c  
	gcc -g -ggdb -Wall -O0 usr_cpu_aff.c -o usr_cpu_aff_dbg
clean:
	rm -fv ${ALL}

