#!/bin/sh

cat dictionary | \
    grep -v \'\'Pluriel\ de\'\'  | \
    grep -v \'\'Pluriel\'\'\ \'\'de\'\'  | \
    grep -v \'\'Pluriel\ de\ \'\' | \
    grep -v Habitant\ d | \
    grep -v Habitante\ d  | \
    grep -v Variante\ de\  | \
    grep -v \'\'Variante\ de\'\'\  | \
    grep -v Synonyme\ de\  \
	 > dictionary-filtered
