#TinyHSPコンソール版

neteruhspとほぼ同じですが、実行時オプションを削り、エラーメッセージを英字化しています。

tinyhsp tiny版との違いですが、ウィンドウ画面が出ませんのでstop命令がありません。
stick命令もなし。
代わりというわけではないですが、inputとmes命令があります。

<pre>
命令    書式      説明
input input p1  変数p1にキーボード入力値を格納する。
mes   mes p1    文字列p1をコンソール画面に出力する。
</pre>
