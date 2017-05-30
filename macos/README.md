# CuteHSP for macOS

こちらはmacOS版の実行ファイルです。同じサンプルプログラムを動かせます。

## コンパイル方法のメモ
<pre>
コンソール版
clang cutehsp.c -o cutehspcl -Os

ミニマム版
clang cutehsp.c -o cutehsp  -lglfw3 -framework OpenGL -framework
      Cocoa -framework IOKit -framework CoreVideo -Os

エクストラ版
clang cutehsp.c -o cutehspx -lglfw3 -framework OpenGL -framework
      Cocoa -framework IOKit -framework CoreVideo -framework OpenAL -Os
</pre>
