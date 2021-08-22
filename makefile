all:license

HDRS:=macro.h log.h misc.h
CFLAGS:=-Wall -g

sec:sec.c misc.c ${HDRS}
	gcc ${CFLAGS} $(filter %.c,$^) -o $@

license:license.c misc.c io.c ${HDRS} io.h
	gcc ${CFLAGS} $(filter %.c,$^) -o $@

.PHONY:clean

clean:
	rm -f a.out sec license