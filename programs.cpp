#include "programs.h"

const uint16_t  program1[] = { 100, 100 } ;
const uint16_t  program2[] = { 200, 200, 100, 100, 2000, 2000 } ;
const uint16_t  program3[] = { 200, 200 } ; // may be variable in length
const uint16_t  program4[] = { 200, 200 } ;
const uint16_t  program5[] = { 200, 200 } ;
const uint16_t  program6[] = { 200, 200 } ;
const uint16_t  program7[] = { 200, 200 } ;
const uint16_t  program8[] = { 200, 200 } ;
const uint16_t  program9[] = { 200, 200 } ;
const uint16_t program10[] = { 200, 200 } ;
const uint16_t program11[] = { 200, 200 } ;
const uint16_t program12[] = { 200, 200 } ;
const uint16_t program13[] = { 200, 200 } ;
const uint16_t program14[] = { 200, 200 } ;
const uint16_t program15[] = { 200, 200 } ;
const uint16_t program16[] = { 200, 200 } ;
const uint16_t program17[] = { 200, 200 } ;
const uint16_t program18[] = { 200, 200 } ;
const uint16_t program19[] = { 200, 200 } ;
const uint16_t program20[] = { 200, 200 } ;

uint8_t counter = 0 ;

#define returnTime(NUMBER)  case NUMBER: \
                            if( ++counter >= sizeof (program##NUMBER )) counter = 0 ; \
                            return program##NUMBER[counter] 

uint16_t getTime( uint8_t program ) {
    switch( program ) {
        default:
        
        case 0: return 0;
        
        case 1: 
        if( ++counter >= sizeof( program1 )) counter = 0 ;
        return program1[counter] ;
        
        case 2: 
        if( ++counter >= sizeof( program2 )) counter = 0 ;
        return program2[counter] ;
        
        returnTime(3) ;
        returnTime(4) ;
        returnTime(5) ;
        returnTime(6) ;
        returnTime(7) ;
    }   
}