#ifndef MODBUS_H
#define MODBUS_H

#include "common_defs.h"

#include "mb.h"
#include "mbport.h"

#include "uptime.h"
#include "gpio.h"

#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 2

#define REG_DISCRET_IN_START 2000
#define REG_DISCRET_IN_NREGS 2

#define REG_COILS_IN_START 3000
#define REG_COILS_IN_NREGS 1


eMBErrorCode modbus_init(void);

void update_reg_input(void);

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,eMBRegisterMode eMode );
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,eMBRegisterMode eMode );
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );

#endif