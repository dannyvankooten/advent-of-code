#!/bin/bash
for FILE in ./*.txt
do
    NAME=${FILE#"./"}
    NAME=${NAME%".txt"}
    echo $NAME
    strliteral --ident input $FILE $NAME.h
done