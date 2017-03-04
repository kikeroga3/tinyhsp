title "TinyPaint"

old_x = -1
old_y = -1
now_x = 0
now_y = 0

repeat
	stick key
	if key & 256 {
		now_x = mousex
		now_y = mousey
		line now_x, now_y, old_x, old_y
		old_x = now_x
		old_y = now_y
	} else {
		old_x = mousex
		old_y = mousey
	}
	wait 5
loop

stop
