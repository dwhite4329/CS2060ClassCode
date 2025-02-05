/*
 * Name: Devon White
 * Class: CS2060 M/W 10:50 AM-12:05 PM
 * MAC OS
 * Due: Sep 26, 2023
 * Description: This program allows vacation renters and the rental property
 * owner to see the ratings given for the rental property. It also allows the
 * renter to rate the property.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void printCategories(const char *categories[], size_t totalCategories);
int getValidInt(int min, int max, int sentinel);
void getRatings(int maxRating, int minRating, const int numRatings, const int numCategories,int ratingsArray[][numCategories], int sentinel);
void printSurveyResults(int numRatings, int numCategories, int ratingsArray[][numCategories]);
void calculateCategoryAverages(int numRatings, int numCategories, int ratingsArray[][numCategories], double averageArray[]);
void printCategoryData(const char *categories[], int numRatings, int numCategories, double averageArray[]);

int main (void) {
    
    int unsigned const RENTER_SURVEY_CATEGORIES = 3;
    int unsigned const MAX_RATING = 5;
    unsigned int const MIN_RATING = 1;
    int const SENTINEL_NEG1 = -1;
    int unsigned const NUM_RATINGS = 5;
    
    const char *surveyCategories[RENTER_SURVEY_CATEGORIES] = {"Check-in Process", "Cleanliness", "Amenities"};
    
    //display rating categories information
    printCategories(surveyCategories, RENTER_SURVEY_CATEGORIES);
    
    //create 2D array to hold ratings of the survey.
    int rentalSurvey[NUM_RATINGS][RENTER_SURVEY_CATEGORIES] = {0};
    
    getRatings(MAX_RATING, MIN_RATING, NUM_RATINGS, RENTER_SURVEY_CATEGORIES, rentalSurvey, SENTINEL_NEG1);
    
    printSurveyResults(NUM_RATINGS, RENTER_SURVEY_CATEGORIES, rentalSurvey);
    
    //declare and initialize 1D array to hold averages
    //declared as double to hold non-whole values
    //initialized to 0 to not have "garbage values" in the array.
    double categoryAverages[RENTER_SURVEY_CATEGORIES] = {0};
    
    calculateCategoryAverages(NUM_RATINGS, RENTER_SURVEY_CATEGORIES, rentalSurvey, categoryAverages);
    
    printCategoryData(surveyCategories, NUM_RATINGS, RENTER_SURVEY_CATEGORIES, categoryAverages);
    
    
    return 0;
} //end main

// Prints categories of the survey
void printCategories(const char *categories[], size_t totalCategories)
{
    //loop to display each category horizontally
    puts("We want to know how your experience was renting out property. Using the rating system 1 to 5 to enter your rating for each category.");
    printf("%s", "Rating Categories:\t");
    for (size_t surveyCategory = 0; surveyCategory < totalCategories; ++surveyCategory)
    {
        printf("\t%zu.%s\t", surveyCategory+1, categories[surveyCategory]);
    }
    puts(""); //start new line of output
    
} //end printCategories

// Function for get valid input
int getValidInt(int min, int max, int sentinel)
{
    int inputNights = 0;
    double scanfReturn = 0;
    bool validInput = false;
    
    // Use a while loop to keep getting input until user input is valid
    do
    {
        //scanf("%d", &inputNights);
        scanfReturn = scanf("%d", &inputNights);
        while (getchar() != '\n');
        
        // if statement for scanfReturn to ensure input is valid
        // Acceptance Criteria 1.2.a
        if (scanfReturn == 1)
        {
         
            // if statement to check the validity of the range of user input
            // initializes validInput to true to exit while loop
            if (inputNights >= min && inputNights <= max)
            {
                validInput = true;
            } //if check nightly range
            
            // else if checking for input of the sentinel value
            // this input will also initialize validInput to true to exit while loop
            else if (inputNights == -1)
            {
                validInput = true;
            } //end else if
            
            // else for error of input not within the range of min and max nights
            // Acceptance Criteria 1.2.a
            else
            {
                printf ("Error: Not within %d and %d. Please enter the value again: ", min, max);
            } //else error 2
            
        } //if check valid input
        
        // else for error of input not being an integer value
        else
        {
            puts ("Error: Not an integer number. Please enter the value again.");
        } //else error 1
        
    } //Do-While loop end
    while (validInput == false);
    
    return inputNights;
    
} //end getValidInt function


// Get ratings from user
void getRatings(int maxRating, int minRating, const int numRatings, const int numCategories,int ratingsArray[][numCategories], int sentinel)
{
    
    for (size_t i = 0; i < numRatings; i++)
    {
        for (size_t j = 0; j < numCategories; j++)
        {
            printf("%s%zu\n", "Renter ", i+1);
            
            puts("Enter your rating for");
            printf("Category %zu: ", j+1);
            ratingsArray[i][j] = getValidInt(minRating, maxRating, sentinel);
            //ratingsArray[i][j] = inputRating;

        } //nested for loop end
    
    } // end for loop
    
} //end getRatings


//Print results of survey
void printSurveyResults(int numRatings, int numCategories, int ratingsArray[][numCategories])
{
    for (size_t i = 0; i < numRatings; i++)
    {
        printf("%s %zu: ", "Survey", i+1);
        
        for (size_t j = 0; j < numCategories; j++)
        {
            printf("%7d", ratingsArray[i][j]);
            
        } //nested for
        puts("");
    } //for
} //printSurveyResults end

//calculate the averages of the ratings
void calculateCategoryAverages(int numRatings, int numCategories, int ratingsArray[][numCategories], double averageArray[])
{
    double average = 0.0;
    int sum = 0;
    
    //for loop to iterate through the columns second
    //allows for easy calculation of sum an averages of each column
    for (size_t i = 0; i < numCategories; i++)
    {
        sum = 0;
        
        //nested for loop to iterate through each survey in the first column
        //to obtain sum and average
        for (size_t j = 0; j < numRatings; j++)
        {
            sum = sum + ratingsArray[j][i];
            
            //if used for when the sum of all columns have been obtained
            //then able to find the average and store in the averageCatergories array in main
            if (j == numRatings - 1)
            {
                average = (double)sum / numRatings;
                averageArray[i] = average;
            }
            
        } //end nested for
        
        
    } //end for
    
    
} //calculateCategoryAverages end

// Print averages by reading from the categoryAverages array
// uses *categories parameter to be able to print category info using printCategories function
void printCategoryData(const char *categories[], int numRatings, int numCategories, double averageArray[])
{
    //Call function to reprint category information
    printCategories(categories, numCategories);
    
    printf("%s", "Rating Averages:");
    
    for (size_t i = 0; i < numCategories; i++)
    {
        printf("%20.1f", averageArray[i]);
    } //for loop end
    
    puts("");
    
} // printCategoryData end
