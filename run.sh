#!/bin/bash

PARALLEL_FILE=./parallel_version
SEQUENTIAL_FILE=./original_version
SMALL_FILE=./small.txt
MEDIUM_FILE=./medium.txt
BIG_FILE=./big.txt
TURNING_POINT_FILE=./turning_point.txt

run(){
	for input in small.txt medium.txt big.txt turning_point.txt 
		do
			echo "Running sequential algorithm... Input file = $input"
			START=$(date +%s.%N)
			for ((i=1;i<=times;i++)); do
				$SEQUENTIAL_FILE -k 3 -a $input
			done
			END=$(date +%s.%N)
			TIME_SEQ=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
			echo "It took $TIME_SEQ seconds to run $input sequentialy"
			for threads in 2 4 8 12 16
				do
					echo "Running paralell algorithm with $threads threads... Input file = $input"
					START=$(date +%s.%N)
					for ((i=1;i<=times;i++)); do
						$PARALLEL_FILE -t $threads -k 3 -a $input
					done
					END=$(date +%s.%N)
					TIME_PAR=$(python3 -c "print('{:.2f}'.format(${END} - ${START}))")
					echo "It took $TIME_PAR seconds to run $input paralelly with $threads threads"
					SPEEDUP=$(python3 -c "print('{:.2f}'.format(${TIME_SEQ} / ${TIME_PAR}))")
					echo "Speed up for input $input and $threads threads is $SPEEDUP"
					echo ""
				done
		done
}

helpFunction()
{
   echo ""
   echo "Usage: $0 -n "
   echo -e "\t-n number of times that the programas will run "
}


while getopts "n:" opt
do
   case "$opt" in
      n ) times="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

if [ -z "$times" ]
then
   echo "Parameters are empty";
   helpFunction
fi


if [ ! -f "$SMALL_FILE" ] || [ ! -f "$MEDIUM_FILE" ] || [ ! -f "$BIG_FILE" ] || [ ! -f "$TURNING_POINT_FILE" ]  ; then
	for i in SMALL MEDIUM BIG TURNING_POINT 
	do
		echo "Generating $i input file..."
		case "$i" in
			SMALL ) base64 /dev/urandom | head -c 550502400 > small.txt ;;
			MEDIUM ) base64 /dev/urandom | head -c 1000000000 > medium.txt ;;
			BIG ) base64 /dev/urandom | head -c 1900000000 > big.txt ;;
			TURNING_POINT ) base64 /dev/urandom | head -c 2097000000 > turning_point.txt ;;
		esac
	done
fi

echo ""

if [ -f "$PARALLEL_FILE" ] && [ -f "$SEQUENTIAL_FILE" ] ; then
	run
else 
    echo "$PARALLEL_FILE or $SEQUENTIAL_FILE do not exist. Please run make before running this script!"
fi

