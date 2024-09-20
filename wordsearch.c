#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void findWordIn2dCharArr(LinkedList resultslist, char** arr,  Coords coordinates, char* word);
LinkedList copyLinkedList(LinkedList* original, int dataSize);
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
                // printf("(%c/%c) : (%d,%d) \n", (*(*(arr + i) + j)), toUpper(charToFind), i, j);
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

//run every possibility DFS searching for  ways to find the word from the one starting point
void findWordIn2dCharArr(LinkedList resultslist, char** arr,  Coords coordinates, char* word)
{
    //handle end of word
    if(*word == '\0')
    {
        //this means the right path is found or nothing was entered
        printsLinkedList(&((LinkedList)resultslist->data));
        appendLinkedList(&resultslist, createLinkedList());
        return;
    }
    //find all hits
    LinkedList Hits = findCharInSurroundings(arr, bSize, coordinates, *word);
    // appendLinkedList(&Hits, (findCharInSurroundings(arr, bSize, coordinates, toUpper(*(word)))));
    if(Hits != NULL)
    {
        printsLinkedList(&Hits); 
        // recur with all hits
        Coords* hitCoord = (Coords*) Hits->data;
        appendLinkedList(&(resultslist->data), hitCoord);
        // printf("(%d,%d)\n", ((Coords*)(temp))->x, ((Coords*)(temp))->y);
        // int indexInLinkedList = 1;
        // printf("%c", *((word) + 1))    ;
        while(Hits != NULL) //&& Hits != NULL
        {
            // if(*((word + 1)) != '\0')
            // {
            //     return;
            // }
            findWordIn2dCharArr(resultslist, arr,  *hitCoord, (word + 1));
            Hits = Hits->next;
            hitCoord = (Coords*)(Hits->data);
            // indexInLinkedList++;
            // printf("Hits Explores += %d", indexInLinkedList);
        }
    }
    else
    {
        printf("found bad and freeing at: (%d,%d) char '%c'", coordinates.x, coordinates.y, *word);
        freeLinkedList((LinkedList)resultslist->data);
    }
    //if one of the hits is the end of the word return that list in resultslist
    // resultslist = Hits;
    // if(word == '\0')
    // {
    //     appendLinkedList(resultslist, temp);
    // printsLinkedList(resultslist); 
    // }
}

void searchPuzzle(char** arr, char* word) 
{
    // This function checks if arr contains the search word. If the
    // word appears in arr, it will print out a message and the path
    // as shown in the sample runs. If not found, it will print a
    // different message as shown in the sample runs.
    // Your implementation here...
    //add do while loop for whole function that says while(found != null and print Word not found! based on how many solutions there are)
    LinkedList found = findLetter2dArr(arr, bSize, toUpper(*word)); 
    // printf("(%d,%d)\n", ((Coords*)(found->data))->x, ((Coords*)(found->data))->y);
    if(found == NULL)
    {
        printf("Word not found!");
    }
    else
    {
        LinkedList* x;
        *x = (createLinkedList());
        findWordIn2dCharArr (x, arr, *((Coords*)(found->data)), word);
        // LinkedList x = findCharInSurroundings(arr, bSize, *((Coords*)(found->data)), toUpper(*(word + 1)));
        printsLinkedList(&found);
        printf("--Break--\n");
        printsLinkedList(x);
    }
    /*else if()
    {
        printf("Word found!");
        printf("Printing the search path:");

    }*/
    // char **block = (char**)malloc(bSize * sizeof(char*));   
}
