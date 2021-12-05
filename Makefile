make: setcal.c
	gcc -std=c99 -Wall -Wextra -g setcal.c -o setcal

debug: setcal.c
	gcc -std=c99 setcal.c -g -o -Wall -Wextra setcal

valgrind: setcal
	valgrind ./setcal test.txt

run: setcal
	 ./setcal test.txt

s: setcal.c
	gcc -std=c99 -Wall -Wextra setcal.c -o setcal


