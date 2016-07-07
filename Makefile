HACC_OC_FILES = RestartIO_GLEAN.o testHACC_OPEN_CLOSE.o 
HACC_IO_FILES = RestartIO_GLEAN.o testHACC_IO.o

CXXFLAGS =  -g -O3 -DGLEAN_PRINT_PERROR -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -Wall
LDFLAGS  =  -lstdc++

all: hacc_io hacc_open_close 

hacc_io: $(HACC_IO_FILES)

hacc_open_close: $(HACC_OC_FILES) 

clean:
	-rm -f *.a *.o hacc_io hacc_open_close
