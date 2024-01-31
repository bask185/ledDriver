#include "Led.h"
#include "programs.h"
#include <EEPROM.h>

#define dutycycleAddress 0
#define programAddress 10

#define nProgram 13


LedDriver::LedDriver( uint8_t _ID,  uint8_t _pin) {
	pin = _pin ;
    ID = _ID ;
}

void LedDriver::begin()
{
    dutycycle = EEPROM.read( dutycycleAddress + ID ) ;
    program   = EEPROM.read( programAddress + ID ) ;

    if( program == 255 ) program = 0 ;
    counter = 0 ;

    pinMode( pin, OUTPUT) ;            // I believe this is not needed
    analogWrite( pin, dutycycle ) ;
}

void LedDriver::stopProgram()
{
    digitalWrite( pin, LOW ) ;
    counter = 0 ;       
    interval = 0 ;      // kills any long breaks, so program restarts as soon as it is turned on again
    state = 0 ;         // ensures the blinking starts correctly, prevents from seeing the inversed patterns
}

void LedDriver::runProgram( ) {
    uint32_t currentTime = millis() ;
    
    if( currentTime - previousTime >= interval ) {
        previousTime  = currentTime ;
        
        switch( program ) {
        case firePlace:
            interval = firePlaceProgram( pin ) ; 
            break ;

        case welder:
            interval = welderProgram( pin, &counter ) ; 
            break ;

        case tubelight:
            interval = tubeLightProgram( pin, &counter ) ; 
            break ;


        default:                                            // runs default blinking programs
             interval = getTime( program, &counter ) ;

            if( interval > 0 ) {                           // if time to run is greater than 0, we are running a standard blink program
                //interval = timeToRun ;
            
                state ^= 1; 
                if( state ) analogWrite(  pin, dutycycle ) ; // led goes on
                else        digitalWrite( pin, LOW ) ;

                //if( ++counter >= 20 ) counter = 0 ;         // something with sizeof can't be done :( I forgot where this is for? Programs seems to run without it just fine
            }
            
            else {                                          //  else if timeToRun is 0, we are simply just on, at certain brightness
                analogWrite( pin, dutycycle ) ;
                interval = 100 ;                            // does not really matter what number, updates 10x per second now
            }
            break ;
        }
    }
}

/******** program functions *********/
void LedDriver::setProgram( uint8_t _program)
{
    program = _program ;
}

uint8_t LedDriver::getProgram( )
{
    return program ;
}

void LedDriver::incProgram( )
{
    if( program < nProgram ) program ++ ;
    Serial.println(program);
}

void LedDriver::decProgram( )
{
    if( program > 0 )  program -- ;
    Serial.println(program);
}

/******** brightness functions *********/
void LedDriver::setBrightness( uint8_t _dutycycle )
{
    dutycycle = _dutycycle ;
}

void LedDriver::incBrightness( )
{
    if( dutycycle < 255) dutycycle ++ ;
    Serial.println(dutycycle);
    stopProgram() ;
}

void LedDriver::decBrightness( )
{
    if( dutycycle > 0 ) dutycycle -- ;
    Serial.println(dutycycle);
    stopProgram() ;
}

/********* EEPROM ********/
void LedDriver::storeSettings( )
{
    EEPROM.write( programAddress + ID, program ) ;      //( 10 - 15 )
    EEPROM.write( dutycycleAddress + ID, dutycycle ) ;  //( 0 - 5 )
    stopProgram() ; 
}

/******** ON/OFF *********/
void LedDriver::turnOn()
{
    analogWrite( pin, dutycycle ) ;
}

void LedDriver::turnOff()
{
    analogWrite( pin, 0 ) ;
}

void LedDriver::factoryReset()
{
    EEPROM.write( programAddress + ID,   0 ) ;      //( 10 - 15 )   put program at 0
    EEPROM.write( dutycycleAddress + ID, 255 ) ;    //( 0 - 5 )     put brightness at maximum
    setProgram( 0 );
    setBrightness( 255 ) ;
}