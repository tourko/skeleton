SHELL := /usr/bin/env bash

THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC=g++

CFLAGS += -I./include
CFLAGS += -Wall -Wextra -g -O3 -save-temps -std=gnu++11
IFLAGS += -I include

ifdef NOCOLOR
CFLAGS += -fno-color-diagnostics
endif

# Dynamic libraries
LDFLAGS += -lpthread

PROGRAM = skeleton
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

DEBUG = 1

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -Wl,-Bstatic $(LSFLAGS) -Wl,-Bdynamic $(LDFLAGS) $^ -o $@

# These are the pattern matching rules. In addition to the automatic
# variables used here, the variable $* that matches whatever % stands for
# can be useful in special cases.
%.o: %.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -DDEBUG=$(DEBUG) -MMD -MP -c $< -o $@

%: %.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -o $@ $<

clean:
	$(RM) $(OBJS) $(DEPS) $(PROGRAM) *.s *.i *.ii

.PHONY: all clean

-include $(DEPS)
