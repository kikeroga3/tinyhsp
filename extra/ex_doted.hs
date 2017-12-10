title "Extra Dot Editor"

;3色、7の倍数の縦横サイズで描けるドットエディタです。
;下記3行の変数値を書き換えて実行してください。

l_fnm="chr.bin"		;ロードするファイル名(""だと新規)
s_fnm="chr.bin"		;セーブするファイル名
dsz=2				;編集サイズ指定(一辺が7×dszドットになる)

	font "tiny.ttf",20

	fsz=64000 :sdim cdat,fsz+1
	repeat fsz :poke cdat,cnt,255 :loop
	poke cdat,fsz,0

*cdat_ld
	if l_fnm!"" {
		bload l_fnm,cdat
		gosub *cdat_in :goto *start
	}

*cdat_nw
	xs=7*dsz :dim ch,xs*xs
	repeat 3
		r(cnt)=3-cnt :g(cnt)=0 :b(cnt)=1+cnt
	loop

*start
	sz=15
	pos sz*xs,60 :mes "↑クリックで色変更"
	mes "←クリックでドットを描く"
	mes " [ESC]で保存"
	pos 0,sz*xs+sz :mes "[<<] [< ] [ >] [>>]"

*main
	cln=xs*(xs/7)*2+8 :cmx=fsz/cln
	title "Extra Dot Editor [Chr_No."+cno+"/"+cmx+"]"

	repeat
		redraw 0
		color 0,0,0 :boxf 0,0,sz*xs,sz*xs
		repeat 3 :x=sz*xs :y=20*cnt
			color 80*r(cnt)+15,80*g(cnt)+15,80*b(cnt)+15
			boxf x+2,y,x+60,y+20
			color 0,0,0
			pos x+2,y :mes "R"+r(cnt)
			pos x+22,y :mes "G"+g(cnt)
			pos x+42,y :mes "B"+b(cnt)
		loop

		repeat xs*xs :x=cnt\xs :y=cnt/xs
			c=ch(cnt)-1
			if c>=0 {
				color 80*r(c)+15,80*g(c)+15,80*b(c)+15
				boxf sz*x+1,sz*y+1,sz*x+sz-1,sz*y+sz-1
			}
		loop

		stick key
		if key=128 :gosub *cdat_out :break
		if key&256 {
			x=mousex/sz :y=mousey/sz
			if x<xs and y<xs {
				i=xs*y+x :ch(i)=(ch(i)+1)\4
			}
			x=mousex-sz*xs :y=mousey/20
			if x>0 and x<60 and y<3 {
				x=x/20
				r(y)=r(y)+(x=0) :g(y)=g(y)+(x=1) :b(y)=b(y)+(x=2)
				r(y)=r(y)\4 :g(y)=g(y)\4 :b(y)=b(y)\4
			}
			x=mousex/40 :y=mousey-sz*xs
			if y>0 {
				gosub *cdat_out
				cno=cno-9*(x=0)-(x=1)+(x=2)+9*(x=3)
				if cno<0 :cno=cno+cmx
				cno=cno\cmx
				break
			}
		}
		wait 8 :redraw 1
	loop

	;データ保存
	if key=128 {
		title "Save Data. ["+s_fnm+"]"
		bsave s_fnm,cdat :wait 60
	}
	adr=cno*cln :poke cdat,adr,xs
	gosub *cdat_in
	wait 30 :goto *main

;ch(x) → cdat 変換
*cdat_out
	adr=cno*cln :poke cdat,adr,xs

	;RGB色要素ビット計算
	repeat 3
		a=16*r(cnt)+4*g(cnt)+b(cnt)
		a=a+128*(a=0) :poke cdat,adr+1+cnt,a
	loop

	;キャラクタービット計算
	a=0 :bit=64 :i=4 :m=xs*xs
	repeat 2 :n=1+cnt
		repeat m
			if ch(cnt)&n :a=a+bit
			bit=bit/2
			if bit=0 {
				a=a+128*(a=0)
				poke cdat,adr+i,a :a=0 :bit=64 :i=i+1
			}
		loop
	loop
	return

;cdat → ch(x) 変換
*cdat_in
	adr=cno*cln :xs=peek(cdat,adr)

	;RGB色要素ビット計算
	repeat 3
		a=peek(cdat,adr+1+cnt)&63
		r(cnt)=a/16 :g(cnt)=(a&12)/4 :b(cnt)=a&3
	loop

	;キャラクタービット計算
	dim ch,xs*xs
	dsz=xs/7 :m=xs*dsz :i=0
	repeat m
		c1=peek(cdat,adr+4+cnt) :c2=peek(cdat,adr+4+m+cnt)
		a=64
		repeat 7
			ch(i)=ch(i)+((c1&a)>0)+2*((c2&a)>0)
			a=a/2 :i=i+1
		loop
	loop
	return

