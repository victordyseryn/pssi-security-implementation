clean:
	rm -rf build

a2: clean
	mkdir build
	gcc src/pssi-lemma.c src/pssi-sample.c src/pssi-attack.c src/main-attack.c -DA2 -O2 -Iexternals/rbc-lib/bin/include -Lexternals/rbc-lib/bin -l:librbc.a -o build/main

a3: clean
	mkdir build
	gcc src/pssi-lemma.c src/pssi-sample.c src/pssi-attack.c src/main-attack.c -DA3 -O2 -Iexternals/rbc-lib/bin/include -Lexternals/rbc-lib/bin -l:librbc.a -o build/main

a4: clean
	mkdir build
	gcc src/pssi-lemma.c src/pssi-sample.c src/pssi-attack.c src/main-attack.c -DA4 -O2 -Iexternals/rbc-lib/bin/include -Lexternals/rbc-lib/bin -l:librbc.a -o build/main

b1: clean
	mkdir build
	gcc src/pssi-lemma.c src/pssi-sample.c src/pssi-attack.c src/main-lemma.c -DB1 -O2 -Iexternals/rbc-lib/bin/include -Lexternals/rbc-lib/bin -l:librbc.a -o build/main

b2: clean
	mkdir build
	gcc src/pssi-lemma.c src/pssi-sample.c src/pssi-attack.c src/main-lemma.c -DB2 -O2 -Iexternals/rbc-lib/bin/include -Lexternals/rbc-lib/bin -l:librbc.a -o build/main