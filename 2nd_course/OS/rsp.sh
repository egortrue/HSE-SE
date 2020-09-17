#!/bin/bash

echo -e "ROCK, SCCISSORS and PAPER\n"

while true
do
    	rand_case=$((RANDOM%100))
    	read -p "Your choice: " action
	if [[ $rand_case -le 56 ]]
	then
		if [[ $action = "rock" ]]
		then
			rand=2
		elif [[ $action = "sccissors" ]]
		then
			rand=0
		else
			rand=1
		fi
	else
		rand=$((RANDOM%3))
	fi

	if [[ $action = "rock" ]]
	then
		if [[ $rand = 0 ]]
		then
			echo "Nobody win"
		elif [[ $rand = 1 ]]
		then
			echo "You win"
		else
			echo "You lose"
		fi
	elif [[ $action = "sccissors" ]]
	then
		if [[ $rand = 0 ]]
		then
			echo "You lose"
		elif [[ $rand = 1 ]]
		then
			echo "Nobody win"
		else
			echo "You win"
		fi
	elif [[ $action = "paper" ]]
	then
		if [[ $rand = 0 ]]
		then
			echo "You win"
		elif [[ $rand = 1 ]]
		then
			echo "You lose"
		else
			echo "Nobody win"
		fi
	elif [[ $action = "exit" ]]
	then
		exit 0
	else
		echo "Wrong choice"
	fi
done
