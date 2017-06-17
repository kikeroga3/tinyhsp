# CuteHSP for Raspberry Pi 3

This is the executable file of Raspberry Pi 3 (Jessie) version.

## Compile command
<pre>
gcc cutehsp.c -o cutehspcl -lm -Os -std=c11

gcc cutehsp.c -o cutehsp  -std=gnu99 -lglfw -lrt -lm -lXrandr -lXrender -lXi -lGL -lm
    -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm
    -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp          -Os

gcc cutehsp.c -o cutehspx -std=gnu99 -lglfw -lrt -lm -lXrandr -lXrender -lXi -lGL -lm
    -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm
    -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -lopenal -Os
</pre>
