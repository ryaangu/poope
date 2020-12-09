#include <core/system.hpp>
#include <compiler/compiler.hpp>
#include <compiler/linker.hpp>


// Initialize program
void init()
{
	compile("test.pp", ".code cpy reg1, 123 cpy reg0, reg1 ret");
	link("test.pp");
}