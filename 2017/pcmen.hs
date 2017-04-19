;---初期値設定------------------------------------

*start

randomize
stg=1 :scr=0 :emx=2 :hp=3

;---背景データ用意--------------------------------

*next

dim bk,900
repeat 900 :bk(cnt)=2 :loop

repeat 8 :c=cnt
	repeat (30-4*c)
		bk(cnt+62*c)=1 :bk(870+cnt-58*c)=1
		bk(30*cnt+62*c)=1 :bk(30*cnt+58*c+29)=1
	loop
loop

repeat 26
	bk(392+cnt)=2 :bk(482+cnt)=2
	bk(73+30*cnt)=2 :bk(76+30*cnt)=2
loop

repeat 90
	x=rnd(26) :y=rnd(26) :bk(62+30*y+x)=0
loop

;---エサの総数をカウント--------------------------

m=0 :repeat 900 :m=m+(bk(cnt)=2) :loop

;---PcMen座標-------------------------------------

px=13 :py=13

;---Obake座標と移動方向値-------------------------

repeat emx
*try
	x=rnd(30) :y=rnd(30) :dx=abs(px-x) :dy=abs(py-y)
	i=30*y+x
	if bk(i)=1 or (dx+dy)<20 :goto *try
	ex(cnt)=x :ey(cnt)=y :ev(cnt)=rnd(4) :ec(cnt)=rnd(64)
loop

;---メインルーチン--------------------------------

repeat
	redraw 0
	color :boxf 0,0,639,479

	;---背景データの表示--------------------------

	repeat 900
		x=16*(cnt\30)+80:y=16*(cnt/30)
		if bk(cnt)=1 :color 128,0,0:boxf x,y,x+15,y+15
		if bk(cnt)=2 :color 200,250,0:boxf x+6,y+6,x+10,y+10
	loop

	;---PcMen座標マスのチェックと移動-------------

	i=30*py+px
	if bk(i)=2 {						;マスにエサがあれば配列変数に0をセット
		bk(i)=0 :m=m-1 :scr=scr+10		;エサ総数から-1し、スコアを+10
		if m<1 :break					;エサを全部食べたらloopを抜ける
	}
	stick k
	if bk(i-1)!1 :px=px-(k=1)			;移動方向に壁(1)がなければ座標移動
	if bk(i+1)!1 :px=px+(k=4)			;
	if bk(i-30)!1 :py=py-(k=2)			;
	if bk(i+30)!1 :py=py+(k=8)			;

	;---PcMen表示---------------------------------

	xx=px :yy=py :c=32+16+8+4 :ch=2028666414 :gosub *dot5

	;---Obake移動方向設定-------------------------

	repeat emx
		e=30*ey(cnt)+ex(cnt)
		v(0)=(bk(e-30)!1) :v(1)=(bk(e+1)!1)		;移動可能な方向に1をセット
		v(2)=(bk(e+30)!1) :v(3)=(bk(e-1)!1)		;(上0,下2,左3,右1)
		v1=(ev(cnt)+3)\4 :v2=(ev(cnt)+1)\4		;現在の向きの左右方向値をv1,v2にセット
		r=rnd(3)
		if r=1 and v(v1)!0 :ev(cnt)=v1			;移動方向を現在の向きの左に確定
		if r=2 and v(v2)!0 :ev(cnt)=v2			;移動方向を現在の向きの右に確定
		if v(ev(cnt))!1 :ev(cnt)=(ev(cnt)+2)\4	;現在の方向に行けなければUターン
	loop

	;---Obake移動と表示と当たり判定---------------

	repeat emx
		ex(cnt)=ex(cnt)+(ev(cnt)=1)-(ev(cnt)=3)		;方向に準じた座標移動
		ey(cnt)=ey(cnt)+(ev(cnt)=2)-(ev(cnt)=0)		;
		xx=ex(cnt):yy=ey(cnt) :c=ec(cnt)
		ch=15390709 :gosub *dot5
		if px=ex(cnt) and py=ey(cnt) :hp=hp-(hp>0)	;PcMenとの当たり判定
	loop

	wait 10 :redraw 1

	;---タイトルバー表示--------------------------

	title "Stage:"+stg+" Food Left:"+m+" Score:"+scr+" Life:"+hp

	;---PcMenライフが0ならloop外へ----------------

	if hp<1 :break
loop

;---スペースキー入力待ち--------------------------

*spc
	wait 10
	stick k
	if k!16 :goto *spc

;---次のステージまたは最初からスタートへ----------

	if hp<1 :goto *start
	stg=stg+1 :emx=emx+(emx<16) :hp=hp+(hp<9)
	goto *next

;---5×5ドットキャラ表示サブルーチン--------------

*dot5
	sz=3 :a=16777216
	p1=c/16 :p2=(c&12)/4 :p3=c&3
	color 80*p1+15,80*p2+15,80*p3+15
	repeat 25
		if (ch & a) {
			x=16*xx+sz*(cnt\5)-(sz*5/2)
			y=16*yy+sz*(cnt/5)-(sz*5/2)
			boxf 88+x,8+y,88+x+sz,8+y+sz
		}
		a=a/2
	loop
	return

