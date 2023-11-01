#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <time.h>

/* DEFINITIONS */
#define RED 27   // GPIO Pin 27
#define GREEN 13 // GPIO Pin 13

// Program States
#define TURN_OFF 0
#define TURN_ON 1
#define BLINK 2
#define EXIT 3

// LED Blink Selection
#define BLINK_GREEN 1
#define BLINK_RED 2
#define CONFIRM 1

// MONITORING
#define STUDENTID "2101234" // the student ID is not needed in the group project of 2023

/* FUNCTION PROTOTYPES */
void setupProgram();
void startProgram();
int getUserSelection();
void turnOffLeds();
void turnOnLeds();
void blink();
int getBlinkLed();
int getBlinkFrequency();
int getBlinkDutyCycle();
int getBlinkBrightness();
int confirmBlinkSelection();
int connectToMonitorDevice();
void blinkLedWithConfig();
void endProgram();

/* MAIN PROGRAM */
int main(void)
{
    setupProgram();
    startProgram();
    endProgram();
    return 0;
}

/*
Sets up the LED GPIO pins as output and PWM
*/
void setupProgram()
{
    wiringPiSetupGpio();
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    softPwmCreate(GREEN, 0, 100);
    softPwmCreate(RED, 0, 100);
    system("clear");
}

/*
Takes the input of the user selection and direct it to different states of the program
*/
void startProgram()
{
    int selection;

    do
    {
        selection = getUserSelection();

        switch (selection)
        {

        case TURN_OFF:
            turnOffLeds();
            break;
        case TURN_ON:
            turnOnLeds();
            break;
        case BLINK:
            blink();
            break;
        case EXIT:
            break;
        default:
            printf("\nInvalid Input. Try Again...\n");
            break;
        }

    } while (selection != EXIT);

    return;
}

/*
The main menu that determines what the user wants to do
*/
int getUserSelection()
{
    int selection;

    printf("\n===== LAD STUDENT DEVICE =====\n");
    printf("\n[0] Turn off both LEDs\n");
    printf("[1] Turn on both LEDs\n");
    printf("[2] Blink LED\n");
    printf("[3] Exit\n");
    printf("\nYour Selection: ");

    scanf("%d", &selection);

    return selection;
}

/*
For troubleshooting, turning off LEDs and PWM
*/
void turnOffLeds()
{
    system("clear");
    printf("\nTurning off both LEDs...\n");
    digitalWrite(GREEN, LOW);
    softPwmWrite(GREEN, 0);
    digitalWrite(RED, LOW);
    softPwmWrite(RED, 0);
}

/*
For troubleshooting, turning on LEDs and PWM at 100
*/
void turnOnLeds()
{
    system("clear");
    printf("\nTurning on both LEDs...\n");
    digitalWrite(GREEN, HIGH);
    softPwmWrite(GREEN, 100);
    digitalWrite(RED, HIGH);
    softPwmWrite(RED, 100);
}

/*
When user wants to blink LED, this function will get all the blinking configuration
It gets from the user the LED to blink, frequency and brightness.
Then, it will call a function to attempt handshake with Monitor before executing the blink
*/
void blink()
{
    system("clear");
    printf("\nBlink...\n");

    int blinkLed = getBlinkLed(); // Either Green [1] or Red [2]
    int frequency = getBlinkFrequency();
    int dutyCycle = getBlinkDutyCycle();
    int brightness = getBlinkBrightness();

    if (confirmBlinkSelection(blinkLed, frequency, dutyCycle, brightness) == CONFIRM)
    {
        blinkLedWithConfig(blinkLed, frequency, dutyCycle, brightness); 

        system("clear");
        // if (connectToMonitorDevice(blinkLed, frequency, brightness) < 0)
        // {
        //     blinkLedWithConfig(blinkLed, frequency, brightness);
        //     printf("Connection failed, please make sure monitor device is ready.\n");
        // }
        // else
        // {
        //     blinkLedWithConfig(blinkLed, frequency, brightness);         // This chunk of code is commented out to prevent the connectToMonitorDevice from running

        //     system("clear");
        // }
    }
    else
        return;
}

/*
Menu to get user selction on LED to blink
*/
int getBlinkLed()
{
    int selection;

    printf("\nSelect LED to blink.\n\n");
    printf("[1] Green LED\n");
    printf("[2] Red LED\n");
    printf("\nYour Selection: ");

    scanf("%d", &selection);

    if (selection != BLINK_GREEN && selection != BLINK_RED)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkLed();
    }
    else
    {
        system("clear");
        return selection;
    }
}

/*
Menu to get user selction on Frequency to blink
*/
int getBlinkFrequency()
{
    int selection;

    printf("Enter frequency to blink.\n\n");
    printf("Enter whole numbers between 0 to 10\n");
    printf("\nFrequency (Hz): ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 10)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkFrequency();
    }
    else
    {
        system("clear");
        return selection;
    }
}

/*
Menu to get user selction on LED brightness
*/
int getBlinkBrightness()
{
    int selection;

    printf("Select LED brightness during blink.\n\n");
    printf("Enter whole numbers between 0 to 100\n");
    printf("Brightness (%%): ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 100)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkBrightness();
    }
    else
    {
        system("clear");
        return selection;
    }
}

/*
Menu to get user selction on duty cycle
*/
int getBlinkDutyCycle()                                                     // Added another menu selection for user to input duty cycle
{
    int selection;

    printf("Enter duty cycle percentage.\n\n");
    printf("Enter whole numbers between 0 to 100\n");
    printf("\nDuty Cycle (%%): ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 100)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkDutyCycle();
    }
    else
    {
        system("clear");
        return selection;
    }
}

/*
Menu for user to acknowldge the blink configurations input
*/
int confirmBlinkSelection(int blinkLed, int blinkFrequency, int blinkDutyCycle, int blinkBrightness)
{
    int selection;
    char blinkLedString[] = "Green";

    if (blinkLed == BLINK_RED)
    {
        strcpy(blinkLedString, "Red");
    }

    printf("Confirm your blink configrations.\n\n");
    printf("LED to blink: %s\n", blinkLedString);
    printf("Blink Frequency: %dHz\n", blinkFrequency);
    printf("Blink Duty Cycle: %d%%\n", blinkDutyCycle);                     // Added another menu selection for user to input duty cycle
    printf("Blink Brightness: %d%%\n\n", blinkBrightness);
    printf("[1] Confirm Configuration\n");
    printf("[0] Return to Home\n");
    printf("\nYour Selection: ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 1)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        confirmBlinkSelection(blinkLed, blinkFrequency, blinkDutyCycle, blinkBrightness);
    }
    else
    {
        return selection;
    }
}

/*
Handshake algorithm to connect and send blink configurations
*/
int connectToMonitorDevice(int blinkLed, int blinkFrequency, int blinkBrightness)
{
    system("clear");
    printf("Connecting to Monitor Device...\n");

    int serial_port;
    char studentid[] = STUDENTID;

    char blinkLedString[1];
    char blinkFrequencyString[2];
    char blinkBrightnessString[3];

    sprintf(blinkLedString, "%d", blinkLed);
    sprintf(blinkFrequencyString, "%d", blinkFrequency);
    sprintf(blinkBrightnessString, "%d", blinkBrightness);

    // Open the serial port
    serial_port = serialOpen("/dev/ttyAMA0", 9600);
    if (serial_port < 0)
    {
        fprintf(stderr, "Error opening serial port\n");
        return -1;
    }

    // Write data to the serial port
    serialPuts(serial_port, studentid);

    // Read data from the serial port
    char buffer[256];
    int n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';

    if (strlen(buffer) == 7)
    {
        printf("Acknowledge Student ID: %s\n", buffer);
    }
    else
        return -1;

    // Send Blink Configuration
    serialPuts(serial_port, blinkLedString);
    n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("%s\n", buffer);

    serialPuts(serial_port, blinkFrequencyString);
    n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("%s\n", buffer);

    serialPuts(serial_port, blinkBrightnessString);
    n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("%s\n", buffer);

    serialClose(serial_port);
    printf("Connection Successful!\n");
    delay(5000);
    return 1;
}

/*
Blinks the LED according to the user configuration
*/
void blinkLedWithConfig(int blinkLed, int blinkFrequency, int blinkDutyCycle, int blinkBrightness)
{
    // Array
    int **dataArray;                                                        // Setting up the array
    int cols = 2;
    int rows = 1;
    dataArray = malloc(sizeof(int *) * rows);                               // Allocating memory to the array
    for (int i = 0; i < rows; i++)
    {
        dataArray[i] = malloc(sizeof(int) * cols);
    }

    printf("\nBlinking...\n");
    
    // Setting Frequency
    float dutyCycle = blinkDutyCycle * 10;                                  // Calculate the duty cycle from user input
    float onTime = 1.0f / blinkFrequency * dutyCycle;                       // Time for LED to be in HIGH state depending on duty cycle
    float offTime = 1.0f / blinkFrequency * (1000 - dutyCycle);             // Time for LED to be in LOW state depending on duty cycle
    unsigned long theTime = 0;
    unsigned long previousTime = 0;
    int loopDuration = 120 * blinkFrequency;
          
    // Setting Blink LED
    char blinkLedString[] = "Green";
    if (blinkLed == BLINK_GREEN)
    {
        blinkLed = GREEN;
    }
    else
    {
        blinkLed = RED;
        strcpy(blinkLedString, "Red");
    }

    // Blinking
    unsigned long previousMillis = 0;
    int ledState = LOW;

    for (int blink = 0; blink < loopDuration;)
    {
        int interval = 20;                                                  // 20 milliseconds (To save data every 20ms)  (Linked to the usleep below)
        
        while (blink < loopDuration)                                        // Timer
        {
            if (previousMillis != 0) {                                      // This is to get the timing intervals between each iteration to be saved in CSV
                unsigned long currentTime = millis();
                theTime += currentTime - previousTime;
            }

            unsigned long currentMillis = millis();                         // Get current millisecond
            previousTime = currentMillis;
            //printf("Current millis: %lu\n",currentMillis);                // Just to check if the millis values change between currentMillis and previousMillis
            //printf("Previous millis: %lu\n",previousMillis);              // ^
            //printf("Printing every 20ms...\n");

            if (currentMillis - previousMillis >= offTime && ledState == LOW)
            {
                previousMillis = currentMillis;
                ledState = HIGH;
                //printf("Set to High\n");                                  // Just for testing to show LED being set to HIGH (on)
                softPwmWrite(blinkLed, blinkBrightness);                    // DO NOT REMOVE (Commented out to test with printf)
                blink++;
                digitalWrite(blinkLed, ledState);                           // DO NOT REMOVE (Commented out to test with printf)
            } else if (currentMillis - previousMillis >= onTime && ledState == HIGH) {
                previousMillis = currentMillis;
                ledState = LOW;
                //printf("Set to Low\n");                                   // Just for testing to show LED being set to LOW (off)
                softPwmWrite(blinkLed, 0);                                  // DO NOT REMOVE (Commented out to test with printf)
                blink++;
                digitalWrite(blinkLed, ledState);   
            }

            //printf("LED State is: %i\n", ledState);                       // Show the current LED state (on/off) every 20ms
            fflush(stdout);                                                 // Flush the output buffer to ensure immediate printing

            usleep(interval * 1000);                                        // Sleep for the specified interval in microseconds (Linked to the 20 milliseconds above)
            
            // This saves the data of LED state and timing to the dataArray
            dataArray[rows - 1][0] = ledState;
            dataArray[rows - 1][1] = theTime;
            rows++;
            dataArray = realloc(dataArray, sizeof(int *) * rows);           // This 2 reallocates additional memory to the dataArray as the array size increases
            dataArray[rows - 1] = malloc(sizeof(int) * cols);
        }
    }

    for (int i = 0; i < rows-1; i++)                                        // Show content
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%02d", dataArray[i][j]);
        }
        printf("\n");
    }

    // Save to CSV
    FILE *saveFile = fopen("waveformData.csv", "a");                        // Open/Create file
    fprintf(saveFile, "START\n");
    fprintf(saveFile, "%s LED. Running at %dHz frequency with a duty cycle of %d%%. Brightness set to %d%%.\n", blinkLedString, blinkFrequency, blinkDutyCycle, blinkBrightness);
    for (int i = 0; i < rows-1; i++)
    {
        fprintf(saveFile, "%d, %d\n", dataArray[i][0], dataArray[i][1]);    // Save from array
    }
    fprintf(saveFile, "END\n");

    fclose(saveFile);

    // Free memory from dataArray to prevent memory leak (Reduce wastage of memory)
    for (int i = 0; i < rows; i++)
    {
        free(dataArray[i]);
    }
    free(dataArray);
}

/*
Resetting and cleaning up before safely exiting the program.
*/
void endProgram()
{
    system("clear");
    printf("\nCleaning Up...\n");
    // Turn Off LEDs
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);

    // Turn Off LED Software PWM
    softPwmWrite(GREEN, 0);
    softPwmWrite(RED, 0);

    // Reset Pins to Original INPUT State
    pinMode(GREEN, INPUT);
    pinMode(RED, INPUT);

    printf("Bye!\n\n");
}