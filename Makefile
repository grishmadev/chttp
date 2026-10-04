compile:
	gcc -o build/main src/main.c

clean:
	rm -f build/main

run: compile
	build/main
