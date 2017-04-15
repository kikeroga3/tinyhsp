;redraw 0
color :boxf 0,0,640,480
color 255,255,255
px=20:py=30:sz=4
A=2127133266:ch=A:gosub *dot5
B=2143908444:ch=B:gosub *dot5
C=2129150478:ch=C:gosub *dot5
D=2143898204:ch=D:gosub *dot5
E=2145942046:ch=E:gosub *dot5
F=2145939984:ch=F:gosub *dot5
G=2129156686:ch=G:gosub *dot5
H=2133424722:ch=H:gosub *dot5
I=2128744590:ch=I:gosub *dot5
J=2116094540:ch=J:gosub *dot5
K=2133484178:ch=K:gosub *dot5
L=2131247646:ch=L:gosub *dot5
M=2132661809:ch=M:gosub *dot5
N=2132596337:ch=N:gosub *dot5
O=2127120972:ch=O:gosub *dot5
P=2143908368:ch=P:gosub *dot5
Q=2127121101:ch=Q:gosub *dot5
R=2143908498:ch=R:gosub *dot5
S=2129145948:ch=S:gosub *dot5
T=2146570372:ch=T:gosub *dot5
U=2133412428:ch=U:gosub *dot5
V=2132329796:ch=V:gosub *dot5
W=2132334250:ch=W:gosub *dot5
X=2132087121:ch=X:gosub *dot5
Y=2132086916:ch=Y:gosub *dot5
Z=2146504991:ch=Z:gosub *dot5

px=20:py=80
ch=2127120972:gosub *dot5
ch=2118520974:gosub *dot5
ch=2127106334:gosub *dot5
ch=2127106636:gosub *dot5
ch=2118538180:gosub *dot5
ch=2145939548:gosub *dot5
ch=2129162828:gosub *dot5
ch=2145456392:gosub *dot5
ch=2127114828:gosub *dot5
ch=2127116364:gosub *dot5
redraw 1
stop

*dot5
	a=16777216
	repeat 25
		if (ch & a) {
			x=px+sz*(cnt\5)-(sz*5/2)
			y=py+sz*(cnt/5)-(sz*5/2)
			boxf x,y,x+sz,y+sz
		}
		a=a/2
	loop
	px=px+(sz*5)+2
	return

