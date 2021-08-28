.PHONY: all

stub:
	gcc -S -o src/stub/stub.s src/stub/stub.c && python3 src/transformer/randomizer.py src/stub/stub.s src/transformer/output.s && rm src/stub/stub.s && gcc -o src/stub/stub.bin src/transformer/output.s && rm src/transformer/output.s && strip src/stub/stub.bin && strip --remove-section=.note.* src/stub/stub.bin

all: stub
