	title "TINY TREK"
	color :boxf 0,0,640,480
	font "tiny_en.ttf",20 :color 0,200,0

	randomize
	dim map,200
	sdim gyo,256,20
;---
*startrek
	y=2999 :gi=0 :gs=0 :redraw 1
	msg="DO YOU WANT A DIFFICULT GAME?  (Y OR N)" :gosub *lnset :gosub *lnprt
	gosub *y_or_n
	if a="Y" or a="y" :y=999

	repeat
		k=0 :b=0 :d=30
		repeat 64
			j=(rnd(99)<4) :b=b+j
			m=rnd(y)+1 :m=(m<209)+(m<99)+(m<49)+(m<24)+(m<9)+(m<2) :k=k+m
			map(cnt)=-100*m-10*j-rnd(8)-1
		loop
		if b>1 and k>3 :break
	loop

	msg="STARDATE 3200:  YOUR MISSION IS TO DESTROY "+k+" KLINGONS IN 30 STARDATES." :gosub *lnset
	msg="THERE ARE "+b+" STARBASES." :gosub *lnset
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
	mes "\n" :mes "MISSION ACCOMPLISHED."
	if d<3 :mes "BOY, YOU BARELY MADE IT."
	if d>5 :mes "GOOD WORK..."
	if d>9 :mes "FANTASTIC!"
	if d>13 :mes "UNBELIEVABLE!"
	d=30-d :i=h*100/d*10 :mes ""+H+" KLINGONS IN "+d+" STARDATES. ("+i+")"
	j=100*(c=0)-5*c:mes ""+c+" CASUALTIES INCURRED. ("+j+")"
	mes "YOUR SCORE:"+(i+j):goto *ln110

*ln95
	if d<0 :mes "IT'S TOO LATE, THE FEDERATION HAS BEEN CONQUERED.":goto *ln110
	if e>=0 :goto *captain
	mes "ENTERPRISE DESTROYED"
	if (h-k)>9 :mes "BUT YOU WERE A GOOD MAN"

*ln110
	y=987:mes "\nANOTHERE GAME? (Y OR N)" :input a,1,2
	if a="Y" or a="y" :goto *startrek
	mes "GOOD BYE." :stop
;---
*captain
	msg="CAPTAIN " :gosub *lnset :gosub *lnprt
	gosub *input
	if ci=0 :goto *report
	if ci=1 :goto *sr_sensor
	if ci=2 :goto *lr_sensor
	if ci=3 :goto *galaxy_map
	if a="P" or a="p" :goto *phaser
	if a="T" or a="t" :goto *torpedo
	if ci=8 :goto *warp

	msg="--- Key operation ---" :gosub *lnset
	msg="Cursor key LEFT and RIGHT : Select Command, Select Yes or No" :gosub *lnset
	msg="Cursor key UP and DOWN    : Seting Value(ENERGY/COURSE/SPEED)" :gosub *lnset
	msg="SPACE or ENTER key        : Command execution" :gosub *lnset
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
		mes "SULU: CAPTAIN, WE ARE DOCKED AT STARBASE."
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
	msg="ENTERPRISE IN Q-"+u+""+v+" S-"+x+""+y :gosub *lnset
	return
;---
*galaxy_map
	gosub *ln175 :j=2 :gosub *damaged :if i!0 :goto *captain
	msg=" OF GALAXY MAP" :gosub *lnset
	repeat 8
		i=cnt :z=""+(i+1)+":"
		repeat 8
			m=map(8*i+cnt) :q=((m>0)*m)
			if q<100 :z=z+" "
			if q<10 :z=z+" "
			z=z+" "+q
		loop
		msg=""+z :gosub *lnset
	loop
	z="  " :repeat 8 :z=z+"  .." :loop :msg=""+z :gosub *lnset
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
			if m=1 :z=z+" K"
			if m=2 :z=z+" B"
			if m=3 :z=z+" *"
			if m=4 :z=z+" E"
		loop
		msg=""+z :gosub *lnset
	loop
	z=" "
	repeat 8 :z=z+" "+(1+cnt) :loop
	msg=""+z :gosub *lnset :goto *captain
;---
*phaser
	j=4 :gosub *damaged :if i!0 :goto *captain
	mes " ENERGIZED. UNITS TO FIRE" :input a,4,2
	a=int(a)
	if a<1 :goto *captain
	if a>e :mes "SPOCK: WE HAVE ONLY "+e+" UNITS." :goto *captain
	e=e-a :if n<1 :mes "PHASER FIRED AT EMPTY SPACE." :goto *ln65
	a=a/n
	repeat 6 :m=135+cnt
		if map(m)!0 {
			gosub *fire :z=" "+s+" UNITS HIT " :gosub *hits
		}
	loop
	goto *ln65

*fire
	if a>1090 :mes "...OVERLOADED.." :j=4 :map(67)=1 :a=9 :gosub *damaged
	i=map(m+6)-x:j=map(m+12)-y :s=a*30/(30+i*i+j*j)+1 :return

*hits
	z=z+"KLINGON AT S-"+map(m+6)+""+map(m+12)
	map(m)=map(m)-s
	if map(m)>0 :mes ""+z+" **DAMAGED**" :return
	map(m)=0 :i=8*u+v-9 :j=map(i)/abs(map(i)) :map(i)=map(i)-100*j :k=k-1
	i=8*map(m+6)+map(m+12)+62 :map(i)=0 :n=n-1 :mes ""+z+" ***DESTROYED***" :return

*klingon_atk
	if n=0 :return
	mes "KLINGON ATTACK"
	if o!0 :mes "STARBASE PROTECTS ENTERPRISE" :return
	t=0
	repeat 6
		m=135+cnt
		if map(m)!0 {
			a=(map(m)+rnd(map(m))+1)/2 :gosub *fire
			t=t+s :i=map(m+6) :j=map(m+12)
			mes " "+s+" UNITS HIT FROM KLINGON AT S-"+i+""+j
		}
	loop
	e=e-t :if e<=0 :mes "*** BANG ***" :return
	mes ""+e+" UNITS OF ENERGY LEFT."
	if (rnd(e/4)+1)>t :return

*casualties
	if map(70)=0 :map(70)=rnd(t/50+1)+1 :j=7 :goto *damaged
	j=rnd(6)+1 :map(j+63)=rnd(t/99+1)+1+map(j+63) :i=rnd(8)+2 :c=c+i
	msg="MC COY: SICKBAY TO BRIDGE, WE SUFFERED "+i+" CASUALTIES." :gosub *lnset
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
	msg=""+z+" DAMAGED, "+i+" STARDATES ESTIMATED FOR REPAIR" :gosub *lnset
	return
;---
*report
	msg="STATUS REPORT:" :gosub *lnset
	msg="STARDATE      "+(3230-d) :gosub *lnset
	msg="TIME LEFT     "+d :gosub *lnset
	z="GREEN"
	if o!0 :z="DOCKED"
	if n!0 :z="RED"
	if e<999 :z="YELLOW"
	msg="CONDITION     "+z :gosub *lnset
	msg="POSITION      Q-"+u+""+v+" S-"+x+""+y :gosub *lnset
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
	j=5 :gosub *damaged :;if i=0 :mes "\n"	HSPCL‚Å‚Í•s—v

*ln470
;	mes "SECTOR DISTANCE" :input w,2,2
	w=int(sp)
	if w<1 :goto *captain
	if i*(w>2) :msg="CHEKOV: WE CAN TRY 2 AT MOST, SIR." :gosub *lnset:goto *ln470
	if w>91 :w=91 :msg="SPOCK: ARE YOU SURE, CAPTAIN?" :gosub *lnset
	if e<(w*w/2) :msg="SCOTTY: SIR, WE DO NOT HAVE THE ENERGY." :gosub *lnset :goto *captain
	gosub *course:if r=0 :goto *captain
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
		msg="**EMERGENCY STOP**" :gosub *lnset
		msg="SPOCK: TO ERR IS HUMAN." :gosub *lnset
	}
	p=u*72+p/5+w/5*s/r-9 :u=p/72 :g=v*72+g/5+w/5*t/r-9 :v=g/72
	if rnd(9)<1 :msg="***SPACE STORM***" :gosub *lnset :t=100 :gosub *casualties
	if (u>0)*(u<9)*(v>0)*(v<9) :x=(p+9-72*u)/9 :y=(g+9-72*v)/9 :goto *ln45
	msg="**YOU WANDERED OUTSIDE THE GALAXY**" :gosub *lnset
	msg="ON BOARD COMPUTER TAKES OVER, AND SAVED YOUR LIFE" :gosub *lnset :goto *ln40
;---
*torpedo
	j=6 :gosub *damaged :if i!0 :goto *captain
	if f=0 :mes " EMPTY" :goto *captain
	mes " LOADED" :gosub *course :if r=0 :goto *captain
	mes "TORPEDO TRACK " :z=""
	f=f-1 :p=45*x+22 :g=45*y+22

	q=0
	repeat 8
		p=p+s :g=g+t :i=p/45 :j=g/45
		if (i<1)+(i>8)+(j<1)+(j>8)=0 {
			l=8*i+j+62 :w=8*u+v-9 :r=map(w)/abs(map(w))
			mes ""+i+""+j+" "
			q=map(l) :if q>0 :break
		}
	loop
	if q=1 :goto *ln590
	if q=2 :goto *ln595
	if q=3 :goto *ln600
	if q=4 :goto *ln605
	if q=5 :goto *ln610
	mes "...MISSED" :goto *ln65

*ln590
	s=rnd(99)+281
	repeat 6 :m=135+cnt
		if (map(m+6)=i)*(map(m+12)=j) :gosub *hits
	loop
	goto *ln65

*ln595
	b=b-1 :map(l)=0 :map(w)=map(w)-10*r
	mes "STARBASE DESTROYED"
	mes "SPOCK: I OFTEN FIND HUMAN BEHAVIOUR FASCINATING."
	goto *ln65

*ln600
	mes "HIT A STAR"
	if rnd(9)<2 :mes "TORPEDO ABSORBED"
	goto *ln65

*ln605
	map(l)=0 :map(w)=map(w)-r
	if rnd(9)<5 :mes "STAR DESTROYED"
	goto *ln65

*ln610
	t=300
	mes "IT NOVAS    ***RADIATION ALARM***" :gosub *casualties
	goto *ln65

*course
;	mes "COURSE (0-360)" :input i,3,2
	i=int(vc)
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
	is=gi-gs :if is<0 :is=is+20
	pos 0,0
	repeat gs
		ic=is+cnt :if ic>19 :ic=ic-20
		mes gyo(ic)
	loop
	redraw 1
	return

*lnset
	gyo(gi)=msg :gs=gs+(gs<19) :gi=gi+1 :if gi>19 :gi=0
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
	pos 10,400
	mes "***PLEASE USE ONE OF THESE COMMANDS***"
	repeat
		redraw 0 :color :boxf 10,420,640,480
		cm(0)="REPORT" :cm(1)="SR. SENSOR" :cm(2)="LR. SENSOR" :cm(3)="GALAXY MAP"
		cm(4)="PHASER" :cm(5)="ENERGY:"+pw :cm(6)="TORPEDO"    :cm(7)="COURSE:"+an
		cm(8)="WARP"   :cm(9)="SPEED:"+sp :cm(10)="COURSE:"+vc
		repeat 11 :xx=100*(cnt\4) :yy=16*(cnt/4)
			color 200*(cnt=ci),200,0
			pos 10+xx,420+yy :mes cm(cnt)
			color 0,200,0
		loop
		wait 5
		stick key
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
		if key & 48 :break
		redraw 1
	loop
	return
