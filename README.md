# Assignment-1: CS525 Advanced Database Organization

## Storage Manager

**Team Members:**
- Omkar Shewale
- Hamdan Patel
- Merlin Simoes
- Mikkilineni Lekhana Sai
- Rajashekar Mudigonda

## Project Modules

- **C source files:** `storage_mgr.c`, `dberror.c`, `test_assign1_1.c`, `test_assign1_2.c`
- **Header files:** `storage_mgr.h`, `dberror.h`, `test_helper.h`

## Aim

The goal of this assignment is to implement a simple storage manager - a module that is capable of reading blocks from a file on disk into memory and writing blocks from memory to a file on disk. The storage manager deals with pages (blocks) of fixed size (PAGE_SIZE). In addition to reading and writing pages from a file, it provides methods for creating, opening, and closing files. The storage manager has to maintain several types of information for an open file: The number of total pages in the file, the current page position (for reading and writing), the file name, and a POSIX file descriptor or FILE pointer.

## Contributions

- **Merlin Simoes:** `initStorageManager()`, `CreatePageFile()`, Readme
- **Hamdan Patel:** `writeBlock()`, `writeCurrentBlock()`, `appendEmptyBlock()`, `ensureCapacity()`
- **Omkar Shewale:** `readBlock()`, `getBlockPos()`, `readFirstBlock()`, `readPreviousBlock()`, `readNextBlock()`, `readLastBlock()`
- **Mikkilineni Lekhana Sai:** `OpenPageFile()`, `ClosePageFile()`, Readme
- **Rajashekar Mudigonda:** `DestroyPageFile()`, `readCurrentBlock()`, Readme

## Contents

1. Instructions to run the code
2. Description of functions used
3. Implementation

### 1. Instructions to run the code

#### For executing mandatory test cases:

1. In the terminal, navigate to the assignment directory.

2. Type the following commands: 
  
  ### ` bash make -f Makefile./storage_mgr `
   


## Description of Functions Used

### `initStorageManager`

- This function is a placeholder and currently has no functionality.
- It serves as an entry point to the program but does nothing in this implementation.

### `CreatePageFile`

- This function takes `fileName` as a parameter and creates a new page file with the given `fileName`.
- It opens the file in binary write mode ("wb") and allocates an empty page of size 4KB in memory.
- If memory allocation fails, it returns `RC_WRITE_FAILED` error code.
- If the memory allocation is successful, it writes the empty page to the file.
- The file is then closed, memory allocated for the empty page is freed, and an error code is returned if the write fails.
- It finally returns `RC_OK` if the Page File is successfully created.

### `OpenPageFile`

- This function opens an existing page file specified by `fileName` in binary read/write mode ("rb+").
- It calculates the total number of pages, resets the file position to the beginning, and initializes fields of the file handle (`SM_FileHandle`).
- It returns an error code if the file doesn't exist.

### `ClosePageFile`

- This function closes an open page file.
- It returns an error code if the file is already closed and returns an error "file handle not initialized".
- It calls the `closeFileHandle` function to perform the actual closing.

### `DestroyPageFile`

- It first checks whether the file exists or not.
- This function attempts to delete the specified file using the `remove` function.
- It returns an `RC_FILE_NOT_FOUND` error code if the file is not found or if deletion fails.

### `readBlock`

- This function accepts `int pageNum`, `SM_FileHandle fhandle`, and `SM_PageHandle memPage` as parameters and reads a block (page) from the specified page number (`pageNum`) into a memory page (`memPage`).
- It performs error checks to ensure a valid file handle, page number, and existence of the page. It also retrieves the file pointer from the management information `mgmtInfo`.
- It seeks to the desired file position using `fseek()` function, reads the page data into `memPage` using `fread()`, and stores the data in `bytesRead` variable.
- It then updates the current page position in the file handle to `pageNum`.
- It returns `RC_READ_NON_EXISTING_PAGE` error code for any failures.

### `getBlockPos`

- This function returns the current block (page) position in the file handle.
- It first checks for a valid file handle and management information, then it gets the current Page Position from `SM_FileHandle` metadata.

### `readFirstBlock`

- This function reads the first block (page) in the file by calling `readBlock` with `pageNum` set to 0 and stores its content in `memPage`.

### `readPreviousBlock`

- This function reads the block preceding the current block in the file by calling `readBlock` method with `pageNum` set to `curPagePos – 1` and stores its content in `memPage`.

### `readCurrentBlock`

- This function takes in `fHandle` and `memPage` as parameters to read data from disk to memory page.
- This function reads the current block (page) by calling `readBlock` with `pageNum` set to `curPagePos` and stores its content in `memPage`.

### `readNextBlock`

- This function reads the block following the current block in the file by calling `readBlock` with `pageNum` set to `curPagePos + 1` and stores its content in `memPage`.

### `readLastBlock`

- This function reads the last block (page) in the file by calling `readBlock` with `pageNum` set to `totalNumPages – 1` and stores its content in `memPage`.

### `writeBlock`

- This function takes `int pageNum`, pointer to `SM_FileHandle`, and pointer to `SM_PageHandle` and writes a block (page) specified by `pageNum` from `memPage` to the file.
- It checks if the file is open for writing and if the file exists.
- It then checks whether `pageNum` is valid or not by checking `pageNum < totalNumPages`.
- It calculates the offset to find the position in the file and stores it in the `offset` long variable.
- It then moves the file pointer `mgmtInfo` to the calculated offset using `fseek()` function.
- If the seek is successful, then it writes the content of `memPage` to the file using `fwrite()` function.
- Also, it updates the `SM_FileHandle` metadata by updating the current page position.
- It returns `RC_WRITE_FAILED` error code at any point of failure.

### `writeCurrentBlock`

- This function takes `int pageNum`, pointer to `SM_FileHandle`, and writes the current block (page) from `memPage` to the file by calling `writeBlock` with `pageNum` set to `curPagePos`.
- It follows the steps similar to `writeBlock` method by calling `fseek()` and `fwrite()` methods to write one page of data from `memPage` to the file.
- Also, it updates the current page position by incrementing the `curPagePos`.

### `appendEmptyBlock`

- This function appends an empty block (page) to the end of the file.
- It checks if the file is open for writing; if not, it returns `RC_FILE_NOT_FOUND` error code.
- It calculates the offset to the end of the file by passing `offset, SEEK_SET` to `fseek()` method.
- It then creates an empty page filled with '\0' bytes, writes the empty page to the file using `fwrite()` function.
- It then updates the total number of pages by incrementing `totalNumPages` and the current page position in the file handle.
- It finally releases memory allocated for the empty page.

### `ensureCapacity`

- This function ensures that the file has at least `numberOfPages` by appending empty blocks if necessary.
- It checks if the file is open for writing and determines the current number of pages in the file from `totalNumPages`.
- It then compares the parameter `numberOfPages` with `totalNumPages` to check if the file already has enough pages.
- If it has enough pages, then it's not required to ensure capacity.
- If more pages are needed, it calculates the number of additional pages required by finding required pages from `numberOfPages – currentNumPages`.
- It then appends empty pages to the file as per the number of additional pages.
- It first seeks the end of the file by passing `SEEK_END` to `fseek()` method, then starts writing to the file by iterating `additionalPages` times.
- At each iteration, it appends a block or empty page to the file and updates the total number of pages in the file handle as the number of pages are appended.

## Implementation

All the test cases required for the project are analyzed, and tasks are divided into various categories. The present cursor position is taken as the book-keeping information (`mgmntInfo`). Based on the cursor position, the block numbers for the current, next, and previous blocks are calculated.

The implementation covers the following concepts:

1. Create, Open, Close, Destroy a file.
2. Read blocks based on the page number - previous, current, next, and last.
3. Write blocks based on the page number.
4. Ensure (Increase) capacity of the file by appending the required blocks.

--- END ---
