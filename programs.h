#include <Arduino.h>

#define specialProgramInterval  500 

extern uint32_t getTime( uint8_t, uint8_t*  ) ;
extern uint16_t welderProgram( uint8_t, uint8_t* ) ;
extern uint16_t tubeLightProgram( uint8_t, uint8_t* ) ;
extern uint16_t firePlaceProgram( uint8_t ) ;
extern void resetTubeLight() ;

enum specials {
	tubelight = 6, 
	welder,
	firePlace,
	runningLight1,
	runningLight2,
	runningLight3,
} ;
