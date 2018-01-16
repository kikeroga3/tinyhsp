; Correct Numbers

	mes "When you run the program, mixes the contents of the sequence 0123456789 is displayed, and it waits for input."
	mes "If you enter a number between 2 and 10, it inverts the sequence by the number specified from the left end."
	mes "Please invert it several times to arrange the sequence in order 0123456789."
	mes "You are quite good if it can be done in 5 times or less."

	nums="0123456789" :tc=0

	; Shake
	repeat 5
		rc=rnd(9)+2 :gosub *invert
	loop

*main
	mes "\n"+nums
	if nums="0123456789" {
		mes "Clear!"
		if tc<5 :mes "Congratulations!"
		end
	}
	input a,2,2
	rc=int(a) :gosub *invert :tc=tc+1
	goto *main

*invert
	repeat rc/2
		p1=peek(nums,cnt)
		p2=peek(nums,rc-1-cnt)
		poke nums,rc-1-cnt,p1
		poke nums,cnt,p2
	loop
	return
