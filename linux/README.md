# CuteHSP for Linux

こちらはLinux版の実行ファイルです。同じサンプルプログラムを動かせます。

## 実行画面
<img src="https://cloud.githubusercontent.com/assets/24917310/23199814/193572da-f914-11e6-9665-ed890d633750.png" width="35%" height="35%" alt="tinyhsp_linux"><img src="https://cloud.githubusercontent.com/assets/24917310/23199813/1932d106-f914-11e6-8fa5-33a6a8674876.png" width="35%" height="35%" alt="tinyhspx_linux"><img src="https://cloud.githubusercontent.com/assets/24917310/23306131/5f1a4960-fae5-11e6-88a0-2d58cea2b206.png" width="29%" height="29%" alt="tinyhspcl_linux">

### 補足1：エクストラ版は要OpenAL

「cutehspx」の実行にはLinuxにOpenALライブラリの導入が必要になります。
<pre>
sudo apt-get install libopenal-dev
</pre>
OpenALライブラリが導入できない場合は「extra_noal_version」か「extra_nosound_version」をお試しください。

### 補足2：geditから実行

Linuxでは定番のテキストエディタ「gedit」で「外部コマンド」を有効にすればプログラムをエディタ上で編集しながら実行することができます。

1. 設定画面の「プラグイン」タブで「外部コマンド」にチェックを入れる。
2. 「Manage External Tools...」から「外部ツールの管理」を開き、左下にある「＋」で新規コマンドを追加。
3. 任意のコマンド名(例:CuteHSP実行)を設定し、実行する外部コマンドの内容を記述する。
4. ショートカットキーを F5 (※)に設定。

例）外部コマンドの内容
<pre>
#!/bin/sh
echo $GEDIT_CURRENT_DOCUMENT_PATH
/home/userid/cutehsp_lnx/cutehsp $GEDIT_CURRENT_DOCUMENT_PATH
</pre>
/home/userid/cutehsp_lnx/cutehsp の部分は自分で実行ファイル「cutehsp」を置いたパスを指定。

※ショートカットキーを指定しない場合、geditのメニューから
「ツール」→「External Tools」→「CuteHSP実行」と選択しないといけないので
ショートカットキーを設定しておいたほうが便利。

<b>geditの外部コマンド設定画面</b>
![gedit_cmd](https://cloud.githubusercontent.com/assets/24917310/24007396/c3f20f04-0ab1-11e7-8500-c54c364aa6ed.png)

<b>参考情報：gedit - 外部コマンドで利用できる環境変数の一覧</b><br>
https://kledgeb.blogspot.jp/2014/09/ubuntu-gedit-76.html


## コンパイル方法のメモ
<pre>
g++ cutehsp.cpp -o cutehsp
  -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

g++ cutehspx.cpp -o cutehspx -lopenal
  -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

g++ cutehspcl.c -o cutehspcl -std=c++11
</pre>
