/*
 * Name: Devon White
 * Class: CS2060 M/W 10:50AM - 12:05PM
 * MAC OS
 * Due: Nov 28, 2023
 * Description: This program rents out properties to vactioners from a company
 * called AirUCCS. The pricing has three different tiers. The base rate ranging
 * from the minimum days allowed to interval1. Tier 2 which is a discount rate for
 * days ranging from one day more than interval1 to interval2. And tier 3 where the
 * discount is doubled for day ranging from one day more than interval2 to the
 * maximum days allowed. The program will prompt for a userID and password for the
 * owner to enter "owner mode" and set up properties containing the previously stated
 * parameters. The owner can create more than one property. Properties will be added to a
 * linked list for access in the program. In addition, the owner can input the sentinel value
 * when in "renter mode" to have the progrom print out a summary report of each property's
 * rental numbers, total amount made, and average rankings provided by the renter.
 * The summary will also be written to a txt file for easy viewing. Renters are
 * able rent a property while in "rental mode" and rank the property based onthe provided categories.
 * The renter will be asked to select which property they want to rent by name, and the program
 * will compare the input to each property's name (not case sensitive), and then they will proceed
 * through the rest of the rental process.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

// Symbolic constants to prevent hardcoding
//Maximum length of a string
#define  STRING_LENGTH 80
//Two dimensional array storage amounts for rows and columns of survey data
#define VACATION_RENTERS 5
#define RENTER_SURVEY_CATEGORIES 3
#define MIN_RATING 1
#define MAX_RATING 5
//Rental property login and sentinal values
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "ABCD"
#define LOGIN_MAX_ATTEMPTS 3
#define SENTINAL_NEG1 -1
//rental property constant ranges
#define MIN_RENTAL_NIGHTS 1
#define MAX_RENTAL_NIGHTS 14
#define MIN_RATE 1
#define MAX_RATE 1000
#define DISCOUNT_MULTIPLIER 2


// Structure for rental property information
// Holds the values each property needs to hold
// typedef allows for easy creation and reference of the property
typedef struct Property {
    char name[STRING_LENGTH];
    char location[STRING_LENGTH];
    int interval1;
    int interval2;
    int totalRenters;
    double rate;
    double discount;
    int ratings[VACATION_RENTERS][RENTER_SURVEY_CATEGORIES];
    char categories[RENTER_SURVEY_CATEGORIES][STRING_LENGTH];
    double averageRatings[RENTER_SURVEY_CATEGORIES];
    double totalRevenue;
    int totalNights;
    int ratingsEntered;
    struct Property* nextPropPtr;
} Property;




// Function Prototypes
void printRetnalPropertyInfo(Property *currentPropPtr);
void setUpProperty(int minNights, int maxNights, int minRate, int maxRate, Property** propertyPtr);
int getValidInt(unsigned int min, unsigned int max);
int scanInt(char* str);
char *fgetsWrapper (char *str, int size, FILE *stream);
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount, int multiplier);
bool ownerLogin(const char* username, const char* passcode, unsigned int attempts);
int getValidNights(unsigned int min, unsigned int max, const int sentinel);
void printNightsCharges(unsigned int nights, double charges);
void rentalMode(Property *currentPropPtr);
void getRatings(int maxRating, int minRating, const int numRatings, const int numCategories,Property *arrayPtr);
void printCategories(Property *categoryPtr);
void calculateCategoryAverages(Property *currentProp);
void printSurveyResults(Property *propPtr);
void ownerReportMode(Property *currentProp);
char validateYesNo(void);
int compareNames(Property* name1, Property* name2);
void writeReportToFile(FILE* filePtr, Property* headPtr);
Property* comparePropertyName(Property *propPtr);


int main (void){
    
    Property* headPropPtr = NULL;
    
    // User Story 1: Rental Property Owner Login
    if (ownerLogin(CORRECT_ID, CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS) == true)
    {
        puts("Login Successful\n");
        
        // User Story 2: Rental Property Owner Set-up
        setUpProperty(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, MIN_RATE, MAX_RATE, &headPropPtr);
        
        // User Story 3: Vacationer Rental Mode
        rentalMode(headPropPtr);
    }
    else
    {
        puts("Incorrect Login more than 3 times. Exiting AirUCCS.");
    }
    
    free(headPropPtr);
    
    return 0;
} //main

// Task 3.1 - Display rental property information and Ratings
// Prints property information using a pointer to the information held in the
// property structure.
// Receives a property object
// Returns void because method is printing information.
void printRetnalPropertyInfo(Property *currentPropPtr)
{
    printf("Rental Property: %s\n", currentPropPtr->name);
    printf("Location: %s\n", currentPropPtr->location);
    printf("Rental Property can be rented for %d to %d nights.\n", MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS);
    printf("$%.2f rate a night for the first %d nights.\n", currentPropPtr->rate, currentPropPtr->interval1);
    printf("$%.2f discount rate a night for nights %d to %d\n", currentPropPtr->discount, (currentPropPtr->interval1 + 1), currentPropPtr->interval2);
    printf("$%.2f discount rate a night for each remaining night over %d.\n\n", (currentPropPtr->discount * DISCOUNT_MULTIPLIER), currentPropPtr->interval2);
} //printRentalPropertyInfo


// setUpProperty
// Sets up the property once a valid userID and password have been
// entered within 3 attempts.
// Recieves Symbolic constant values for min & max nights, and min & max rate, and a double pointer
// to the property structure.
// Returns void but initializes values in the property structure and adds multiple properties
// into a linked list.
void setUpProperty(int minNights, int maxNights, int minRate, int maxRate, Property** headPropPtr)
{
    char yesOrNo = ' ';
    
    // Task 2.1.1 - Allow owner to create more than one property and add to a linked list
    // properties must be added in alphabetical order.
    do
    {
        Property* newPropPtr = malloc(sizeof(Property));
        
        if (newPropPtr != NULL)
        {
            newPropPtr->nextPropPtr = NULL;
            
            Property* previousPtr = NULL;
            
            Property* currentPtr = *headPropPtr;
            
            // Task 2.1 - Get interval 1 number of nights.
            puts("\nEnter the number of nights until the first discount: ");
            newPropPtr->interval1 = getValidInt(minNights, maxNights);
            
            // Task 2.2 - Get interval 2 number of nights.
            puts("\nEnter the number of night suntil the second discount: ");
            newPropPtr->interval2 = getValidInt(minNights, maxNights);
            
            // Task 2.3 - Get nightly rental rate.
            puts("\nEnter the nightly rental rate: ");
            newPropPtr->rate = (double)getValidInt(minRate, maxRate);
            
            // Task 2.4 - Get the discount amount.
            puts("\nEnter the discount: ");
            newPropPtr->discount = (double)getValidInt(minRate, maxRate);
            
            // Task 2.5 * 2.6 - Get rental property name and location.
            puts("\nEnter the location of the property: ");
            fgetsWrapper(newPropPtr->location, STRING_LENGTH, stdin);
            puts("\nEnter the name of the property: ");
            fgetsWrapper(newPropPtr->name, STRING_LENGTH, stdin);
            puts("");
            
            while (currentPtr != NULL && compareNames(newPropPtr, currentPtr) >= 0)
            {
                previousPtr = currentPtr;
                currentPtr = currentPtr->nextPropPtr;
            }
            if (previousPtr == NULL)
            {
                *headPropPtr = newPropPtr;
            }
            else
            {
                previousPtr->nextPropPtr = newPropPtr;
            }
            newPropPtr->nextPropPtr = currentPtr;
            
            // Task 2.1.2 - Prompt owner if they want to add another property
            puts("Would you like to add another property?\n");
            yesOrNo = validateYesNo();
        }
        else
        {
            puts ("No memory to create a property.");
        }
        
    } while (yesOrNo == 'y');
    puts ("");
    
} //setUpProperty


// getValidInt
// Checks the range of the input from the user
// Recieves an min and max value greater or equal to zero (using symbolic constants)
// returns a valid integer value that is between the parameters set for min and max.
int getValidInt(unsigned int min, unsigned int max)
{
    bool intIsValid = false;
    int validInt = 0;
    char input[STRING_LENGTH];
    
    while (intIsValid == false)
    {
        validInt = scanInt(input);
        
        if (validInt >= min && validInt <= max)
        {
            intIsValid = true;
        }
        else
        {
            printf("Error: Input must be between %d and %d.\n", min, max);
        }
    }
    return validInt;
} //getValidInt


// scanInt
// Obtains a valid integer value and loops if a valid integer is not input by user.
// Will replace stdio function "scanf"
// Recieves a pointer to a string and returns an integer value.
int scanInt(char* str)
{
    char* end;
    errno = 0;
    int validInt = 0;
    bool intIsValid = false;
    
    do
    {
        // Use fgetsWrapper method to get rid of '\n' and replace with '\0'
        fgetsWrapper(str, STRING_LENGTH, stdin);
        long intTest = strtol(str, &end, 10);
        
        if (end == str)
        {
            fprintf(stderr, "Not an integer value. Please try again.\n");
        }
        else if ('\0' != *end) 
        {
            fprintf(stderr, "Input has extra characters at end: %s Please try again.\n", end);
        }
        else if ((LONG_MIN == intTest || LONG_MAX == intTest) && ERANGE == errno) 
        {
            fprintf(stderr, "Input is out of range. Please try again.\n");
        }
        else if (intTest > INT_MAX) 
        {
            fprintf(stderr, "%ld is too large. Please try again.\n", intTest);
        }
        else if (intTest < INT_MIN) 
        {
            fprintf(stderr, "%ld is too small. Please try again.\n", intTest);
        }
        else 
        {
            validInt = (int)intTest;
            intIsValid = true;
        }
    }
    while (intIsValid == false);
    return validInt;
} //scanInt


// fgetsWrapper
// Passing variable "str" not as constant because value will be changed at its address
// Receives a pionter to a string, an int for the size of the char array the pointer is pointing
// to, and the file stream in which to determine how the function is reading characters.
// Returns a string in where the new line '\n' character is replaced with a null terminator '\0'.
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
    else{
        return NULL;
    }
} //fgetsWrapper


// calcualteCharges
// calculates the charge based on the number of nights rented
// The function receives non negative number of nights, interval1 & 2 nights, and symbolic constant
// numbers for the dicount and multiplier.  The rate is received from the input of the user.
// Returns the fully calculated charges as a double value.
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount, int multiplier)
{
    double totalCharge = 0.0;
    
    if (nights <= interval1Nights)
    {
        totalCharge = (nights * rate);
    } //if base chage end
    else if (nights > interval1Nights && nights <= interval2Nights)
    {
        totalCharge = (rate * nights) - ((nights - interval1Nights) * discount);
    } //else interval1Nights discount
    else if (nights > interval2Nights)
    {
        totalCharge = (rate * nights) - ((nights - interval2Nights) * (discount * multiplier) + (interval2Nights - interval1Nights) * discount);
    }
    return totalCharge;
} //calculateCharges


// Task 1.1
// ownerLogin
// This method will return a bool value if the login for the owner is valid.
// Pass constant pointers to username and passcode because values will not be changed
// Use of fgetsWrapper method to obtain string values from user input -- do not need to be
// validated.
// strcmp function used to compare input string (character by character) for valid login
bool ownerLogin(const char* username, const char* passcode, unsigned int attempts)
{
    bool validLogin = false;
    int counter = 1;
    char inputID[STRING_LENGTH] = {'\0'};
    char inputPassword[STRING_LENGTH] = {'\0'};
    
    while (counter <= attempts && validLogin == false)
    {
        puts("Enter User ID:");
        fgetsWrapper(inputID, STRING_LENGTH, stdin);
        puts("Enter User Password: ");
        fgetsWrapper(inputPassword, STRING_LENGTH, stdin);
        
        if (strcmp(username, inputID) == 0 && strcmp(passcode, inputPassword) == 0)
        {
            validLogin = true;
        }
        else
        {
            puts("User ID or Password incorrect. Try again.");
            printf("Attempts remaining: %d\n\n", attempts - counter);
            counter++;
        }
    }
    return validLogin;
} //ownerLogin


// getValidNight
// Checks the range of the input from the user
// Receives the symbolic constant values for min and max nights and the sentinel value.
// Returns an integer value that is between the ranges of the min and max
// Also allows the value sentinel value to be returned which is outside of the range.
int getValidNights(unsigned int min, unsigned int max, const int sentinel)
{
    bool intIsValid = false;
    int validInt = 0;
    char input[STRING_LENGTH];
    
    puts("Enter the number of nights you want to stay.");
    
    while (intIsValid == false)
    {
        validInt = scanInt(input);
        
        if (validInt >= min && validInt <= max)
        {
            intIsValid = true;
        }
        else if (validInt == sentinel)
        {
            intIsValid = true;
        }
        else
        {
            printf("Error: Input must be between %d and %d.\n", min, max);
        }
    }
    return validInt;
} //getValidNights

// printNightsCharges
// Recieves values of the values of nights and charges to be printed.
// Returns output of nights and charges for the current customer.
void printNightsCharges(unsigned int nights, double charges)
{
    puts("Nights\tCharge");
    printf("%d\t\t$%.2f\n\n", nights, charges);
} //printNightsCharges function end


// rentalMode
// vacationer  rental mode to select the property they want to rent
// get the deails and provide a rating for the property.
// Recevies a pointer to a property structure containing the information needed to rent
// a property.
// No return, but is a constant loop until the sentinel value is entered which will exit
// the loop.
void rentalMode(Property *currentPropPtr)
{
    bool sentinalEntered = false;
    int validInt = 0;
    double totalCost = 0.0;
    currentPropPtr->ratingsEntered = 0;
    
    do
    {
        Property* currentListPtr = currentPropPtr;
        
        // Task 3.1.1 - Print properties in alphabetical order from linked list
        if (currentPropPtr != NULL)
        {
            while (currentListPtr != NULL)
            {
                // Task 3.1 - Display rental property information and Ratings
                printRetnalPropertyInfo(currentListPtr);
                if (currentListPtr->ratingsEntered <= VACATION_RENTERS)
                {
                    printSurveyResults(currentListPtr);
                }
                currentListPtr = currentListPtr->nextPropPtr;
            }
        }
        else
        {
            puts ("\nThere are no properties to display.\n");
        }
        
        
        // Task 3.1.2 - Get property name customer wants to rent.
        Property* selectedProp = comparePropertyName(currentPropPtr);
        
        // Task 3.2 - Get number of nights
        validInt = getValidNights(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, SENTINAL_NEG1);
        
        if (validInt == SENTINAL_NEG1)
        {
            // get owner login
            puts("");
            if (ownerLogin(CORRECT_ID, CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS) == true)
            {
                sentinalEntered = true;
                // User Story 4: Rental Property Owner Report mode
                // Task 4.1 - Display property report
                ownerReportMode(currentPropPtr);
                
                FILE* filePointer = NULL;
                
                // Task 4.1.1 - Write report to a file
                writeReportToFile(filePointer, currentPropPtr);
            }
           else
           {
               puts("");
               puts("\nIncorrect Login - returning to Rental Menu.\n");
           }
        }
        else
        {
            // calculate charge
            totalCost = calculateCharges(validInt, selectedProp->interval1, selectedProp->interval2, selectedProp->rate, selectedProp->discount, DISCOUNT_MULTIPLIER);
            
            // Increment totalRenters element in property structure
            selectedProp->totalRenters++;
            
            // Print charges for current stay
            puts("");
            printNightsCharges(validInt, totalCost);
            
            // Add to totalRevenue element in property structure
            selectedProp->totalRevenue = selectedProp->totalRevenue + totalCost;
            
            // Add to totalNights element in property structure
            selectedProp->totalNights = selectedProp->totalNights + validInt;
            
            // Task 3.3 - Get property ratings from Renter
            if (selectedProp->ratingsEntered < VACATION_RENTERS)
            {
                getRatings(MAX_RATING, MIN_RATING, selectedProp->totalRenters, RENTER_SURVEY_CATEGORIES, selectedProp);
                puts("");
            }
            else
            {
                puts("Maximum number of rating has been reached.\n\n");
            }
        }
    
    } while (sentinalEntered == false);
    
} //rentalMode


// getRatings
// Get ratings from user
// A function that points to the property structure to access and mainuplate the
// 2D property array in the structure.
// Receives symbolic constant values for max and min ratings, and number of categories.
// Also receives a const int for number of ratings and pointer to a property structure.
// This fuction is used to Ask for and update the ratings for property structure.
void getRatings(int maxRating, int minRating, const int numRatings, const int numCategories, Property *arrayPtr)
{
    // use of char array to place the strings needed for the categories in categories structure
    // element.
    // declared as const for use with strcpy() function.
    const char *surveyCats[RENTER_SURVEY_CATEGORIES] = {"Check-in Process", "Cleanliness", "Amenities"};
    
    // Puts categories into array located in structure
    // strcpy function copies strings from "surveyCats[i]" and puts in categories[i] element in
    // the structure.
    // for loop used to iterate through both arrays and put in corresponding indexes.
    for (size_t i = 0; i < RENTER_SURVEY_CATEGORIES; i++)
    {
        strcpy(arrayPtr->categories[i], surveyCats[i]);
    }
    
    puts("We want to know how your experience was renting out property. Using the rating system 1 to 5 to enter your rating for each category:");
    for (size_t i = 0; i < numCategories; i++)
    {
        printf("%zu: %s\n", i + 1, arrayPtr->categories[i]);
    }
    puts("");
    if (arrayPtr->ratingsEntered < VACATION_RENTERS)
    {
        for (size_t i = arrayPtr->ratingsEntered; i <= arrayPtr->ratingsEntered; i++)
        {
            for (size_t j = 0; j < numCategories; j++)
            {
                puts("Enter your rating for");
                printf("Category %zu: ", j+1);
                arrayPtr->ratings[i][j] = getValidInt(minRating, maxRating);
            } //inner
        } //outer
        puts("");
        arrayPtr->ratingsEntered++;
    }
} //getRatings



// printCategories
// Displays the categories of the survey that are stored in the categories element
// in the property structure.
// Receives the pointer to the category element of a property structure
// Returns a print statement of the categories in the property element of the
// property structure.
void printCategories(Property *categoryPtr)
{
    //loop to display each category horizontally
    puts("Survey Results");
    printf("%s", "Rating Categories:\t");
    for (size_t surveyCategory = 0; surveyCategory < RENTER_SURVEY_CATEGORIES; ++surveyCategory)
    {
        printf("\t%zu.%s\t", surveyCategory+1, categoryPtr->categories[surveyCategory]);
    }
    puts("");
} //printCategories


// calculateCategoryAverages
// Calcuates the averages of each category and stores that value into
// an averages array elemeent inside the property structure.
// Use of an array element to store category averages so it can be stored with this
// specific property and not lost once method is completed.
// Receives the pointer to the current property to read the each rating of each category
// Returns a print statment displaying the average rating of each category
void calculateCategoryAverages(Property *currentProp)
{
    double average = 0.0;
    int sum = 0;
    
    // For loop to iterate through the columns second
    // allows for easy calculation of sum an averages of each column
    for (size_t i = 0; i < RENTER_SURVEY_CATEGORIES; i++)
    {
        sum = 0;
        
        // Nested for loop to iterate through each survey in the first column
        // to obtain sum and average
        for (size_t j = 0; j < currentProp->ratingsEntered; j++)
        {
            sum = sum + currentProp->ratings[j][i];
            
            // If used for when the sum of all columns have been obtained
            // then able to find the average and store in the averageCatergories array in main
            if (j == (currentProp->ratingsEntered - 1))
            {
                average = (double)sum / currentProp->ratingsEntered;
                currentProp->averageRatings[i] = average;
            }
            
        } //nested for
    } //for
} //calculateCategoryAverages end


// printSurveyResults
// Receives a property structure pointer to access the categories element of the structure.
// This function prints the ratings for all categories from each surveyor
void printSurveyResults(Property *propPtr)
{
    if (propPtr->totalRenters == 0)
    {
        puts("Survey Results");
        puts("No Ratings Currently");
        puts("");
    }
    else
    {
        puts("");
        printCategories(propPtr);
        for (size_t i = 0; i < propPtr->ratingsEntered; i++)
        {
            printf("%s %zu: ", "Survey", i+1);
            
            for (size_t j = 0; j < RENTER_SURVEY_CATEGORIES; j++)
            {
                printf("%21d", propPtr->ratings[i][j]);
            } //nested for
            puts("");
        } //for
    }
    puts("");
} //printSurveyResults end


// ownerReportMode
// Prints the report of total property renters, nights rented, revenue made and ratings
// Receives a pointer to the current property structure to access elements needed to print a
// total report for the owner.
void ownerReportMode(Property *currentProp)
{
    Property* reportPtr = currentProp;
    
    if (reportPtr != NULL)
    {
        puts("\nRental Property Report");
        
        while (reportPtr != NULL)
        {
            printf("Name: %s\n", reportPtr->name);
            printf("Location: %s\n\n", reportPtr->location);
            
            puts("Rental Property Totals");
            puts("Renters\t\tNights\t\tCharges");
            printf("%d\t\t\t\t%d\t\t$%d\n\n", reportPtr->totalRenters, reportPtr->totalNights, (int)reportPtr->totalRevenue);
            
            calculateCategoryAverages(reportPtr);
            
            puts("Category Rating Averages");
            for (size_t i = 0; i < RENTER_SURVEY_CATEGORIES; i++)
            {
                printf("%s: %.1lf\n", reportPtr->categories[i], reportPtr->averageRatings[i]);
            }
            puts("");
            reportPtr = reportPtr->nextPropPtr;
        }
    }
    else
    {
        puts ("\nNo Properties Have Been Rented!\n");
    }
} //ownerReportMode


// validateYesNo
// Validates userinput for a yes or no response
// Receives no parameters
// Returns a char that is converted to a lowercase 'y' or 'n'
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


// compareNames
// Receives a pointer to a string for a name and a pointer to a node
// Returns an integer value used to determine where in linked list property will be placed
// Returned vales allows for determination of which name should be placed first in a
// linked list.
int compareNames(Property* name1, Property* name2)
{
    int result = 0;
    int counter = 0;
    char nameToCompare[STRING_LENGTH] = {'\0'};
    char nodeName[STRING_LENGTH] = {'\0'};
    
    strcpy(nameToCompare, name1->name);
    strcpy(nodeName, name2->name);
    
    
    while (nameToCompare[counter] != '\0')
    {
        nameToCompare[counter] = tolower(nameToCompare[counter]);
        counter++;
    }
    
    counter = 0;
    
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


 // writeReportToFile
 // Receives a pointer to a file and a pointer to a property structure
 // outputs a file as a txt so the owner can read or print the report
 void writeReportToFile(FILE* filePtr, Property* headPtr)
 {
     char name[STRING_LENGTH];
     char folderPath[STRING_LENGTH] = {"/Users/devonwhite/Desktop/fundraiser/"};
     char fileName[STRING_LENGTH];
     
     
     Property* current = malloc(sizeof(headPtr));
     
     current = headPtr;
     
     while (current != NULL)
     {
         strcpy(name, current->name);
         
         for (size_t i = 0; name[i] != '\0'; i++)
         {
             if (name[i] == ' ')
             {
                 name[i] = '_';
             }
         }
         
         sprintf(fileName, "%s%s", folderPath, name);
         
         if ((filePtr = fopen(fileName, "w")) == NULL)
         {
             puts("File could not be opened.");
         }
         else
         {
             fprintf(filePtr, "%s\n", "Rental Property Report");
             fprintf(filePtr, "Name: %s\n", current->name);
             fprintf(filePtr, "Location: %s\n\n", current->location);
             
             fprintf(filePtr, "%s\n", "Rental Property Totals");
             fprintf(filePtr, "%s\t\t%s\t\t%s\n", "Renters", "Nights", "Charges");
             fprintf(filePtr, "%d\t\t%d\t\t$%d\n\n", current->totalRenters, current->totalNights, (int)current->totalRevenue);
             
             calculateCategoryAverages(current);
             
             fprintf(filePtr, "%s\n", "Category Rating Averages");
             for (size_t i = 0; i < RENTER_SURVEY_CATEGORIES; i++)
             {
                 fprintf(filePtr, "%s: %.1lf\n", current->categories[i], current->averageRatings[i]);
             }
             current = current->nextPropPtr;
         } //else
        
     } //while to loop thorugh all properties
    
     fclose(filePtr);
     free(current);
 } // writeNamesToFile
 

//comprePropertyName
// Receives: a pointer to a property object
// Returns: pointer to the selected property
// strcpy: takes user input name and puts the in name element of the userInput node
// compareNames: compares selected name of user to every node in linked list (not case sensitive)
Property* comparePropertyName(Property *propPtr)
{
    char propName[STRING_LENGTH] = {'\0'};
    puts ("Enter the name of the property you want to rent: ");
    fgetsWrapper(propName, STRING_LENGTH, stdin);
    Property* userInput = malloc(sizeof(Property));
    strcpy(userInput->name, propName);
    
    Property* previousProp = NULL;
    Property* current = propPtr;
    
    while (current != NULL && compareNames(userInput, current) != 0)
    {
        previousProp = current;
        current = current->nextPropPtr;
        
        if (current == NULL)
        {
            current = propPtr;
            puts("\nError, the property you entered doesn't match. Enter the property again.");
            fgetsWrapper(userInput->name, STRING_LENGTH, stdin);
            puts ("");
        }
    }
    return current;
}
