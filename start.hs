title "TinyPaint"

old_x = -1
old_y = -1
now_x = 0
now_y = 0

	color :boxf 0,0,640,480
	color 255,255,255

repeat

;   1   カーソルキー左(←)
;   2   カーソルキー上(↑)
;   4   カーソルキー右(→)
;   8   カーソルキー下(↓)
;  16   スペースキー
;  32   Enterキー
;  64   Ctrlキー
; 128   ESCキー
; 256   マウスの左ボタン
; 512   マウスの右ボタン
;1024   TABキー

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

