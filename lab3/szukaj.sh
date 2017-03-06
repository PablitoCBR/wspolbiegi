#!/bin/bash

declare -i FCOUNT
FCOUNT=0
CHILDREN=

if [ "$#" -lt 2 ]; then
	echo "Za malo argmetnow, prawidlowy format to ./szukaj.sh [gdzie] [co]"
	exit
else
	if [ ! -d "$1" ]; then
		echo "'$1' nie jest katalogiem"
		exit
	fi
fi

for item in $1/*; do
	if [ -d "$item" ]; then
		"$0" "$item" "$2" "$$" &
		CHILDREN+=("$!")
	elif [ -f "$item" ]; then
		if [ $(basename $item) == "$2" ]; then
			echo "Znaleziono '$2' w folderze '$1'"
			FCOUNT=$((FCOUNT+1))
		fi
	fi
done

for i in "${CHILDREN[@]}"; do
	if [ "$i" ]; then
		wait "$i"
		FCOUNT=$((FCOUNT+$?))
	fi
done

if [ -z "$3" ]; then
	if [ "$FCOUNT" -eq 0 ]; then
		echo "Nie znaleziono"
	else
		echo "Znaleziono $FCOUNT plikow"
	fi
fi

exit "$FCOUNT"
