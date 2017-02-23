#TinyHSPコンソール版

中身は neteruhsp とほぼ同じですが、実行時オプションを削り、WindowsコンソールのSJIS仕様でも文字化けしないようエラーメッセ―ジを英字に変更しています。

tinyhsp tiny版との違いは、コンソール版ではウィンドウ画面がないのでまずstop命令がありません。
マウス入力、リアルタイム性もないのでstickやwait命令もなし。

代わりというわけではないですが、inputとmes命令があります。

<pre>
命令      書式        説明
input   input p1    変数p1にキーボード入力値を格納する。
mes     mes p1      文字列p1をコンソール画面に出力する。
</pre>

##実行方法
コンソール画面を開き、以下コマンドを実行。
<pre>
tinyhspcl start.hs
</pre>
