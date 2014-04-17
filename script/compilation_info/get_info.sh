#!/bin/bash

COMPILER=gcc
OUTPUT_FILE=info.h
COMPILE_FLAG=FLAGS

GEN_VERSION=$(grep "genflash version:" main.c | sed "s/.*genflash version: *\(.*\)\"/\1/")
GCC_VERSION=$(${COMPILER} -v 2>&1 | grep "gcc version")
PLATFORM=$(cat -v /etc/issue | grep "[a-zA-Z0-9 ]" | grep -v "\^" | sed 's+\\+\\\\+g')
COMPILE_PARA=$(grep "\<$COMPILE_FLAG\> *[+: ]=" Makefile | sed -e "s/\<$COMPILE_FLAG\> *[+: ]=\(.*\)/\1/" -e " s/\(.*\)#.*/\1/" -e "s/[ \t]\+/ /g") 

echo "#ifndef __INFO_H__" > ${OUTPUT_FILE}
echo "#define __INFO_H__" >> ${OUTPUT_FILE}
echo -n '#define GEN_VERSION "Genflash version: ' >> ${OUTPUT_FILE}
echo "$GEN_VERSION \"" >> ${OUTPUT_FILE}
echo -n '#define GCC_VERSION "Genflash compiler version: ' >> ${OUTPUT_FILE}
echo "$GCC_VERSION \"" >> ${OUTPUT_FILE}
echo -n '#define PLATFORM_INFO "Genflash platform: ' >> ${OUTPUT_FILE}
echo "$PLATFORM \"" >> ${OUTPUT_FILE}
echo -n '#define COMPILE_PARA "Genflash complier para: ' >> ${OUTPUT_FILE}
echo $COMPILE_PARA\" >> ${OUTPUT_FILE}
echo "#endif" >> ${OUTPUT_FILE}

