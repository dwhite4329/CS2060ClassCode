/*
 * Name: Devon White
 * Class: CS2060 M/W 10:50AM - 12:05PM
 * MAC OS
 * Due: Nov 13, 2023
 * Description: This program will allow the user to enter pet names
 * and ages where memory will be allocated and they will be stored
 * alphabetically in a linked list.  After pets are enetered, the
 * user can remove the pet and memory will be deallocated.  Before
 * the program ends all the memory will be deallocated for any
 * remaining pets.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define STRING_LENGTH 80

// Requirement 1 - Create a structure
typedef struct node
{
    char name[STRING_LENGTH];
    int age;
    struct node* nextNodePtr;
}Pets;


int compareNames(char* name, Pets* node1);
void insertPet(Pets** headPtr);
char *fgetsWrapper (char *str, int size, FILE *stream);
void printList(Pets* listPtr);
char validateYesNo(void);
void writeNamesToFile(FILE* filePtr, Pets* petPtr);
void removePets(Pets** headPtr);
void removeAllPets(Pets** headPtr);


int main (void){
    
    // Requirement 2 - Use a linked list to store the pet information in
    // alphabetical order
    Pets* headNodePtr = NULL;
    
    insertPet(&headNodePtr);
    
    FILE *nameFilePtr = NULL;
    
    writeNamesToFile(nameFilePtr, headNodePtr);
    
    removePets(&headNodePtr);
    
    puts("Removing all animals from memory before exiting program.");
    removeAllPets(&headNodePtr);
    
    // Requirement 9 - Display list of pets
    printList(headNodePtr);
    
    return 0;
} //main


// Requirement 3 - Create a function to string compare names but ignore the case.
int compareNames(char* name, Pets* node1)
{
    int result = 0;
    int counter = 0;
    char nameToCompare[STRING_LENGTH] = {'\0'};
    char nodeName[STRING_LENGTH] = {'\0'};
    
    strcpy(nodeName, node1->name);
    
    for (size_t i = 0; i < sizeof(name); i++)
    {
        nameToCompare[i] = tolower(name[i]);
    }
    while (nodeName[counter] != '\0')
    {
        nodeName[counter] = tolower(nodeName[counter]);
        counter++;
    }
    counter = 0;
    
    result = strcmp(nameToCompare, nodeName);
    
    if (result < 0)
    {
        return -1;
    }
    else if (result > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
} // compareNames


// Requirement 4 - Create a function to insert pets
void insertPet(Pets** headPtr)
{
    char name[STRING_LENGTH] = {'\0'};
    int age = 0;
    char yesOrNo = ' ';
    
    printf("%s", "Do you want to enter a pet? ");
    yesOrNo = validateYesNo();
    
    while (yesOrNo == 'y')
    {
        printf("%s", "\nEnter the name of your pet: ");
        fgetsWrapper(name, STRING_LENGTH, stdin);
        printf("%s", "Enter the age of your pet: ");
        scanf("%d", &age);
        while (getchar() != '\n');
        
        Pets* newNodePtr = malloc(sizeof(Pets));
        
        if (newNodePtr != NULL)
        {
            strcpy(newNodePtr->name, name);
            newNodePtr->age = age;
            
            newNodePtr->nextNodePtr = NULL;
            
            Pets* previousPtr = NULL;
            
            Pets* currentPtr = *headPtr;
            
            while (currentPtr != NULL && compareNames(name, currentPtr) >= 0)  // Need to find where to insert the new node.
            {
                previousPtr = currentPtr;
                currentPtr = currentPtr->nextNodePtr;
            }
            
            if (previousPtr == NULL)
            {
                *headPtr = newNodePtr;
            }
            else
            {
                previousPtr->nextNodePtr = newNodePtr;
            }
            
            newNodePtr->nextNodePtr = currentPtr;
        }
        else
        {
            printf("No memory to create node for %s\n", name);
        }
        
        printf("%s", "\nDo you want to add another pet? ");
        yesOrNo = validateYesNo();
    }

    printList(*headPtr);
} // insertPetName


char *fgetsWrapper (char *str, int size, FILE *stream)
{
    size_t length = 0;
                  
    fgets(str, size, stream);
                  
    if (str != NULL)
    {
        length = strlen(str);
                      
        if (length > 0 && str[length - 1] == '\n')
        {
            str[length - 1] = '\0';
        }
        return str;
    }
    else
    {
        return NULL;
    }
} //fgetsWrapper

// Requirement 5 - Creat a function to prints the linked list of pets
// Will report if there are no pets in the list
void printList(Pets* listPtr)
{
    if (listPtr != NULL)
    {
        printf("%s", "\nThe names in alphabetical order:\n");
        
        Pets* currentPtr = listPtr;

        while (currentPtr != NULL)
        {
            // display and go to next node
            printf("%s is %d years old.\n", currentPtr->name, currentPtr->age);
            currentPtr = currentPtr->nextNodePtr;
        }
    }
    else
    {
        puts("\nThere are no pets in the list.\n");
    }
} // printList

char validateYesNo(void)
{
    char validYesNo;

    do {
        printf("%s", "Please enter (y)es or (n)o: ");
        validYesNo = getchar();
        while (getchar() != '\n');

        validYesNo = tolower(validYesNo);

    } while (validYesNo != 'y' && validYesNo != 'n');

    return  validYesNo;
} //End validateYesNo


// Requirement 6 - Function to write the names and ages of the pets to a file
void writeNamesToFile(FILE* filePtr, Pets* headPtr)
{
    Pets* current = malloc(sizeof(headPtr));
    
    current = headPtr;
    
    if ((filePtr = fopen("/Users/devonwhite/Desktop/Github/CS2060ClassCode/CS2060ClassCodeFall2023-main/examples/petNames.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        while (current != NULL)
        {
            fprintf(filePtr, "%s\t%d\n", current->name, current->age);
            current = current->nextNodePtr;
        }
    }
    fclose(filePtr);
} // writeNamesToFile


// Requirement 7 - Function to remove a pet
void removePets(Pets** headPtr)
{
    char yesOrNo = ' ';
    char nameToDelete[STRING_LENGTH] = {'\0'};
    
   do
    {
        if (*headPtr != NULL)
        {
            printf("%s", "\nDo you want to remove a pet from the list? ");
            yesOrNo = validateYesNo();
            
            if (yesOrNo == 'y')
            {
                Pets* previousPtr = NULL;
                Pets* currentPtr = *headPtr;
                
                printf("%s", "\nEnter the name you want removed: ");
                fgetsWrapper(nameToDelete, STRING_LENGTH, stdin);
                
                if (strcmp((*headPtr)->name, nameToDelete) == 0)
                {
                    *headPtr = (*headPtr)->nextNodePtr;
                    free(currentPtr);
                    currentPtr = NULL;
                }
                else
                {
                    while (currentPtr != NULL && strcmp(currentPtr->name, nameToDelete) != 0)
                    {
                        previousPtr = currentPtr;
                        currentPtr = currentPtr->nextNodePtr;
                    }
                    if (currentPtr != NULL)
                    {
                        previousPtr->nextNodePtr = currentPtr->nextNodePtr;
                        free(currentPtr);
                        currentPtr = NULL;
                    }
                    else
                    {
                        printf("'%s' was not found in the list of pets!\n", nameToDelete);
                    }
                }
            }
            else
            {
                yesOrNo = 'n';
            }
            printList(*headPtr);
        }
        else
        {
            printList(*headPtr);
            yesOrNo = 'n';
        }
    }  while (yesOrNo == 'y');
} // removePets


// Requirement 8 - Function to remove any remaining pets in the list
void removeAllPets(Pets** headPtr)
{
    Pets* currentPtr = *headPtr;
    Pets* nextPtr = NULL;
    
    while (currentPtr != NULL)
    {
        nextPtr = currentPtr->nextNodePtr;
        free(currentPtr);
        currentPtr = nextPtr;
    }
    
    *headPtr = NULL;
} // removeAllPets
