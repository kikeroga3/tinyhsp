title "RPG MAP MOVE"

	; マップチップデータ読み込み
	sdim map_bf,65536
	bload "map.bin",map_bf

	; キャラクタデータ読み込み
	sdim chr_bf,64000
	bload "rpg.bin",chr_bf

	dim cel,50
	px=128:py=128:p_sz=5
	color 15,15,15 :boxf 0,0,640,480

; マップ描画
*mapdraw
	redraw 0
	cel(24)=-1	; 中央のマップチップは常に描画
	repeat 49
		p_x=30+40*(cnt\7):p_y=30+40*(cnt/7)
		x=px-3+(cnt\7):y=py-3+(cnt/7):i=x+256*y
		p_no=peek(map_bf,i)-1
		; マップチップが前と違えば描画(無駄な処理はしない)
		if cel(cnt)!p_no {
			color 48,32,0 :boxf p_x-20,p_y-20,p_x+20,p_y+20
			cel(cnt)=p_no :gosub *chrput
		}
	loop
	; 主人公表示
	p_no=4+((px+py)\2):p_x=150:p_y=150:gosub *chrput
	redraw 1

	repeat
		wait 15 :stick k
		if k=1 and cel(23)<3 :px=px-1
		if k=4 and cel(25)<3 :px=px+1
		if k=2 and cel(17)<3 :py=py-1
		if k=8 and cel(31)<3 :py=py+1
		if k!0 :break
	loop
	goto *mapdraw

stop

;キャラクター表示ルーチン
*chrput
	p_xs=peek(chr_bf,0) :p1=p_xs*(p_xs/7) :p_ln=p1*2+8 :p_ad=p_no*p_ln
	;RGB色情報ゲット
	repeat 3
		p2=peek(chr_bf,p_ad+1+cnt)&63
		p_r(cnt)=80*(p2/16)+15 :p_g(cnt)=80*((p2&12)/4)+15 :p_b(cnt)=80*(p2&3)+15
	loop
	;キャラクタービット計算＆表示
	p3=0
	repeat p1
		p_c1=peek(chr_bf,p_ad+4+cnt) :p_c2=peek(chr_bf,p_ad+4+p1+cnt)
		p2=64
		repeat 7
			p_xx=p_x-(p_xs*p_sz/2)+(p3\p_xs)*p_sz :p_yy=p_y-(p_xs*p_sz/2)+(p3/p_xs)*p_sz
			p_c3=((p_c1&p2)>0)+2*((p_c2&p2)>0)
			if p_c3>0 {
				color p_r(p_c3-1),p_g(p_c3-1),p_b(p_c3-1)
				boxf p_xx,p_yy,p_xx+p_sz-1,p_yy+p_sz-1
			}
			p2=p2/2 :p3=p3+1
		loop
	loop
	return
