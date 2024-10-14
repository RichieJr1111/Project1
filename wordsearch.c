#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
typedef struct struct_Coord Coords;
typedef struct Node struct_Node;
typedef struct Node* LinkedList;
struct struct_Coord {
    int x;
    int y;
};
struct Node {
    void* data;
    LinkedList next;
};

void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
char toLower(char c);
char findWordIn2dCharArr(LinkedList* resultslist, int size, char** arr,  Coords coordinates, char* word);
void makeSolutionFromCoords(LinkedList resultslist, int sizeOfWords, int foundASolution);
LinkedList copyLinkedList(LinkedList* original, int dataSize);
int bSize;
int foundSolution;
// LinkedList solution2dArray;
int** solutionChart;
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
 * initializes a linkedList
 * @return a linkedlist pointer
 */
LinkedList createLinkedList()
{
    return NULL;
}

/**
 * adds data to the linked list
 * @param inputList a pointer to the linkedList pointer (we will add to this list)
 * @param data the data we are adding
 */
void appendLinkedList(LinkedList* inputList, void* data)
{
    LinkedList lastHead = *inputList;
    LinkedList newHead = (LinkedList)malloc(sizeof(struct_Node));
    newHead->data = data;
    newHead->next = lastHead;
    (*inputList) = newHead;
}

/**
 * pops the first element out of the linked list
 * @param inputList the linked list to pop
 */
void popLinkedList(LinkedList* inputList)
{
    LinkedList frontNode = *inputList;
    *inputList = (*inputList)->next;
    free(frontNode->data);
    free(frontNode);
}

/**
 * prints a whole linkedlist for debugging puposes. only works for linkedLists of COORDS which is what we are using as we need a library to do more type based stuff easily
 * @param inputList a pointer to the linkedList to print
 * @returns linked lists of COORDS only
 */
void printsLinkedList(LinkedList* inputList)
{
    LinkedList stepList = *inputList;
    //Loop until we reach a NULL pointer for next meaning we hit the end freeing each one
    while(stepList != NULL)
    {
        printf("(%d,%d)\n",((Coords*)(stepList->data))->x,((Coords*)(stepList->data))->y);
        stepList = stepList->next;
    }
}

/**
 * Flips a linked list
 * @param list the list to flip
 */
void flipLinkedList(LinkedList* list)
{
    LinkedList stepList = *list;
    LinkedList curList;

    *list = createLinkedList();
    //Loop until we reach a NULL pointer for next meaning we hit the end freeing each one
    while(stepList != NULL)
    {
        curList = stepList;
        stepList = stepList->next;
        appendLinkedList(list, curList->data);
        //make sure to free data of the old list since we are moving it to flip it
        free(curList);
    }
}

/**
 * the method called by copyLinkedList. Slightly faster but gives an inverted copy. Useful if you don't care about order
 */
LinkedList copyInvertedLinkedList(LinkedList* original, int size)
{
    LinkedList newList = createLinkedList();
    LinkedList stepList = *original;
    //Loop until we reach a NULL pointer for next meaning we hit the end freeing each one
    while(stepList != NULL)
    {
        appendLinkedList(&newList, memcpy(malloc(size), stepList->data, size));
        stepList = stepList->next;
    }
    return newList;
}

/**
 * copies a linkedList by value
 * @param original the linked list to be copied
 * @param size the size of the datatype the linked list stores ex:(sizeof(int))
 * @returns the new linkedlist that has pointers to new locations in memory containing the same data
 */
LinkedList copyLinkedList(LinkedList* original, int size)
{
    LinkedList result = copyInvertedLinkedList(original, size);
    flipLinkedList(&result);
    return result;
}

/**
 * frees an entire linkedList
 * @param inputList a pointer to the linkedList pointer we are removing
 */
void freeLinkedList(LinkedList* inputList)
{
    LinkedList curList;
    LinkedList nextLink = *inputList;
    //Loop until we reach a NULL pointer for next meaning we hit the end freeing each one
    while(nextLink != NULL)
    {
        curList = nextLink;
        nextLink = nextLink->next;
        free(curList->data);
        free(curList);
    }
    //set our linkedList that has been empty to the NULL pointer
    (*inputList) = nextLink;
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

/**
 * prints a 2d int array to the console in a readable format
 */
void printSquareIntArray(int** arr)
{
    //loops through and prints the char*** array
    for(int i = 0; i < bSize; i++)
    {
        for(int j = 0; j < bSize; j++)
        {
            //pointer array magic
            printf("%d\t", *(*(arr + i) + j));
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
            //should probably use the array creation function for this to make it cleaner since that's already a method but eh
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

int strSize(char* str)
{
    int size = 0;
    char curChar = *str;
    while(*(str + size) != '\0')
    {
        size++;
    }

    return size+1;
}

char* strToLower(char* str)
{
    char* outputStr = (char*)intializeArray(strSize(str),sizeof(char));

    int size = 0;
    while(*(str + size) != '\0')
    {
        size++;
        *(outputStr + size) = toLower(*(str + size));
    }
    *(outputStr + size + 1) = '\0';
    return outputStr;
}

/**
 * a function for creating coords more easily, handles allocation of memory and setting all in one
 */
Coords* createCoords(int x, int y)
{
    Coords* hit = (Coords*)malloc(sizeof(Coords));
    hit->x = x;
    hit->y = y;
    return hit;
}

/**
 * finds all chars in a squared 2d array that match the given char
 * @param arr the squared 2d array to search
 * @param size the size of the input 2d array
 * @param the charecter to find
 * @returns a linkedlist with all found coordinates matching the starting char
 */
LinkedList findLetter2dArr(char** arr, int size, char c)
{
    LinkedList hits = createLinkedList();
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(*(*(arr + i) + j) == c)
            {
                Coords* hit = createCoords(i,j);
                appendLinkedList(&hits, hit);
            }
        }
    }
    return hits;
}

/**
 * finds all instances of the specified char surrounding the input coords in a squared 2d array
 * @param arr the squared 2d array to search 
 * @param size the size of the input 2d array
 * @param cordinates the coords to get of place to start the ring in the 2d array
 * @param charToFind the charecter to find in the 2d array
 * @returns a linked list contating the coordinates all found instances of a char in the surrounding grid
 */
LinkedList findCharInSurroundings(char** arr, int size, Coords cordinates, char charToFind)
{
    //create our return linked list
    LinkedList hits = createLinkedList();
    //we need a ring around the charecters so this makes the input one back (if we did not hit an edge)
    //probably a smarter way to do this, but ehh...
    int x = cordinates.x;
    if(x > 0)
    {
        x--;
    }
    int y = cordinates.y;
    if(y > 0)
    {
        y--;
    }
    //loop the 9 digits with the middle one as the center, unless we go over the far edge (x+2 is upper bound, x lower bound at this point)
    for(int i = x; i <= cordinates.x+1 && i < size; i++)
    {
        for(int j = y; j <= cordinates.y+1 && j < size; j++)
        {
            // printf("(%d,%d) ", i,j);
            //edge case for the middle coordinate not finding itself
            if(i != cordinates.x || j != cordinates.y)
            {  
                //printf("(%c/%c) : (%d,%d) \n", (*(*(arr + i) + j)), toUpper(charToFind), i, j);
                //if the charecter we found is the one we want, then we return the coordinates as an array
                if(*(*(arr + i) + j) == charToFind)
                {
                    // printf("(%c) : (%d,%d) \n", *(*(arr + i) + j), i, j);
                    Coords* hit = createCoords(i,j);
                    appendLinkedList(&hits, hit);
                    // printsLinkedList(&hits); 
                }
            }
        }
    }
    return hits;
}


char findWordIn2dCharArr(LinkedList* resultslist, int size, char** arr,  Coords coordinates, char* word)
{
    printf("word: %s, char: %c, coord: (x:%d, y:%d)\n", word, *word, coordinates.x, coordinates.y);
    
    //start by adding out cordinates to our result list (since we need to return the path)
    appendLinkedList(resultslist, createCoords(coordinates.x, coordinates.y));
    //if we have reached the end of the string then we don't need to keep going return
    if(*word == '\0' || strlen(word) < 1)
    {
        return 1;
    }
    //find all hits from the coord if we are not at the end
    LinkedList hits = findCharInSurroundings(arr, size, coordinates, *word);

    //loop through the whole linkedList 
    while(hits != NULL)
    {
        //get the coords of our current iteration
        Coords* point = hits->data;
        //run this function again and store the result in found
        char found = findWordIn2dCharArr(resultslist, size, arr, *point, word+1);
        //if we reached the end tell the higher up recursive version that we reached the end
        if(found == 1)
        {
            return 1;
        }
        //next loop since we didn't find it
       popLinkedList(&hits);
    }
    //we reached the end of this iteration so we should pop before returning up the line, so as to not screw with the data
    popLinkedList(resultslist);
    return 0;
}

int concatTwoInts(int right, int left)
{
    double tempRight = 0;
    tempRight = right;
    double tempLeft = 0;
    tempLeft = left;
    int count = 1;
    // printf("%f\n", tempRight);
    while(fabs(tempRight) > 0.9) 
    {
        count *= 10;
        tempRight /= 10;
        // printf("%f\n", tempRight);
    }
    // printf("%f\n", tempRight);
    // printf("number: %f\n", (tempLeft + tempRight));
    double temp = round(((tempLeft + tempRight)) * count);
    // printf("%f\n", temp);
    return (int)(temp);
}

void makeSolutionFromCoords(LinkedList resultslist, int sizeOfWords, int foundASolution)
{
    if(foundSolution == 0)
    {
        foundSolution = 1;
        int countOfPath = sizeOfWords;
        while(resultslist->data != NULL)
        {
            for(int i = 0; i < bSize; i++)
            {
                for(int j = 0; j < bSize; j++)
                {
                    printsLinkedList(&resultslist);
                    if(i == ((Coords*)(resultslist->data))->x && j == ((Coords*)(resultslist->data))->y)
                    {
                        if((*(*(solutionChart + i)+j)) == 0)
                        {
                            *(*(solutionChart + i)+j) = countOfPath;
                        }
                        else
                        {
                            *(*(solutionChart + i)+j) = concatTwoInts(*(*(solutionChart + i)+j), countOfPath);
                        }
                        // i--;
                        // j -= 2;
                        countOfPath--;
                        if(countOfPath == 0)
                        {
                            return;
                        }
                        resultslist = resultslist->next;
                    }
                }   
            }
        }
    }
}

void searchPuzzle(char** arr, char* word) 
{
    // This function checks if arr contains the search word. If the
    // word appears in arr, it will print out a message and the path
    // as shown in the sample runs. If not found, it will print a
    // different message as shown in the sample runs.
    // Your implementation here...
    solutionChart = (int**)malloc(bSize * sizeof(int*));
    for(int i = 0; i < bSize; i++)
    {
        *(solutionChart + i) = (int*)malloc(bSize * sizeof(int));
        for(int j = 0; j < bSize; j++)
        {
            *(*(solutionChart + i)+j) = 0;
        }   
    }

    //add do while loop for whole function that says while(found != null and print Word not found! based on how many solutions there are)
    LinkedList found = findLetter2dArr(arr, bSize, toUpper(*word)); 
    if(found != NULL)
    {
        LinkedList result = createLinkedList();
        if(findWordIn2dCharArr(&result, bSize, arr, *((Coords*)(found->data)), word+1))
        {
            printf("cool solution\n");
        }
        printf("printing path:\n");
        printsLinkedList(&result);
    }

    //add check to see if solution coords stores information

    /*int isSolution = 0;
    for(int i = 0; i < bSize; i++)
    {
        for(int j = 0; j < bSize; j++)
        {
            if(*(*(solutionChart + i)+j) != 0)
            {
                isSolution = 1;
            }
        }   
    }

    if(isSolution)
    {
        printf("Word found!\n");
        printf("Printing the search path:\n");
        printSquareIntArray(solutionChart);
    }
    else
    {
        printf("Word not found!");
    }*/
}
