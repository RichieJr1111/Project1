#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;
// Main function, DO NOT MODIFY
int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;
    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) 
    {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }
    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));
    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) 
    {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) 
        {
            fscanf(fptr, "%c ", *(block + i) + j);
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);
    printf("Enter the word to search: ");
    scanf("%s", word);
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    return 0;
}


/**
 * initializes an array without the use of array syntax
 * @param arraySize how long you want your array to be
 * @param dataMemorySize sizeof(value) or the size in memory of your dataType
 */
void* intializeArray(int arraySize, int dataMemorySize)
{
    //add the empty memoryspace at the end
    return malloc((arraySize+1) * dataMemorySize);
}

/**
 * just a simple helper for making string 2d arrays. filled with garbage data on initialize the char* pointers must be allocated and set still
 * @param rowSize number of rows in the 2d array
 * @param colombSize number of colombs in the array
 */
char*** initialize2dStrArray(int rowSize, int colombSize)
{
    char*** finalArray = (void*)intializeArray(rowSize, sizeof(char***));
    for(int i = 0; i < rowSize; i++)
    {
        *(finalArray + i) = (void*)intializeArray(colombSize, sizeof(char***));
    }
    return finalArray;
}

/**
 * prints a 2d string array to the console in a readable format
 */
void printSquareStrArray(char*** arr, int arraySize)
{
    //loops through and prints the char*** array
    for(int i = 0; i < arraySize; i++)
    {
        for(int j = 0; j < arraySize; j++)
        {
            //pointer array magic
            printf("%s ", *(*(arr + i) + j));
        }
        printf("\n");
    }
    printf("\n");    
}

void printPuzzle(char** arr) 
{
    // This function will print out the complete puzzle grid (arr).
    // It must produce the output in the SAME format as the samples
    // in the instructions.
    // Your implementation here...


    //initalize and array to the size of bSize
    char*** strArray = initialize2dStrArray(bSize, bSize);

    //set each char pointer to the char pointer of the puzzle
    for(int i = 0; i < bSize; i++)
    {
        for(int j = 0; j < bSize; j++)
        {
            //set a newly initialized memory to 2 chars for our char string
            char* var = (char*)malloc(sizeof(char*)*2);
            //set the first char of our string to the char to be displayed
            (*var) = *(*(arr + i) + j);
            //add the string end charecter
            *(var + 1) = '\0';
            //add the string pointer to the 3d array we made
            *(*(strArray + i) + j) = var;
        }
    }

    //tada we now have something we can print with our method without code copyPaste!
    printSquareStrArray(strArray, bSize);

    //free the memory we just allocated

    //should probably have a helper method for freeing strings in an array
    for(int i = 0; i < bSize; i++)
    {
        for(int j = 0; j < bSize; j++)
        {
            //free the strings in the array
            free(*(*(strArray + i) + j));
        }
        //free the pointers to the strings
        free(*(strArray + i));
    }
    //free the outer array of pointers
    free(strArray);
}

void searchPuzzle(char** arr, char* word) 
{
    // This function checks if arr contains the search word. If the
    // word appears in arr, it will print out a message and the path
    // as shown in the sample runs. If not found, it will print a
    // different message as shown in the sample runs.
    // Your implementation here...
    printf("Word found!");
    // char **block = (char**)malloc(bSize * sizeof(char*));   
    printf("Printing the search path:");
}
