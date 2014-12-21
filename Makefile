all: agsender agreceiver

agsender:
	gcc -std=c99 src/main_sender.c src/AudioSink_Alsa.c src/FileSource.c src/BitToSymbol.c src/Repeater.c src/FrequencyModulator.c src/Multiply.c src/FirFilter.c src/agmath.c -o agsender -lm

agreceiver:
	gcc -std=c99 src/main_receiver.c src/AudioSource_Alsa.c src/FileSink.c src/BinarySlicer.c src/ClockRecovery.c src/QuadraturDemodulator.c src/Multiply.c src/FirFilter.c src/agmath.c -o agreceiver -lm

clean:
	rm -f agsender agreceiver
