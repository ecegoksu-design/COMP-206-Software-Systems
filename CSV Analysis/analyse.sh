#!/bin/bash
#Ece Goksu
#ID: 261138642

if [[ $1 = "avg" ]]
then
        if [[ $# -ne 2 ]]
        then
                echo "Error: Expected one additional input parameter."
                echo "Usage: ./analyse avg <country>"
                exit 2
        fi

	avg_population=$(awk -v country="$2" '
	BEGIN {FS=","} 
	$4 == country { 
		total_population += $8 
		country_occurence ++ 
	} 
	END { 
		printf "%.0f", total_population / country_occurence 
       	}' worldcities.csv)
    
	echo $avg_population

elif [[ $1 = "total" ]]
then
        if [[ $# -ne 2 ]]
        then
                echo "Error: Expected one additional input parameter."
                echo "Usage: ./analyse total <country>"
                exit 3
        fi

        total_population=$(awk -v country="$2" '
        BEGIN {FS=","} 
        $4 == country { 
                total_population += $8 
        } 
        END { 
                printf total_population 
        }' worldcities.csv)

        echo $total_population

elif [[ $1 = "diff" ]]
then
	if [[ $# -ne 3 ]]
        then
                echo "Error: Expected two additional input parameters."
                echo "Usage: ./analyse diff <city1> <city2>"
                exit 4
        fi

        diff_population=$(awk -v city1="$2" -v city2="$3" '
        BEGIN {FS=","}
        $1 == city1 {
            population1 = $8
        }
        $1 == city2 {
            population2 = $8
        }
        END {
            printf "%.0f", population1 - population2
        }' worldcities.csv)

        echo $diff_population

else
        echo "Error: Task must be specified. Supported tasks: avg, total, diff."
        exit 1
fi
