
#include "storage_mgr.h"
#include "dberror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// It is a Dummy function that serves as entry point to this program, has no use.

void initStorageManager(void) {
    // we left this function undefined  as of no use.
}


// Function to deinitialize the file handle and free any allocated memory
void closeFileHandle(SM_FileHandle *fHandle) {
    // Check if the file handle is initialized
    if (fHandle != NULL) {
        // Check if the management information (file pointer) is not NULL
        if (fHandle->mgmtInfo != NULL) {
            FILE *file = (FILE *)fHandle->mgmtInfo;
            fclose(file); // Close the file
            fHandle->mgmtInfo = NULL; // Set the management info to NULL to indicate it's closed
        }
        // Free any allocated memory (if applicable)
        // Example: free(fHandle->additionalData);
    }
}




// Create a new page file with the given fileName

RC createPageFile(char *fileName) {
    FILE *file = fopen(fileName, "w+"); // Open the file in binary write mode
    printf("open...\n");


    if (file == NULL)
        return RC_WRITE_FAILED; // Return an error code if the file couldn't be opened

    // Create a single page filled with '\0' bytes
    SM_PageHandle emptyPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));

    if (emptyPage == NULL) {
        fclose(file); // Close the file if memory allocation failed
        return RC_WRITE_FAILED;
    }
    printf("open...\n");

    // Write the empty page to the file
    if (fwrite(emptyPage, sizeof(char), PAGE_SIZE, file) < PAGE_SIZE) {
        free(emptyPage); // Free allocated memory
        fclose(file);    // Close the file if write failed
        return RC_WRITE_FAILED;
    }
    printf("open.2..\n");

    // Update metadata in the file handle
    fclose(file); // Close the file
    free(emptyPage); // Free allocated memory for emptyPage
    printf("open.3..\n");
    return RC_OK;
}

// Open an existing page file specified by fileName


RC openPageFile(char *fileName, SM_FileHandle *fHandle) {
    FILE *file = fopen(fileName, "rb+"); // Open the file in binary read/write mode

    if (file == NULL) {
        return RC_FILE_NOT_FOUND; // Return FILE NOT FOUND if the file doesn't exist
    }

    // Calculate the total number of pages
    fseek(file, 0, SEEK_END); // Move the file position to the end
    fHandle->totalNumPages = ftell(file) / PAGE_SIZE;

    // Reset the file position to the beginning
    fseek(file, 0, SEEK_SET);

    // Initialize other fields of fHandle
    fHandle->fileName = fileName;
    fHandle->curPagePos = 0;
    fHandle->mgmtInfo = file;

    return RC_OK;
}


RC closePageFile(SM_FileHandle *fHandle) {
    // Check if the file is already closed
    if (fHandle == NULL || fHandle->mgmtInfo == NULL)
        return RC_FILE_HANDLE_NOT_INIT;

    // Call closeFileHandle to deinitialize the file handle
    closeFileHandle(fHandle);

    return RC_OK;
}



extern RC destroyPageFile(char *fileName) {
    FILE *pageFile;
    
    // Opening file stream in read mode. 'r' mode creates an empty file for reading only.
    pageFile = fopen(fileName, "r");
    
    if (pageFile == NULL)
        return RC_FILE_NOT_FOUND;
    
    // Closing the file stream
    fclose(pageFile);
    
    // Deleting the given filename so that it is no longer accessible.
    if (remove(fileName) != 0)
        return RC_FILE_NOT_FOUND; // Failed to delete the file
    
    return RC_OK;
}




RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    // Check for valid file handle and page number
    if (fHandle == NULL || fHandle->mgmtInfo == NULL || pageNum < 0 || pageNum >= fHandle->totalNumPages)
        return RC_READ_NON_EXISTING_PAGE; // Invalid handle or page number

    // Retrieve the FILE pointer from the management information
    FILE *file = (FILE *)fHandle->mgmtInfo;

    // Calculate the file position to read from
    long position = (long)pageNum * PAGE_SIZE;

    // Seek to the desired file position
    if (fseek(file, position, SEEK_SET) != 0)
        return RC_READ_NON_EXISTING_PAGE; // Failed to move to the desired position

    // Read a page of data into memPage
    size_t bytesRead = fread(memPage, sizeof(char), PAGE_SIZE, file);

    if (bytesRead != PAGE_SIZE)
        return RC_READ_NON_EXISTING_PAGE; // Partial or failed read

    // Update the current page position in the file handle
    fHandle->curPagePos = pageNum;

    return RC_OK; // Successful read operation
}



int getBlockPos(SM_FileHandle *fHandle) {
    // Ensure the provided file handle is valid
    if (fHandle == NULL || fHandle->mgmtInfo == NULL)
        return RC_FILE_HANDLE_NOT_INIT; // Indicate an invalid or uninitialized file handle

    // Return the current block/page position
    return fHandle->curPagePos;

}


// Function to the read first page in a file

RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {

    // Make use of previously written readBlock() and pass the pageNum as 0 instead of re-writing function from scratch.
    // to improve code readability and usability

    return readBlock(0, fHandle, memPage);
}

// Function to the read the previous page in a file and store it's contents in memory page

RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {

    // Make use of previously written readBlock() and pass the pageNum as current page position - 1
    // instead of re-writing function from scratch
    // to improve code readability and usability

    return readBlock(fHandle->curPagePos-1, fHandle, memPage);
}


// Read the current page in the specified file and store its content in memory page

RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {

    // Make use of previously written readBlock() and pass the pageNum as current page position
    // instead of re-writing function from scratch
    // to improve code readability and usability

    return readBlock(fHandle->curPagePos, fHandle, memPage);
}

// Read the next page in the specified file and store its content in memory page

RC readNextBlock(SM_FileHandle* fHandle, SM_PageHandle memPage){

    // Make use of previously written readBlock() and pass the pageNum as current page position + 1
    // instead of re-writing function from scratch
    // to improve code readability and usability

    return readBlock(fHandle->curPagePos+1, fHandle, memPage);
}

// Read the last page in the specified file and store its content in memory page

RC readLastBlock(SM_FileHandle* fHandle, SM_PageHandle memPage){

    // Make use of previously written readBlock() and pass the pageNum as total pages - 1
    // instead of re-writing function from scratch
    // to improve code readability and usability
    
    return readBlock(fHandle->totalNumPages-1, fHandle, memPage);
}

RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    // Check if the file is open for writing
    if (fHandle->mgmtInfo == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }

    // Check if the given pageNum is valid
    if (pageNum < 0 || pageNum >= fHandle->totalNumPages) {
        return RC_READ_NON_EXISTING_PAGE;
    }

    // Calculate the offset to the desired page
    long offset = (long)pageNum * PAGE_SIZE;

    // Move the file pointer to the calculated offset
    if (fseek(fHandle->mgmtInfo, offset, SEEK_SET) != 0) {
        return RC_WRITE_FAILED;
    }

    // Write the content of memPage to the file
    if (fwrite(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE) {
        return RC_WRITE_FAILED;
    }

    // Update the current page position
    fHandle->curPagePos = pageNum;

    return RC_OK;
}

RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    // Check if the file is open for writing
    if (fHandle->mgmtInfo == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }

    // Calculate the offset to the current page
    long offset = (long)fHandle->curPagePos * PAGE_SIZE;

    // Move the file pointer to the calculated offset
    if (fseek(fHandle->mgmtInfo, offset, SEEK_SET) != 0) {
        return RC_WRITE_FAILED;
    }

    // Write one page (block) of data from memPage to the file
    if (fwrite(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE) {
        return RC_WRITE_FAILED;
    }

    // Update the current page position
    fHandle->curPagePos++;

    return RC_OK;
}

RC appendEmptyBlock(SM_FileHandle *fHandle) {
    // Check if the file is open for writing
    if (fHandle->mgmtInfo == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }

    // Calculate the offset to the end of the file
    long offset = (long)(fHandle->totalNumPages) * PAGE_SIZE;

    // Move the file pointer to the calculated offset
    if (fseek(fHandle->mgmtInfo, offset, SEEK_SET) != 0) {
        return RC_WRITE_FAILED;
    }

    // Create an empty page filled with '\0' bytes
    SM_PageHandle emptyPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));
    if (emptyPage == NULL) {
        return RC_WRITE_FAILED;
    }

    // Write the empty page to the file
    if (fwrite(emptyPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE) {
        free(emptyPage);
        return RC_WRITE_FAILED;
    }

    // Update the total number of pages and current page position
    fHandle->totalNumPages++;
    fHandle->curPagePos = fHandle->totalNumPages - 1;

    // Free allocated memory
    free(emptyPage);

    return RC_OK;
}

RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle) {
    // Check if the file is open for writing
    if (fHandle->mgmtInfo == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }

    // Determine the current number of pages in the file
    int currentNumPages = fHandle->totalNumPages;

    // If the file already has enough pages, no need to ensure capacity
    if (numberOfPages <= currentNumPages) {
        return RC_OK;
    }

    // Calculate the number of additional pages needed
    int additionalPages = numberOfPages - currentNumPages;

    // Create an empty page to be used for padding
    SM_PageHandle emptyPage = (SM_PageHandle) calloc(PAGE_SIZE, sizeof(char));
    if (emptyPage == NULL) {
        return RC_WRITE_FAILED; // Failed to allocate memory for an empty page
    }

    // Move to the end of the file
    if (fseek(fHandle->mgmtInfo, 0, SEEK_END) != 0) {
        free(emptyPage);
        return RC_WRITE_FAILED;
    }

    // Append empty pages to the file to meet the desired capacity
    int i; // Declare the loop variable here
    for (i = 0; i < additionalPages; i++) {
        if (fwrite(emptyPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE) {
            free(emptyPage);
            return RC_WRITE_FAILED;
        }
        currentNumPages++; // Increment the total number of pages
    }

    // Update the total number of pages in the file handle
    fHandle->totalNumPages = currentNumPages;

    free(emptyPage);
    return RC_OK;
}


