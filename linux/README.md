#TinyHSP for Linux

こちらはLinux版の実行ファイルです。

###コンパイル方法のメモ
<pre>
g++ tinyhsp.cpp  -o tinyhsp -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
g++ tinyhspx.cpp  -o tinyhspx -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
</pre>
