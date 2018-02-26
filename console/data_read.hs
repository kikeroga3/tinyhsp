; Store multiple values in array variables

	sdim data,1000
	dim n,256

	; n(i) <- data
	data="0,0,-50,2,-40,0,50,2,40,0,50,2,0,0,-50,2,0,20,50,2,20,0,50,2,0,0,-50,2,-20,0,50,2,0,20,50,3,"

	gosub *data_read

	repeat p3
		mes "n("+cnt+")="+n(cnt)
	loop
	mes "Total number:"+p3

	end

*data_read
	s1="String"
	p1=0 :p2=0 :p3=0
*data_rep
	a=peek(data,p1) :if a=0 :return
	if a=44 {
		poke s1,p2,0 :p2=0 :n(p3)=int(s1) :p3=p3+1
	} else {
		poke s1,p2,a :p2=p2+1
	}
	p1=p1+1
	goto *data_rep
