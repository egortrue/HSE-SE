#!/bin/bash

number=0 # five three one seven ===> 5317
function num_parser()
{
    number=0
    if [ -z "$1" ]; then
        return 1
    fi

    if [ "$(echo $1 | awk '/^[0-9]+$/')" = "$1" ]; then
        number=$1
        return 0
    fi

    while [ -n "$1" ]; do
        case "$1" in
            "$(echo $1 | awk '/^[zZ]*[eEiI]+[rR]+[oO]+$/'       )") num=0  ;;
            "$(echo $1 | awk '/^[oOaA]*[nN]+[eE]+$/'            )") num=1  ;;
            "$(echo $1 | awk '/^[tT]*[wW]+[oO]+$/'              )") num=2  ;;
            "$(echo $1 | awk '/^[tTfF]+[hH]*[rR]+[eE]+$/'       )") num=3  ;;
            "$(echo $1 | awk '/^[fF]+[oO]*[uUiI]+[rR]+$/'       )") num=4  ;;
            "$(echo $1 | awk '/^[fFvV]+[iI]+[vV]+[eE]+$/'       )") num=5  ;;
            "$(echo $1 | awk '/^[sSdD]*[iI]+[xX]+$/'            )") num=6  ;;
            "$(echo $1 | awk '/^[sS]*[eEiI]+[vV]+[eEiI]+[nN]+$/')") num=7  ;;
            "$(echo $1 | awk '/^[eEiI]*[iI]+[gG]*[hH]+[tT]+$/'  )") num=8  ;;
            "$(echo $1 | awk '/^[nN]*[iIeE]+[nN]+[eEiI]+$/'     )") num=9  ;;
            *) return 1 ;;
        esac
        number=$(( $number * 10 + $num ))
        shift
    done
    return 0
}

operation=""
function oper_parser()
{
    operation=""
    if [ -z "$1" ]; then
        return 1
    fi

    case "$1" in
        "+") operation="+" ;;
        "-") operation="-" ;;
        [*]) operation="*" ;;
        "/") operation="/" ;;
        "$(echo $1 | awk '/^[pP]+[lL]+[uUaA]+[sSzZ]+$/'                   )") operation="+" ;;
        "$(echo $1 | awk '/^[aA]+[dD]+$/'                                 )") operation="+" ;;
        "$(echo $1 | awk '/^[mM]+[iI]+[nN]+[uU]+[sS]+$/'                  )") operation="-" ;;
        "$(echo $1 | awk '/^[sS]+[uU]+[bB]+[a-zA-Z]*$/'                   )") operation="-" ;;
        "$(echo $1 | awk '/^[yYuU]+[mM]+[nN]+[oO]+[zZgG]+$/'              )") operation="*" ;;
        "$(echo $1 | awk '/^[mM]+[uU]+[lL]+[tT]+[a-zA-Z]*$/'              )") operation="*" ;;
        "$(echo $1 | awk '/^[rR]+[aAoO]+[zZsS]+[dD]+[eEiI]+[lL]+[iIyY]*$/')") operation="/" ;;
        "$(echo $1 | awk '/^[dD]+[iI]+[vV]+[iI]*[dD]*[eEiI]*$/'           )") operation="/" ;;
        *) return 1 ;;
    esac
    return 0 
}

while true
do

    # First number
    read -p "x = " raw_x
    num_parser $raw_x
    while [ "$?" -eq "1" ]; do
        echo "Number is incorrect. Repeat, please:"
        read -p "x = " raw_x
        num_parser $raw_x
    done
    x=$number

    # Second number
    read -p "y = " raw_y
    num_parser $raw_y
    while [ "$?" -eq "1" ]; do
        echo "Number is incorrect. Repeat, please:"
        read -p "y = " raw_y
        num_parser $raw_y
    done
    y=$number

    # Operation
    read -p "oper = " raw_oper
    oper_parser "$raw_oper"
    while [ "$?" -eq "1" ]; do
        echo "Operation is incorrect. Repeat, please:"
        read -p "oper = " raw_oper
        oper_parser "$raw_oper"
    done
    oper=$operation

    # Answer
    echo -e "\n$x $oper $y = $(expr $x "$oper" $y)"
    
    # Continue
    echo -ne "\nDo you want to continue?(Y/n): "
    read cont
    if [ "$cont" == "n" ]; then
        break
    fi

done
