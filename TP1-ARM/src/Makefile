sim: shell.c sim.c
	gcc -g -O0 $^ -o $@ utils/functions.c utils/opcodes.c

.PHONY: clean
clean:
	rm -rf *.o *~ sim
