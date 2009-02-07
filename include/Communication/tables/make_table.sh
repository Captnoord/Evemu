#!/bin/sh

QUERY="$1"
TABLE="$2"

echo "#ifndef EVEDB_${TABLE}_H__"
echo "#define EVEDB_${TABLE}_H__"
echo
echo " //THIS FILE IS AUTOMATICALLY GENERATED BY $0. DO NOT EDIT"
echo 
echo "namespace EVEDB {"
echo " namespace $TABLE {"
echo
echo "typedef enum {"

first_line=1

echo "$QUERY" | ~/eve/runmysql -B \
| sed "s/[ #'-]/_/g" \
| while read field value
do
	if [ $first_line -eq 1 ]; then
		first_line=0
	else
		echo "	$field = $value,"
	fi
done

echo " } $TABLE;"
echo
echo " }"
echo "}"
echo
echo "#endif"
echo






