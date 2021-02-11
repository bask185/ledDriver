#include "Encoder.h"
#include "Led.h"

const int sw1 = A0 ;
const int pwmPin[] = { 3, 5, 6, 9, 10, 11 } ;

const int clkPin = 4 ;
const int dataPin = 7 ;
const int switchPin = 8 ;

const int nLeds = 6 ;
const int nPrograms = 20 ;

uint8_t second ;
uint8_t encoderState ;
int8_t  currentLed ;

uint8_t ledMask ;

#define allLedsOn 0b00111111
#define allLedsOff 0


#define REPEAT_MS(x)    { \
                            static uint32_t previousTime ; \
                            uint32_t currentTime = millis() ; \
                            if( currentTime - previousTime >= x ) \
                            {
                         
#define END_REPEAT          } \
                        }

Encoder encoder( clkPin, dataPin, switchPin ) ;

LedDriver led[ nLeds ] {     // this correct?
    LedDriver( 0, pwmPin[0] ),
    LedDriver( 1, pwmPin[1] ),
    LedDriver( 2, pwmPin[2] ),
    LedDriver( 3, pwmPin[3] ),
    LedDriver( 4, pwmPin[4] ),
    LedDriver( 5, pwmPin[5] )
} ;


void setup()
{
    encoder.begin() ;
    
    ledMask = allLedsOn ;
    
    for( int i = 0 ; i < nLeds ; i ++ )
    {
        led[i].begin() ;
    }
}

void loop() {    
    encoderStateMachine() ;
 
    for( int i = 0 ; i < nLeds ; i ++ )
    {
        if( digitalRead( sw1 + i ) & ledMask )  // if both are, true, led may be on
        {
            led[i].runProgram() ;
        }
        else
        {
            digitalWrite( pwmPin[i], LOW) ;     // If a led is not enabled, turn it down
        }
    }
}

/**************** ENCODER STATE MACHINE ******************/

// CONSTANTS
enum states {
    waitChange,
    selectLed,
    changeBrightness,
    changeProgram,
} ;

// MACROS
#define stateFunction(x) static bool x##F(void)
#define entryState if(runOnce) 
#define onState runOnce = false; if(!runOnce)
#define exitState if(!exitFlag) return false; else
#define State(x) break; case x: if(runOnce) if(x##F())
#define STATE_MACHINE_BEGIN switch( state ) {\
	default:
#define STATE_MACHINE_END break;}



// VARIABLES
static unsigned char state = waitChange ;
static bool runOnce = true, exitFlag = false;

void nextState( uint8_t _state ) {
    state = _state ;     
}

// default state, turn all leds on and do nothing else but to monitor encoder change
stateFunction( waitChange ) {
    entryState
    {
        ledMask = allLedsOn ;
    }
    onState
    {
        if( encoderState != HIGH ) exitFlag = true ; // if the encoder did something..
    }
    exitState
    {
        ledMask = allLedsOff ;
        currentLed = 0 ;
        
        return true ;
    }
}

// select a led, light it up, and press the button for 1s to adjust brightness and hold for 2s to adjust program
stateFunction( selectLed ) {    
    static uint32_t previousTime ;
    entryState
    {
        second = 0 ;
        ledMask = 1 << currentLed ;         // turn on the led, by enabling it
    }
    onState
    {
        uint32_t currentTime = millis() ;
        
        if(( encoderState ==  leftPulse )
        || ( encoderState == rightPulse ) 
        || ( encoderState ==     RISING )) exitFlag = true ;         // if any of these states, exit
        
        if( encoderState == LOW )
        {
            if( currentTime - previousTime > 2000 )    // buttonPress > 2s
            {
                second = 2;
            }
            else 
            if( currentTime - previousTime > 1000 )    // 1s < buttonPress < 2s
            {
                second = 1;
            }
        }
        else
        {
            previousTime = currentTime ;
        }
    }
    exitState
    {
        if( encoderState ==  leftPulse ) if( --currentLed < 0            ) currentLed = nLeds - 1 ; // select new led, if encoder turned left or right
        if( encoderState == rightPulse ) if( ++currentLed > ( nLeds - 1 )) currentLed = 0 ;
        
        ledMask = 1 << currentLed ;
        return true ;
    }
}

stateFunction( changeBrightness ) {
    entryState
    {
        
    }
    onState
    {
        if( encoderState ==  leftPulse ) led[currentLed].decBrightness() ;
        if( encoderState == rightPulse ) led[currentLed].incBrightness() ;
        
        if( encoderState == RISING) exitFlag = true ;
    }
    exitState
    {
        led[currentLed].storeSettings() ;
        return true ;
    }
}

stateFunction( changeProgram ) {
    entryState
    {
        
    }
    onState
    {
        if( encoderState ==  leftPulse ) led[currentLed].decProgram();
        if( encoderState == rightPulse ) led[currentLed].incProgram();
        
        if( encoderState == RISING) exitFlag = true ;
    }
    exitState
    {
        led[currentLed].storeSettings() ;
        return true ;
    }
}

void encoderStateMachine() {
    
    REPEAT_MS( 5 ) ;
    encoder.update() ;  // debounce all encoder inputs every x ms
    END_REPEAT ;
    
    encoderState = encoder.getState() ;
    
    STATE_MACHINE_BEGIN
    
    State( waitChange) {
        nextState( selectLed ) ; }
    
    State( selectLed ) {
        if( second == 0 ) nextState( selectLed ) ;
        if( second == 1 ) nextState( changeBrightness ) ;
        if( second == 2 ) nextState( changeProgram ) ; }
        
    State( changeBrightness )  {
        nextState( waitChange ) ; }
        
    State( changeProgram ) {
        nextState( waitChange ) ; }
        
    STATE_MACHINE_END
}