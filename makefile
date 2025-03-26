my_echo: my_echo.c my_echo.h
	gcc -c -g  -Wall -m32 -o  my_echo.o my_echo.c
	gcc -g -Wall -m32 -o my_echo my_echo.o

clean: 
	rm my_echo.o my_echo
