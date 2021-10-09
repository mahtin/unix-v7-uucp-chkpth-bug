

all:	chkpth-test

chkpth-test: chkpth-test.o

chkpth-test.o: chkpth-test.c

clean:
	rm -f chkpth-test chkpth-test.o

run:	chkpth-test test-chkpth-input.txt
	./chkpth-test < test-chkpth-input.txt

