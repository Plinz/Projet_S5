# file Makefile
#CC= gcc
#RM= rm -vf
#CFLAGS= -W -Wall -g
#CPPFLAGS= -I.
#SRCFILES= $(wildcard *.c)
#OBJFILES= $(patsubst %.c, %.o, $(SRCFILES))
#PROGFILES= $(patsubst %.c, %, $(SRCFILES))
#
#.PHONY: all clean
#
#all: $(PROGFILES)
#clean: $(RM) $(OBJFILES) $(PROGFILES) *~
## eof Makefile

CC = gcc
CFLAGS = -W -Wall -g
LDFLAGS = 
 
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
 
all : fusion reader 

reader : elf_reader.c elf_header.c elf_rechercheSection.c elf_relocate.c elf_sectionTable.c elf_symboleTable.c endian.c
	$(CC) $(LDFLAGS) -o $@ $^
fusion : elf_fusion.c elf_fusionSimple.c elf_header.c elf_sectionTable.c endian.c elf_fusionSymbole.c
	$(CC) $(LDFLAGS) -o $@ $^

clean :
	@rm *.o
cleaner : clean
	@rm fusion reader
