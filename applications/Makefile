FILE=covcol.ptile.PRO.c

all:	PRO PRO_util

PRO:
	icc -I ../PRO -qopenmp -O3 -lm covcol.ptile.PRO.c ../PRO/PRO.c -o PRO

PRO_util:
	icc -I ../PRO -qopenmp -O3 -lm covcol.PRO_util.c ../PRO/PRO.c ../PRO/PRO_util.c -o PRO_util

clean:
	rm PRO
	rm PRO_util
