.PHONY: all

stub:
	gcc -g -o src/stub/stub.bin src/stub/stub.c

packer:
	gcc -o src/packer/packer.bin src/packer/packer.c

all: stub packer
