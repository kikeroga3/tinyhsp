; Camera vector is fixed at 0,0,1 (see the back from the center)

	; Read data of 3D
	dim ob,256
	sdim bf,1000

	bf="0,0,-50,2,-40,0,50,2,40,0,50,2,0,0,-50,2,0,20,50,2,20,0,50,2,0,0,-50,2,-20,0,50,2,0,20,50,3,"
	gosub *data_read

	; Initial values
	PI=3.14159265358979323846
	argp=PI*30.0/180.0
	tanp=sin(argp)/cos(argp)
	scx=640.0 :scy=480.0
	schx=scx/2.0 :schy=scy/2.0
	ang=0.0
	ox=0.0 :oy=0.0 :oz=200.0

*main
	redraw 0
	color 0,0,0: boxf 0,0,640,480
	color 255,255,255 :gosub *obj_draw
	redraw 1 :wait 1
	ang=ang+0.5 :if ang>360.0 :ang=0.0
	goto *main

*obj_draw
	vx=0.9 :vy=0.6 :vz=0.3		; Rotational axis vector
	va=PI*ang/180.0				; Angle to radians
	n=1 :i=0
*obj_rep
	mpx=double(ob(i)) :mpy=double(ob(i+1)) :mpz=double(ob(i+2))
	gosub *xyz_rotate
	p1=x1+ox :p2=y1+oy :p3=z1+oz
	gosub *trans3Dto2D :xx2=dx :yy2=dy
	if n=3 :return
	if n>1 :line xx1,yy1,xx2,yy2		; Drawing
	n=ob(i+3)
	xx1=xx2 :yy1=yy2 :i=i+4
	goto *obj_rep

; Convert 3D(p1,p2,p3) to 2D(dx,dy)
*trans3Dto2D
	if p3=0.0 :p3=1.0
	dx1=(schx*p1*scy+p3*schx*scx*tanp)/(scx*tanp)
	dy1=(p3*schy*tanp-schy*p2)/tanp
	dx=int(dx1/p3) :dy=int(dy1/p3)
	return

; Spatial rotation operation around an arbitrary axis
*xyz_rotate
	r=sqrt(vx*vx+vy*vy+vz*vz)
	ax=vx/r :ay=vy/r :az=vz/r
	sin1=sin(va) :cos1=cos(va) :l_cos1=1.0-cos1
	x1=(ax*ax*l_cos1+cos1)*mpx+(ax*ay*l_cos1-az*sin1)*mpy+(az*ax*l_cos1+ay*sin1)*mpz
	y1=(ax*ay*l_cos1+az*sin1)*mpx+(ay*ay*l_cos1+cos1)*mpy+(ay*az*l_cos1-ax*sin1)*mpz
	z1=(az*ax*l_cos1-ay*sin1)*mpx+(ay*az*l_cos1+ax*sin1)*mpy+(az*az*l_cos1+cos1)*mpz
	return

; Storing multiple values in array variables
*data_read
	s1="String"
	p1=0 :p2=0 :p3=0
*data_rep
	a=peek(bf,p1) :if a=0 :return
	if a=44 {
		poke s1,p2,0 :p2=0 :ob(p3)=int(s1) :p3=p3+1
	} else {
		poke s1,p2,a :p2=p2+1
	}
	p1=p1+1
	goto *data_rep

