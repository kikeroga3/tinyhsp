; for cutehspx

; --- Key operation ---
; Cursor key LEFT and RIGHT : Select Command, Select Yes or No
; Cursor key UP and DOWN    : Seting Value(ENERGY/COURSE/SPEED)
; SPACE or ENTER key        : Command execution

	title "TINY TREK"
	gy_m=47 :sdim gyo,256,gy_m
	sdim moj_bf,180

; Font Data
	sdim hex,350
		hex="7EC94A4C7E46108E7EC9111E7EC9124C7E4653C47FE8705C7EE8724C7FE111087EC9324C7EC9384C7EE713"
	hex=hex+"F17E477EB17E2220827F5711D57E8208887EEF91C47EEFFFEE7EC97A527FC9725C7EE8420E7FC94A5C7FE8"
	hex=hex+"7A1E7FE872107EE85A4E7F297A527EE2108E7E210A4C7F2A62927F08421E7F1DD6317F1CD6717EC94A4C7F"
	hex=hex+"C972107EC94ACD7FC972927EE8305C7FF210847F294A4C7F18C5447F18D6AA7F1511517F1510847FF1111F"

; Hex to Bin
	repeat 172
		p1=peek(hex,cnt*2) :p2=peek(hex,cnt*2+1)
		p1=p1-48 :p1=p1-7*(p1>16) :p2=p2-48 :p2=p2-7*(p2>16)
		poke moj_bf,cnt,16*p1+p2
	loop

	dim map,200
;---
*startrek
	color :boxf 0,0,640,480 :redraw 1 :wait 30
	y=2999 :gi=0 :gs=0
	msg="DO YOU WANT A DIFFICULT GAME <Y OR N>" :gosub *lnset :gosub *lnprt
	gosub *y_or_n :if a="Y" or a="y" :y=999

	repeat
		k=0 :b=0 :d=30
		repeat 64
			j=(rnd(99)<4) :b=b+j
			m=rnd(y)+1 :m=(m<209)+(m<99)+(m<49)+(m<24)+(m<9)+(m<2) :k=k+m
			map(cnt)=-100*m-10*j-rnd(8)-1
		loop
		if b>1 and k>3 :break
	loop

	msg="STARDATE 3200 YOUR MISSION IS TO DESTROY "+k+" KLINGONS IN 30 STARDATES" :gosub *lnset
	msg="THERE ARE "+b+" STARBASES" :gosub *lnset
	gosub *repair_ok :c=0 :h=k

*ln40
	u=rnd(8)+1 :v=rnd(8)+1 :x=rnd(8)+1 :y=rnd(8)+1

*ln45
	repeat 82 :map(71+cnt)=0 :loop
	map(8*x+y+62)=4 :m=abs(map(8*u+v-9)) :n=m/100
	i=1 :if n!0 {
		repeat n
			gosub *ln165 :map(cnt+135)=300 :map(cnt+141)=s :map(cnt+147)=t
		loop
	}
	gosub *ln175 :m=m-100*n :i=2 :if (m/10)!0 :gosub *ln165
	m=m-m/10*10 :i=3 :if m!0 :repeat m :gosub *ln165 :loop

*ln65
	gosub *dock :gosub *klingon_atk :if k!0 :goto *ln95
	msg="MISSION ACCOMPLISHED" :gosub *lnset
	if d<3 :msg="BOY YOU BARELY MADE IT" :gosub *lnset
	if d>5 :msg="GOOD WORK" :gosub *lnset
	if d>9 :msg="FANTASTIC" :gosub *lnset
	if d>13 :msg="UNBELIEVABLE" :gosub *lnset
	d=30-d :i=h*100/d*10 :msg=""+h+" KLINGONS IN "+d+" STARDATES <"+i+">" :gosub *lnset
	j=100*(c=0)-5*c:msg=""+c+" CASUALTIES INCURRED <"+j+">" :gosub *lnset
	msg="YOUR SCORE "+(i+j) :gosub *lnset :goto *ln110

*ln95
	if d<0 :msg="IT S TOO LATE THE FEDERATION HAS BEEN CONQUERED" :gosub *lnset :goto *ln110
	if e>=0 :goto *captain
	msg="ENTERPRISE DESTROYED" :gosub *lnset
	if (h-k)>9 :msg="BUT YOU WERE A GOOD MAN" :gosub *lnset
	gosub *lnprt :wait 30

*ln110
	y=987 :msg="ANOTHERE GAME <Y OR N>" :gosub *lnset :gosub *lnprt
	gosub *y_or_n
	if a="Y" or a="y" :goto *startrek
	msg="GOOD BYE" :gosub *lnset :gosub *lnprt :stop
;---
*captain
	msg="CAPTAIN " :gosub *lnset :gosub *lnprt
	gosub *input
	if ci=0 :goto *report
	if ci=1 :goto *sr_sensor
	if ci=2 :goto *lr_sensor
	if ci=3 :goto *galaxy_map
	if ci=4 :goto *phaser
	if ci=6 :goto *torpedo
	if ci=8 :goto *warp

	msg="<<< KEY OPERATION >>>" :gosub *lnset
	msg="CURSOR KEY LEFT AND RIGHT > SELECT COMMAND AND YES OR NO" :gosub *lnset
	msg="CURSOR KEY UP AND DOWN    > SETING VALUE ENERGY COURSE SPEED" :gosub *lnset
	msg="SPACE OR ENTER KEY        > COMMAND EXECUTION" :gosub *lnset
	goto *captain
;---
*dock
	i=x-(x>1) :aa=1
	repeat ((x+(x<8))-i+1) :q=cnt
		j=y-(y>1)
		repeat ((y+(y<8))-j+1)
			if map(8*(i+q)+(j+cnt)+62)=2 :aa=0 :break
		loop
	loop
	if aa :o=0 :return

	if o=0 {
		msg="SULU CAPTAIN WE ARE DOCKED AT STARBASE" :gosub *lnset
	} else {
		return
	}

*repair_ok
	e=4000 :f=10 :o=1
	repeat 7 :map(64+cnt)=0:loop
	return

*ln165
	s=rnd(8)+1 :t=rnd(8)+1 :a=8*s+t+62 :if map(a)!0 :goto *ln165
	map(a)=i :return

*ln175
	msg="ENTERPRISE IN Q "+u+""+v+" S "+x+""+y :gosub *lnset
	return
;---
*galaxy_map
	gosub *ln175 :j=2 :gosub *damaged :if i!0 :goto *captain
	msg=" OF GALAXY MAP" :gosub *lnset
	repeat 8
		i=cnt :z=""+(i+1)+" "
		repeat 8
			m=map(8*i+cnt) :q=((m>0)*m)
			if q<100 :z=z+" "
			if q<10 :z=z+" "
			z=z+" "+q
		loop
		msg=""+z :gosub *lnset
	loop
	z="  " :repeat 8 :z=z+"    " :loop :msg=""+z :gosub *lnset
	z="  " :repeat 8 :z=z+"   "+(cnt+1) :loop :msg=""+z :gosub *lnset
	goto *captain
;---
*lr_sensor
	gosub *ln175 :j=3 :gosub *damaged :if i!0 :goto *captain
	repeat 3 :i=u-1+cnt
		z=""
		repeat 3 :j=v-1+cnt
			m=8*i+j-9 :a=0
			if (i>0)*(i<9)*(j>0)*(j<9) :a=abs(map(m)) :map(m)=a
			if a<100 :z=z+" "
			if a<10 :z=z+" "
			z=z+" "+a
		loop
		msg=""+z :gosub *lnset
	loop
	goto *captain
;---
*sr_sensor
	gosub *ln175 :j=1 :gosub *damaged :if i!0 :goto *captain
	m=8*u+v-9 :map(m)=abs(map(m))
	repeat 8 :i=1+cnt
		z=""+i
		repeat 8
			m=map(8*i+cnt+63) :if m=0 :z=z+"  "
			if m=1 :z=z+" ;"
			if m=2 :z=z+" ?"
			if m=3 :z=z+" @"
			if m=4 :z=z+" :"
		loop
		msg=""+z :gosub *lnset
	loop
	z=" "
	repeat 8 :z=z+" "+(1+cnt) :loop
	msg=""+z :gosub *lnset :goto *captain
;---
*phaser
	j=4 :gosub *damaged :if i!0 :goto *captain
	a=int(pw)
	if a<1 :goto *captain
	if a>e :msg="SPOCK WE HAVE ONLY "+e+" UNITS" :gosub *lnset :goto *captain
	e=e-a :if n<1 :msg="PHASER FIRED AT EMPTY SPACE" :gosub *lnset :goto *ln65
	a=a/n
	repeat 6 :m=135+cnt
		if map(m)!0 {
			gosub *fire :z=" "+s+" UNITS HIT " :gosub *hits
		}
	loop
	goto *ln65

*fire
	if a>1090 :msg="   OVERLOADED" :gosub *lnset :j=4 :map(67)=1 :a=9 :gosub *damaged
	i=map(m+6)-x:j=map(m+12)-y :s=a*30/(30+i*i+j*j)+1 :return

*hits
	z=z+"KLINGON AT S "+map(m+6)+""+map(m+12)
	map(m)=map(m)-s
	if map(m)>0 :msg=""+z+" ==DAMAGED==" :gosub *lnset :return
	map(m)=0 :i=8*u+v-9 :j=map(i)/abs(map(i)) :map(i)=map(i)-100*j :k=k-1
	i=8*map(m+6)+map(m+12)+62 :map(i)=0 :n=n-1 :msg=""+z+" ===DESTROYED===" :gosub *lnset :return

*klingon_atk
	if n=0 :return
	msg="KLINGON ATTACK" :gosub *lnset
	if o!0 :msg="STARBASE PROTECTS ENTERPRISE" :gosub *lnset :return
	t=0
	repeat 6
		m=135+cnt
		if map(m)!0 {
			a=(map(m)+rnd(map(m))+1)/2 :gosub *fire
			t=t+s :i=map(m+6) :j=map(m+12)
			msg=" "+s+" UNITS HIT FROM KLINGON AT S "+i+""+j :gosub *lnset
		}
	loop
	e=e-t :if e<=0 :msg="=== BANG ===" :gosub *lnset :return
	msg=""+e+" UNITS OF ENERGY LEFT" :gosub *lnset
	if (rnd(e/4)+1)>t :return

*casualties
	if map(70)=0 :map(70)=rnd(t/50+1)+1 :j=7 :goto *damaged
	j=rnd(6)+1 :map(j+63)=rnd(t/99+1)+1+map(j+63) :i=rnd(8)+2 :c=c+i
	msg="MC COY SICKBAY TO BRIDGE WE SUFFERED "+i+" CASUALTIES" :gosub *lnset
;---
*damaged
	i=map(j+63)
	if j=1 :z="SHORT RANGE SENSOR"
	if j=2 :z="COMPUTER DISPLAY"
	if j=3 :z="LONG RANGE SENSOR"
	if j=4 :z="PHASER"
	if j=5 :z="WARP ENGINE"
	if j=6 :z="PHOTON TORPEDO TUBES"
	if j=7 :z="SHIELD"
	if i=0 :msg=""+z :gosub *lnset :return
	msg=""+z+" DAMAGED "+i+" STARDATES ESTIMATED FOR REPAIR" :gosub *lnset
	return
;---
*report
	msg="STATUS REPORT>" :gosub *lnset
	msg="STARDATE      "+(3230-d) :gosub *lnset
	msg="TIME LEFT     "+d :gosub *lnset
	z="GREEN"
	if o!0 :z="DOCKED"
	if n!0 :z="RED"
	if e<999 :z="YELLOW"
	msg="CONDITION     "+z :gosub *lnset
	msg="POSITION      Q "+u+""+v+" S "+x+""+y :gosub *lnset
	msg="ENERGY        "+e :gosub *lnset
	msg="TORPEDOES     "+f :gosub *lnset
	msg="KLINGONS LEFT "+k :gosub *lnset
	msg="STARBASES     "+b :gosub *lnset
	repeat 7 :j=1+cnt
		if map(j+63) :gosub *damaged
	loop
	goto *captain
;---
*warp
	j=5 :gosub *damaged

*ln470
	w=int(sp)
	if w<1 :goto *captain
	if i*(w>2) :msg="CHEKOV WE CAN TRY 2 AT MOST SIR" :gosub *lnset :goto *ln470
	if w>91 :w=91 :msg="SPOCK ARE YOU SURE CAPTAIN" :gosub *lnset
	if e<(w*w/2) :msg="SCOTTY SIR WE DO NOT HAVE THE ENERGY" :gosub *lnset :goto *captain
	i=vc :gosub *course :if r=0 :goto *captain
	d=d-1 :e=e-w*w/2 :map(8*x+y+62)=0
	repeat 7 :m=64+cnt :map(m)=(map(m)-1)*(map(m)>0) :loop
	p=45*x+22 :g=45*y+22 :w=45*w

	q=0
	repeat 8
		w=w-r :if w<-22 :q=1 :break
		p=p+s :g=g+t :i=p/45 :j=g/45
		if (i<1)+(i>8)+(j<1)+(j>8) :q=2 :break
		if map(8*i+j+62)=0 :x=i :y=j
	loop
	if q=1 :map(8*x+y+62)=4 :gosub *ln175 :goto *ln65
	if q!2 {
		msg="==EMERGENCY STOP==" :gosub *lnset
		msg="SPOCK TO ERR IS HUMAN" :gosub *lnset
	}
	p=u*72+p/5+w/5*s/r-9 :u=p/72 :g=v*72+g/5+w/5*t/r-9 :v=g/72
	if rnd(9)<1 :msg="===SPACE STORM===" :gosub *lnset :t=100 :gosub *casualties
	if (u>0)*(u<9)*(v>0)*(v<9) :x=(p+9-72*u)/9 :y=(g+9-72*v)/9 :goto *ln45
	msg="==YOU WANDERED OUTSIDE THE GALAXY==" :gosub *lnset
	msg="ON BOARD COMPUTER TAKES OVER AND SAVED YOUR LIFE" :gosub *lnset :goto *ln40
;---
*torpedo
	j=6 :gosub *damaged :if i!0 :goto *captain
	if f=0 :msg=" EMPTY" :gosub *lnset :goto *captain
	msg=" LOADED" :gosub *lnset :i=an :gosub *course :if r=0 :goto *captain
	msg="TORPEDO TRACK " :gosub *lnset :z=""
	f=f-1 :p=45*x+22 :g=45*y+22

	q=0
	repeat 8
		p=p+s :g=g+t :i=p/45 :j=g/45
		if (i<1)+(i>8)+(j<1)+(j>8)=0 {
			l=8*i+j+62 :w=8*u+v-9 :r=map(w)/abs(map(w))
			msg=""+i+""+j+" " :gosub *lnset
			q=map(l) :if q>0 :break
		}
	loop
	if q=1 :goto *ln590
	if q=2 :goto *ln595
	if q=3 :goto *ln600
	if q=4 :goto *ln605
	if q=5 :goto *ln610
	msg="   MISSED" :gosub *lnset :goto *ln65

*ln590
	s=rnd(99)+281
	repeat 6 :m=135+cnt
		if (map(m+6)=i)*(map(m+12)=j) :gosub *hits
	loop
	goto *ln65

*ln595
	b=b-1 :map(l)=0 :map(w)=map(w)-10*r
	msg="STARBASE DESTROYED" :gosub *lnset
	msg="SPOCK I OFTEN FIND HUMAN BEHAVIOUR FASCINATING" :gosub *lnset
	goto *ln65

*ln600
	msg="HIT A STAR" :gosub *lnset
	if rnd(9)<2 :msg="TORPEDO ABSORBED" :gosub *lnset
	goto *ln65

*ln605
	map(l)=0 :map(w)=map(w)-r
	if rnd(9)<5 :msg="STAR DESTROYED" :gosub *lnset
	goto *ln65

*ln610
	t=300
	msg="IT NOVAS    ===RADIATION ALARM===" :gosub *lnset :gosub *casualties
	goto *ln65

*course
	i=int(i)
	if (i>360)+(i<0) :r=0 :return
	s=(i+45)/90 :i=i-s*90 :r=(45+i*i)/110+45 :q=(s<4)*s
	if q=0 :s=-45 :t=i
	if q=1 :s=i :t=45
	if q=2 :s=45 :t=-i
	if q=3 :s=-i :t=-45
	return

*lnprt
	redraw 0
	color :boxf 0,0,640,420 :color 0,200,0
	is=gi-gs :if is<0 :is=is+gy_m
	moj_x=0 :moj_y=0
	repeat gs
		ic=is+cnt :if ic>(gy_m-1) :ic=ic-gy_m
		s1=gyo(ic) :gosub *mojput
	loop
	redraw 1
	return

*lnset
	gyo(gi)=msg :gs=gs+(gs<(gy_m-1)) :gi=gi+1 :if gi>(gy_m-1) :gi=0
	return

*y_or_n
	repeat
		wait 5
		stick key
		if key & 1 :a="Y" :break
		if key & 4 :a="N" :break
	loop
	return

*input
	moj_x=10 :moj_y=425
	redraw 0
	s1="<<< PLEASE USE ONE OF THESE COMMANDS >>>" :gosub *mojput
	color :boxf 10,440,640,480
	cm(0)="REPORT" :cm(1)="SR SENSOR" :cm(2)="LR SENSOR" :cm(3)="GALAXY MAP"
	cm(4)="PHASER" :cm(5)="ENERGY "+pw :cm(6)="TORPEDO"    :cm(7)="COURSE "+an
	cm(8)="WARP"   :cm(9)="SPEED "+sp :cm(10)="COURSE "+vc
	repeat 11 :xx=100*(cnt\4) :yy=10*(cnt/4)
		color 200*(cnt=ci),200,0
		moj_x=10+xx :moj_y=440+yy :s1=cm(cnt) :gosub *mojput
		color 0,200,0
	loop
	redraw 1

	repeat
		wait 10 :stick key
		if key & 1 {
			ci=ci-1 :if ci<0 :ci=10
		}
		if key & 4 {
			ci=ci+1 :if ci>10 :ci=0
		}
		if key & 2 {
			pw=pw+10*(ci=5) :if pw>5000 :pw=5000
			an=an+5*(ci=7) :if an>360 :an=0
			sp=sp+(ci=9) :if sp>100 :sp=100
			vc=vc+5*(ci=10) :if vc>360 :vc=0
		}
		if key & 8 {
			pw=pw-10*(ci=5) :if pw<0 :pw=0
			an=an-5*(ci=7) :if an<0 :an=360
			sp=sp-(ci=9) :if sp<0 :sp=0
			vc=vc-5*(ci=10) :if vc<0 :vc=360
		}
		if key & 63 :break
	loop
	if key & 15 :goto *input
	return

*mojput
	p5=moj_x
	repeat
		p1=peek(s1,cnt) :if p1=0 :break
		if p1=32 :goto *spc
		if p1>47 :p1=p1-48
		p2=p1*4
		p3=16777216*peek(moj_bf,p2)+65536*peek(moj_bf,p2+1)+256*peek(moj_bf,p2+2)+peek(moj_bf,p2+3)
		p4=16777216
		repeat 25
			if (p3&p4) {
				p_x=moj_x+(cnt\5)+5 :p_y=moj_y+(cnt/5)+5
				pset p_x,p_y
			}
			p4=p4/2
		loop
*spc
		moj_x=moj_x+7
	loop
	moj_x=p5 :moj_y=moj_y+9
	return
