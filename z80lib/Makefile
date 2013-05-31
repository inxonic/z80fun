TARGETS=	
OBJECTS=	crt0.rel getchar.rel


AS=		sdcc-sdasz80


all:		$(OBJECTS) $(TARGETS)

clean:
		$(RM) $(OBJECTS) $(TARGETS) \
		    $(addsuffix .lst,$(basename $(OBJECTS))) \
		    $(addsuffix .sym,$(basename $(OBJECTS)))


%.rel : %.s
		$(AS) $(ASFLAGS) -los $@ $<
