clean:
	rm -rf build

all: clean
	mkdir build
	gcc src/pssi-sample.c src/pssi-attack.c src/main.c -Iinclude -Llib -l:librbc.a -o build/main
