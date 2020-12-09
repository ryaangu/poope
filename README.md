## 💩 What is poope?
**poope** is basically an assembler, but **VERY** different!

## ⌨ Example Code
```
.data

	; Define a byte variable called hello_world
	byte hello_world: "Hello, World!"

.code

	; Print 'Hello, World!' to console
	cpy reg0, hello_world
	call print

	; Success (Exit Code: 0)
	cpy reg, 0
	ret
```

## 🚀 TODO List!
- [ ] sections
- [x] labels
- [x] copy register, value
- [x] copy register, register
- [x] jump label
- [x] return

- [ ] entry point

## 📝 NOTES
- **ALWAYS** optimize the source code.
- **ALWAYS** try to comment everything.

## ✍ LICENSE
Please refer to [**LICENSE**](https://github.com/ryaangu/poope/blob/main/LICENSE).