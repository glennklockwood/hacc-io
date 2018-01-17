#  HACC-IO
#
#  make CXX=CC fpp - build file-per-process benchmarks (default)
#  make CXX=CC all - build shared file benchmark
#
# -DHACC_IO_FILE_PER_PROCESS to build for file-per-process I/O
# -DHACC_IO_DISABLE_READ to build version that only creates a checkpoint
# -DHACC_IO_DISABLE_WRITE to build version that only reads a checkpoint

HACC_EXE = hacc_io hacc_io_write hacc_io_read hacc_open_close hacc_printinfo
CXXFLAGS =  -g -O3 -DGLEAN_PRINT_PERROR -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -Wall

### Targets - do file-per-process by default
fpp: CXXFLAGS += -DHACC_IO_FILE_PER_PROCESS
fpp: all

all: $(HACC_EXE)

hacc_io: restartio_glean.o testhacc_io.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

hacc_io_write: restartio_glean.o testhacc_io_write.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

hacc_io_read: restartio_glean.o testhacc_io_read.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

hacc_open_close: restartio_glean.o testhacc_open_close.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

hacc_printinfo: restartio_glean.o testhacc_printinfo.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

testhacc_io_read.o: testhacc_io.cc always
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DHACC_IO_DISABLE_WRITE -c $< -o $@

testhacc_io_write.o: testhacc_io.cc always
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DHACC_IO_DISABLE_READ -c $< -o $@

testhacc_printinfo.o: testhacc_printinfo.cc always
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f *.a *.o $(HACC_EXE)

.PHONY: always
always:
