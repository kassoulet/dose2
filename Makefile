CC = ccache gcc
CFLAGS = -O2 -ffast-math `$(SDLPREFIX)sdl-config --cflags`

# -- These are for Mac OS X only, remove for other targets
#SDLPREFIX = /usr/local/bin/
#CFLAGS += -DOSX
# --


# Endianness of the system. Leave commented for MSB systems (PPC/MIPS/etc.)
CFLAGS += -DINTEL

objs = alue.o argb.o blob.o demo.o layer.o line.o main.o obu2d.o ogdecode.o palette.o schaibe.o taso.o

dose2: $(objs)
	gcc -g -o dose2 $(objs) `$(SDLPREFIX)sdl-config --libs` -lm
	#strip dose2

clean:
	rm *.o dose2
