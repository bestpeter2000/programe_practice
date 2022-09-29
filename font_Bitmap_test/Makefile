#
# Makefile for Local test operation
#

# CC = /home/peter/nvidia/nv_toolchain/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
# CFLAGS += -I../install/include -I./
# LDFLAGS += -L../install/lib -L./

# LIBS += -lasound -lpthread -lpesi_pcm_sharememory -lspeexdsp


# PRJOBJS = pesi_main.o pesi_pcm.o 
# OBJECTS_INC = pesi_pcm.h

#$(CFLAGS)
all: clean thailand_bitmap_test 

thailand_bitmap_test: 
	$(CC) $(CFLAGS) $(LDFLAGS) thailand_bitmap_test.c -o $@  $(LIBS)    
    
clean:
	rm -rf thailand_bitmap_test

	
	
	