#TinyHSP for Linux

こちらはLinux版の実行ファイルです。同じサンプルスクリプトが動かせます。

##実行画面
<img src="https://cloud.githubusercontent.com/assets/24917310/23199814/193572da-f914-11e6-9665-ed890d633750.png" width="45%" height="45%" alt="tinyhsp_linux">
<img src="https://cloud.githubusercontent.com/assets/24917310/23199813/1932d106-f914-11e6-8fa5-33a6a8674876.png" width="45%" height="45%" alt="tinyhspx_linux">

##コンパイル方法のメモ
<pre>
g++ tinyhsp.cpp -o tinyhsp
  -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

g++ tinyhspx.cpp -o tinyhspx
  -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

g++ -static tinyhspcl.c -o tinyhspcl -std=c++11
</pre>
