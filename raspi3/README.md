# CuteHSP for Raspberry Pi 3

![rpi3_cutehsp](https://user-images.githubusercontent.com/24917310/27611683-2a97e5d6-5bce-11e7-8d1d-c69951b15a6e.png)

![rpi3_cutehspcl_run](https://user-images.githubusercontent.com/24917310/27611603-c99f098a-5bcd-11e7-98e0-fb44762dac03.png)

This is the executable file of Raspberry Pi 3 (Jessie) version.

* Installation of GLFW3 is necessary.

<pre>
sudo apt-get install libglfw3-dev
</pre>

![threedomingos_001](https://user-images.githubusercontent.com/24917310/27260537-8510170c-5469-11e7-8dbd-9ada087ccb99.png)

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
