import struct
from pwnlib import elf
from pwn import disasm
from ctypes import CDLL
import random
import sys

def encrypt_buffer(buffer, key):
	libc = CDLL("libc.so.6")
	libc.srand(key)
	for i in range(0, len(buffer)):
		buffer[i] ^= libc.random() % 256
	return buffer

def update_stub(buffer, key):
	key_pattern = struct.pack("<I", 0x6b657931)
	payload_offset_pattern = struct.pack("<I", 0x704f6666)
	packed_key = struct.pack("<I", key)
	buffer = buffer.replace(key_pattern, packed_key)
	buffer = buffer.replace(payload_offset_pattern, struct.pack("<I", len(buffer)))
	return buffer

def main():
	stub_path = "../stub/stub.bin"
	key = random.randint(0, 2147483647)
	
	stub_fp = open(stub_path, "rb")
	stub_bytes = bytearray(stub_fp.read())
	stub_fp.close()
	stub_bytes = update_stub(stub_bytes, key)

	payload_fp = open(sys.argv[1], "rb")
	payload_bytes = bytearray(payload_fp.read())
	payload_fp.close()
	payload_bytes = encrypt_buffer(payload_bytes, key)
	
	stub_bytes += payload_bytes
	output_fp = open(sys.argv[2], "wb")
	output_fp.write(stub_bytes)
	output_fp.close()

if __name__ == "__main__":
	main()