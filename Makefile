make: setcal.c
	gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal

easy: setcal.c
	gcc -std=c99 setcal.c -g -o setcal

s: setcal.c
	gcc -std=c99 -Wall -Wextra setcal.c -o setcal
check:
	echo "test"
