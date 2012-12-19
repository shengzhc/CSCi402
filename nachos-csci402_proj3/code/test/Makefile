# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# If you are running on SunOS 5.x machines, uncomment the next line
GCCDIR = /auto/home-scf-22/csci402/testgnu/decstation-ultrix/bin/
LDFLAGS = -T newscript -N
ASFLAGS = -mips1
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)


CC = $(GCCDIR)gcc
AS = $(GCCDIR)as
LD = $(GCCDIR)ld
CPP = $(GCCDIR)cpp

INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt matmult sort testfiles p1 p2 p3 p4 p5 part2_test part1_test test1_c1 test1_c2 test1_c3 test2_c1 test2_c2 test3_c1 test3_c2 test3_c3 test3_c4

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.s > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c        
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

testfiles.o: testfiles.c
	$(CC) $(CFLAGS) -c testfiles.c
testfiles: testfiles.o start.o
	$(LD) $(LDFLAGS) start.o testfiles.o -o testfiles.coff
	../bin/coff2noff testfiles.coff testfiles

p1.o: p1.c
	$(CC) $(CFLAGS) -c p1.c  
p1: p1.o start.o
	$(LD) $(LDFLAGS) start.o p1.o -o p1.coff
	../bin/coff2noff p1.coff p1   

p2.o: p2.c
	$(CC) $(CFLAGS) -c p2.c  
p2: p2.o start.o
	$(LD) $(LDFLAGS) start.o p2.o -o p2.coff
	../bin/coff2noff p2.coff p2
    
p3.o: p3.c
	$(CC) $(CFLAGS) -c p3.c  
p3: p3.o start.o
	$(LD) $(LDFLAGS) start.o p3.o -o p3.coff
	../bin/coff2noff p3.coff p3  
    
p4.o: p4.c
	$(CC) $(CFLAGS) -c p4.c  
p4: p4.o start.o
	$(LD) $(LDFLAGS) start.o p4.o -o p4.coff
	../bin/coff2noff p4.coff p4  

p5.o: p5.c
	$(CC) $(CFLAGS) -c p5.c  
p5: p5.o start.o
	$(LD) $(LDFLAGS) start.o p5.o -o p5.coff
	../bin/coff2noff p5.coff p5  
    
part2_test.o: part2_test.c
	$(CC) $(CFLAGS) -c part2_test.c  
part2_test: part2_test.o start.o
	$(LD) $(LDFLAGS) start.o part2_test.o -o part2_test.coff
	../bin/coff2noff part2_test.coff part2_test
 
part1_test.o: part1_test.c \
	/usr/sfw/lib/gcc/sparc-sun-solaris2.10/3.4.3/include/stdio.h
	$(CC) $(CFLAGS) -c part1_test.c  
part1_test: part1_test.o start.o
	$(LD) $(LDFLAGS) start.o part1_test.o -o part1_test.coff
	../bin/coff2noff part1_test.coff part1_test 

test1_c1.o: test1_c1.c
	$(CC) $(CFLAGS) -c test1_c1.c  
test1_c1: test1_c1.o start.o
	$(LD) $(LDFLAGS) start.o test1_c1.o -o test1_c1.coff
	../bin/coff2noff test1_c1.coff test1_c1
    
test1_c2.o: test1_c2.c
	$(CC) $(CFLAGS) -c test1_c2.c  
test1_c2: test1_c2.o start.o
	$(LD) $(LDFLAGS) start.o test1_c2.o -o test1_c2.coff
	../bin/coff2noff test1_c2.coff test1_c2
    
test1_c3.o: test1_c3.c
	$(CC) $(CFLAGS) -c test1_c3.c  
test1_c3: test1_c3.o start.o
	$(LD) $(LDFLAGS) start.o test1_c3.o -o test1_c3.coff
	../bin/coff2noff test1_c3.coff test1_c3

test2_c1.o: test2_c1.c
	$(CC) $(CFLAGS) -c test2_c1.c  
test2_c1: test2_c1.o start.o
	$(LD) $(LDFLAGS) start.o test2_c1.o -o test2_c1.coff
	../bin/coff2noff test2_c1.coff test2_c1

test2_c2.o: test2_c2.c
	$(CC) $(CFLAGS) -c test2_c2.c  
test2_c2: test2_c2.o start.o
	$(LD) $(LDFLAGS) start.o test2_c2.o -o test2_c2.coff
	../bin/coff2noff test2_c2.coff test2_c2

test3_c1.o: test3_c1.c
	$(CC) $(CFLAGS) -c test3_c1.c  
test3_c1: test3_c1.o start.o
	$(LD) $(LDFLAGS) start.o test3_c1.o -o test3_c1.coff
	../bin/coff2noff test3_c1.coff test3_c1

test3_c2.o: test3_c2.c
	$(CC) $(CFLAGS) -c test3_c2.c  
test3_c2: test3_c2.o start.o
	$(LD) $(LDFLAGS) start.o test3_c2.o -o test3_c2.coff
	../bin/coff2noff test3_c2.coff test3_c2

test3_c3.o: test3_c3.c
	$(CC) $(CFLAGS) -c test3_c3.c  
test3_c3: test3_c3.o start.o
	$(LD) $(LDFLAGS) start.o test3_c3.o -o test3_c3.coff
	../bin/coff2noff test3_c3.coff test3_c3

test3_c4.o: test3_c4.c
	$(CC) $(CFLAGS) -c test3_c4.c  
test3_c4: test3_c4.o start.o
	$(LD) $(LDFLAGS) start.o test3_c4.o -o test3_c4.coff
	../bin/coff2noff test3_c4.coff test3_c4


clean:
	rm -f *.o *.coff
