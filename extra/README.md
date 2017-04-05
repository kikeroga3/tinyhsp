# CuteHSP エクストラ <img src="https://raw.githubusercontent.com/kikeroga3/tinyhsp/master/source/cutehsp.ico">

エクストラはミニマムの上位互換版。CuteHSP ミニマムに下記の命令、関数、システム変数を追加したものになります。

### 命令
<pre>
命令		書式			説明
font		font p1,p2,p3		フォントの設定を行います。p1は使用するTTFファイルを拡張子まで含めて指定。
					p2はフォントサイズ(上限100)、
					p3はスムージングの指定(0でスムージングなし、16でスムージングあり)です。
mes		mes p1			文字列p1を画面に描画します。
picload		picload p1		p1で指定した画像ファイルを読み込み、画面に表示します。
bload		bload p1,p2		ファイル名p1の内容を文字型変数p2に読み込む。
poke		poke p1,p2,p3		文字型変数p1のp2バイトめにバイト値p3を書き込む。
beep		beep p1,p2,p3,p4	p1は周波数、p2は再生する長さ(ミリ秒)、p3は波形の種類、
					p4はボリューム(0～30000)を指定して音を鳴らします。
						波形の種類(デフォルト:2)は以下の通り。
						0=正弦波
						1=ノコギリ波
						2=矩形波
						3=三角波
						4=ホワイトノイズ
</pre>
### 関数
<pre>
命令		書式		説明
peek		peek(p1,p2)	文字型変数p1のp2バイトめのバイト値を取得して返す。
</pre>
### システム変数
<pre>
変数名		説明
strsize		bload命令で読み込んだファイルのバイト数が格納される。
</pre>

### 実行ファイル
<pre>
cutehspx.exe
</pre>
cutehspx.exe と glfw3.dll と OpenAL32.dll と tiny.ttf を同じフォルダに置いてください。
使い方は cutehsp.exe と同じです。

※ tinytrek.hs を実行する場合は tiny_en.ttf (等幅フォント)が必要です。

※ font命令、mes命令を使用しなければ tiny.ttf は不要です。

### スクリーンショット

![cutehspx_ok](https://cloud.githubusercontent.com/assets/24917310/23577156/751e972c-00fb-11e7-9e9f-187855a24823.png)

### スクリプトの内容
<pre>
title "CuteHSP Extra"
pos 20,10
picload "tamarin.jpg"
font "tiny.ttf",48
pos 80,360
mes "Hello, World!?"
mes "文字と画像を表示します。"
pos 200,100
picload "cutehsp.png"
redraw 1
stop
</pre>
