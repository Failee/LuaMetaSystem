#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lua\MetaFunction.h"
#include "lua\MetaClassType.h"
#include "lua\MetaUtility.h"
#include "lua\luaRegister.h"



void error(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

class MyMemberClass{

public:
	blues::string m_name = "MyMemberClass";
};

BEGIN_META_CLASS_DEFINITION(MyMemberClass);
ADD_ATTRIBUTE("name", &MyMemberClass::m_name);
END_META_CLASS_DEFINITION(MyMemberClass);

class MyClass{
public: 
	MyClass(){
		i = 5;
		pi = 5.4f;
		str = "300";
		mmc.m_name = "who gives a shit?\n";
	}
	void Hi(int a, float32 b, MyMemberClass str){
		std::cout << a << "\t" << b << "\t" << str.m_name << std::endl;
	}
	int Servus(){
		std::cout << "Servus!\n";
		return 42;
	}
	int i;
	float32 pi;
	blues::string str;
	MyMemberClass mmc;
};

BEGIN_META_CLASS_DEFINITION(MyClass);
ADD_MEMBER_FUNCTION("Hi", &MyClass::Hi);
ADD_MEMBER_FUNCTION("Servus", &MyClass::Servus);
ADD_ATTRIBUTE("i", &MyClass::i);
ADD_ATTRIBUTE("pi", &MyClass::pi);
ADD_ATTRIBUTE("str", &MyClass::str);
ADD_ATTRIBUTE("mmc", &MyClass::mmc);
END_META_CLASS_DEFINITION(MyClass);

template <typename type>
void luaSetGlobal(lua_State *L, const type& _t, const char* _name){
	getMetaTypeByType<type>()->luaSet(L, (void*)&_t);
	lua_setglobal(L, _name);
}

template <typename type>
void luaGetGlobal(lua_State *L, type& _t, const char* _name){
	lua_getglobal(L, _name);
	getMetaTypeByType<type>()->luaGet(L, -1, (void*)&_t);
}

int main(int argc, char* argv[]){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	MetaBind(L);
	/*MyClass mc;
	mc.i = 42;
	mc.pi = 3.1415926f;
	mc.str = "Hello World";
	int32 i = 42;
	I32* iptr = &i;
	Variable intPtr(&i);

	std::cout << intPtr.m_type->toString(intPtr.m_address) << std::endl;
	std::cout << Cast<UI32>(intPtr) << std::endl;

	std::cout<<getMetaTypeByType<int**>()->sizeOf() << std::endl;
	std::cout << getMetaTypeByType<int*>()->AddressType()->name() << std::endl;
	if (!glfwInit()){
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}
	
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *window = glfwCreateWindow(1024, 768, "luaTest", nullptr, nullptr);
	if (!window){
		std::cout << "Failed to open GLFW Window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do{
		if (luaL_dofile(L, "VectorTest.lua"))
			error(L, "cannot run configuration file: %s",
			lua_tostring(L, -1));

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	*/
	if (luaL_dofile(L, "VectorTest.lua"))
		error(L, "cannot run configuration file: %s",
		lua_tostring(L, -1));
	
	lua_close(L);
	return 0;
}