;Landing Game

*start
	randomize
	dim map,768				; 32×24=768
	stg=0 :sc=0 :hi=0 :e=0

*stgmk
	; 面生成
	stg=stg+1 :x=15 :w=13-(stg/3) :if w<5 :w=5
	repeat 672 :map(96+cnt)=1 :loop		; 32*21=672
	repeat 20
		y=3+cnt :x=x+(rnd(5)-2) :ww=w-(cnt/8)
		repeat ww
			xx=x-(ww/2) :if xx<0 :xx=0
			map(y*32+xx+cnt)=0
		loop
	loop

	; 得点計算
	sc=sc+e :if sc>hi :hi=sc

	; 着陸船 変数初期化
	px=320 :py=15 :gx=0 :gy=0 :e=500

	; 画面表示 待ち
	gosub *stgpr :x=px :y=py :gosub *ship
	title "HIT SPACE KEY TO START!"
	gosub *pause

; メインループ
	repeat
		title "STAGE:"+stg+" ENERGY:"+e+" SCORE:"+sc+" HIGH-SCORE:"+hi
		redraw 0

		; 画面表示
		gosub *stgpr :x=px :y=py :gosub *ship

		; 重力
		gy=gy+(gy<30)

		; 慣性移動
		px=px+(gx/2) :py=py+(gy/3)
		if px<0 :px=0
		if px>640 :px=640
		if py<0 :py=0

		if py>450 {
			if gy<3 {
				title "GOOD LANDING!" :wait 300 :goto *stgmk
			} else [
				gosub *bomb :gosub *pause :goto *start
			}
		} else {
			x=px/20 :y=py/20
			if map(y*32+x)=1 :gosub *bomb :gosub *pause :goto *start
		}

		redraw 1
		wait 10
	loop


stop

*pause
	; スペースキー押下待ち
	repeat
		stick key :if key & 16 :break
		wait 5
	loop
	return

*stgpr
	; 面表示
	color 80,0,0 :boxf 0,0,640,480
	color
	repeat 768
		x=20*(cnt\32) :y=20*(cnt/32)
		if map(cnt)=0 :boxf x,y,x+19,y+19
	loop
	return

*ship
	; 着陸船 表示
	color 160,160,160
	boxf x-8,y-8,x+8,y+10
	boxf x-7,y+8,x-15,y+15
	boxf x+7,y+8,x+15,y+15
	color 0,240,255
	boxf x-3,y-2,x+3,y+3
	color 255,255,0
	stick key
	if (key & 26) and e>1 {
		e=e-2 :gy=gy-2 :if gy<-10 :gy=-10
		boxf x-1,y+13,x+1,y+23+rnd(10)
		line x,y+13,x+rnd(10)-5,y+13+rnd(10)
	}
	if (key & 1) and e>0 {
		e=e-1 :gx=gx+(gx<20)
		boxf x-18,y+11,x-20-rnd(6),y+13
	}
	if (key & 4) and e>0 {
		e=e-1 :gx=gx-(gx>-20)
		boxf x+18,y+11,x+20+rnd(6),y+13
	}
	return

*bomb
	; 着陸船 粉々
	color 160,160,160
	repeat 9
		rx=rnd(30)-15 :ry=rnd(30)-15 :b=1+rnd(6)
		x=px+rx-3 :y=py+ry-3
		boxf x,y,x+b,y+b
	loop
	return

