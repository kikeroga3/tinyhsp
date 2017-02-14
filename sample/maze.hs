title "MAZE"

	sz=32

	randomize
	color :boxf 4,4,8*sz+7,8*sz+7
	color 255,255,255
	repeat sz :x=cnt*8+4+4
		repeat sz :y=cnt*8+4+4
			boxf x,y,x+3,y+3
			a=rnd(4)
			if a=1 :boxf x,y-4,x+3,y-4+3
			if a=2 :boxf x+4,y,x+4+3,y+3
			if a=3 :boxf x,y+4,x+3,y+4+3
			if a=0 :boxf x-4,y,x-4+3,y+3
		loop
	loop
	stop
