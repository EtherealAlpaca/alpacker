import random
import sys

fp = open(sys.argv[1], "r+")
lines = fp.readlines()
fp.close()

original_instr = []
new_instr = []

line_num = 0
prog_started = False
prog_stopped = False
instr_count = 0

chunk_list = []

begin_instr = []
end_instr = []

junk_instr = ["mov %rax, %rax\n",
			  "mov %rbx, %rbx\n",
			  "mov %rcx, %rcx\n",
			  "mov %rdx, %rdx\n",
			  "push %rax\npop %rax\n", 
			  "push %rbx\npop %rbx\n", 
			  "push %rcx\npop %rcx\n", 
			  "push %rdx\npop %rdx\n"]

instr_list_fp = open("src/transformer/instructions.s", "r")
real_instr = instr_list_fp.readlines()
instr_list_fp.close()

for line in lines:
	line_num += 1
	stripped = line.strip()
	if stripped == ".cfi_startproc":
		prog_started = True
		continue
	if prog_started & (stripped != ".cfi_endproc"):
		original_instr.append(line)
		instr_count += 1
	elif stripped == ".cfi_endproc":
		prog_started = False
		prog_stopped = True
	elif not prog_started and not prog_stopped:
		begin_instr.append(line)
	elif prog_stopped:
		end_instr.append(line)

instr_num = 0
for instruction in original_instr:
	new_instr.append(f"lbl{instr_num}:\n")
	new_instr.append(original_instr[instr_num])
	for i in range(0, random.randint(0, 50)):
		new_instr.append(junk_instr[random.randint(0, len(junk_instr) - 1)])
	
	instr_num += 1
	if instr_num < instr_count:
		new_instr.append(f"jmp lbl{instr_num}\n")
	
	for i in range(0, random.randint(0, 50)):
		random_instr = real_instr[random.randint(0, len(real_instr) - 1)]
		if "\n" in random_instr:
			new_instr.append(random_instr)

	chunk_list.append(new_instr[:])
	new_instr.clear()

random.shuffle(chunk_list)
modified_instruction = [".cfi_startproc\n", "jmp lbl0\n"]

for i in range(0, len(chunk_list)):
	for instruction in chunk_list[i]:
		modified_instruction.append(instruction)
modified_instruction.append(".cfi_endproc\n")
modified_instruction = begin_instr + modified_instruction + end_instr

fp = open(sys.argv[2], "w")
fp.writelines(modified_instruction)
fp.close()