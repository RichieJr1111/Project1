#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
char toLower(char c);
void findHeadChar(char** arr, char c, int* x, int* y);
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
 * takes a string and a string to add to it and return a new string with the elements of the old and new
 * @param original the string to add to
 * @param append the string being appended / added
 */
/*
char* strAppend(char* original, char* append)
{
    int originalSize = 0;
    char finishedRead = 0;
    //loop until we made it all the way through the string
    while(finishedRead == 0)
    {
        //if this charecter is the end of the string end the loop and stop increasing size
        if(*(original + originalSize) == '\0')
        {
            finishedRead = 1;
        }
        else
        {
            originalSize++;
        }
    }
    int appendSize = 0;
    finishedRead = 0;
    //loop until we made it all the way through the string
    while(finishedRead == 0)
    {
        //if this charecter is the end of the string end the loop and stop increasing size
        if(*(append + appendSize) == '\0')
        {
            finishedRead = 1;
        }
        else
        {
            appendSize++;
        }
    }

    sprintf(malloc(appendSize + originalSize), "%s%s", original, append);
}*/

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

char toLower(char c)
{
    //97 - 65
    int temp = (int)c;
    if(temp < 97)
    {
        temp = temp + (97 - 65);
    }
    return ((char)temp);
}

char toUpper(char c)
{
    //97 - 65
    int temp = (int)c;
    if(temp >= 97)
    {
        temp = temp - (97 - 65);
    }
    return ((char)temp);
}

void findHeadChar(char** arr, char c, int* x, int* y)
{
    // add check to skip if solutions contations this as 1 (the head of a word already
    *x = -1;
    *y = -1;
    for(int i = 0; i < bSize; i++)
    {
        for(int j = 0; j < bSize; j++)
        {
            if(*(*(arr + i) + j) == c)
            {
                *x = i;
                *y = j;
                break;
            }
        }
    }
}

//branching checks every possible way to make the word
/*
arr is array to search
c is string we are searching for
indexLetter is letter we are looking for
x is x coord we ar currently at
y is y coord (not sure of row vs collumn for these)
*/
int findConnectedChars(char** arr, char* c, int indexLetter, int x, int y)
{
    // add check to skip if solutions contations this as 1 (the head of a word already
    //if end of word return BLANK
    if(indexLetter >= strlen(c) && *(c + indexLetter) == '\0') 
    {
        return 1;
    }
    //else if 
    else if(*(c + indexLetter) != *(*(arr + x) + y))
    {
        return 0;
    }
    else
    {
        printf("%c\n\n", *(*(arr + x) + y));
        if(x + 1 < bSize)
        {
            findConnectedChars(arr, c, indexLetter + 1, x + 1, y);
            if(y - 1 >= 0)
            {
                findConnectedChars(arr, c, indexLetter + 1, x + 1, y - 1);
            }
            if(y + 1 < bSize)
            {
                findConnectedChars(arr, c, indexLetter + 1, x + 1, y + 1);
            }
        }
        if(x - 1 >= 0)
        {
            findConnectedChars(arr, c, indexLetter + 1, x - 1, y);
            if(y - 1 >= 0)
            {
                findConnectedChars(arr, c, indexLetter + 1, x - 1, y - 1);
            }
            if(y + 1 < bSize)
            {
                findConnectedChars(arr, c, indexLetter + 1, x - 1, y + 1);
            }
        }
        if(y - 1 >= 0)
        {
            findConnectedChars(arr, c, indexLetter + 1, x, y - 1);
        }
        if(y + 1 < bSize)
        {
            findConnectedChars(arr, c, indexLetter + 1, x, y + 1);
        }
        return 0;
    }
}

void searchPuzzle(char** arr, char* word) 
{
    // This function checks if arr contains the search word. If the
    // word appears in arr, it will print out a message and the path
    // as shown in the sample runs. If not found, it will print a
    // different message as shown in the sample runs.
    // Your implementation here...
    int x;
    int y;
    findHeadChar(arr, *(word), &x, &y); 
    // printf("%d %d", x, y);
    if(x == -1)
    {
        printf("ERRIR MESSAGE");
    }
    else if(findConnectedChars(arr, word, 0, x, y))
    {
        printf("Word found!");
        printf("Printing the search path:");

    }
    // char **block = (char**)malloc(bSize * sizeof(char*));   
}
