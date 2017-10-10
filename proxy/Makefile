CC := g++

MODULE = proxy 

ROOT_DIR := ..
MODULE_DIR := $(ROOT_DIR)/$(MODULE)
UTILS_DIR := $(ROOT_DIR)/utils
COMMON_DIR := $(ROOT_DIR)/common

CPP_SRC := $(shell find $(MODULE_DIR) -name "*.cpp")
CPP_SRC += $(shell find $(UTILS_DIR) -name "*.cpp")
CPP_SRC += $(shell find $(COMMON_DIR) -name "*.cpp")
OBJECTS := $(patsubst %.cpp,%.o, $(CPP_SRC))

H_SRC := $(shell find $(MODULE_DIR) -name "*.h")
H_SRC += $(shell find $(UTILS_DIR) -name "*.h")
H_SRC += $(shell find $(COMMON_DIR) -name "*.h")
H_DIR := $(addprefix -I, $(sort $(dir $(H_SRC))))

LIB := -lpthread
LIB += -lmysqlclient

CFLAGS := -g -Wall

TARGET := $(MODULE)

all : clean $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $^ -o $@ $(LIB)

$(OBJECTS) : $(CPP_SRC)
	$(CC) -c $(CFLAGS) $(H_DIR) $(patsubst %.o,%.cpp, $@) -o $@

.PHNOY: clean
clean:
	-rm -rf $(TARGET) $(OBJECTS)
