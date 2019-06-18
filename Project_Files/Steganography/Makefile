
CFLAGS= -g -Wall

Stego:
	gcc Stego.c -c $(CFLAGS)
	gcc image.c -c $(CFLAGS)
	gcc Stego.o image.o -o Stego $(CFLAGS)

StegoExtract:
	gcc StegoExtract.c -c $(CFLAGS)
	gcc image.c -c $(CFLAGS)
	gcc StegoExtract.o image.o -o StegoExtract $(CFLAGS)

clean:
	rm -f Stego
	rm -f Stego.o
	rm -f image.o
	rm -f StegoExtract.o
	rm -f StegoExtract
	rm -f Stego1.pgm
	rm -f Stego2.pgm
	cp Img1.pgm Stego1.pgm
	cp Img2.pgm Stego2.pgm
