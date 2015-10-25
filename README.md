# LuaMetaSystem
Automatic lua POD, function and class bindings. This is an extension of the Model shown by John Edwards of thatgamecompany at DigiPen (https://www.youtube.com/watch?v=hWDZ3Yy-NMA&amp;ab_channel=GameEngineArchitectureClub) for pointer and classes

# Example 1

To compile Example 1 just throw example1.cpp into your favorite win32 compiler and link it against opengl32.lib and the lua static libs. If everything works you should see an image like this:
[example1img]: https://raw.githubusercontent.com/Failee/LuaMetaSystem/master/example/example1.jpg
![][example1img]

Alright! Now why should we bother? Well becaue we can now call arbitrary c/c++ functions from lua at RUNTIME. In projects that get relatively large recompiling the program (or parts of it) for a small change cuts productivity down by a lot. But the lua code is interpreted while the program is running. I "drew" the complete example program output at runtime which would be impossible in c or c++. [Note that OpenGL immediate mode is considered deprecated and shouldn't be used anymore {for performance reasons}. But the immediate mode is a bit nicer to illustrate the concept of this library to people that aren't familiar with rendering APIs]

# Todo

 - do extensive testing of Class Meta Types
 - add const member function support
 - add access to private members
