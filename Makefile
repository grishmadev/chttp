compile:
	gcc -o main main.c

clean:
	rm -f main

run: compile
	./main
