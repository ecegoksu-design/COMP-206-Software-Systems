#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "database.h"

// Determine universal maximum handle and comment length
#define MAXIMUM_HANDLE 32
#define MAXIMUM_COMMENT 64

// Assume unsaved changes until saved
int unsaved = 1;

void convert_time(long unsigned int timestamp, char *buffer, size_t size);

// Validate handle
int validate_handle(const char *handle, Database *db) {

	// Print error if handle length exceeds maximum
	if (strlen(handle) >= MAXIMUM_HANDLE) {
        	printf("Error: handle too long.\n");
        	return 0; 
    	}

	// Print error if handle already exists for add function
	if (db_lookup(db, handle) != NULL) {
        	printf("Error: handle %s already exists.\n", handle);
        	return 0;
    }
	// Appropriate handle
	return 1; 
}

// Validate comment
int validate_comment(const char *comment) {

	// Print error if comment contains commas
	if (strchr(comment, ',') != NULL) {
		printf("Error: comment cannot contain commas.\n");
		return 0;
	}	

	// Appropriate comment
	return 1;
}


// Print database
void print_database(const Database *db) {

    // Print header
    printf("%-32.32s | %-12s | %-24s | %-64.64s\n", "HANDLE", "FOLLOWERS", "LAST MODIFIED", "COMMENT");

    // Convert Epoch time to readable format
    for (size_t index = 0; index < db->size; index++) {
        char converted_time[24];
        convert_time(db->records[index].date_last_modified, converted_time, sizeof(converted_time));

        // Print database
        printf("%-32.32s | %-12lu | %-24.24s | %-64.64s\n",
               db->records[index].handle,
               db->records[index].follower_count,
               converted_time,
               db->records[index].comment);
    }
}

// Add record to database
int add_database(Database *db, char *input) {

	// Get method add
	char *token = strtok(input, " \n"); 
	token = strtok(NULL, " \n");

	// Print error message if method entered without arguments
	if (token == NULL) {
		printf("Error: usage: add HANDLE FOLLOWERS\n");
		return -1;
	}

	// Get next argument which is handle
	char handle[MAXIMUM_HANDLE];
	strncpy(handle, token, MAXIMUM_HANDLE);

	// Validate handle
	if (!validate_handle(handle, db)) {
		return -1;
	}

	// Get next argument which is follower count
	token = strtok(NULL, " \n"); 
    	if (token == NULL) {
        	printf("Error: Follower count is missing\n");
        	return -1;
    	}

	// Pointer to follower count
	char *end_pointer;
	unsigned long follower_count = strtoul(token, &end_pointer, 10);

	// Print error message if follower count is not an integer
	if (*end_pointer != '\0') {
		printf("Error: Follower count must be an integer\n");
		return -1;
	}

	// Get user input for comment
	printf("Comment > ");
	char comment[MAXIMUM_COMMENT];

	// Validate input
	if (fgets(comment, sizeof(comment), stdin) == NULL) {
        	printf("Cannot read comment.\n");
        	return -1;
    	}

	// Remove new line character at the end of comment
	comment[strcspn(comment, "\n")] = 0;

	// Validate comment
	if (!validate_comment(comment)) {
		return -1;
	}

	// Create a record with user input
	Record account = {.date_last_modified = time(NULL)};
    	strncpy(account.handle, handle, MAXIMUM_HANDLE - 1);
    	account.follower_count = follower_count;
    	strncpy(account.comment, comment, MAXIMUM_COMMENT - 1);

	// Append database
	db_append(db, &account);

	// Successful operation
	return 0;
}

int update_database(Database *db, char *input) {
	
	// Get method update
	char *token = strtok(input, " ");

	// Initialize features
	char handle[MAXIMUM_HANDLE];
	unsigned long new_followers;

	// Get next argument which is handle
	token = strtok(NULL, " ");

	// Print error message if method called without arguments
	if (token == NULL) {
        	printf("Error: usage: update HANDLE FOLLOWERS\n");
        	return -1;
    	}

	// Get next argument which is follower count
    	strncpy(handle, token, MAXIMUM_HANDLE - 1);
    	token = strtok(NULL, " ");

	// Print error if follower count missing
	if (token == NULL) {
		printf("Error: Follower count is missing\n");
		return -1; 
	}

	// Get pointer to follower count
    	char *end_pointer;
    	new_followers = strtoul(token, &end_pointer, 10);

	// Print error message if follower count is not an integer
	if (*end_pointer != '\0') {
        	printf("Error: Follower count must be an integer\n");
        	return -1;
    	}

	// Lookup the record
	 Record *record = db_lookup(db, handle);

	// Print error message if handle doesn't exist
	if (!record) {
		printf("Error: No entry with handle %s\n", handle);
		return -1;
	}

	// Get user input for comment
    	printf("Comment > ");
    	char comment[MAXIMUM_COMMENT];

    	// Validate user input
    	if (fgets(comment, sizeof(comment), stdin) == NULL) {
        	printf("Cannot read comment.\n");
        	return -1;
    	}

    	// Remove new line character at the end of comment
    	comment[strcspn(comment, "\n")] = 0;

    	// Validate comment
    	if (!validate_comment(comment)) {
        	return -1;
    	}

	// Update the record with user input
	strncpy(record->comment, comment, sizeof(record->comment) - 1);
    	record->follower_count = new_followers;
    	record->date_last_modified = time(NULL);

	// Successful operation
	return 0;
}

// Save the changes 
void save_database(Database *db, const char *path) {
	db_write_csv(db, path);
    	printf("Wrote %d records.\n", db->size);

    	// Reset unsaved changes after call to save
    	unsaved = 0; 
}

// Main loop for methods
int main_loop(Database * db) {
	char user_input[256];

	// Print the loaded number of records
    	printf("Loaded %d records.\n", db->size);

	// Loop for methods
    	while (1) {
        	printf("> ");
        	if (!fgets(user_input, sizeof(user_input), stdin)) {
            		continue;
        	}

		// Remove new line character at the end of input
        	user_input[strcspn(user_input, "\n")] = 0;

		// 1. List database
       		if (strncmp(user_input, "list", 4) == 0) {
            		print_database(db);
        	}

		// 2. Add record to database
        	else if (strncmp(user_input, "add", 3) == 0) {

            		// Update unsaved changes if there is an added record
            		if (add_database(db, user_input) == 0) {
                		unsaved = 1;
            		}
        	}

		// 3. Update record in database
        	else if (strncmp(user_input, "update", 6) == 0) {

            		// Update unsaved changes if there is an updated record
            		if (update_database(db, user_input) == 0) {
                		unsaved = 1;
            		}
        	}

		// 4. Save the changes
        	else if (strncmp(user_input, "save", 4) == 0) {

            		// Reset unsaved changes after saving
            		save_database(db, "database.csv");
            		unsaved = 0;
        	}

		// 5. Exit program
        	else if (strncmp(user_input, "exit", 4) == 0) {

            		// Warn the user if there are unsaved changes
            		if (unsaved) {
                		printf("Error: you did not save your changes. Use `exit fr` to force exiting anyway.\n");
                		unsaved = 0;
            		}	 
            
            		// Exit the program if the changes are saved
            		else {
                		break; 
            		}
        	}

		// 6. Force exit without changes
        	else if (strncmp(user_input, "exit fr", 7) == 0) {
            		break;
        	}

       	 	// 7. Inappropriate command warning
        	else {
            		printf("Enter appropriate command.\n");
        	}
	}

	// Successful operation
    	return 0;
}

int main() {

	// Initialize database
    	Database db = db_create();

    	// Load records from CSV file
    	db_load_csv(&db, "database.csv");

    	// Enter main loop and get return code
    	int return_code = main_loop(&db);

    	// Free memory allocated
    	db_free(&db);

    	// Return successfull or unsuccessful
    	return return_code;
}


