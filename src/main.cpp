#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "MetaFunction.h"
#include "MetaClassType.h"
#include "MetaUtility.h"
#include "luaRegister.h"
#include "window.h"
#include "gl.h"

#define META_REGISTER_GL_CONSTANT(constant) \
	GLuint g_##constant = constant;\
	MetaVariable g_Meta##constant(#constant, g_##constant);


void error(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	//lua_close(L);
	//exit(EXIT_FAILURE);
}

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

//create a rotation matrix around _x and _y and place in on top of the matrix stack
void createGlRotationMatrix(float32 _x, float32 _y, float32 angle){
	float32 m[] = {
		std::cosf(angle), -std::sinf(angle), 0, 0,
		std::sinf(angle), std::cosf(angle), 0, 0,
		0,0,1,0,
		_x,_y,0,1
	};
	glLoadMatrixf(m);
}

META_REGISTER_GL_CONSTANT(GL_COLOR_BUFFER_BIT);
META_REGISTER_GL_CONSTANT(GL_DEPTH_BUFFER_BIT);
META_REGISTER_GL_CONSTANT(GL_TRIANGLE_STRIP);
META_REGISTER_GL_CONSTANT(GL_QUADS);
META_REGISTER_GL_CONSTANT(GL_MODELVIEW);
META_REGISTER_GL_CONSTANT(GL_PROJECTION);


META_REGISTER_FUN(glMatrixMode, "glMatrixMode");
META_REGISTER_FUN(glLoadIdentity, "glLoadIdentity");
META_REGISTER_FUN(glBegin, "glBegin");
META_REGISTER_FUN(glEnd, "glEnd");
META_REGISTER_FUN(glClear, "glClear");
META_REGISTER_FUN(glClearColor, "glClearColor");
META_REGISTER_FUN(glColor3f, "glColor3f");
META_REGISTER_FUN(glVertex2f, "glVertex2f");

META_REGISTER_FUN(createGlRotationMatrix, "glRotationf");


int main(int argc, char* argv[]){
	Window wnd = Window("Lua extension demo", 1024, 768, 40, 40, DEBUG_CONTEXT | MULTISAMPLE16 | RGB24 | ALPHA8 | DEPTH16 | DOUBLE_BUFFER);

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	MetaBind(L);

	glEnable(GL_DEPTH_TEST);
/*	MyClass mc;
	mc.i = 42;
	mc.pi = 3.1415926f;
	mc.str = "Hello World";*/
	const char* i = "42.112312";
	Variable intPtr(i);

	std::cout << intPtr.m_type->toString(intPtr.m_address) << std::endl;

	std::cout<<getMetaTypeByType<int32**>()->sizeOf() << std::endl;
	std::cout << getMetaTypeByType<int32*>()->AddressType()->name() << std::endl;
	float64 startTime = wnd.elapsedTime();
	int32 framecount = 0;
	do{
		if (luaL_dofile(L, "example1.lua"))
			error(L, "cannot run configuration file: %s",
			lua_tostring(L, -1));

		wnd.swapBuffers();

		float64 currentTime = wnd.elapsedTime();
		framecount++;
		if (currentTime - startTime >= 1){
			float64 fps = (float64)framecount / (currentTime - startTime);
			framecount = 0;
			std::cout << "Rendering at ~ " << fps << " FPS or " << 1000.0 / fps << "ms per frame" << std::endl;
			startTime = currentTime;
		}

	} while (wnd.processAllMessages() && wnd.getKey('c') != InputHandler::ButtonState::PRESSED);
	lua_close(L);
	return 0;
}
