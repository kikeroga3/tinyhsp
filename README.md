# tinyhsp tiny版
<pre>
dolphilia版TinyHSPからCUI用のinput、mes命令、実行時オプションなどを削りシェイプアップしたものです。
使い方、導入方法、仕様など、もしこのページで不足のものがあったら下記ページも確認してみてください。

TinyHSP
https://github.com/dolphilia/tinyhsp

neteruhsp
https://github.com/exrd/neteruhsp

[命令]

wait
stop
title
pset
line
boxf
redraw
pos
color
stick
end
return
goto
gosub
repeat
loop
continue
break
if
else
dim
ddim
sdim
randomize

[関数]

int		int(p1)		p1を整数値として返します。
double	double(p1)	p1を実数値として返します。
abs		abs(p1)		p1を絶対値として返します。
str		str(p1)		p1を文字列として返します。
rnd		rnd(p1)		0からp1-1までの乱数値を返します。

[システム変数]

stat      命令や関数実行後のステータス(整数値)が格納される
refdval	  実数型の戻り値はstatではなくrefdvalに格納される
refstr	  文字列方の戻り値が格納される
cnt	      repeat-loopのカウンタ値
mousex	  マウスのx座標
mousey	  マウスのy座標
mousel	  マウスの左ボタンが押されていれば1、押されていなければ0
mouser	  マウスの右ボタンが押されていれば1、押されていなければ0

命令、関数、システム変数の所作は本家のHSPとほぼ同じですが、全部あわせても40個に満たない、
まさにタイニーなプログラム言語となっています。
こんなんで一体何ができるんでしょう？　マ、とりあえず始めてみましょう(ﾟーﾟ)σビシッ

[インストール方法]
任意のフォルダでよいので tinyhsp.exe と glfw3.dll を同じフォルダに置いてください。

[実行方法]
スクリプトファイル(例：start.hs)を tinyhsp.exe にドラッグ＆ドロップするか、
拡張子「.hs」を tinyhsp.exe に関連付けしてスクリプトファイルをダブルクリックします。

あるいはコマンドラインから次のように入力しても実行できます。

tinyhsp start.hs

また「start.hs」というファイル名はデフォルトスクリプトファイル名なので
tinyhsp.exe と同じフォルダにあれば、単に

tinyhsp

と入力するだけでもスクリプト「start.hs」の内容が実行されます。

補足：
tinyhsp_c.exe はコンソールから実行するデバッグ用EXEです。エラーがあればコンソールに表示されるようになってます。
自分でスクリプトを組みながら試すときは、こちらで実行するほうがいいかもしれません。

tinyhsp_c start.hs

その場合はコマンドラインから上記のように入力して実行してください。

[教育用言語としての適性について]
「命令がいっぱいあってわかんなーい！」じゃなくて「これしかない」から始められるのが唯一最大のメリットです。
ビギナーは命令や機能がたくさんあると気持ちが飽和してかえって迷うものです。
「この位なら出来るかも…」と感じる動機づけからスタートし、慣れてくれば今度は、
少ない命令数だからこその「工夫して目的を果たす」という頭の使い方や、
「ないと作れない」じゃなくて「なければ作る！」という発想をもてるようになる。
そんな逞しい学習効果が見込めると思います。
TinyHSPで「できた！」という成功体験をへて、TinyHSPに限界を感じてから次のプログラム言語へステップアップする。
プログラミングのファーストステップとして楽しみ、活用してもらえればと思います。
…決して機能が少ないことへのいいわけではありませんよー(^^;

[ソースのコンパイル方法のメモ]
Windowsでの例ですが、MinGW(gcc)とGLFW3で開発環境をつくって以下コマンドでコンパイルします。

コンソールアプリケーション生成
g++ -static tinyhsp.cpp -o tinyhsp_c -std=gnu++11 -lglfw3dll -lopengl32

Windowsアプリケーション生成
g++ -static tinyhsp.cpp -o tinyhsp -std=gnu++11 -lglfw3dll -lopengl32 -mwindows

macOSやLinux版の実行ファイルも作れるはずですが、私はまだどちらもコンパイルまで試せてません。
各環境用のコンパイル方法については<a href="https://github.com/dolphilia/tinyhsp">dolphiliaさんのページ</a>が詳しいです。
もしmacOS版やLinux版の実行ファイルを作成された方がおりましたらぜひお知らせください。
こちらのページに掲載させていただきたいと思っています。

[MIT license]
https://github.com/YukinobuKurata/YouTubeMagicBuyButton/blob/master/MIT-LICENSE.txt0
</pre>
