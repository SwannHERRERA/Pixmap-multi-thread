CC=gcc
CFLAGS= -Wall -Wextra
dev: build start

build:
	$(CC) $(CFLAGS) -o executable.out main.c ppm_image.c -pthread
start:
	./executable.out dark.ppm
