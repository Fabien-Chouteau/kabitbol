#!/bin/sh

RULES=Makefile.rules

echo > $RULES

echo "CC = $(which leon3-elf-gcc)" >> $RULES
echo "AR = $(which leon3-elf-ar)" >> $RULES
echo "LD = $(which leon3-elf-ld)" >> $RULES
echo "OBJCOPY = $(which leon3-elf-objcopy)" >> $RULES
echo "AR = $(which leon3-elf-ar)" >> $RULES

#-DPPAGES_ALLOC_BUDDY
echo "CFLAGS += -pipe -Werror -Wall -fno-builtin -msoft-float -mno-app-regs -mcpu=v8 -O3 #-g -ggdb
" >> $RULES
echo "ARFLAGS += " >> $RULES
echo "INDENTFLAGS += -nbad -bap -nsob -bl -bli0 -cbi0 -cdw -npcs -bs -saf -sai -saw -nbc -npsl -bls -blf -ppi 1 -bbo -sc -hnl -lp -il1 -cli 1 -l80 -ut -ts2 -dj -cd0" >> $RULES

echo "KABITBOL_PATH = $(pwd)" >> $RULES
