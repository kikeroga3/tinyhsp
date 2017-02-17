title "DIGITAL NUMBER"

	color :boxf 0,0,640,480

	b=10 :x=300 :y=230
	color 255,0,0
	p=2+4			:gosub *suji	; 1を表示
	p=1+2+64+16+8	:gosub *suji	; 2を表示
	p=32+64+2+4		:gosub *suji	; 4を表示

	b=20 :x=100 :y=150
	color 80,255,0
	p=1+2+4+8+16+32+64	:gosub *suji	; 8を表示
	p=1+4+8+32+64		:gosub *suji	; 5を表示

	stop

*suji
	if p & 1 :line x,y,x+b,y
	if p & 2 :line x+b,y,x+b,y+b
	if p & 4 :line x+b,y+b,x+b,y+b+b
	if p & 8 :line x+b,y+b+b,x,y+b+b
	if p & 16 :line x,y+b+b,x,y+b
	if p & 32 :line x,y+b,x,y
	if p & 64 :line x,y+b,x+b,y+b
	x=x+b+(b/2)
	return

