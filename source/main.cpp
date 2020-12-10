#include <core/system.hpp>
#include <compiler/compiler.hpp>
#include <compiler/linker.hpp>


// Initialize program
void init()
{
	compile("main.pp", ".code init: cpy reg0, 14 dec reg0 ret");
	link("main.pp");
}