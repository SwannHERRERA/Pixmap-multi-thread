CC=gcc
CFLAGS= -Wall -Wextra
dev: build start
# main.c
build:
	$(CC) $(CFLAGS) -o executable.out main.c ppm_image.c
start:
	./executable.out
