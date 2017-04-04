#!/bin/bash
DANE=/home/studinf/pcendrowski/Pulpit/lab1_zad_ocen/dane.txt
WYNIKI=/home/studinf/pcendrowski/Pulpit/lab1_zad_ocen/wyniki.txt
if [ -f $DANE ]; then
  while true; do
    LICZBA=$(< $DANE)
    if [ $LICZBA ]; then
      > $DANE
      if [ -f $WYNIKI ]; then
        echo $[$LICZBA*$LICZBA*2] > $WYNIKI
      else
        echo "Brak pliku '$WYNIKI'"
        break
      fi
    else
      continue
    fi
  done
else
   echo "Brak pliku '$DANE'"
fi
