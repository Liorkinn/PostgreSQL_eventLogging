#!/bin/bash

BASENAME=$1
SCHEMA=$2
TABLENAME=$3
SQLVALUE=$4
TABLEPARAM=`psql -U postgres -d $BASENAME -c "SELECT column_name FROM information_schema.columns WHERE table_name ='$TABLENAME' AND table_schema='$SCHEMA';"`
TABLEPARAM=$(echo $TABLEPARAM | sed 's/^ |$//g' | sed 's/([^)]*)//g' | sed 's/.$//' | cut -d' ' -f3- | sed "s/\s/,/g")
echo $TABLEPARAM
echo $SQLVALUE
psql -U postgres -d $BASENAME -c "INSERT INTO $SCHEMA.\"$TABLENAME\"($TABLEPARAM) VALUES $SQLVALUE ;"
