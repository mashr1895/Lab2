myshell: myshell.c LineParser.c
	gcc -Wall myshell.c LineParser.c -o myshell

mypipe: mypipe.c
	gcc -Wall mypipe.c -o mypipe
