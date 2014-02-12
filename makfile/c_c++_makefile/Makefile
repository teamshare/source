#Makefile for C/C++ GNU/linux Program   
#   
# License: GPL (General Public License)   
# Author:  liu yixiong <517002683@qq.com>   
# Date:    2013/10/29 (version 1.0)   
#   
# Description:   
# ------------   
# This is an easily customizable makefile template. The purpose is to   
# provide an instant building environment for C/C++ programs.   
#   
# It searches all the C/C++ source files in the specified directories,   
# makes dependencies, compiles and links to form an executable.   
#   
# Besides its default ability to build C/C++ programs which use only   
# standard C/C++ libraries, you can customize the Makefile to build   
# those using other libraries. Once done, without any changes you can   
# then build programs using the same or less libraries, even if source   
# files are renamed, added or removed. Therefore, it is particularly   
# convenient to use it to build codes for experimental or study use.   
#   
# GNU make is expected to use the Makefile. Other versions of makes   
# may or may not work.   
#   
# Usage:   
# ------   
# 1. Copy the Makefile to your program directory.   
# 2. Customize in the "Customizable Section" only if necessary:   
#    * to search sources in more directories, set to <SRCDIRS>   
#    * to search sources in more directories, set to <SRCDIRS>   
#    * to specify your favorite program name, set to <PROGRAM>   
# 3. Type make to start building your program.   
#
# Make Target:   
# ------------   
# The Makefile provides the following targets to make:   
#   $ make           compile and link   
#   $ make NODEP=yes compile and link without generating dependencies   
#   $ make lib       compile and create libraries files(.a)
#   $ make objs      compile only (no linking)   
#   $ make clean     clean objects and the executable file   
#   $ make distclean clean objects, the executable and dependencies   
#   $ make help      get the usage of the makefile   
#   
#===========================================================================   
# Cross compile, turn off for x86
CROSS_COMPILE   = arm-linux-

AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CXX             = $(CROSS_COMPILE)g++
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
RANLIB          = $(CROSS_COMPILE)ranlib

MAKE = make
# The command used to delete file.   
RM = -rm -f

###################################################################################
#       Optimization options (add to CCFLAGS ):
#       -O0 -g  = Level 0       no optimization, debug info is enabled
#       -O1     = Level 1       (incompatible with -g option (gebug info))
#       -O2     = Level 2       (incompatible with -g option)
#       -O3     = Level 3       (incompatible with -g option)
###################################################################################
# The pre-processor and compiler options.   
#MY_CFLAGS = -Wall -O0 -g -static 
#MY_CFLAGS = -mlittle-endian -c -O0 -g 
MY_CFLAGS = -c -O0 -g \
		    -Wpointer-arith -Wundef \
		    -fomit-frame-pointer -fno-builtin -fno-strict-aliasing \
		    -ffunction-sections -fdata-sections -Wall -Wstrict-prototypes -Wno-unused-function

# The linker options.   
MY_LIBS   = -lc -lm -lgcc 

# The options used in linking as well as in any direct use of ld.   
#LDFLAGS   = -nostdlib -Wl,--gc-sections -Wl,-EL -T link.ld
LDFLAGS   = -Wl,--gc-sections -Wl,-EL 

# The options used in archive as well as in any direct use of ar.   
ARFLAGS = -rcs

CCFLAGS = $(MY_CFLAGS)

APP_INCLUDE_DIR = 

APP_LIB_DIR = 

## Customizable Section: adapt those variables to suit your program.   
##==========================================================================   
# The directories in which source and header files reside.   
# If not specified, only the current directory will be serached.   
SRCDIRS   =.
INCLUDE_CMPL_DIR =$(addprefix -I ,$(SRCDIRS) $(APP_INCLUDE_DIR))
INCLUDE_LD_DIR   =$(addprefix -L ,$(SRCDIRS) $(APP_LIB_DIR))


# The executable file name.   
# If not specified, current directory name or `a.out' will be used.   
FILE_NAME =$(notdir $(shell pwd))
LIB_PACK = lib$(strip $(FILE_NAME)).a
PROGRAM  =$(FILE_NAME).elf

## Implicit Section: change the following only when necessary.   
##==========================================================================   

# The source file types (headers excluded).   
# .c indicates C source files, and others C++ ones.
SRCEXTS = .s .S .c .C .cc .cpp .CPP .c++ .cxx .cp

# The header file types.
HDREXTS = .h .H .hh .hpp .HPP .h++ .hxx .hp

## Stable Section: usually no need to be changed. But you can add more.   
##==========================================================================   
SHELL   = /bin/sh
EMPTY =
SPACE  = $(EMPTY) $(EMPTY)   
ifeq ($(strip $(PROGRAM)),)
  CUR_PATH_NAMES = $(subst /,$(SPACE),$(subst $(SPACE),_,$(CURDIR)))   
  PROGRAM = $(word $(words $(CUR_PATH_NAMES)),$(CUR_PATH_NAMES))   
ifeq ($(strip $(PROGRAM)),)
   PROGRAM = a.out   
  endif   
endif   
ifeq ($(SRCDIRS),)   
  SRCDIRS = .   
endif   
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))   
HEADERS = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(HDREXTS))))   
SRC_CXX = $(filter-out %.c,$(SOURCES))   
OBJS    = $(addsuffix .o, $(basename $(SOURCES)))   
DEPS    = $(OBJS:.o=.d)   
  
## Define some useful variables.   
DEP_OPT     = $(shell if `$(CC) --version | grep "GCC" >/dev/null`; then echo "-MM -MP"; else echo "-M"; fi )
DEPEND      = $(CC)  $(DEP_OPT)  $(CCFLAGS) $(INCLUDE_CMPL_DIR)
DEPEND.d    = $(subst -g ,,$(DEPEND)) 
COMPILE.c   = $(CC)  $(CCFLAGS) $(INCLUDE_CMPL_DIR) -c 
COMPILE.cxx = $(CXX) $(CCFLAGS) $(INCLUDE_CMPL_DIR) -c 
LINK.c      = $(CC)  $(INCLUDE_LD_DIR) $(LDFLAGS) 
LINK.cxx    = $(CXX) $(INCLUDE_LD_DIR) $(LDFLAGS) 
  
.PHONY: all lib objs clean distclean help show   
  
# Delete the default suffixes   
.SUFFIXES:   

all: $(PROGRAM) 

lib:$(LIB_PACK)

# Rules for creating dependency files (.d).   
#------------------------------------------   
  
%.d:%.s   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
%.d:%.S   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
%.d:%.c   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
%.d:%.C   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@

%.d:%.cc   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
  
%.d:%.cpp   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
  
%.d:%.CPP   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
  
%.d:%.c++   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
  
%.d:%.cp   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@
  
%.d:%.cxx   
	@echo -n $(dir $<) > $@   
	@$(DEPEND.d) $< >> $@ || rm $@

#Rules for generating libraries files(.a)
#----------------------------------------   
$(LIB_PACK):$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
  
# Rules for generating object files (.o).   
#----------------------------------------   
objs:$(OBJS)   
  
%.o:%.s   
	$(COMPILE.c) $< -o $@   
  
%.o:%.S   
	$(COMPILE.c) $< -o $@   

%.o:%.c   
	$(COMPILE.c) $< -o $@   
  
%.o:%.C   
	$(COMPILE.cxx) $< -o $@   
  
%.o:%.cc   
	$(COMPILE.cxx) $< -o $@   
  
%.o:%.cpp   
	$(COMPILE.cxx) $< -o $@   
  
%.o:%.CPP   
	$(COMPILE.cxx) $< -o $@   
  
%.o:%.c++   
	$(COMPILE.cxx) $< -o $@   
  
%.o:%.cp   
	$(COMPILE.cxx) $< -o $@   
  
%.o:%.cxx   
	$(COMPILE.cxx) $< -o $@   
  
# Rules for generating the executable.   
#-------------------------------------   
$(PROGRAM):$(OBJS)

ifeq ($(strip $(SRC_CXX)),)  # C program  
	$(LINK.c)   $(OBJS) $(MY_LIBS) -o $@  
	@echo Type ./$@ to execute the program.   
else     # C++ program  
	$(LINK.cxx) $(OBJS) $(MY_LIBS) -o $@   
	@echo Type ./$@ to execute the program.   
endif   
  
ifndef NODEP   
ifneq ($(DEPS),)   
  sinclude $(DEPS)   
endif   
endif   
  
clean:   
	$(RM) $(OBJS) $(PROGRAM) $(LIB_PACK)  
  
distclean: clean   
	$(RM) $(DEPS)
  
# Show help.   
help:
	@echo 'Generic Makefile for C/C++ Programs (makefile) version 0.1'  
	@echo 'Copyright (C) 2013 liu yixiong <517002683@qq.com>'  
	@echo   
	@echo 'Usage: make [TARGET]'  
	@echo 'TARGETS:'  
	@echo '  all       (=make) compile and link.'  
	@echo '  NODEP=yes make without generating dependencies.'  
	@echo '  lib       compile and create libraries files(.a)'
	@echo '  objs      compile only (no linking).'  
	@echo '  clean     clean objects and the executable file.'  
	@echo '  distclean clean objects, the executable and dependencies.'  
	@echo '  show      show variables (for debug use only).'  
	@echo '  help      print this message.'  
	@echo   
	@echo 'Report bugs to <517002683@qq.com>.'  
  
# Show variables (for debug use only.)   
show:   
	@echo 'PROGRAM     :' $(PROGRAM) 
	@echo 'LIB_PACK    :' $(LIB_PACK)  
	@echo 'SRCDIRS     :' $(SRCDIRS)   
	@echo 'HEADERS     :' $(HEADERS)   
	@echo 'SOURCES     :' $(SOURCES)   
	@echo 'SRC_CXX     :' $(SRC_CXX)   
	@echo 'OBJS        :' $(OBJS)   
	@echo 'DEPS        :' $(DEPS)   
	@echo 'DEPEND      :' $(DEPEND)   
	@echo 'COMPILE.c   :' $(COMPILE.c)   
	@echo 'COMPILE.cxx :' $(COMPILE.cxx)   
	@echo 'link.c      :' $(LINK.c)   
	@echo 'link.cxx    :' $(LINK.cxx)   
  
## End of the Makefile ##  
##############################################################  


