
# Alpacker [![MIT License](https://img.shields.io/apm/l/atomic-design-ui.svg?)](https://github.com/tterb/atomic-design-ui/blob/master/LICENSE)

Alpacker is an executable packer for linux elf binaries. It's purpose is to deter or
inconvenience would be reverse engineers. 


## Features

- Packed program is encrypted with a stream cipher
- Randomized jump instructions
- Random key generation
- Junk code insertion at compile-time
- x64_86 support
## Usage

```bash
python3 src/packer/packer program.bin output.bin
```

  
