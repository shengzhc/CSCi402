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

all: halt matmult sort testfiles wrn patient doctor cashier xray nurse p1 p2 parent child p3

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

wrn.o: wrn.c
	$(CC) $(CFLAGS) -c wrn.c  
wrn: wrn.o start.o
	$(LD) $(LDFLAGS) start.o wrn.o -o wrn.coff
	../bin/coff2noff wrn.coff wrn   

patient.o: patient.c
	$(CC) $(CFLAGS) -c patient.c  
patient: patient.o start.o
	$(LD) $(LDFLAGS) start.o patient.o -o patient.coff
	../bin/coff2noff patient.coff patient
    
doctor.o: doctor.c
	$(CC) $(CFLAGS) -c doctor.c  
doctor: doctor.o start.o
	$(LD) $(LDFLAGS) start.o doctor.o -o doctor.coff
	../bin/coff2noff doctor.coff doctor  
    
cashier.o: cashier.c
	$(CC) $(CFLAGS) -c cashier.c  
cashier: cashier.o start.o
	$(LD) $(LDFLAGS) start.o cashier.o -o cashier.coff
	../bin/coff2noff cashier.coff cashier  

xray.o: xray.c
	$(CC) $(CFLAGS) -c xray.c  
xray: xray.o start.o
	$(LD) $(LDFLAGS) start.o xray.o -o xray.coff
	../bin/coff2noff xray.coff xray  

nurse.o: nurse.c
	$(CC) $(CFLAGS) -c nurse.c  
nurse: nurse.o start.o
	$(LD) $(LDFLAGS) start.o nurse.o -o nurse.coff
	../bin/coff2noff nurse.coff nurse
    
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

p4.o: p4.c
	$(CC) $(CFLAGS) -c p4.c  
p4: p4.o start.o
	$(LD) $(LDFLAGS) start.o p4.o -o p4.coff
	../bin/coff2noff p4.coff p4

parent.o: parent.c
	$(CC) $(CFLAGS) -c parent.c  
parent: parent.o start.o
	$(LD) $(LDFLAGS) start.o parent.o -o parent.coff
	../bin/coff2noff parent.coff parent

child.o: child.c
	$(CC) $(CFLAGS) -c child.c  
child: child.o start.o
	$(LD) $(LDFLAGS) start.o child.o -o child.coff
	../bin/coff2noff child.coff child

p3.o: p3.c
	$(CC) $(CFLAGS) -c p3.c  
p3: p3.o start.o
	$(LD) $(LDFLAGS) start.o p3.o -o p3.coff
	../bin/coff2noff p3.coff p3


clean:
	rm -f *.o *.coff
