#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Convert Epoch format to readable format
void convert_time(long unsigned int timestamp, char *buffer, size_t size) {
	time_t Epoch_format = timestamp;
	struct tm *converted_time = localtime(&Epoch_format);

	strftime(buffer, size, "%Y-%m-%d %H:%M:%S", converted_time);
}

//Create database with initial features
Database db_create() {
	Database db;
    	db.size = 0;
    	db.capacity = 4; 
    	db.records = (Record *)malloc(sizeof(Record) * db.capacity);
    	return db;
}

//Append the database
void db_append(Database *db, const Record *item) {

	//Double the size and copy the items into new array if capacity not enough
	if ((*db).size >= (*db).capacity) {
		int doubleCapacity = 2 * (*db).capacity;
        	(*db).records = (Record *)realloc((*db).records, sizeof(Record) * doubleCapacity);
        	(*db).capacity = doubleCapacity;
    	}

	//Append the database
    	(*db).records[(*db).size] = *item; 
    	(*db).size++;
}

//Get pointer to data at index
Record *db_index(Database *db, int index) {

	// If index between bounds return the data pointer
	if (index >= 0 && index < (*db).size) {
		return &(*db).records[index];
    	}

	//Otherwise return null
    	return NULL;
}

//Returns pointer to the item with provided handle 
Record *db_lookup(Database *db, const char *handle) {

	//Look for handle
	for (int i = 0; i < (*db).size; i++) {
		if (strcmp((*db).records[i].handle, handle) == 0) {
			return &(*db).records[i];
        	}
    	}

	//Return null if handle doesn't exist
    	return NULL;
}

//Release memory
void db_free(Database *db) {
	free((*db).records);
    	(*db).records = NULL;
    	(*db).size = 0;
    	(*db).capacity = 0;
}

//Parse single CSV file into a record
Record parse_record(const char *line) {

	//Create record with initial features
	Record record;
    	char *token;
    	char buffer[512];
    	strcpy(buffer, line);

	//Copy handle into record
    	token = strtok(buffer, ",");
    	strncpy(record.handle, token, sizeof(record.handle) - 1);

	//Copy follower count into record
    	token = strtok(NULL, ",");
    	record.follower_count = strtoul(token, NULL, 10);

	//Copy comment into record
    	token = strtok(NULL, ",");
    	strncpy(record.comment, token, sizeof(record.comment) - 1);

	//Copy date last modified into record
    	token = strtok(NULL, ",");
    	record.date_last_modified = strtoul(token, NULL, 10);

	//Return record with copied features
    	return record;
}

//Appends records read from the CSV file into database
void db_load_csv(Database *db, const char *path) {

	//Opens file in read mode
	FILE *csv_file = fopen(path, "r");

	//Prints and error message if path to csv file doesn't exist
	if(!csv_file) {
		fprintf(stderr, "This file doesn't exist: %s\n", path);
		return; 
	}

	//Initialize features
    	char *current_line = NULL;
    	size_t length = 0;
    	ssize_t current;

	//Append the database until it reaches the end of the line and file
    	while ((current = getline(&current_line, &length, csv_file)) != -1) {
		Record record = parse_record(current_line);
        	db_append(db, &record);
    	}

	//Frees last line and closes file
    	free(current_line);
    	fclose(csv_file);
}

//Overwrites new file with the contents of database
void db_write_csv(Database *db, const char *path) {

	//Open file in write mode
    	FILE *csv_file = fopen(path, "w");

	//Overwrites the new file with the contents of database until it reaches to the end of database
    	for (size_t index = 0; index < (*db).size; ++index) {
        	Record *r = &(*db).records[index];
        	fprintf(csv_file, "%s,%lu,%s,%lu\n", (*r).handle, (*r).follower_count, (*r).comment, (*r).date_last_modified);
    	}	

	//Close file
    	fclose(csv_file);
}
