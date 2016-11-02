all: ws1.c
	gcc -o ws1 ws1.c
	# gnu99???
	#-g -Wall -o fp_parse fp_parse.c -lm
clean: ws1
	rm ws1
