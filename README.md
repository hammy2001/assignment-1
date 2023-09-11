# assignment-1

CS525 Advanced Database Organization Assignment 1: Storage Manager

Team Members:- Omkar Shewale, Hamdan Patel, Merlin Simoes, Mikkilineni Lekhana Sai, Rajashekar Mudigonda.

Project Modules
C source files: storage_mgr.c, dberror.c, test_assign1_1.c, test_assign1_2.c 
Header files: storage_mgr.h, dberror.h, test_helper.h

#Aim
The goal of this assignment is to implement a simple storage manager - a module that is capable of reading blocks from a file on disk into memory and writing blocks from memory to a file on disk. The storage manager deals with pages (blocks) of fixed size (PAGE_SIZE). In addition to reading and writing pages from a file, it provides methods for creating, opening, and closing files. The storage manager has to maintain several types of information for an open file: The number of total pages in the file, the current page position (for reading and writing), the file name, and a POSIX file descriptor or FILE pointer.

Contributions
Merlin Simoes: initStorageManager(), CreatePageFile(), Readme
Hamdan Patel: writeBlock(), writeCurrentBlock(), appendEmptyBlock(), ensureCapacity()
Omkar Shewale: readBlocK(), getBlockPos(), readFirstBlock(), readPreviousBlock(), readNextBlock(), readLastBlock()
Mikkilineni Lekhana Sai: OpenPageFile(), ClosePageFile(), Readme
Rajashekar Mudigonda: DestroyPageFile(), readCurrentBlock(), Readme

     #CONTENTS
1) Instructions to run the code
2) Description of functions used
3) Implementation

1.	Instructions to run the code

a)	For executing mandatory test cases:

1) In the terminal, navigate to the assignment directory.

2) Type: what’s this?
	make -f Makefile

3)  ./storage_mgr

###    2. Description of functions used

•	initStorageManager

i.	This function is a placeholder and currently has no functionality. 
ii.	It serves as an entry point to the program but does nothing in this implementation.

•	CreatePageFile
i.	This function takes fileName as parameter and creates a new page file with the given fileName. i.e createPageFile(char *fileName);
ii.	Then fopen() function is used to open file in binary write mode ("wb") and the pointer to file is stored in file variable.
iii.	An empty page of size 4kb of memory is allocated for SM_PageHandle memory page and it is filled with null bytes. If memory allocation is failed, immediately it returns RC_WRITE_FAILED error code.
iv.	If the memory allocation is success, then fwrite() function is used to write the empty page to the file. 
v.	File is closed, memory is freed for memory allocated for empty page and error code is returned, if the write is failed.
vi.	It finally returns RC_OK if Page File is successfully created.

•	OpenPageFile
i.	This function opens an existing page file specified by fileName in binary read/write mode ("rb+").
ii.	It calculates the total number of pages, resets the file position to the beginning, and initializes fields of the file handle (SM_FileHandle)
iii.	It returns an error code if the file doesn't exist.

•	ClosePageFile

i.	This function closes an open page file. 
ii.	It returns an error code if the file is already closed and returns an error file handle not initialized.
iii.	It calls the closeFileHandle function to perform the actual closing. 

•	DestroyPageFile

i.	It first checks whether the file exists or not.
ii.	This function attempts to delete the specified file using the remove function. 
iii.	It returns an RC_FILE_NOT_FOUND error code if the file is not found or if deletion fails.

•	readBlocK

i.	This function accepts int pageNum, SM_FileHandle fhandle, SM_PageHandle memPage as parameters and reads a block (page) from the specified page number (pageNum) into a memory page (memPage). 
ii.	It performs error checks to ensure a valid file handle, page number, and existence of the page. It also retrieves the file pointer from the management information mgmtInfo.
iii.	It seeks to the desired file position using fseek() function, reads the page data into memPage using fread() and stores the data in bytesRead variable.
iv.	It then updates the current page position in the file handle to pageNum.
v.	It returns RC_READ_NON_EXISTING_PAGE error code for any failures.

•	getBlockPos

i.	This function returns the current block (page) position in the file handle. 
ii.	It first checks for a valid file handle and management information, then it gets current Page Position from SM_FileHandle metadata.


•	readFirstBlock

i.	This function reads the first block (page) in the file by calling readBlock with pageNum set to 0 and stores its content in memPage.
ii.	Instead of rewriting the code again we can directly call readBlock() method and pass the pageNum as 0.


•	readPreviousBlock

i.	This function reads the block preceding the current block in the file by calling readBlock method with pageNum set to curPagePos – 1 and stores its content in memPage.


•	readCurrentBlock

i.	This function takes in fHandle and memPage as parameters to read data from disk to memory page.
ii.	This function reads the current block (page) by calling readBlock with pageNum set to curPagePos and stores its content in memPage.


•	readNextBlock

i.	This function reads the block following the current block in the file by calling readBlock with pageNum set to curPagePos + 1 and stores its content in memPage.

•	readLastBlock

i.	This function reads the last block (page) in the file by calling readBlock with pageNum set to totalNumPages – 1 and stores its content in memPage.

•	writeBlock

i.	This function takes int pageNum, pointer(fHandle) to SM_FileHandle and pointer(memPage) to SM_PageHandle and writes a block (page) specified by pageNum from memPage to the file.
ii.	It checks if the file is open for writing and if the file exists.
iii.	It then checks whether given pageNum is valid or not by checking pageNum < totalNumPages.
iv.	It calculates the offset to find the position in the file and stores in the offset long variable.
v.	It then moves the file pointer mgmtInfo to the calculated offset using fseek( ) function.
vi.	If seek successful, then it writes the content of memPage to the file using fwrite( ) function.
vii.	Also, it updates the SM_FileHandle metadata by updating current page position.
viii.	It returns  RC_WRITE_FAILED error code at any point of failure.

•	writeCurrentBlock

ix.	This function takes int pageNum, pointer(fHandle) to SM_FileHandle and writes the current block (page) from memPage to the file by calling writeBlock with pageNum set to curPagePos.
x.	It follows the steps similar to writeblock method by calling fseek( ) and fwrite() methods to write one page of data from memPage to the file.
xi.	Also, it updates the current page position by incrementing the curPagePos.

•	appendEmptyBlock

i.	This function appends an empty block (page) to the end of the file. 
ii.	It checks if the file is open for writing, if not it returns RC_FILE_NOT_FOUND error code.
iii.	It calculates the offset to the end of the file by passing offet, SEEK_SET to fseek( ) method.
iv.	It then creates an empty page filled with ‘\0’ bytes, writes the empty page to the file using fwrite( ) function.
v.	It then updates the total number of pages by incrementing totalNumPages, and the current page position in the file handle 
vi.	It finally releases memory allocated for the empty page in the step iv.






•	ensureCapacity

i.	This function ensures that the file has at least numberOfPages by appending empty blocks if necessary. 
ii.	It checks if the file is open for writing and determines the current number of pages in the file from totalNumPages.
iii.	It then compares the parameter numberOfPages with totalNumPages to check if the file already has enough pages.
iv.	If it has enough pages, then not required to ensure capacity.
v.	If more pages are needed, it calculates the number of additional pages required by finding required pages from numberOfPages – currentNumPages.
vi.	It then appends empty pages to the file as per the number of additional pages
vii.	It first seek the end of the file by passing SEEK_END to fseek( ) method, then starts writing to the file by iterating additionalPages times.
viii.	At each iteration it appends a block or empty page to the file and updates the total number of pages in the file handle as the number of the pages are appended.

3) Implementation
All the test cases required for the project are analyzed and task got divided into various categories.
The present cursor position is taken as the book-keeping information (mgmntInfo). Based on the cursor position, the block numbers for the current, next and previous blocks are calculated.
The implementation covers the following concepts:-
i)   Create, Open, Close, Destroy a file.
ii)  Read blocks based on the page number - previous, current, next and last.
iii) Write blocks based on the page number.
iv) Ensure (Increase) capacity of the file by appending the required blocks.

------------------------------------------------------END--------------------------------------------------------------------------
