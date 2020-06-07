#version 330 core

void func_1() {  }
void func_2() {  }

uniform int func;

void
main () 
{
	if (func == 1)
		func_1();
	else 
		func_2();
}