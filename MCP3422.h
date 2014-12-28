/* 
 * File:   MCP3422.h
 * Author: Javi
 *
 * Created on 10 de noviembre de 2014, 19:28
 */

#ifndef MCP3422_H
#define	MCP3422_H

#include <xc.h>
#include "i2c_hal.h"
#include "GenericTypeDefs.h"


#define _XTAL_FREQ 16000000



#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif


void initMCP3422();
WORD readMCP3422();

#endif	/* MCP3422_H */

