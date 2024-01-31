#include "programs.h"

static uint8_t tubeLightCounter = 1 ;
//static uint8_t counter[ 6 ] ;

/*  BLINKING PROGRAMS
*  The below 5 programs can contain custom made blinking patterns. The numbers are represented in ON time and OFF time in sequence
*   You can alter a program to your own liking. There are however a few rules you must obey.

*   1). The amount of numbers should be an equal amount. This will make sure the sequence will repeat itself
*   2). In order to have 2 identical programs to run in contra (police car lights). You need to add the same sequence of numbers with once alteration
        One of the programs, should look like: "1,1, sequence" and the counter program should be "1, sequence, 1". See program 1 and 2 to see an example 
    3). 2 countering programs should always be enabled and disabled by the same switch. Turning off 1 LED while the other remains on, will cause a desynch
        which cannot be solved other than a press on the reset button
    4). program 0 is simply just ON
    5). programs 6 - 13 are special. From 6 and on the sequene = tubelight, welder, firePlace. Program 9 and 10 are running lights and inverse running lights respectively
    6). In order to have running lights, all 6 leds will form a running light. To achieve this put the program of channel on either 9 or 10
    7). In order to have to a fireplace effect you need to use atleast 2 leds, 1 yellow and 1 red. Put both led channels on the firePlace prorgram (8). 
        adding a 3rd led (yellow) will have a nice effect.
    8). Using a blue led in combination with the firePlace program, you may create a tellivison of some sort.
    9). The first number you enter, is the OFF time

*/

  // enter your own custom programs.
const uint32_t  program1[] = {   1,   50, 100,  50, 400, 1 } ;   // blauwe lamp 1
const uint32_t  program2[] = {   1,1,  400,  50, 100,  50 } ;   // blauwe lamp 2
const uint32_t  program3[] = {    500,  40, 100, 40 } ;         // oranje lamp
const uint32_t  program4[] = {    40,  40 } ;
const uint32_t  program5[] = {    50,  50, 10, 10,  } ;



#define returnTime(NUMBER)  case NUMBER: {\
                            if( ++ *counter >= sizeof (program##NUMBER ) / 2 ) *counter = 0 ; \
                            return program##NUMBER[ *counter ]; }

uint32_t getTime( uint8_t program, uint8_t *counter ) {

    switch( program ) {
        default:
        case 0: return 0;
        returnTime(1) ;
        returnTime(2) ;
        returnTime(3) ;
        returnTime(4) ;
        returnTime(5) ;
        //#define tubelight 6
        //#define welder 7
        //#define firePlace 8

        // case tubelight: return 0 ;

        

        
        //case firePlace: return( random( 90,  75) ) ;   // fire place
    }   
}



uint16_t welderProgram ( uint8_t _pin, uint8_t *counter )
{
    *counter ^= 1;

    if( *counter )                               // on time
    {
        digitalWrite( _pin, HIGH ) ;
        return random(20,70) ;
    }
    else                                        // off time
    {
        digitalWrite( _pin, LOW ) ;

        #define MIN 0
        #define MAX 75                          // insert chance, bigger number > smaller change a break will occur

        #define shortBreak  15000  // 15 seconds
        #define mediumBreak 60000  // 1 minute
        #define longBreak   300000 // 5 minutes

        #define SHORT 1
        #define MEDIUM SHORT+1
        #define LONG MAX-1
        
        #define SHORT 1
        uint8_t chance = random( MIN, MAX ) ;
        if( chance <= SHORT )  { /*Serial.print(millis()); Serial.println("prev time, short break") ;*/   return  shortBreak ; }   // short break has higher chance than long break ;
        if( chance == MEDIUM ) { /*Serial.print(millis()); Serial.println("prev time, medium break") ;*/  return mediumBreak ; }
        //if( chance == LONG )   { /*Serial.print(millis()); Serial.println("prev time, long break") ; */   return   longBreak ; } // turn on if you want long breaks of 5 minutes
        return random(50,800) ;
    }
}

const int nFlickers = 75 ;
uint16_t tubeLightProgram( uint8_t _pin, uint8_t *counter )
{
    switch( *counter )
    {
    default:                                    // flicker like a fireplace
        *counter += 1;
        analogWrite( _pin, random( 255 ) /10 *10 ) ;
        return random( 10, 30 ) ; // new time to run
    
    case nFlickers:
        analogWrite( _pin, 255 ) ;
        if( random(0,150) == 0 ) *counter = 0 ;   // normally, TL tube is on, and sometimes it may flicker a bit
        return  75 ;
    }
}

uint16_t firePlaceProgram( uint8_t _pin )
{
    analogWrite( _pin, random( 120 ) + 135 ) ;
    return random( 90,  75 ) ; // new time to run
}