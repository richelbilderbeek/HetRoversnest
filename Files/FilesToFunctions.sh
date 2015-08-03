#!/bin/bash
for NUMBER in `seq 1 450`
do

  INPUTFILENAME=$NUMBER.txt

  if [ ! -e $INPUTFILENAME ] 
  then 
    continue 
  fi

  OUTPUT_HEADER_FILENAME=$NUMBER.h

  echo "#include <string>" > $OUTPUT_HEADER_FILENAME
  echo "std::string Get$NUMBER();" >> $OUTPUT_HEADER_FILENAME

  #echo ".h:\n"
  #cat $OUTPUT_HEADER_FILENAME

  OUTPUT_CPP_FILENAME=$NUMBER.cpp

  echo "#include \"$OUTPUT_HEADER_FILENAME\"" > $OUTPUT_CPP_FILENAME
  echo "std::string Get$NUMBER()" >> $OUTPUT_CPP_FILENAME
  echo "{" >> $OUTPUT_CPP_FILENAME
  echo "  return" >> $OUTPUT_CPP_FILENAME
  sed 's/.*/    "&"/' $INPUTFILENAME >> $OUTPUT_CPP_FILENAME
  echo "  ;" >> $OUTPUT_CPP_FILENAME
  echo "}" >> $OUTPUT_CPP_FILENAME

done