#include <core/system.hpp>
#include <compiler/compiler.hpp>
#include <compiler/linker.hpp>


// Initialize program
void init()
{
	compile("test.pp", ".code  ok: cpy reg0, 97 ret test: cpy reg0, 10238 ret jmp ok cpy reg0, 281 jmp test");
	link("test.pp");
}