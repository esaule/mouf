#!/bin/sh

cat dictionary | \
    grep -v \'\'Pluriel\ de\'\'  | \
    grep -v \'\'Pluriel\'\'\ \'\'de\'\'  | \
    grep -v \'\'Pluriel\ de\ \'\' | \
    grep -v Habitant\ de\  | \
    grep -v Habitante\ de\  | \
    grep -v Variante\ de\  | \
    grep -v \'\'Variante\ de\'\'\  | \
    grep -v Synonyme\ de\  \
	 > dictionary-filtered
