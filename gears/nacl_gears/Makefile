# Copyright 2011, Dennis Jarosch. All rights reserved.
# Use of this source code is governed by a BSD-style license that can
# be found in the LICENSE file.

# Makefile for the NaCl Gears.

.PHONY: all clean

CCFILES = nacl_gears_module.cc nacl_gears.cc gear.cc npp_gate.cc scripting_bridge.cc npn_bridge.cc transforms.cc shader_program.cc

OBJECTS_X86_32 = $(CCFILES:%.cc=%_x86_32.o)
OBJECTS_X86_64 = $(CCFILES:%.cc=%_x86_64.o)

NACL_SDK_ROOT = ../..

CFLAGS = -Wall -Wno-long-long -pthread -DXP_UNIX -Werror
INCLUDES = -I$(NACL_SDK_ROOT)
LDFLAGS = -lgoogle_nacl_imc \
          -lgoogle_nacl_npruntime \
          -lpthread \
          -lsrpc \
          -lgoogle_nacl_pgl \
          -lgoogle_nacl_gpu
OPT_FLAGS = -O2

all: check_variables gears_x86_32.nexe gears_x86_64.nexe

# common.mk has rules to build .o files from .cc files.
-include ../common.mk

gears_x86_32.nexe: $(OBJECTS_X86_32)
	$(CPP) $^ $(LDFLAGS) -m32 -o $@

gears_x86_64.nexe: $(OBJECTS_X86_64)
	$(CPP) $^ $(LDFLAGS) -m64 -o $@

clean:
	-$(RM) $(OBJECTS_X86_32) $(OBJECTS_X86_64) \
	    gears_x86_32.nexe gears_x86_64.nexe

# This target is used by the SDK build system to produce a pre-built version
# of the .nexe.  You do not need to call this target.
install_prebuilt: gears_x86_32.nexe gears_x86_64.nexe
	-$(RM) $(OBJECTS_X86_32) $(OBJECTS_X86_64)
