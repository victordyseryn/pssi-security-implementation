clean:
	rm -rf build

all: clean
	mkdir build
	gcc src/pssi-debug.c src/pssi-sample.c src/pssi-attack.c src/main.c -O2 -Iexternals/rbc-lib/bin/include -Lexternals/rbc-lib/bin -l:librbc.a -o build/main
