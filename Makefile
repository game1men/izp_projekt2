make: setcal.c
	gcc -std=c99 -Wall -Wextra -g setcal.c -o setcal

valgrind: setcal
	valgrind ./setcal test.txt

run: setcal
	 ./setcal test.txt

s: setcal.c
	gcc -std=c99 -Wall -Wextra setcal.c -o setcal


