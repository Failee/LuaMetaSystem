#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <stddef.h>
#include <cstdint>


//Standard Types

typedef char char8;
typedef unsigned char uchar8;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef float float32;
typedef double float64;


//GL types

/* This code block is duplicated in glxext.h, so must be protected */
#define GLEXT_64_TYPES_DEFINED
/* Define int32_t, int64_t, and uint64_t types for UST/MSC */
/* (as used in the GL_EXT_timer_query extension). */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <inttypes.h>
#elif defined(__sun__) || defined(__digital__)
#include <inttypes.h>
#if defined(__STDC__)
#if defined(__arch64__) || defined(_LP64)
typedef long int int64_t;
typedef unsigned long int uint64_t;
#else
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* __arch64__ */
#endif /* __STDC__ */
#elif defined( __VMS ) || defined(__sgi)
#include <inttypes.h>
#elif defined(__SCO__) || defined(__USLC__)
#include <stdint.h>
#elif defined(__UNIXOS2__) || defined(__SOL64__)
typedef long int int32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;
#elif defined(_WIN32) && defined(__GNUC__)
#include <stdint.h>
#elif defined(_WIN32)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
/* Fallback if nothing above works */
#include <inttypes.h>
#endif

typedef uint64_t GLuint64;
typedef int64_t GLint64;

typedef uint32 GLenum;
typedef unsigned char GLboolean;
typedef uint32 GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned short GLhalf;
typedef uint32 GLuint;
typedef int GLsizei;
typedef float32 GLfloat;
typedef float32 GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef char GLchar;
typedef char GLcharARB;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef struct __GLsync *GLsync;

#define Kilobytes(bytes) (bytes * 1024LL)
#define Megabytes(kBytes) (Kilobytes(kBytes) * 1024LL)
#define Gigabytes(mBytes) (Megabytes(mBytes) * 1024LL)
#define Terabytes(gBytes) (Gigabytes(gBytes) * 1024LL)



#endif //TYPE_INFO_H
