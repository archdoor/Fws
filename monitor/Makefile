CC := g++

MODULE = monitor 

ROOT_DIR := ..
MODULE_DIR := $(ROOT_DIR)/$(MODULE)
UTILS_DIR := $(ROOT_DIR)/utils

CPP_SRC := $(shell find $(MODULE_DIR) -name "*.cpp")
CPP_SRC += $(UTILS_DIR)/log.cpp
CPP_SRC += $(UTILS_DIR)/crc.cpp

OBJECTS := $(patsubst %.cpp,%.o, $(CPP_SRC))

H_SRC := $(shell find $(MODULE_DIR) -name "*.h")
H_SRC += $(UTILS_DIR)/log.h
H_SRC += $(UTILS_DIR)/crc.h
H_DIR := $(addprefix -I, $(sort $(dir $(H_SRC))))

LIB := -lpthread

CFLAGS := -g -Wall -lpthread

TARGET := $(MODULE)

all : clean $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@ $(LIB)

$(OBJECTS) : $(CPP_SRC)
	$(CC) -c $(CFLAGS) $(H_DIR) $(patsubst %.o,%.cpp, $@) -o $@

.PHNOY: clean
clean:
	-rm -rf $(TARGET) $(OBJECTS)
