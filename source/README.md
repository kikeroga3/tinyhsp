### Build method

Comment in (To enable) the version you want to build on the first three lines of "cutehsp.c" before compiling.

<pre>
#define __HSPCUI__
//#define __HSPSTD__
//#define __HSPEXT__
</pre>

The part corresponding to CuteHSP console version (__HSPCUI__) of this source (cutehsp.c)
Since it is organized using only the following C99 compliant library,
Compile and build as it is in C language environment compliant with this standard.

<pre>
assert.h
stdarg.h
stdbool.h
stddef.h
stdio.h
stdint.h
stdlib.h
string.h
time.h
math.h
</pre>

The CuteHSP minimum version (__HSPSTD__) is limited to platforms that can use GLFW3,
In addition, the CuteHSP extra version (__HSPEXT__) is limited to platforms that can use GLFW3, OpenAL.
