#include "Led.h"
#include "programs.h"
#include <EEPROM.h>

#define dutycycleAddress 0
#define programAddress 10

#define nProgram 20

LedDriver::LedDriver( uint8_t _ID,  uint8_t _pin) {
	pin = _pin ;
    ID = _ID ;
}

void LedDriver::begin()
{
    dutycycle = EEPROM.read( dutycycleAddress + ID ) ;
    program   = EEPROM.read( programAddress + ID ) ;
    
    pinMode( pin, OUTPUT) ;            // I believe this is not needed
    analogWrite( pin, dutycycle ) ;
}

void LedDriver::runProgram( ) {
    uint32_t currentTime = millis() ;
    
    if( currentTime - previousTime >= interval ) {
        previousTime  = currentTime ;
        
        uint16_t timeToRun = getTime( program ) ;
        
        if( timeToRun > 0 ) {                                       // if new time is greater than  0, just burn led
            interval = timeToRun ;
        
            state ^= 1; 
            if( state ) analogWrite(  pin, dutycycle ) ; // led goes on
            else        digitalWrite( pin, LOW ) ;

            if( ++counter >= 20 ) counter = 0 ;         // something with sizeof can't be done :(
        }
        
        else {                                                  // return time was 0, so just turn led on
            analogWrite( pin, dutycycle ) ;
            interval = 100 ;                                // does not really matter what number, updates 10x per second now
        }
    }
}

/******** program functions *********/
void LedDriver::setProgram( uint8_t _program)
{
    program = _program ;
}

void LedDriver::incProgram( )
{
    program ++ ;
    if( program > nProgram) program = 0 ;
}

void LedDriver::decProgram( )
{
    program -- ;
    if( program < 0 )  program = nProgram ;
}

/******** brightness functions *********/
void LedDriver::setBrightness( uint8_t _dutycycle )
{
    dutycycle = _dutycycle ;
}

void LedDriver::incBrightness( )
{
    if( dutycycle < 255) dutycycle ++ ;
}

void LedDriver::decBrightness( )
{
    if( dutycycle > 0 )dutycycle -- ;
}

/********* EEPROM ********/
void LedDriver::storeSettings( )
{
    EEPROM.write( programAddress + ID, program ) ;
    EEPROM.write( dutycycleAddress + ID, dutycycle ) ;
}
