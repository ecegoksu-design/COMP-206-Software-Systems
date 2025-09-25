#!/bin/bash

if [[ $1 = "backup" ]]
then
	#Displays absolute pathname and lists files
	echo "$(pwd)"
	ls *.txt	
	
	#Moves into the created backup directory, displays absolute pathname
	mkdir backup
	cd backup
	echo "Moved to backup directory"
	echo "$(pwd)"
	
	#Copies all the txt files in parent directory to backup 
	cp ../*.txt .
	echo "Copied all text files to backup directory"

	#Displays and appends backup date in date.txt
	echo "Current backup:" > date.txt
	date >> date.txt
	cat date.txt

elif [[ $1 = "archive" ]]
then 
	#Makes sure file format is specified
	if [[ -z "$2" ]]
	then
		echo "Error: Archive task requires file format"
		echo "Usage: ./setup.sh archive <fileformat>"
	
	#Creates an archive for the files with specified format	
	else 
		archive="archive-$(date +"%Y-%m-%d")-.tgz"
		tar czvf "$archive" *.$2
		echo "Created archive $archive"
		ls -l
	fi

elif [[ $1 = "sortedcopy" ]]
then
	#Makes sure there are 3 input parameters
	if [[ $# -ne 3 ]]
	then
		echo "Error: Expected two additional input parameters."
		echo "Usage: ./setup.sh sortedcopy <sourcedirectory> <targetdirectory>"
		exit 1
	
	#Makes sure that the 2nd input parameter is a directory	
	elif [[ ! -d $2 ]]
	then
		echo "Error: Input parameter #2 '$2' is not a directory."
	        echo "Usage: ./setup.sh sortedcopy <sourcedirectory> <targetdirectory>"
		exit 2
	fi
	
	#Determine what to do if the target directory already exists
	if [[ -d $3 ]]
	then
		echo "Directory '$3' already exists. Overwrite? (y/n)"
		read answer
		if [[ $answer = "y" ]]
		then
			rm -r $3
		else 
			exit 3
		fi
	fi

	#Create source and target directories
	source_dir=$2
	target_dir=$3

	#Make target directory
	mkdir "$target_dir"

	#Set the initial file number	
	i=1
	
	#Find and sort the files in source directory in reverse alphabetical order
	for file in $(ls -r "$source_dir" | grep -v '/')
	do
		#Make sure the items are files
		if [[ -f "$source_dir/$file" ]]
		then
			#Copy the files into target directory
			cp "$source_dir/$file" "$target_dir/$i.$file"
			#Increment initial file number
			((i++))
		fi
	done

	exit 0
		
else
	echo "Error: Task must be specified. Supported tasks: backup, archive, sortedcopy."
	echo "Usage: ./setup.sh <task> <additional_arguments>"
fi

