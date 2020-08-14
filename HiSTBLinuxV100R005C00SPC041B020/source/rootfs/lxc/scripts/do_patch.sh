#!/bin/sh

DEST_DIR=$1
PATCH_DIR=$2
PATCH_LIST=$2/patch.list

if test -f $PATCH_LIST; then 
	patches=`cat $PATCH_LIST` ; 
else      
	patches="";  
fi  

for i in $patches; do 
	cat $PATCH_DIR/$i | patch -d  $DEST_DIR -p1 -E; 
done
