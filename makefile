myshell: myshell.c LineParser.c
	gcc myshell.c LineParser.c -o myshell

mypipe: mypipe.c
	gcc mypipe.c -o mypipe