; THE DICE

	title "HIT SPACE KEY or MOUSE CLICK!"
	repeat
		stick key
		if key & 272 :gosub *dice
		wait 10
	loop
	stop

*dice
	color :boxf 0,0,640,480
	color 255,255,255 :boxf 240,160,400,320
	r=rnd(6)+1
	if r=1 :color 255,0,0 :boxf 304,224,336,256
	color
	if r>1 :boxf 360,180,380,200 :boxf 260,280,280,300
	if r=3 or r=5 :boxf 310,230,330,250
	if r>3 :boxf 260,180,280,200 :boxf 360,280,380,300
	if r=6 :boxf 260,230,280,250 :boxf 360,230,380,250
	return

