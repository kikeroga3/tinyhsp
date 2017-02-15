title "THE MAZE"

xs=38 :ys=28 :bs=8
randomize
color :boxf bs,bs,bs+xs*bs*2+bs-1,bs+ys*bs*2+bs-1
color 255,255,255
repeat xs :x=bs+cnt*bs*2+bs
	repeat ys :y=bs+cnt*bs*2+bs
		boxf x,y,x+bs-1,y+bs-1
		a=rnd(4)
		if a=0 :boxf x,y-bs,x+bs-1,y-bs+bs-1
		if a=1 :boxf x+bs,y,x+bs+bs-1,y+bs-1
		if a=2 :boxf x,y+bs,x+bs-1,y+bs+bs-1
		if a=3 :boxf x-bs,y,x-bs+bs-1,y+bs-1
	loop
loop
stop
