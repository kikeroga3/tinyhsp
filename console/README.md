# CuteHSP コンソール <img src="https://raw.githubusercontent.com/kikeroga3/tinyhsp/master/source/cutehsp.ico">

中身は <a href="https://github.com/exrd/neteruhsp">neteruhsp</a> とほぼ同じですが、実行時オプションを削り、WindowsコンソールのSJIS仕様でも文字化けしないようエラーメッセ―ジを半角英字に変更しています。

CuteHSP ミニマムとの違いは、コンソール版にはまずグラフィカルなウィンドウ画面がなく、コンソール画面へのテキスト表示(mes命令)のみ。
入力はキーボードからの待機入力(input命令)のみで、マウス入力、リアルタイム性もない処理系なので stop, stick, wait命令がありません。

<pre>
命令      書式              説明
input   input p1,p2,p3    キー入力を取得する。
                          p1:入力値を格納する変数名。p2:変数に代入される最大文字数
                          p3:改行コード認識フラグ(0=なし/1の場合はLFを、2の場合はCR+LFを改行と認識)
mes     mes p1            文字列p1をコンソール画面に出力する。
</pre>

## 実行方法
コンソール画面を開き、以下コマンドを実行。
<pre>
cutehspcl start.hs
</pre>
glfw3.dll は不要。cutehspcl.exe 単体で動きます。
