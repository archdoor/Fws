CC := g++

MODULE = interface

ROOT_DIR := ..
SRC_DIR := .
UTILS_DIR := $(ROOT_DIR)/utils
COMMON_DIR := $(ROOT_DIR)/common

CPP_SRC := $(shell find $(SRC_DIR) -name "*.cpp")
CPP_SRC += $(shell find $(UTILS_DIR) -name "*.cpp")
CPP_SRC += $(shell find $(COMMON_DIR) -name "*.cpp")

H_SRC := $(shell find $(SRC_DIR) -name "*.h")
H_SRC += $(shell find $(UTILS_DIR) -name "*.h")
H_SRC += $(shell find $(COMMON_DIR) -name "*.h")

H_DIR := $(addprefix -I, $(sort $(dir $(H_SRC))))
H_DIR += -I/usr/include/mysql
OBJECTS := $(patsubst %.cpp,%.o, $(CPP_SRC))

LIB := -lpthread
LIB += -lmysqlclient

CFLAGS := -g -Wall
CFLAGS += -DDEBUG

TARGET := $(MODULE)

all : clean $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@ $(LIB)

$(OBJECTS) : $(CPP_SRC)
	$(CC) -c $(CFLAGS) $(H_DIR) $(patsubst %.o,%.cpp, $@) -o $@

.PHNOY: clean
clean:
	-rm -rf $(TARGET) $(OBJECTS)
