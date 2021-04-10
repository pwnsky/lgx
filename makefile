# This is a makefile for complie lgx server
# Create time: 2020-08-26
# Author: i0gan

GCC     := gcc
CC      := g++ 
CFLAGS  := -O3 -std=c++14 
INCLUDE := -Isrc
LDFLAGS := -lpthread -lmysqlclient -lcrypto -lssl -ldl -lcurses

#LDFLAGS += -lmariadb
RM      := rm -rf 
CP      := cp -r
MKDIR   := mkdir -p
BUILD_PATH   := ./bin
INSTALL_PATH := /usr/bin
CONFIG_PATH  := /etc/lgx
WWW_PATH     := /var/www
#DEBUGFLAGS := -DDEBUG -g

# src path
NET_PATH    :=  ./src/net
THREAD_PATH :=  ./src/thread
WORK_PATH   :=  ./src/work
LOG_PATH    :=  ./src/log
THIRD_PATH  :=  ./src/third
UTIL_PATH   :=  ./src/util
SECURITY_PATH := ./src/security
MYSQL_PATH  := ./src/util/mysql
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

# security src
SECURITY_SRC := $(wildcard $(SECURITY_PATH)/*.cc)  
SECURITY_OBJ := $(patsubst %.cc, %.o, $(SECURITY_SRC)) 
OBJS += $(SECURITY_OBJ)

# util mysql src
MYSQL_SRC := $(wildcard $(MYSQL_PATH)/*.cc)  
MYSQL_OBJ := $(patsubst %.cc, %.o, $(MYSQL_SRC)) 
OBJS += $(MYSQL_OBJ)

# complie
lgx:$(OBJS)
	$(CC) $^ -o $(BUILD_PATH)/$@ $(LDFLAGS) $(CFLAGS) $(INCLUDE)

static:$(OBJS)
	$(CC) $^ -o $(BUILD_PATH)/$@ $(LDFLAGS) $(CFLAGS)  --static

$(OBJS):%.o:%.cc
	$(CC) -c $^ -o $@ $(DEBUGFLAGS) $(INCLUDE)

print:
	@echo $(COBJS)

test:
	$(GCC) test_src/webbench.c -o $(BUILD_PATH)/webbench

.PHONY:clean
clean:
	$(RM) $(BUILD_PATH)/lgx
	$(RM) $(OBJS)
	$(RM) $(BUILD_PATH)/webbench

install:
	@sudo $(MKDIR) $(CONFIG_PATH)
	@sudo $(MKDIR) $(WWW_PATH)
	@sudo $(CP) $(BUILD_PATH)/lgx $(INSTALL_PATH)
	@sudo $(CP) conf/* $(CONFIG_PATH)
	@sudo $(CP) www/* $(WWW_PATH)
	@echo 'lgx has installed'

uninstall:
	@sudo $(RM) $(INSTALL_PATH)/lgx
	@sudo $(RM) $(CONFIG_PATH)
	@sudo $(RM) $(WWW_PATH)
	@echo 'lgx has uninstalled'
