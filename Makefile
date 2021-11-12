make: setcal.c
	gcc -std=c99 -Wall -Wextra -g -Werror setcal.c -o setcal

debug: setcal.c
	gcc -std=c99 setcal.c -g -o -Wall -Wextra setcal

valgrind: setcal
	valgrind ./setcal

s: setcal.c
	gcc -std=c99 -Wall -Wextra setcal.c -o setcal
check: setcal
	(cd ../tests-izp-projekt-2 && python3 test.py ../izp_projekt2/setcal)


