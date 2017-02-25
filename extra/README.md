# tinyhsp extra版

tinyhsp tiny版に下記GUI命令を追加したもの。
<pre>
命令			書式				説明
font		font p1,p2,p3	フォントの設定を行います。p1は使用するTTFファイルを拡張子まで含めて指定。
							p2はフォントサイズ(上限100)、
							p3はスムージングの指定(0でスムージングなし、16でスムージングあり)です。
mes			mes p1			文字列p1を画面に描画します。
picload		picload p1		p1で指定した画像ファイルを読み込み、画面に表示します。
</pre>
###実行ファイル
<pre>
tinyhspx.exe
tinyhspx_c.exe (デバッグ用EXE)
</pre>
tinyhspx.exe と glfw3.dll と tiny.ttf を同じフォルダに置いてください。
使い方は tinyhsp.exe と同じです。

※ tinytrek.hs を実行する場合は tiny_en.ttf (等幅フォント)が必要です。

###スクリーンショット

![tinyhspx_ok](https://cloud.githubusercontent.com/assets/24917310/23071003/45e199a0-f570-11e6-8e74-21320d8e72de.jpg)

###スクリプトの内容
<pre>
title "TinyHSP Extra"
pos 20, 10
picload "tamarin.jpg"
font "tiny.ttf",48
pos 80, 360
mes "Hello, World!?"
mes "文字と画像を表示します。"
redraw 1
stop
</pre>
