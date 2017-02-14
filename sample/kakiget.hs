
	color :boxf 0,0,640,480

	x=320 :y=440 :sc=0 :hp=3
	repeat 16 :ky(cnt)=488 :loop

	repeat
		redraw 0
		color :boxf 0,0,640,480

		m=sc/5+5 :if m>30 :m=30


		i=rnd(16)
		if rnd(30)<m and ky(i)>=480 {
			kx(i)=rnd(600) :ky(i)=0 :ks(i)=9+rnd(18) :ka(i)=rnd(2)
		}

		repeat 16
		if ky(cnt)<480 {
			if ka(cnt) :color 64,64,64 :else :color 255,64,0
			boxf kx(cnt)-20,ky(cnt)-20,kx(cnt)+20,ky(cnt)+20
			if ka(cnt)=0 :color 0,240,0 :boxf kx(cnt)-10,ky(cnt)-24,kx(cnt)+10,ky(cnt)-16
			ky(cnt)=ky(cnt)+ks(cnt)
		}

		if abs(x-kx(cnt))<20 and abs(y-ky(cnt))<20 {
			ky(cnt)=488
			if ka(cnt) {
				hp=hp-1
			} else {
				sc=sc+1
			}
		}

		loop

		stick key
		if key=1 {
			x=x-8 :if x<20 :x=20
		}
		if key=4 {
			x=x+8 :if x>620 :x=620
		}

		color 160,80,0
		boxf x-20,y-20,x+20,y+20
		boxf x-28,y-20,x+28,y-12

		title "KAKI="+sc+" LIFE="+hp
		if hp<1 :break

		redraw 1
		wait 3
	loop

stop
