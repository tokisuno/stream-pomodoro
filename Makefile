main:
	gcc main.c -lraylib
clean:
	rm -rf a.out
run:
	./a.out
windows:
	gcc -o pomo.exe main.c -lraylib -lgdi32 -lwinmm

