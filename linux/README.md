#TinyHSP for Linux

こちらはLinux版の実行ファイルです。同じサンプルスクリプトが動かせます。

##実行画面
<img src="https://cloud.githubusercontent.com/assets/24917310/23199814/193572da-f914-11e6-9665-ed890d633750.png" width="35%" height="35%" alt="tinyhsp_linux">
<img src="https://cloud.githubusercontent.com/assets/24917310/23199813/1932d106-f914-11e6-8fa5-33a6a8674876.png" width="35%" height="35%" alt="tinyhspx_linux">
<img src="https://cloud.githubusercontent.com/assets/24917310/23306131/5f1a4960-fae5-11e6-88a0-2d58cea2b206.png" width="29%" height="29%" alt="tinyhspcl_linux">


###補足：geditから実行

Linuxでは定番のテキストエディタ「gedit」で「外部コマンド」を有効にすればプログラムをエディタ上で編集しながら実行することができます。

1. 設定画面の「プラグイン」タブで「外部コマンド」にチェックを入れる。
2. 「Manage External Tools...」から「外部ツールの管理」を開き、左下にある「＋」で新規コマンドを追加。
3. 任意のコマンド名(例:tinyhspx)を設定し、実行する外部コマンドの内容を記述する。
4. ショートカットキーを F5 (※)に設定。

例）外部コマンドの内容
<pre>
#!/bin/sh
echo $GEDIT_CURRENT_DOCUMENT_PATH
/home/maika/tinyhsp/tinyhspx $GEDIT_CURRENT_DOCUMENT_PATH
</pre>

※ショートカットキーを指定しない場合、geditのメニューから
「ツール」→「External Tools」→「tinyhspx」と選択しないといけないので
ショートカットキーを設定しておいたほうが便利です。

<b>geditの外部コマンド設定画面</b>
![gedit_tool](https://cloud.githubusercontent.com/assets/24917310/23336405/a954a68c-fc12-11e6-93f1-81154568583f.png)

##コンパイル方法のメモ
<pre>
g++ tinyhsp.cpp -o tinyhsp
  -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

g++ tinyhspx.cpp -o tinyhspx
  -lglfw3 -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lpthread -ldl

g++ -static tinyhspcl.c -o tinyhspcl -std=c++11
</pre>
