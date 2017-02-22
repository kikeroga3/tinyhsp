#TinyHSP for Linux

##コンパイル方法のメモ
g++ tinyhsp.cpp  -o tinyhsp -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

g++ tinyhspx.cpp  -o tinyhspx -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl
