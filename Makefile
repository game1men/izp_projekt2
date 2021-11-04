make: setcal.c
	gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal

s: setcal.c
	gcc -std=c99 -Wall -Wextra setcal.c -o setcal
