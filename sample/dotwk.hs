; 4x4 DOT WORK

	b=8
	px=320 :py=240 :ch=38646 :gosub *dot
	px=100 :py=100 :ch=28662 :gosub *dot
	px=400 :py=300 :ch=63903 :gosub *dot

	stop

*dot
	a=1
	repeat 16
		if (ch & a) :x=px+b*(cnt\4) :y=py+b*(cnt/4) :boxf x,y,x+b,y+b
		a=a+a
	loop
	return

