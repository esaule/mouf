#!/bin/sh

grep -v \'\'Pluriel\ de\'\' dictionary | \
    grep -v \'\'Pluriel\ de\ \'\' \
	 > dictionary-filtered
