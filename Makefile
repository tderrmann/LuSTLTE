.PHONY: all makefiles clean

all:
	+make -C inet
	+make -C simulte
	+make -C veins

makefiles:
	make -C inet makefiles
	make -C simulte makefiles
	cd veins && ./configure --with-inet=../inet --with-simulte=../simulte

clean:
	+make -C inet clean
	+make -C simulte clean
	+make -C veins clean

