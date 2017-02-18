# tinyhsp extra版
<pre>
tinyhsp tiny版に下記GUI命令を追加したもの。

命令			書式				説明
font		font p1,p2,p3	フォントの設定を行います。p1は使用するTTFファイルを拡張子まで含めて指定。
							p2はフォントサイズ(上限100)、
							p3はスムージングの指定(0でスムージングなし、16でスムージングあり)です。
mes			mes p1			文字列p1を画面に描画します。
picload		picload p1		p1で指定した画像ファイルを読み込み、画面に表示します。

[実行ファイル]
tinyhspx.exe
tinyhspx_c.exe (デバッグ用EXE)

tinyhspx.exe と glfw3.dll と tiny.ttf を同じフォルダに置いてください。
使い方は tinyhsp.exe と同じです。
</pre>
![tinyhspx_ok](https://cloud.githubusercontent.com/assets/24917310/23071003/45e199a0-f570-11e6-8e74-21320d8e72de.jpg)
