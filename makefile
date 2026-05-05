myshell: myshell.c LineParser.c
	gcc -Wall myshell.c LineParser.c -o myshell

mypipe: mypipe.c
	gcc -Wall mypipe.c -o mypipe

looper: Looper.c
	gcc -Wall Looper.c -o looper

printers: Printers.c
	gcc -Wall Printers.c -o printers
	
clean:
	rm -f myshell mypipe looper  printers
