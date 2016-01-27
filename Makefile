.PHONY: all makefiles clean

all:
	+make -C inet
	+make -C simulte
	+make -C veins
	+make -C common
	+make -C DwellTimeStudy
	+make -C MinimumExample
	+make -C WardropLTE

makefiles:
	make -C inet makefiles
	make -C simulte makefiles
	cd veins && ./configure --with-inet=../inet --with-simulte=../simulte
	make -C common makefiles
	make -C DwellTimeStudy makefiles
	make -C MinimumExample makefiles
	make -C WardropLTE makefiles

clean:
	+make -C inet clean
	+make -C simulte clean
	+make -C veins clean
	+make -C common clean
	+make -C DwellTimeStudy clean
	+make -C MinimumExample clean
	+make -C WardropLTE clean
