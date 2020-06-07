#version 450 core
subroutine vec4 LightFunc ( vec3 ); // Step1

subroutine ( LightFunc ) vec4 ambient (vec3 n)
{
	return vec4 ( n, 0.0 );
}

subroutine (LightFunc) vec4 diffuse(vec3 n)  // step 2 (again)
{
	return vec4 ( n, 0.0 );
}

subroutine uniform LightFunc materialShader;


// subroutine of multiple types
subroutine void Type_1();
subroutine void Type_2();
subroutine void Type_3();

subroutine (Type_1, Type_2) void Func_1(){}
subroutine (Type_1, Type_3) void Func_2(){}

subroutine uniform Type_1 func_1;
subroutine uniform Type_2 func_2;
subroutine uniform Type_3 func_3;