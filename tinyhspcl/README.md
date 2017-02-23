#TinyHSPコンソール版

中身は <a href="https://github.com/exrd/neteruhsp">neteruhsp</a> とほぼ同じですが、実行時オプションを削り、WindowsコンソールのSJIS仕様でも文字化けしないようエラーメッセ―ジを英字に変更しています。

tinyhsp tiny版との違いは、コンソール版ではウィンドウ画面がありませんのでまずstop命令がありません。
マウス入力、リアルタイム性もないのでstickやwait命令もなし。

代わりというわけではないですが、inputとmes命令があります。

<pre>
命令      書式              説明
input   input p1,p2,p3    キー入力を取得する。
                          p1:入力値を格納する変数名。p2:変数に代入される最大文字数
                          p3:改行コード認識フラグ(0=なし/1の場合はLFを、2の場合はCR+LFを改行と認識)
mes     mes p1            文字列p1をコンソール画面に出力する。
</pre>

##実行方法
コンソール画面を開き、以下コマンドを実行。
<pre>
tinyhspcl start.hs
</pre>
