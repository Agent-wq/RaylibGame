all:
	gcc -o main main.c -Wall -std=c99 -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 
