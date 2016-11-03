all: ws1.c
	gcc -o ws1 ws1.c
	gcc -o ws2 ws2.c
clean: ws1
	rm ws1
	rm ws2
