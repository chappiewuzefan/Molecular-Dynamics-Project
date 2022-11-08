# Name: Zefan Wu
# Student Number: u6474528
# Course: COMP3320
# Assignment Number: 1
# Name of this file: Makefile
# I declare that the material I am submitting in this file is entirely my own work. I have not collaborated with anyone to produce it, nor have I copied it, in part or in full, from work produced by someone else. I have not given access to this material to any other student in this course.

C_LIB_EXT :=
ifeq ($(OS),Windows_NT)
	C_LIB_EXT = .dll
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		C_LIB_EXT = .so
	else ifeq ($(UNAME_S),Darwin)
		C_LIB_EXT = .so
	endif
endif

CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -mavx -mfma
SHARED_CFLAGS = -fPIC -shared 
OPTIMISATION_FLAGS = -O3
TARGET = md_lib_SoA$(C_LIB_EXT) md_lib_AoS$(C_LIB_EXT) md_lib_Opt$(C_LIB_EXT) md_SoA md_AoS md_Opt
LIBS = -lm

ifneq (, $(shell which papi_avail))
	LIBS += -lpapi -D PAPI
endif

all:	$(TARGET)	

md_lib_SoA$(C_LIB_EXT): md_lib_SoA.c
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(OPTIMISATION_FLAGS) -o $@ $? $(LIBS)

md_lib_AoS$(C_LIB_EXT): md_lib_AoS.c
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(OPTIMISATION_FLAGS) -o $@ $? $(LIBS)

md_lib_Opt$(C_LIB_EXT): md_lib_Opt.c
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(OPTIMISATION_FLAGS) -o $@ $? $(LIBS)

md_SoA: md_SoA.c md_lib_SoA.c 
	$(CC) $(CFLAGS) $(OPTIMISATION_FLAGS) -o $@ $@.c $(LIBS)

md_AoS: md_AoS.c md_lib_AoS.c 
	$(CC) $(CFLAGS) $(OPTIMISATION_FLAGS) -o $@ $@.c $(LIBS)

md_Opt: md_Opt.c md_lib_Opt.c 
	$(CC) $(CFLAGS) $(OPTIMISATION_FLAGS) -o $@ $@.c $(LIBS)

clean:	
	rm -f *.o *.dll *.so *~ *.out *.gcno *.gcda *.gcov *.pyc *.out.* $(TARGET)
