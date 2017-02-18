# tinyhsp tiny版

<img src="https://cloud.githubusercontent.com/assets/24917310/23020538/e2142540-f48a-11e6-9019-a880e5599b8c.png" width="33%" height="33%" alt="kakiget">
<img src="https://cloud.githubusercontent.com/assets/24917310/23020539/e214b8b6-f48a-11e6-8bf0-03d504987fa6.png" width="33%" height="33%" alt="landing">
<img src="https://cloud.githubusercontent.com/assets/24917310/23020537/e212ccc2-f48a-11e6-91f9-b4ba683c5cf4.png" width="33%" height="33%" alt="maze">

<pre>
ただでさえタイニーな「<a href="https://github.com/dolphilia/tinyhsp">dolphilia版TinyHSP</a>」からCUI用のinput、mes命令、実行時オプションなどを削り
シェイプアップしたものです。画面処理こそglfw3ライブラリ頼りですが、実行ファイルの本体サイズは100KB以下。
使い方、導入方法、仕様など、もしこのページで不足のものがあったら下記ページも確認してみてください。

TinyHSP
https://github.com/dolphilia/tinyhsp

neteruhsp
https://github.com/exrd/neteruhsp

[命令]

命令		書式					説明
wait	wait p1				p1ミリ秒待つ
stop	stop				ウィンドウが[×]でクローズされるまで待つ
title	title p1			タイトルバーに文字列p1を表示する
pset	pset p1,p2			座標p1,p2にドットを描画する。p1,p2が省略された場合はカレントポジションに描画する
line	line p1,p2,p3,p4	座標p1,p2から座標p3,p4まで線を描画する
boxf	line p1,p2,p3,p4	座標p1,p2から座標p3,p4まで矩形を塗りつぶす
redraw	redraw p1			p1が0なら再描画スイッチをオフに、1ならオンにする。p1が省略されたらオンにする
pos		pos p1,p2			座標p1,p2をカレントポジションに設定する
color	color p1,p2,p3		RGBカラーp1,p2,p3をカレントカラーに設定する
stick	stick p1			数値変数p1にキー情報を格納する。
							(本家HSPの stick p1,1+2+4+8+16+32+64+128+256+512+1024 相当の動作)
[キー情報]
   1   カーソルキー左(←)
   2   カーソルキー上(↑)
   4   カーソルキー右(→)
   8   カーソルキー下(↓)
  16   スペースキー
  32   Enterキー
  64   Ctrlキー
 128   ESCキー
 256   マウスの左ボタン
 512   マウスの右ボタン
1024   TABキー

end			end					プログラムを終了
goto		goto p1				ラベルp1へジャンプする
gosub		gosub p1			サブルーチンのラベルp1へジャンプする
return		return				サブルーチンから戻る
repeat		repeat p1			repeat～loopで囲まれた範囲をp1回繰り返し処理する
loop		loop				繰り返し処理の終端命令
continue	continue			繰り返し処理中でrepeat命令まで戻る
break		break				繰り返し処理から抜ける
if			if p1				条件p1を満たしていればその行の以降の命令を実行する
else		else				if条件が満たされなかった場合にelse命令以降が実行される
dim			dim p1,p2			変数名p1、配列数p2の配列変数(整数)を作成する
ddim		ddim p1,p2			変数名p1、配列数p2の配列変数(実数)を作成する
sdim		sdim p1,p2,p3		変数名p1、文字数p2、配列数p3の文字列型配列変数を作成する
randomize	randomize			乱数発生パターンを初期化

[関数]

int		int(p1)		p1を整数値として返す
double	double(p1)	p1を実数値(倍精度浮動小数点)として返す
abs		abs(p1)		p1を絶対値として返す
str		str(p1)		p1を文字列として返す
rnd		rnd(p1)		0からp1-1までの乱数値を返す

[システム変数]

stat      命令や関数実行後のステータス(整数値)が格納される
refdval	  実数型の戻り値はstatではなくrefdvalに格納される
refstr	  文字列型の戻り値が格納される
cnt	      repeat-loopのカウンタ値
mousex	  マウスのx座標
mousey	  マウスのy座標
mousel	  マウスの左ボタンが押されていれば1、押されていなければ0
mouser	  マウスの右ボタンが押されていれば1、押されていなければ0

表示画面は640×480ドット固定の1枚だけ、命令、関数、システム変数の所作は本家のHSPとほぼ同じですが、
全部あわせても40個に満たない、まさにタイニーなプログラム言語となっています。
こんなんで一体何ができるんでしょう？　マ、とりあえず始めてみましょう(ﾟーﾟ)σビシッ

[インストール方法]
任意のフォルダでよいので tinyhsp.exe と glfw3.dll を同じフォルダに置いてください。

[実行方法]
スクリプトファイル(例：start.hs)を tinyhsp.exe にドラッグ＆ドロップするか、
拡張子「.hs」を tinyhsp.exe に関連付けしてスクリプトファイルをダブルクリックします。
(※スクリプトファイルは文字コードUTF-8のテキスト形式です)

あるいはコマンドラインから次のように入力しても実行できます。

tinyhsp start.hs

また「start.hs」というファイル名はデフォルトスクリプトファイル名なので
tinyhsp.exe と同じフォルダにあれば、単に

tinyhsp

と入力するだけでもスクリプト「start.hs」の内容が実行されます。

[補足1：サンプルスクリプト]
「sample」フォルダにサンプルスクリプトがいくつか入っています。
試してみるだけでも何が出来るのか、ざっとわかると思いますので参考にしてください。

[補足2：デバッグ用EXE]
tinyhsp_c.exe はコンソールから実行するデバッグ用EXEです。エラーがあればコンソールに表示されるようになってます。
自分でスクリプトを組みながら試すときは、こちらで実行するほうがいいかもしれません。

tinyhsp_c start.hs

その場合はコマンドラインから上記のように入力して実行してください。

[補足3：TeraPadから実行]
<a href="http://forest.watch.impress.co.jp/library/software/terapad/">TeraPad</a>なら「ツールの設定」からコマンドを追加すればプログラムを編集しながら実行することができます。

1. TeraPadを開いてメニューの「ツール」→「ツールの設定」→「追加」を選択。
2. 開いた「ツールの編集」画面上で、
	「名前」に「tinyhsp_c」(任意)
	「実行ファイル」に tinyhsp_c.exe を置いたフルパスを指定
	「コマンドラインパラメータ」に %t を指定
	最後に「OK」で設定を保存します。
3. 以後、メニューの「ツール」を選ぶと「tinyhsp_c」が追加されているはずですので、
   エディタにTinyHSPのプログラムを書いて、このメニューを選択すれば
   TeraPadで編集中のプログラムを実行できるようになります。

軽快に動作するフリーのテキストエディター「TeraPad」
http://forest.watch.impress.co.jp/library/software/terapad/
</pre>
![terapad_tool](https://cloud.githubusercontent.com/assets/24917310/23092822/a8580e56-f616-11e6-8746-0e344295c9bd.png)

<pre>
[提言：教育用プログラム言語としての適性について]
「命令がいっぱいあってわかんなーい！」じゃなくて「これしかない」から始められるのが唯一最大のメリットです。
ビギナーは命令や機能がたくさんあると気持ちが飽和してかえって迷うものです。
「この位なら出来るかも…」と感じる動機づけからスタートし、慣れてくれば今度は、
少ない命令数だからこその「工夫して目的を果たす」という頭の使い方や、
「ないと作れない」じゃなくて「なければ作る！」という発想をもてるようになる。
そんな逞しい学習効果が見込めると思います。
TinyHSPで「できた！」という成功体験をへて、TinyHSPに限界を感じてから次のプログラム言語へステップアップする。
プログラミングのファーストステップとして楽しみ、利用してもらえればと思います。
…決して機能が少ないことへのいいわけではありません(^^;

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
