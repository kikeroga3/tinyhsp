; Random Stars

color :boxf 0,0,640,480
repeat 300
	title "Star="+(1+cnt)
	r=rnd(256) :g=rnd(256) :b=rnd(256)
	color r,g,b
	x=rnd(640) :y=rnd(480)
	pset x,y
	wait 1
loop

stop
