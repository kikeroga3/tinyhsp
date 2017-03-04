; for cutehspx bload poke peek test program

	;文字型変数bfを用意
	sdim bf,512
	bf="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	mes bf

	;バイト値を取得
	a=peek(bf,11)
	mes "'B'=ASC("+a+")"

	;バイト値を4バイトめに書込み
	poke bf,3,a
	mes bf

	;98('b')を12バイトめに書込み
	poke bf,11,66+32
	mes bf

	;変数bfにファイル内容を読込み
	bload "excmd.hs",bf
	mes bf
	stop
