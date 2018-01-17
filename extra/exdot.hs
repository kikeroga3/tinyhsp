;Extra Dot Character Draw

	;文字型変数bfにキャラクターデータを読み込む
	sdim bf,64000 :bload "chr1.bin",bf
	title "strsize="+strsize

	redraw 0 :color :boxf 0,0,640,480

	repeat 20

		;cno=キャラクター番号
		;x,y=表示する座標
		;sz=1ドットのサイズ
		;sw=左右上下反転回転スイッチ

		sz=5 :sw=rnd(8) :x=rnd(640) :y=rnd(480) :cno=cnt\13 :gosub *chrput
	loop

	redraw 1
	stop

;キャラクター表示ルーチン
*chrput
	xs=peek(bf,0) :m=xs*(xs/7) :cln=m*2+8 :adr=cno*cln

	;RGB色情報ゲット
	repeat 3
		a=peek(bf,adr+1+cnt)&63
		r(cnt)=80*(a/16)+15 :g(cnt)=80*((a&12)/4)+15 :b(cnt)=80*(a&3)+15
	loop

	;キャラクタービット計算＆表示
	i=0
	repeat m
		c1=peek(bf,adr+4+cnt) :c2=peek(bf,adr+4+m+cnt)
		a=64
		repeat 7
			;sw不要なら以下1行だけでよい
			;xx=x-(xs*sz/2)+(i\xs)*sz :yy=y-(xs*sz/2)+(i/xs)*sz
			;sw使用なら以下if文3行で制御
			if (sw&1)>0 :xx=(xs*sz/2)-(i\xs)*sz :else :xx=(i\xs)*sz-(xs*sz/2)		;左右反転
			if (sw&2)>0 :yy=(xs*sz/2)-(i/xs)*sz :else :yy=(i/xs)*sz-(xs*sz/2)		;上下反転
			if (sw&4)>0 { c=xx :xx=x+yy :yy=y+c } else { xx=x+xx :yy=y+yy }		;90度回転
			c=((c1&a)>0)+2*((c2&a)>0)
			if c>0 {
				color r(c-1),g(c-1),b(c-1)
				boxf xx,yy,xx+sz-1,yy+sz-1
			}
			a=a/2 :i=i+1
		loop
	loop
	return

