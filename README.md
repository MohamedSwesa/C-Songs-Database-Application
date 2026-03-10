# C-Songs-Database-Application
Developed a terminal-based database application in C (C90 standard) to manage music catalogs. Designed an in-memory singly linked list architecture with dynamic memory allocation to efficiently handle complex string data, alongside robust file I/O operations for reliable data persistence.

**Key Features Implemented**
**Data Structures:** Engineered a custom singly linked list to store, traverse, and manage complex song records efficiently.
My apologies! I focused so much on the list that I skipped the introductory paragraph from your reference image.

Here is the complete, corrected version with the description included at the top:

# Songs Database (C)

A console-based music catalog manager written in C (C90 standard) that allows users to store, sort, and filter song records using an in-memory linked list architecture.

## Features

* Add, delete, and view song entries
* Sort database dynamically by Title, Artist, or Year (Insertion Sort)
* Filter and search tracks by Artist, Album, Year, or Genre
* Import song records from external files
* File-based database persistence

## Data Structures Used

* Structs
* Singly linked list
* Dynamic memory allocation (malloc, free)
* File I/O
* Pointers and string manipulation

```markdown
# Songs Database (C)

A console-based music catalog manager written in C (C90 standard) that allows users to store, sort, and filter song records using an in-memory linked list architecture.

## Features

* Add, delete, and view song entries
* Sort database dynamically by Title, Artist, or Year (Insertion Sort)
* Filter and search tracks by Artist, Album, Year, or Genre
* Import song records from external files
* File-based database persistence

## Data Structures Used

* Structs
* Singly linked list

* Dynamic memory allocation (malloc, free)
* File I/O
* Pointers and string manipulation

## File Format & Parsing

The database relies on a plain text file to load and save records. Below is an example of the `songs.txt` structure, which uses a standard comma-separated format:

```text
Bohemian Rhapsody,Queen,A Night at the Opera,1975,Rock
Billie Jean,Michael Jackson,Thriller,1982,Pop
Smells Like Teen Spirit,Nirvana,Nevermind,1991,Grunge
Hotel California,Eagles,Hotel California,1976,Classic Rock

```

**Technical Note on Parsing:** When loading the database, the application reads the file line-by-line (e.g., using `fgets`). It then parses each line using string manipulation functions (like `strtok` or `sscanf`) to separate the song attributes by the comma delimiter. The year field is converted to an integer, dynamic memory is allocated for the text fields (Title, Artist, Album, Genre), and a new struct node is populated and appended to the singly linked list.

## Files

* **songs.txt** – default database for storing song entries on startup and exit
* **main.c** – contains the core logic and execution loop

## Compilation

Compile using GCC:

`gcc main.c -o songs`

Run:

`./songs`
