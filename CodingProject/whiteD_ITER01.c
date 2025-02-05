/*
 * Name: Devon White
 * Class: CS2060 M/W 10:50 AM-12:05 PM
 * MAC OS
 * Due: Sep 21, 2023
 * Description: This program rents out properties to vactioners from a company
 * called AirUCCS. The pricing has three different tiers. The base rate ranging
 * from the minimum days allowed to interval1. Tier 2 which is a discount rate for
 * days ranging from one day more than interval1 to interval2. And tier 3 where the
 * discount is doubled for day ranging from one day more than interval2 to the
 * maximum days allowed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// function prototypes
// prints the rental property information
void printRetnalPropertyInfo(unsigned int minNights, unsigned int maxNights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount);

// retruns only valid input from the user based on the min and max values inclusively
int getValidInt(int min, int max);

// calculates the charge based on the number of nights rented
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount);

//Prints the number of nights, and the charge if there were any rentals
// Use for printing vacationer charge or for property owner summay
void printNightsCharges(unsigned int nights, double charges);

char getString(char array[]);

int main(void){
    
    // constants to be declared in main for rental propterty information
    int const SENTINEL_NEG1 = -1;
    int unsigned const MIN_RENTAL_NIGHTS = 1;
    unsigned int const MAX_RENTAL_NIGHTS = 14;
    unsigned int const INTERVAL_1_NIGHTS = 3;
    unsigned int const INTERVAL_2_NIGHTS = 6;
    double const RENTAL_RATE = 400;
    double const DISCOUNT = 50;
    int numberNights = 0;
    double rentalCharges = 0.0;
    int totalNights = 0;
    double totalCharges = 0.0;
    int unsigned const RENTER_SURVEY_CATEGORIES = 3;
    int unsigned const MAX_RATING = 5;
    unsigned int const MIN_RATING = 1;
    int unsigned const NUM_RATINGS = 5;
    
    const char *surveyCategories[RENTER_SURVEY_CATEGORIES] = {"Check-in Process", "Cleanliness", "Amenities"};
    
    for 
    
    //do-while loop ensures the program continues to run for multiple customers
    //until the sentinel value is entered and the program enters rental property
    //owner mode.
    //Acceptance Criteria 2.1
    do
    {
        //calling the printRentalPropertyInfo function to display information
        //for vacationer.
        //Acceptance Criteria 1.1
        printRetnalPropertyInfo(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS, RENTAL_RATE, DISCOUNT);
        
        //Assigning the value of numberNights to the returned value of the
        //function call getValidInt. This function will not end until a valid integer
        //value is enetered.
        //so a valid integer value is stored in numberNights.
        //Acceptance Criteria 1.2
        numberNights = getValidInt(MIN_RENTAL_NIGHTS, MAX_RENTAL_NIGHTS);
        
        //If statement to determine which mode the program will run in.
        //Customer mode will run, and continue to run as long as numberNights
        //does not equal the sentinel value (-1).
        if (numberNights != SENTINEL_NEG1)
        {
            totalNights = totalNights + numberNights;
            
            //Assigning the value rentalCharges to the value returned from the function
            //calcualteCharges. Values will be valid due to prior getValidInt function call.
            //Acceptance Critera 1.2.b
            rentalCharges = calculateCharges(numberNights, INTERVAL_1_NIGHTS, INTERVAL_2_NIGHTS, RENTAL_RATE, DISCOUNT);
            
            totalCharges = totalCharges + rentalCharges;
            
            puts ("Rental Charges\n");
            printNightsCharges(numberNights, rentalCharges);
        } //if customer mode end
        
        //Else for event that valid user input is the sentinel value of (-1) and puts
        //the program into property owner mode.
        //Acceptance Criteria 1.2.c
        else if (numberNights == SENTINEL_NEG1)
        {
            if (totalNights == 0)
            {
                puts ("Rental Property Owner Total Summary\n");
                puts ("There were no rentals\n");
            } // if no rentals
            
            else
            {
                puts ("\n");
                puts ("Rental Propterty Owener Total Summary\n");
                printNightsCharges(totalNights, totalCharges);
            } //else rentals
            
        } //else rental property owener mode end
        
    } //do-while end
    while (numberNights != SENTINEL_NEG1);
    
    
    
    return 0;
} //end main

/*
 * Function for printing rental property info
 */
void printRetnalPropertyInfo(unsigned int minNights, unsigned int maxNights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount)
{
    printf ("Rental Property can be rented for %d to %d nights.\n", minNights, maxNights);
    printf ("$%.2f rate a night for the first %d nights.\n", rate, interval1Nights);
    printf ("$%.2f discount rate a night for nights %d to %d\n", discount, (interval1Nights + 1), interval2Nights);
    printf ("$%.2f discount rate a night for each reamining nights over %d.\n\n", (discount * 2), interval2Nights);
    
} //end printRentalPropertyInfo function

/*
 * Function to determine input is valid
 */
int getValidInt(int min, int max)
{
    int inputNights = 0;
    double scanfReturn = 0;
    bool validInput = false;
    
    puts ("Enter the number of nights you want to rent the property: ");
    
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
            /*
            else if (inputNights == -1)
            {
                validInput = true;
            } //end else if
            */
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

/*
 * Function to calculate the charges based off number of nights rented
 */
double calculateCharges(unsigned int nights, unsigned int interval1Nights, unsigned int interval2Nights, double rate, double discount)
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
        totalCharge = (rate * nights) - ((nights - interval2Nights) * (discount * 2) + (interval2Nights - interval1Nights) * discount);
    }
    
    return totalCharge;
} //calculateCharges function end

/*
 * Function to print summary of nights rented and total charges
 */
void printNightsCharges(unsigned int nights, double charges)
{
    
    puts("Nights\tCharge");
    printf("%d\t\t$%.2f\n\n", nights, charges);
    
    
} //printNightsCharges function end

