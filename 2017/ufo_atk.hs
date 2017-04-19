title "UFO ATTACK"

x=320 :y=440	;自機の座標
hp=3			;自機の耐久力
bmx=5			;弾の総数
emx=1			;敵の総数
sc=0			;得点
i=0				;弾の使用No.
ov=0			;ゲームオーバーフラグ

;弾座標用配列変数(16個まではdim宣言不要)
repeat bmx :by(cnt)=-99 :loop

;敵座標用配列変数(16個まではdim宣言不要)
repeat 16 :ey(cnt)=500 :loop

;地面用配列変数(80個使うのでdim宣言)
dim fl,80
repeat 80 :fl(cnt)=1 :loop
fl(0)=0 :fl(79)=0

repeat
	redraw 0

	;全画面消去
	color 0,0,0 :boxf 0,0,639,479

	;地面を描く(変数flの内容が0の部分は描かない)
	color 160,160,160
	repeat 80
		if fl(cnt)=1 :boxf cnt*8,453,cnt*8+7,460
	loop

	stick key

	;自機移動(変数flの内容が0なら移動できない)
	if key&1 {
		if fl(x/8-1)!0 :x=x-8
	}
	if key&4 {
		if fl(x/8+1)!0 :x=x+8
	}
	gosub *own

	;弾発射(初期座標を設定)
	if (key&16)>0 and by(i)<0 {
		bx(i)=x :by(i)=y :i=(i+1)\bmx
	}

	;弾移動(移動と表示)
	repeat bmx :c=cnt
		if by(c)>=0 {
			by(c)=by(c)-16
			color 250,200,0 :boxf bx(c)-1,by(c)-8,bx(c)+1,by(c)+8
		}
	loop

	;敵発生(初期座標を設定)
	r=rnd(16)
	if ey(r)>480 and r<emx {
		ex(r)=rnd(640) :ey(r)=-8
	}
	;敵の移動量を設定
	dx(r)=rnd(16)-8 :dy(r)=rnd(3)+3

	;敵移動(敵座標＋移動量)
	repeat 16 :c=cnt
		if ey(c)<=480 {
			ex(c)=ex(c)+dx(c) :ey(c)=ey(c)+dy(c) :gosub *ufo
			xx=ex(c)/8
			if ey(c)>460 and xx>=0 and xx<80 {
				fl(xx)=0 :ey(c)=500
			}
		}
	loop

	;爆発中
	if oc>0 :gosub *bom

	redraw 1

	;当たり判定(敵と弾)
	repeat bmx :bc=cnt
		repeat emx :ec=cnt

			if abs(ex(ec)-bx(bc))<20 and abs(ey(ec)-by(bc))<16 {
				oc=5 :ox=ex(ec) :oy=ey(ec)
				by(bc)=-99 :ey(ec)=500
				sc=sc+1 :title "SCORE:"+sc
				emx=sc/9+1 :if emx>15 :emx=15
			}

		loop
	loop

	;当たり判定(敵と自機)
	repeat emx
		if abs(ex(cnt)-x)<20 and abs(ey(cnt)-y)<10 {
			oc=5 :ox=ex(ec) :oy=ey(ec)
			hp=hp-1 :ey(cnt)=500 :title "HP:"+hp
			if hp<1 :ov=1
		}
	loop
	if ov=1 :break

	wait 3
loop

title "GAME OVER. YOUR SCORE:"+sc
stop

;自機表示
*own
	color 0,100,250
	boxf x-16,y-5,x+16,y+10 :boxf x-6,y-15,x+6,y
	return

;敵表示
*ufo
	color 250,0,0 :boxf ex(c)-20,ey(c)-5,ex(c)+20,ey(c)+5 :boxf ex(c)-8,ey(c)-10,ex(c)+8,ey(c)+10
	color 255,255,255 :boxf ex(c)-2,ey(c)-2,ex(c)+2,ey(c)+2
	return

;爆発表示
*bom
	color 250,0,0
	repeat oc*10
		rx=rnd(40)-20 :ry=rnd(30)-15
		pset ox+rx,oy+ry
	loop
	oc=oc-1
	return

