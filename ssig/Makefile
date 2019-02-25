CC=gcc
CXX=g++
RM=rm -f
SRC_DIR := ssiglib
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS=-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio -lopencv_video

SRCS=main.cpp
OBJS=$(subst .cc,.o,$(SRCS))

all: ssig

ssig: $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $<

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) tool
