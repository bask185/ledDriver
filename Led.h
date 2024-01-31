#include <Arduino.h>

class LedDriver {
public: 
    LedDriver( uint8_t, uint8_t );
    
    void begin() ;
    void runProgram() ;
    void stopProgram() ;
    void setProgram( uint8_t ) ;
    uint8_t getProgram( ) ;
    void incProgram() ;
    void decProgram() ;
    void setBrightness( uint8_t ) ;
    void incBrightness() ;
    void decBrightness() ;
    void storeSettings() ;
    void turnOn() ;
    void turnOff() ;
    void factoryReset() ;
    
private:
    int8_t program ;
    
    uint8_t pin ;
    uint8_t dutycycle ;
    uint8_t ID ;
    uint8_t state ;
    
    uint8_t counter ;
    uint32_t previousTime ;
    uint32_t interval ;
} ;