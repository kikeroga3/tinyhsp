; for tinyhspcl

	randomize
	dim map,200
;---
*startrek
	y=2999
	mes "DO YOU WANT A DIFFICULT GAME?  (Y OR N)":input a,1,2
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

	mes "STARDATE 3200:  YOUR MISSION IS TO DESTROY "+k+" KLINGONS IN 30 STARDATES."
	mes "THERE ARE "+b+" STARBASES."
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
	mes "CAPTAIN " :input a,1,2
	if a="R" or a="r" :goto *report
	if a="S" or a="s" :goto *sr_sensor
	if a="L" or a="l" :goto *lr_sensor
	if a="G" or a="g" :goto *galaxy_map
	if a="P" or a="p" :goto *phaser
	if a="T" or a="t" :goto *torpedo
	if a="W" or a="w" :goto *warp

	mes "R=REPORT       S=SR. SENSOR   L=LR. SENSOR"
	mes "G=GALAXY MAP   P=PHASER       T=TORPEDO"
	mes "W=WARP ENGINE  **PLEASE USE ONE OF THESE COMMANDS***"
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
	mes "ENTERPRISE IN Q-"+u+""+v+" S-"+x+""+y
	return
;---
*galaxy_map
	gosub *ln175 :j=2 :gosub *damaged :if i!0 :goto *captain
	mes " OF GALAXY MAP"
	repeat 8
		i=cnt :z=""+(i+1)+":"
		repeat 8
			m=map(8*i+cnt) :q=((m>0)*m)
			if q<100 :z=z+" "
			if q<10 :z=z+" "
			z=z+" "+q
		loop
		mes "\n"+z
	loop
	z="  " :repeat 8 :z=z+"  .." :loop :mes ""+z
	z="  " :repeat 8 :z=z+"   "+(cnt+1) :loop :mes ""+z+"\n"
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
		mes ""+z
	loop
	goto *captain
;---
*sr_sensor
	gosub *ln175 :j=1 :gosub *damaged :if i!0 :goto *captain
	m=8*u+v-9 :map(m)=abs(map(m))
	repeat 8 :i=1+cnt
		z=""+i
		repeat 8
			m=map(8*i+cnt+63) :if m=0 :z=z+" ."
			if m=1 :z=z+" K"
			if m=2 :z=z+" B"
			if m=3 :z=z+" *"
			if m=4 :z=z+" E"
		loop
		mes ""+z
	loop
	z=" "
	repeat 8 :z=z+" "+(1+cnt) :loop
	mes ""+z :goto *captain
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
	mes "MC COY: SICKBAY TO BRIDGE, WE SUFFERED "+i+" CASUALTIES."
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
	IF i=0 :mes ""+z :return
	mes ""+z+" DAMAGED, "+i+" STARDATES ESTIMATED FOR REPAIR"
	return
;---
*report
	mes "STATUS REPORT:"
	mes "STARDATE      "+(3230-D)
	mes "TIME LEFT     "+D
	z="GREEN"
	if o!0 :z="DOCKED"
	if n!0 :z="RED"
	if e<999 :z="YELLOW"
	mes "CONDITION     "+z
	mes "POSITION      Q-"+u+""+v+" S-"+x+""+y
	mes "ENERGY        "+e
	mes "TORPEDOES     "+f
	mes "KLINGONS LEFT "+k
	mes "STARBASES     "+b
	repeat 7 :j=1+cnt
		if map(j+63) :gosub *damaged
	loop
	goto *captain
;---
*warp
	j=5 :gosub *damaged :;if i=0 :mes "\n"	HSPCL‚Å‚Í•s—v

*ln470
	mes "SECTOR DISTANCE" :input w,2,2
	w=int(w)
	if w<1 :goto *captain
	if i*(w>2) :mes "CHEKOV: WE CAN TRY 2 AT MOST, SIR." :goto *ln470
	if w>91 :w=91 :mes "SPOCK: ARE YOU SURE, CAPTAIN?"
	if e<(w*w/2) :mes "SCOTTY: SIR, WE DO NOT HAVE THE ENERGY." :goto *captain
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
		mes "**EMERGENCY STOP**"
		mes "SPOCK: TO ERR IS HUMAN."
	}
	p=u*72+p/5+w/5*s/r-9 :u=p/72 :g=v*72+g/5+w/5*t/r-9 :v=g/72
	if rnd(9)<1 :mes "***SPACE STORM***" :t=100 :gosub *casualties
	if (u>0)*(u<9)*(v>0)*(v<9) :x=(p+9-72*u)/9 :y=(g+9-72*v)/9 :goto *ln45
	mes "**YOU WANDERED OUTSIDE THE GALAXY**"
	mes "ON BOARD COMPUTER TAKES OVER, AND SAVED YOUR LIFE" :goto *ln40
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
	mes "COURSE (0-360)" :input i,3,2
	i=int(i)
	if (i>360)+(i<0) :r=0 :return
	s=(i+45)/90 :i=i-s*90 :r=(45+i*i)/110+45 :q=(s<4)*s
	if q=0 :s=-45 :t=i
	if q=1 :s=i :t=45
	if q=2 :s=45 :t=-i
	if q=3 :s=-i :t=-45
	return

