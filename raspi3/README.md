# CuteHSP for Raspberry Pi 3

こちらはRaspberry Pi 3 (Jessie) 版の実行ファイルです。同じサンプルプログラムを動かせます。

## コンパイル方法のメモ
<pre>
- コンソール版: gcc cutehsp.c -o cutehspcl -lm -Os -std=c11

-   ミニマム版: gcc cutehsp.c -o cutehsp  -std=gnu99 -lglfw -lrt -lm -lXrandr -lXrender -lXi -lGL -lm -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp          -Os

- エクストラ版: gcc cutehsp.c -o cutehspx -std=gnu99 -lglfw -lrt -lm -lXrandr -lXrender -lXi -lGL -lm -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -lopenal -Os
</pre>
