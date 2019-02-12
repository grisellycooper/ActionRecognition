CC=gcc
CXX=g++
RM=rm -f
LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

SRCS=main.cpp
OBJS=$(subst .cc,.o,$(SRCS))

all: recognition

recognition: $(OBJS)
	$(CXX) -o recognition.out $(OBJS) $(LDFLAGS)

recognition.o: main.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) tool
