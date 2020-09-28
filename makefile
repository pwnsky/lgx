# This is a makefile for complie lgx server
# Create time: 2020-08-26
# Author: i0gan

CC      := g++ 
TARGET  := lgx
CFLAGS  := -O3 -std=c++11 --static
LDFLAGS := -lpthread
RM := rm -f 
CP := cp -r
MKDIR := mkdir

BUILD_PATH := ./bin

# install path
INSTALL_PATH := /usr/share/lgx

# src path
NET_PATH    :=  ./src/net
THREAD_PATH :=  ./src/thread
WORK_PATH   :=  ./src/work
LOG_PATH    :=  ./src/log
JSON_PATH   :=  ./src/json
UTIL_PATH   :=  ./src/util
MAIN_PATH   :=  ./src


#---------------------OBJ-------------------------
OBJS :=
# main
MAIN_SRC := $(wildcard $(MAIN_PATH)/*.cc)  
MAIN_OBJ := $(patsubst %.cc, %.o, $(MAIN_SRC)) 
OBJS += $(MAIN_OBJ)

# util src
UTIL_SRC := $(wildcard $(UTIL_PATH)/*.cc)  
UTIL_OBJ := $(patsubst %.cc, %.o, $(UTIL_SRC)) 
OBJS += $(UTIL_OBJ)

# net src
NET_SRC := $(wildcard $(NET_PATH)/*.cc)  
NET_OBJ := $(patsubst %.cc, %.o, $(NET_SRC)) 
OBJS += $(NET_OBJ)

# thread src
THREAD_SRC := $(wildcard $(THREAD_PATH)/*.cc)  
THREAD_OBJ := $(patsubst %.cc, %.o, $(THREAD_SRC)) 
OBJS += $(THREAD_OBJ)

# work src
WORK_SRC := $(wildcard $(WORK_PATH)/*.cc)  
WORK_OBJ := $(patsubst %.cc, %.o, $(WORK_SRC)) 
OBJS += $(WORK_OBJ)

# log src
LOG_SRC := $(wildcard $(LOG_PATH)/*.cc)  
LOG_OBJ := $(patsubst %.cc, %.o, $(LOG_SRC)) 
OBJS += $(LOG_OBJ)



# complie
$(TARGET):$(OBJS)
	@echo -e "\033[33m\t linking all \033[0m"
	$(CC) $^ -o $(BUILD_PATH)/$@ $(LDFLAGS) $(CFLAGS) 
	@echo -e "\033[34m\t finished \033[0m"

$(OBJS):%.o:%.cc
	$(CC) -c $^ -o $@

print:
	@echo $(OBJS)	

clean:
	$(RM) $(BUILD_PATH)/$(TARGET) $(OBJS)
	$(RM) $(BUILD_PATH)/lgx.log

install:
	@sudo $(MKDIR) $(INSTALL_PATH)
	@sudo $(MKDIR) /etc/$(TARGET)
	@sudo $(CP) $(BUILD_PATH)/$(TARGET) $(INSTALL_PATH)
	@sudo $(CP) $(BUILD_PATH)/etc/config.json /etc/$(TARGET)
	@ln -s $(INSTALL_PATH)/$(TARGET) /usr/bin/$(TARGET)
	@echo 'install complete'
