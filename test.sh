#! /bin/bash
while read -p "input the name" name
do
    if [ "$name" == "yxc" ]
    then echo "1"
    fi
    echo $name
done
