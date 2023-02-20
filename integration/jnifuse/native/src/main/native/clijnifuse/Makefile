#
# The Alluxio Open Foundation licenses this work under the Apache License, version 2.0
# (the "License"). You may not use this work except in compliance with the License, which is
# available at www.apache.org/licenses/LICENSE-2.0
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied, as more fully set forth in the License.
#
# See the NOTICE file distributed with this work for information regarding copyright ownership.

CXX := g++
UNAME := $(shell uname)
INCDIR = -I /usr/include -I $(JAVA_HOME)/include -I ./

# Linux
ifeq ($(UNAME), Linux)
	INCDIR += -I $(JAVA_HOME)/include/linux
	FUSE2_CLI := fuse2cli
endif
# MacOS
ifeq ($(UNAME), Darwin)
	INCDIR += -I $(JAVA_HOME)/include/darwin
	FUSE2_CLI := fuse2cli
	INCDIR += -D__APPLE__
endif

COMMON_FLAGS = -std=c++11 -Wall -g -fPIC -D_FILE_OFFSET_BITS=64 ${INCDIR}
FUSE2_LIB_DIR ?= ../../src/main/resource
CLI_FUSE2_LDFLAGS = -shared ${FUSE2_LIB_DIR}/libjnifuse.so

CLI_CXXFLAGS += ${COMMON_FLAGS}
OUTPUT_DIR ?= .
CLI_SRC_DIR ?= ../../src/main/native/clijnifuse
CLI_FUSE2_OBJS = $(addprefix $(OUTPUT_DIR)/, $(patsubst %.cc,%.cli.o,$(notdir $(wildcard $(CLI_SRC_DIR)/*.cc))))

all: ${FUSE2_CLI}

${OUTPUT_DIR}/%.cli.o: ${CLI_SRC_DIR}/%.cc
	${CXX} ${CLI_CXXFLAGS} -o $@ -c $<

${FUSE2_CLI}: ${CLI_FUSE2_OBJS}
	${CXX} ${CLI_CXXFLAGS} -o ${OUTPUT_DIR}/${FUSE2_CLI} ${CLI_FUSE2_OBJS} ${CLI_FUSE2_LDFLAGS}


clean:
	rm -rf ${CLI_FUSE2_OBJS} ${FUSE2_CLI}
