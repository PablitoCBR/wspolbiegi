#!/bin/bash
DANE=/home/studinf/pcendrowski/Pulpit/lab1_zad_ocen/dane.txt
WYNIKI=/home/studinf/pcendrowski/Pulpit/lab1_zad_ocen/wyniki.txt
read -p "Wpisz liczbe x do wyliczenia wielomianu x^2*2: " x
if [ -f $DANE ]; then
  echo $x > $DANE
  if [ -f $WYNIKI ]; then
    while true; do
      LICZBA=$(< $WYNIKI)
      if [ $LICZBA ]; then
        > $WYNIKI
        echo $LICZBA
        break
      else
        continue
      fi
    done
  else
    echo "Brak pliku '$WYNIKI'"
  fi
else
   echo "Brak pliku '$DANE'"
fi
