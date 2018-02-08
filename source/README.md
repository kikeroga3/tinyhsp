【補足】
本ソース(cutehsp.c)のCuteHSPコンソール版(__HSPCUI__)にあたる箇所は
下記のC99準拠のライブラリのみを使用して組まれているので、
同規格準拠のC言語環境であればそのままコンパイル・ビルドが可能です。

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

CuteHSPミニマム版(__HSPSTD__)はGLFW3を利用できるプラットフォームに限定され、
さらにCuteHSPエクストラ版(__HSPEXT__)はGLFW3、OpenALを利用できるプラットフォームに限定されます。
