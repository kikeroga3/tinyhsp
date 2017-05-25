	title "LineDraw"

	lfnm=""				;ロードするファイル名(""なら新規)
	sfnm="grp001.bin"	;セーブするファイル名

	sdim linbf,64000
	adr=0 :sta=0 :x1=-1 :y1=-1

	if lfnm!"" {
		bload lfnm,linbf :gosub *draw
	}

; メインルーチン
repeat
	stick key
	if key & 256 :gosub *lset
	if key & 512 {
		x1=-1 :title "LINE START"
	}
	if key=128 {
		bsave sfnm,linbf
		title "Save "+sfnm
	}
	wait 10
loop
stop

; 線を描き、データもセット
*lset
	x2=mousex :y2=mousey
	if x1=-1 {
		adr=adr-3*sta
		x1=x2 :y1=y2
		poke linbf,adr,255
		poke linbf,adr+1,x1/3
		poke linbf,adr+2,y1/2
		adr=adr+3 :sta=1
		title "LINE:"+x1+","+y1
	}
	if (x1!x2) or (y1!y2) {
		line x1,y1,x2,y2
		poke linbf,adr,x2/3
		poke linbf,adr+1,y2/2
		adr=adr+2 :sta=0
		title "LINE:"+x1+","+y1+"-"+x2+","+y2
		x1=x2 :y1=y2
	}
	return

; データから絵を描く
*draw
	repeat
		p=peek(linbf,adr)
		if p=0 :break
		if p=255 {
			x1=peek(linbf,adr+1)*3
			y1=peek(linbf,adr+2)*2
			adr=adr+3
		} else {
			x2=peek(linbf,adr)*3
			y2=peek(linbf,adr+1)*2
			line x1,y1,x2,y2
			x1=x2 :y1=y2
			adr=adr+2
		}
	loop
	return
