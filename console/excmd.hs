; for cutehspcl bload poke peek test program

;�����^�ϐ�bf��p��
sdim bf,512
bf="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
mes bf

;�o�C�g�l���擾
a=peek(bf,11)
mes "'B'=ASC("+a+")"

;�o�C�g�l��4�o�C�g�߂ɏ�����
poke bf,3,a
mes bf

;98('b')��12�o�C�g�߂ɏ�����
poke bf,11,66+32
mes bf

;�ϐ�bf�Ƀt�@�C�����e��Ǎ���
bload "excmd.hs",bf
mes bf
