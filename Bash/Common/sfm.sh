#!/bin/bash

echo "========================="
echo "|  Simple File Manager  |"
echo "========================="

while :
do
	read -p ">> " action
	if [[  $action = "help" ]]
	then
		echo "Commands:"
		echo -e "\thelp"
		echo -e "\texit"
		echo -e "\tcreate file"
		echo -e "\tdelete file"
		echo -e "\tcopy file"
		echo -e "\tmove file"
	elif [[ $action = "exit" ]]
	then
		echo -e "Goodbye!"
		exit 0
	elif [[ $action = "create file" ]]
	then
		read -p "Input the name: " name
		$(touch $name)
	elif [[ $action = "delete file" ]]
	then
		read -p "Input the name: " name
		$(rm $name)
	elif [[ $action = "copy file" ]]
	then
		read -p "Input the source file: " source
		read -p "Input the new file: " name
		$(cp $source $name)
	elif [[ $action = "move file" ]]
	then
		read -p "Input file name: " name
		read -p "Input new dir: " dir
		$(mkdir $dir)
		$(mv $name ./$dir)
 	else
		echo "ERROR: Command not found! Use \"help\" for more info."
	fi
done
