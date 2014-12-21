CC=gcc
CFLAGS=-std=c99
LIBS=-lm
SRCDIR=src

SHAREDMODULES = Multiply FirFilter agmath
SENDERMODULES = main_sender AudioSink_Alsa FileSource BitToSymbol Repeater FrequencyModulator $(SHAREDMODULES)
RECEIVERMODULES= main_receiver AudioSource_Alsa FileSink BinarySlicer ClockRecovery QuadraturDemodulator $(SHAREDMODULES)

SENDEROBJ = $(foreach module, $(SENDERMODULES), $(SRCDIR)/$(module).o)
RECEIVEROBJ = $(foreach module, $(RECEIVERMODULES), $(SRCDIR)/$(module).o)

all: agsender agreceiver

agsender: $(SENDEROBJ)
	$(CC) $(CFLAGS) $(SENDEROBJ) -o agsender $(LIBS)

agreceiver: $(RECEIVEROBJ)
	$(CC) $(CFLAGS) $(RECEIVEROBJ) -o agreceiver $(LIBS)


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f agsender agreceiver $(SRCDIR)/*.o
