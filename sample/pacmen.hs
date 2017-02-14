title "PACMEN"

10 CLS:LET Y,0:LET X,0
20 LOCATE 0,Y:? CHR$(1);
30 LOCATE 34,Y:? CHR$(1);
40 LET Y,Y+1:IF Y<>27 GOTO 20
50 LOCATE X,0:? CHR$(1);
60 LOCATE X,26:? CHR$(1);
70 LET X,X+1:IF X<>35 GOTO 50
80 LET X,2
90 LET Y,2
100 LOCATE X,Y:? CHR$(1);
110 LET K,RND(4)
120 IF K==1 LOCATE X-1,Y:GOTO 160
130 IF K==2 LOCATE X,Y+1:GOTO 160
140 IF K==3 LOCATE X+1,Y:GOTO 160
150 LOCATE X,Y-1
160 ? CHR$(1);
170 LET Y,Y+2:IF Y<=24 GOTO 100
180 LET X,X+2:IF X<=32 GOTO 90
190 LET X,1:LET Y,1:LET A,1:LET B,1
200 LOCATE 33,25:? CHR$(2);
210 LOCATE X,Y:? CHR$(5);
220 IF (X==33)*(Y==25) END
230 LET I,INKEY()
240 IF I==28 LET X,X-1
250 IF I==29 LET X,X+1
260 IF I==30 LET Y,Y-1
270 IF I==31 LET Y,Y+1
280 IF (A==X)*(B==Y) GOTO 230
290 IF VPEEK(X,Y)<>1 GOTO 310
300 LET X,A:LET Y,B:GOTO 230
310 LOCATE A,B:? " ";
320 LET A,X:LET B,Y:GOTO 210




;PacTxt
	fnt="MS UI Gothic"

	alloc mm,300
	dim ex,4 :dim ey,4 :dim edr,4

	screen 0,256,256,1 :cls 4
	title "PacTxt"
	font fnt,13,1
	buffer 2,256,288,1 :cls 4
	font fnt,13,1

	; ブロック、透明化エサ、ドットエサのパターンデータ

	wbf="7fffff70ceeeeec001173220000c88800001100000088000"
	cl=$f30,$ffc,$fff
	x=0 :y=32 :gosub *chrpt

	; 自キャラ、透明化した自キャラのパターンデータ

	wbf="37fffc63cefff36c03ffc8700cff31e0140880418201102801c80850083101a0"
	cl=$df0,$df0,$680,$680
	x+=8 :gosub *chrpt

	; 敵キャラのパターンデータ

	wbf="375dffe4cebbffe407fdff720efbfffa"
	cl=$f8f,$f8f
	x=0 :y=40 :gosub *chrpt
	cl=$8ff,$8ff :gosub *chrpt
	cl=$e00,$e00 :gosub *chrpt
	cl=$0f5,$0f5 :gosub *chrpt

	gzoom 128,32,2,0,32,64,16	; 描画したドット絵を倍化コピー

*start
	stg=0 :yu=3 :edt=0

	;	面データ読み込み
*stgrd
	fnm="pacstg"+stg+".bin" :exist fnm
	if strsize<0 { pos 96,106 :mes "Complete"
		pos 68,134 :mes "Congratulation!"
		goto *repwt
	}
	bload fnm,mm

	peek nr,mm,256		; ノルマ餌数
	peek en,mm,257		; 敵数
	peek px,mm,258 :peek py,mm,259
	peek pdr,mm,260		; 自キャラ向き
	px=px*4 :py=py*4	; 自キャラ位置

	repeat en				; 敵キャラの位置
		peek ex.cnt,mm,3*cnt+261
		peek ey.cnt,mm,3*cnt+262
		ex.cnt=ex.cnt*4 :ey.cnt=ey.cnt*4
		peek edr.cnt,mm,3*cnt+263
	loop
	;			面作り
	gsel 2 :gmode 1
	repeat 256 :peek a,mm,cnt : x=cnt\16 :y=cnt/16
		if a=255 :a=0 :else :a=3-a
		pos 16*x,16*y+32 :gcopy 2,16*a,0,16,16
	loop
	;			ステータス表示
	color 255,255,255
	pos 8,33 :a=yu-1 :mes "Rest:"+a
	pos 100,33 :mes "Stage:"+stg
	pos 192,33 :mes "ExDot:"+edt
	;				レディ？
	gsel 0 :cls 4 :palcopy 2
	pos 0,0 :gcopy 2,0,32,256,256
	gmode 2 :pos px*4,py*4 :gcopy 2,64,0,16,16
	repeat en
		pos ex.cnt*4,ey.cnt*4 :gcopy 2,32*cnt,16,16,16
	loop
	wait 300 :tm=0

	;	メインルーチン
	repeat
	redraw 0

	if nr<1 :break		; ノルマ達成したか？

	a=0			; 敵との接触判定
	repeat en :c=cnt
		if (px+3>ex.c)&(px-3<ex.c)&(py+3>ey.c)&(py-3<ey.c)&(tm=0) :a=1
	loop
	if a=1 :snd 2 :break

	;		背景をコピー
	gmode 1
	pos 0,0 :gcopy 2,0,32,256,256

	;					自キャラの移動
	if (px\4>0)|(py\4>0) :goto *skp0
	x=px/4 :y=py/4 :g=16*y+x
	stick ky,15
	peek a,mm,g-1 :if (ky=1)&(a!255) :pdr=3
	peek a,mm,g+1 :if (ky=4)&(a!255) :pdr=1
	peek a,mm,g-16 :if (ky=2)&(a!255) :pdr=0
	peek a,mm,g+16 :if (ky=8)&(a!255) :pdr=2

	peek a,mm,g :if (a=1)|(a=2) :gosub *doteat	; ドット食い判定

	if (pdr\2=0) :b=16*(pdr-1)+g :else :b=2-pdr+g
	peek a,mm,b :if a=255 :goto skp1
*skp0
	if (pdr\2=0) :py=py+pdr-1 :else :px=px+2-pdr
*skp1
	;	自キャラの表示
	gmode 2
	a=px+py/4\2 :if tm>0 :tm- :a+=2
	pos px*4,py*4 :gcopy 2,16*a+64,0,16,16

	;	敵キャラの移動と表示
	repeat en :c=cnt
		if (ex.c\4>0)|(ey.c\4>0) :goto *skp2
		x=ex.c/4 :y=ey.c/4 :g=16*y+x

		rnd r,2 :r=2*r+1 :r=edr.c+r\4		; ランダムに動かす
		if (r\2=0) :b=16*(r-1)+g :else :b=2-r+g
		peek a,mm,b :if a!255 :edr.c=r :goto *skp2

		if (edr.c\2=0) :b=16*(edr.c-1)+g :else :b=2-edr.c+g	; 壁なら転身させる
		peek a,mm,b :if a=255 :edr.c=edr.c+2\4
*skp2
		if (edr.c\2=0) :ey.c=ey.c+edr.c-1 :else :ex.c=ex.c+2-edr.c
		a=ex.c+ey.c/4\2 :b=32*c
		pos ex.c*4,ey.c*4 :gcopy 2,16*a+b,16,16,16
	loop

	redraw 1
	wait 5
	loop

	; 次のステージへ？

	redraw 1
	color 255,255,255
	pos 84,120
	if nr>0 :yu- :else :gosub*incyu
	wait 300 :if yu>0 :goto *stgrd
	mes " Game Over"
*repwt
	repeat
		wait 1 :stick ky,48 :if ky&48 :break
	loop
	goto *start

	stop

	; ドット食いサブルーチン
*doteat
	gsel 2
	pos px*4,py*4+32 :gcopy 2,48,0,16,16
	poke mm,g,0
	if a=1 :nr- :snd 0 :else :tm=120 :snd 1
	gsel 0
	return

	; スペシャル餌10ごとに自キャラ増
*incyu
	mes "Stage Clear"
	stg+ :c=0
	repeat 256
		peek a,mm,cnt :if a=2 :c+
	loop
	edt+=c :if edt>9 :yu+ :edt-=10
	return


	; ドット描画
*dotmk
	mtrx="0123456789ABCDEFabcdef" :strlen ln,ch
	repeat ln :c=cnt :strmid hx,ch,c,1
		instr a,mtrx,hx,0 :if a>15 :a-=6
		repeat 4 :if a&1 :pset x+3-cnt,y+c
		a=a>>1 :loop
	loop :x+=4 :return

*chrpt
	strlen zc,wbf :zc=zc/8
	repeat zc :i=cnt/2 :a=cl.i
		r=a/256*16 :g=a/16&15*16 :b=a&15*16 :color r,g,b
		strmid ch,wbf,8*cnt,8 :gosub *dotmk
	loop :return
