#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TITLE_LENGTH 100
#define MAX_PERFORMER_LENGTH 100
#define MAX_ALBUM_LENGTH 100
#define MAX_GENRE_LENGTH 50
#define MAX_MATCHES 50     /* Max songs we can list for deletion */
#define FILENAME "songs.txt" /* Database filename */

typedef struct SongNode {
    char* title;
    char* performer;
    char* album;
    char* genre;
    int release_year;
    int length_min;
    int length_sec;
    struct SongNode* next;
} SongNode;

/*Core logic functions*/
void add_song_to_list(SongNode** root, const char* title, const char* performer,
                      const char* album, int year, const char* genre,
                      int minutes, int seconds); /*The actual addition*/
int delete_song(SongNode** root, const char* title, const char* performer); /*The actual deleting*/
void free_song_list(SongNode** root); /*Freeing the dynamically allocated memory*/

/*Sorting algorithm functions*/
void insertionSort(SongNode** root, int sort_type);
void sortedInsert(SongNode** sorted_head, SongNode* new_node, int sort_type);

/*File handling functions*/
int load_list_from_file(SongNode** root, const char* filename); /*Read all the data from the file*/
int read_line_from_file(FILE* file, char* buffer, int size); /*Reads a line from the file*/
int save_list_to_file(SongNode* root); /*Saving all the data to the file*/

/*Shortcut functions*/
void read_line(char* buffer, int size); /*read a line from user input*/
int stricmp(const char* s1, const char* s2); /*Case-insensitive matching with user input*/
void wait_for_enter(void);

/*Menu Functions*/
void print_menu(void);
void print_song_details(SongNode* node);
void do_add_song(SongNode** root); /*Interface for addition and passes the exact info to add_song_to_list*/
void do_delete_song(SongNode** root); /*Interface for deleting and passes the exact info to delete_song*/
void do_display_sorted(SongNode** root);/*Gets the sorting type from the user and passes it to insertion_sort*/
void do_list_by_performer(SongNode* root);
void do_list_by_album(SongNode* root);
void do_list_by_genre(SongNode* root);
void do_list_by_year(SongNode* root);
void do_import_songs(SongNode** root); /*Importing Songs from a different file into the memory*/


int main(void) {
    SongNode* root = NULL;
    int choice = 0;
    char choice_buffer[10];

    /* Program Start-up: Load file */
    load_list_from_file(&root, FILENAME);

    do {
        print_menu(); /* Main Menu */
        
        read_line(choice_buffer, 10);
        choice = atoi(choice_buffer);

        switch (choice) {
            case 1:
                do_add_song(&root); /* Add New Song */
                break;
            case 2:
                /*Sorting by user's choice*/
                do_display_sorted(&root);
                break;
            case 3:
                /*List by Artist*/
                do_list_by_performer(root);
                break;
            case 4:
                /*List by Album*/
                do_list_by_album(root);
                break;
            case 5:
                /*List by Year*/
                do_list_by_year(root);
                break;
            case 6:
                /*List by Genre*/
                do_list_by_genre(root);
                break;
            case 7:
                /* Delete Song */
                do_delete_song(&root);
                break;
            case 8:
                /*Import songs from another file*/
                do_import_songs(&root);
                break;
            case 9:
                /* Save and Quit */
                printf("Saving database...\n"); /* */
                int saved_count = save_list_to_file(root);
                
                if (saved_count < 0) {
                    /* Save Failure */
                    printf("ERROR: Could not save database to %s. Quitting without saving.\n", FILENAME);
                } else {
                    /* Save Success */
                    printf("Successfully saved %d songs to %s. Goodbye!\n", saved_count, FILENAME);
                }
                break;
            default:
                /*General Error Handling */
                printf("Invalid choice. Please try again.\n"); /* */
                break;
        }

    } while (choice != 9);

    /* Program Shutdown */
    free_song_list(&root);
    
    return 0;
}

void add_song_to_list(SongNode** root, const char* title, const char* performer,
                      const char* album, int year, const char* genre,
                      int minutes, int seconds) {
    /*Allocating memory for the node*/
    SongNode* new_node = malloc(sizeof(SongNode));
    if (new_node == NULL) {
        exit(1);
    }
    /*Allocating memory for every string in the node*/
    new_node->title = malloc(strlen(title)+1);
    if (new_node->title==NULL)
    exit(1);
    strcpy(new_node->title, title);

    new_node->performer = malloc(strlen(performer)+1);
    if (new_node->performer==NULL)
    exit(1);
    strcpy(new_node->performer, performer);

    new_node->album = malloc(strlen(album)+1);
    if (new_node->album==NULL)
    exit(1);
    strcpy(new_node->album, album);

    new_node->genre = malloc(strlen(genre)+1);
    if (new_node->genre==NULL)
    exit(1);
    strcpy(new_node->genre, genre);

    new_node->release_year = year;
    new_node->length_min = minutes;
    new_node->length_sec = seconds;

    new_node->next = NULL;

    if (*root == NULL) {
        *root = new_node; /*If the linked list is empty*/
        return;
    }
    SongNode* curr = *root;
    while (curr->next != NULL) {
        curr = curr->next; /*Searches for the last node in the list*/
    }
    curr->next = new_node; /*Appends the new node at the end of the list*/
}


int delete_song(SongNode** root, const char* title, const char* performer) {
    SongNode* curr = *root;
    SongNode* prev = NULL;
    if (curr == NULL) return 0;
    /*If the song was the root*/
    if (strcmp(curr->title, title) == 0 && strcmp(curr->performer, performer) == 0) {
        *root = curr->next;
        free(curr->title);
        free(curr->performer);
        free(curr->album);
        free(curr->genre);
        free(curr);
        return 1;
    }
    /*If the song was anywhere else*/
    while (curr != NULL &&
           (strcmp(curr->title, title) != 0 || strcmp(curr->performer, performer) != 0)) {
        prev = curr;    
        curr = curr->next;
    }

    if (curr == NULL) return 0;

    prev->next = curr->next;
    free(curr->title);
    free(curr->performer);
    free(curr->album);
    free(curr->genre);
    free(curr);
    return 1;
}

void free_song_list(SongNode** root) {
    SongNode* curr = *root;
    while (curr != NULL) {
        SongNode* aux = curr;
        curr = curr->next;
        free(aux->title);
        free(aux->performer);
        free(aux->album);
        free(aux->genre);
        free(aux);
    }
    *root = NULL;
}

/*
 * insertionSort: Sorts the linked list by a chosen attribute
 *
 * This implementation uses the Insertion Sort algorithm, which iteratively builds the sorted portion
 * of the list by inserting each element into its correct position.
 * 
 * Sorting is done by comparing nodes based on the chosen criteria (sort_type).
 */
void insertionSort(SongNode** root, int sort_type) {
    SongNode* head = *root;
    
    /* A list of 0 or 1 items is already sorted */
    if (head == NULL || head->next == NULL) {
        return;
    }

    SongNode* sorted_head = NULL; /* This will be the head of the new, sorted list */
    SongNode* curr = head;        /* Iterator for the original, unsorted list */

    while (curr != NULL) {
        /* Save the next node in the *original* list */
        SongNode* next_in_original = curr->next;
        
        /* Insert the 'curr' node into the 'sorted_head' list */
        sortedInsert(&sorted_head, curr, sort_type);

        /* Move to the next node in the *original* list */
        curr = next_in_original;
    }

    /* Update the main head pointer to point to our new sorted list */
    *root = sorted_head;
}

/*
 * sortedInsert: Inserts a single node into its correct position in a (potentially empty) sorted list.
*/
void sortedInsert(SongNode** sorted_head, SongNode* new_node, int sort_type) {
    
    /* We need to determine if new_node should be the new head.
     * This is true if the list is empty OR if new_node "comes before"the current head.
    */
    int is_before = 0;
    if (*sorted_head != NULL) {
        if (sort_type == 1) { /* 1: By Title (A-Z) */
            is_before = (strcmp(new_node->title, (*sorted_head)->title) <= 0);
        } else if (sort_type == 2) { /* 2: By Performer (A-Z) */
            is_before = (strcmp(new_node->performer, (*sorted_head)->performer) <= 0);
        } else if (sort_type == 3) { /* 3: By Release Year (Newest first) */
            is_before = (new_node->release_year >= (*sorted_head)->release_year);
        }
    }

    /* Case 1: Insert at the head (list is empty OR new_node is "smallest") */
    if (*sorted_head == NULL || is_before) {
        new_node->next = *sorted_head;
        *sorted_head = new_node;
        return;
    }

    /* Case 2: Insert in the middle or at the end. */
    SongNode* curr = *sorted_head;
    
    /* Loop until we find the node *before* the insertion point */
    while (curr->next != NULL) {
        
        /* Check if new_node should go *before* curr->next */
        SongNode* next_node = curr->next;
        int insert_here = 0;
        
        if (sort_type == 1) {
            insert_here = (strcmp(new_node->title, next_node->title) <= 0);
        } else if (sort_type == 2) {
            insert_here = (strcmp(new_node->performer, next_node->performer) <= 0);
        } else if (sort_type == 3) {
            insert_here = (new_node->release_year >= next_node->release_year);
        }

        if (insert_here) {
            new_node->next = curr->next;
            curr->next = new_node;
            return;
        }
        
        curr = curr->next;
    }

    /* Case 3: Insert at the very end (loop finished, no spot found) */
    curr->next = new_node;
    new_node->next = NULL;
}

int load_list_from_file(SongNode** root, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("No database file found. Starting with an empty database.\n");
        return 0;
    }

    char title[MAX_TITLE_LENGTH];
    char performer[MAX_PERFORMER_LENGTH];
    char album[MAX_ALBUM_LENGTH];
    char genre[MAX_GENRE_LENGTH];
    char buffer[20];
    int year, minutes, seconds;
    int count = 0;

    while (read_line_from_file(file, title, MAX_TITLE_LENGTH)) {
        if (!read_line_from_file(file, performer, MAX_PERFORMER_LENGTH)) break;
        if (!read_line_from_file(file, album, MAX_ALBUM_LENGTH)) break;
        if (!read_line_from_file(file, buffer, 20)) break;
        year = atoi(buffer);
        if (!read_line_from_file(file, genre, MAX_GENRE_LENGTH)) break;
        if (!read_line_from_file(file, buffer, 20)) break;
        minutes = atoi(buffer);
        if (!read_line_from_file(file, buffer, 20)) break;
        seconds = atoi(buffer);

        add_song_to_list(root, title, performer, album, year, genre, minutes, seconds);
        count++;
    }

    fclose(file);
    printf("Successfully loaded %d songs from %s.\n", count, filename);
    return count;
}

int read_line_from_file(FILE* file, char* buffer, int size) {
    if (fgets(buffer, size, file) == NULL) {
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    return 1;
}

int save_list_to_file(SongNode* root) {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        return -1;
    }
    int count = 0;
    for (SongNode* curr = root; curr != NULL; curr = curr->next) {
        fprintf(file, "%s\n", curr->title);
        fprintf(file, "%s\n", curr->performer);
        fprintf(file, "%s\n", curr->album);
        fprintf(file, "%d\n", curr->release_year);
        fprintf(file, "%s\n", curr->genre);
        fprintf(file, "%d\n", curr->length_min);
        fprintf(file, "%d\n", curr->length_sec);
        count++;
    }
    fclose(file);
    return count;
}

void read_line(char* buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

int stricmp(const char* s1, const char* s2) {
    while (*s1 && *s2 && (tolower((unsigned char)*s1) == tolower((unsigned char)*s2))) {
        s1++;
        s2++;  /*Type casting to handle the extended ASCII characters*/
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

void wait_for_enter(void) {
    char buffer[2];
    printf("\nPress Enter to continue...");
    read_line(buffer, 2);
}

void print_menu() {
    printf("\n==========\n");
    printf("SongDB Music Library\n");
    printf("============================================\n");
    printf("1. Add New Song\n");
    printf("2. Display All Songs (Sorted)\n");
    printf("3. List by Artist\n");
    printf("4. List by Album\n");
    printf("5. List by Year\n");
    printf("6. List by Genre\n");
    printf("7. Delete Song\n");
    printf("8. Import from another file\n");
    printf("9. Save and Quit\n");
    printf("==========\n");
    printf("Enter your choice (1-9): ");
}

void print_song_details(SongNode* node) {
    printf("  Title: %s\n", node->title);
    printf("  Performer: %s\n", node->performer);
    printf("  Album: %s (%d)\n", node->album, node->release_year);
    printf("  Genre: %s (Length: %d:%02d)\n\n",
           node->genre, node->length_min, node->length_sec);
}

void do_add_song(SongNode** root) {
    char title[MAX_TITLE_LENGTH], performer[MAX_PERFORMER_LENGTH], album[MAX_ALBUM_LENGTH], genre[MAX_GENRE_LENGTH];
    char buffer[20];
    int year = -1, minutes = -1, seconds = -1;

    printf("Enter Title: ");
    read_line(title, MAX_TITLE_LENGTH);
    printf("Enter Performer: ");
    read_line(performer, MAX_PERFORMER_LENGTH);
    printf("Enter Album: ");
    read_line(album, MAX_ALBUM_LENGTH);
    do {
        printf("Enter Release Year: ");
        read_line(buffer, 20);
        year = atoi(buffer);
        if (year <= 0) {
            printf("Invalid year. Please enter a positive number.\n");
        }
    } while (year <= 0);
    printf("Enter Genre: ");
    read_line(genre, MAX_GENRE_LENGTH);
    do {
        printf("Enter Length (Minutes): ");
        read_line(buffer, 20);
        minutes = atoi(buffer);
        if (minutes < 0) {
            printf("Invalid minutes. Please enter a positive number.\n");
        }
    } while (minutes < 0);
    do {
        printf("Enter Length (Seconds): ");
        read_line(buffer, 20);
        seconds = atoi(buffer);
        if (seconds < 0 || seconds > 59) {
            printf("Invalid seconds. Please enter a number between 0 and 59.\n");
        }
    } while (seconds < 0 || seconds > 59);

    add_song_to_list(root, title, performer, album, year, genre, minutes, seconds);
    printf("Success: \"%s by %s\" has been added.\n", title, performer);
}

void do_delete_song(SongNode** root) {
    char title_to_find[MAX_TITLE_LENGTH];
    char confirm_buffer[10];
    SongNode* matches[MAX_MATCHES];
    int match_count = 0;
    SongNode* node_to_delete = NULL;
    char deleted_title_copy[MAX_TITLE_LENGTH];

    /*Empty Check*/
    if (*root == NULL) {
        printf("The database is empty. No songs to delete.\n");
        return;
    }

    /*Search for song*/
    printf("Enter the Title of the song to delete: ");
    read_line(title_to_find, MAX_TITLE_LENGTH);
    SongNode* curr = *root;
    while (curr != NULL && match_count < MAX_MATCHES) {
        if (stricmp(curr->title, title_to_find) == 0) {
            matches[match_count] = curr;
            match_count++;
        }
        curr = curr->next;
    }

    if (match_count == 0) {
        printf("No song found with title matching \"%s\".\n", title_to_find);
        return;
    }
    if (match_count == 1) {
        node_to_delete = matches[0];
        printf("Found one song:\n");
        printf("  Title: %s, Performer: %s, Album: %s (%d)\n",
               node_to_delete->title, node_to_delete->performer,
               node_to_delete->album, node_to_delete->release_year);
        printf("Are you sure you want to delete this song? (y/n): ");
        read_line(confirm_buffer, 10);
        if (stricmp(confirm_buffer, "Y") != 0) {
            printf("Deletion cancelled.\n");
            return;
        }
    }
    else {
        printf("Found multiple songs matching \"%s\":\n", title_to_find);
        int i;
        for (i = 0; i < match_count; i++) {
            printf("  %d. Title: %s, Performer: %s\n", i + 1,
                   matches[i]->title, matches[i]->performer);
        }
        printf("  0. Cancel\n");
        printf("Enter the number of the song to delete (or 0 to cancel): ");
        read_line(confirm_buffer, 10);
        int choice = atoi(confirm_buffer);
        if (choice == 0) {
            printf("Deletion cancelled.\n");
            return;
        }
        if (choice < 1 || choice > match_count) {
            printf("Invalid choice.\n");
            return;
        }
        node_to_delete = matches[choice - 1];
    }
    if (node_to_delete != NULL) {
        strncpy(deleted_title_copy, node_to_delete->title, MAX_TITLE_LENGTH);
        deleted_title_copy[MAX_TITLE_LENGTH - 1] = '\0';
        if (delete_song(root, node_to_delete->title, node_to_delete->performer)) {
            printf("Song \"%s\" has been deleted.\n", deleted_title_copy);
        } else {
            printf("Error: Song cound not be deleted.\n");
        }
    }
}

void do_display_sorted(SongNode** root) {
    SongNode* head = *root;
    
    if (head == NULL) {
        printf("The database is empty. No songs to display.\n");
        return;
    }

    char buffer[10];
    int sort_choice = -1;
    printf("How would you like to sort?\n");
    printf("  1. By Title (A-Z)\n");
    printf("  2. By Performer (A-Z)\n");
    printf("  3. By Release Year (Newest first)\n");
    printf("  0. Cancel\n");
    printf("Enter choice: ");
    read_line(buffer, 10);
    sort_choice = atoi(buffer);

    if (sort_choice == 0) {
        printf("Cancelling.\n");
        return;
    }

    if (sort_choice >= 1 && sort_choice <= 3) {
        
        insertionSort(root, sort_choice);
        
        printf("\n--- Displaying All Songs ---\n");
        for (SongNode* curr = *root; curr != NULL; curr = curr->next) {
            print_song_details(curr);
        }
        wait_for_enter();
    } else {
        printf("Invalid choice.\n");
    }
}

void do_list_by_performer(SongNode* root) {
    char performer[MAX_PERFORMER_LENGTH];
    int found = 0;
    printf("Enter Performer name to search for: ");
    read_line(performer, MAX_PERFORMER_LENGTH);
    printf("\n--- Songs by %s ---\n", performer);
    for (SongNode* curr = root; curr != NULL; curr = curr->next) {
        if (stricmp(curr->performer, performer) == 0) {
            print_song_details(curr);
            found = 1;
        }
    }
    if (!found) {
        printf("  (No songs found for the performer \"%s\")\n", performer);
    }
    wait_for_enter();
}

void do_list_by_album(SongNode* root) {
    char album[MAX_ALBUM_LENGTH];
    int found = 0;
    printf("Enter an album to search for: ");
    read_line(album, MAX_TITLE_LENGTH);
    printf("\n--- Songs by %s ---\n", album);
    for (SongNode* curr = root; curr != NULL; curr = curr->next) {
        if (stricmp(curr->album, album) == 0) {
            print_song_details(curr);
            found = 1;
        }
    }
    if (!found) {
        printf("  (No songs found in the album \"%s\")\n", album);
    }
    wait_for_enter();
}

void do_list_by_year(SongNode* root) {
    char buffer[20];
    int year;
    int found = 0;
    printf("Enter a genre to search for: ");
    read_line(buffer, 20);
    year = atoi(buffer);
    printf("\n--- Songs released in %d ---\n", year);
    for (SongNode* curr = root; curr != NULL; curr = curr->next) {
        if (curr->release_year == year) {
            print_song_details(curr);
            found = 1;
        }
    }
    if (!found) {
        printf("  (No songs found in the year \"%d\")\n", year);
    }
    wait_for_enter();
}

void do_list_by_genre(SongNode* root) {
    char genre[MAX_GENRE_LENGTH];
    int found = 0;
    printf("Enter a genre to search for: ");
    read_line(genre, MAX_TITLE_LENGTH);
    printf("\n--- Songs by %s ---\n", genre);
    for (SongNode* curr = root; curr != NULL; curr = curr->next) {
        if (stricmp(curr->genre, genre) == 0) {
            print_song_details(curr);
            found = 1;
        }
    }
    if (!found) {
        printf("  (No songs found in the genre \"%s\")\n", genre);
    }
    wait_for_enter();
}

void do_import_songs(SongNode** root) {
    char filename[100];
    printf("Enter the filename to import from: ");
    read_line(filename, 100);

    /*Prevent double loading the main database*/
    if (strcmp(filename, FILENAME) == 0) {
        printf("Warning: '%s' is your main database and is already loaded.\n", FILENAME);
        return;
    }

    int count = load_list_from_file(root, filename);
    if (count >= 0) {
        printf("Success: Imported %d songs from %s.\n", count, filename);
    } else {
        printf("Error: Could not open file '%s'.\n", filename);
    }
    wait_for_enter();
}