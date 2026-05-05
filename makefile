myshell: myshell.c LineParser.c
	gcc -Wall myshell.c LineParser.c -o myshell

mypipe: mypipe.c
	gcc -Wall mypipe.c -o mypipe

looper: Looper.c
	gcc -Wall Looper.c -o looper

clean:
	rm -f myshell mypipe looper
