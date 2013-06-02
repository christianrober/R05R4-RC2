# $Id: Makefile,v 1.5 2005/08/12 03:32:53 magicyang Exp $
#
# Makefile for the linux kernel.
#

extra-y	:= head.o


obj-y   := entry.o traps.o shadows.o debugport.o irq.o \
	   process.o setup.o signal.o traps.o time.o ptrace.o

obj-$(CONFIG_ETRAX_KGDB) += kgdb.o
obj-$(CONFIG_ETRAX_FAST_TIMER) += fasttimer.o
obj-$(CONFIG_MODULES)    += crisksyms.o

clean:

