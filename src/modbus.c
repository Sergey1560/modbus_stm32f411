#include "modbus.h"

static USHORT usRegInputBuf[REG_INPUT_NREGS];
static UCHAR usDicreteInputBuf[REG_DISCRET_IN_NREGS];
static UCHAR usCoilsBuf[REG_COILS_IN_NREGS];

eMBErrorCode modbus_init(void){
    eMBErrorCode eStatus;

    eStatus = eMBInit( MB_RTU, 0x0A, 0, 38400, MB_PAR_NONE );

	if(eStatus){
		ERROR("MB init error");
        return eStatus;
	}
	
	/* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable();

	if(eStatus){
		DEBUG("MB enable error");
		return eStatus;
	}


    return MB_ENOERR;
}


void update_reg_input(void){
    usRegInputBuf[0] = uptime_get_value();
    usRegInputBuf[1] = (SystemCoreClock/1000000);
}

void update_dicrete_reg(void){
    usDicreteInputBuf[0] = led_state();
    usDicreteInputBuf[1] = btn_state();
}


void update_coils_reg(void){
    usCoilsBuf[0] = led_state();
}

void set_coils_reg(uint8_t num, uint8_t value){

    DEBUG("Set %d value %d",num,value);

    switch (num)
    {
    case 0:
        led_change(value);
        break;
    
    default:
        break;
    }
}



eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs ){
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    DEBUG("Input");

    if( ( usAddress >= REG_INPUT_START ) && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) ){

        update_reg_input();
        DEBUG("Input %d at %d", usNRegs,usAddress);

        iRegIndex = ( int )( usAddress - REG_INPUT_START );
        while( usNRegs > 0 ){
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );            
            iRegIndex++;
            usNRegs--;
        }
    }else{
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,eMBRegisterMode eMode ){
    DEBUG("Holding");
    return MB_ENOREG;
}

eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,eMBRegisterMode eMode ){
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    uint8_t i;

    DEBUG("Coils");
    if( ( usAddress >= REG_COILS_IN_START ) && ( usAddress + usNCoils <= REG_COILS_IN_START + REG_COILS_IN_NREGS ) ){

        iRegIndex = ( int )( usAddress - REG_COILS_IN_START);

        switch (eMode){
            case MB_REG_READ:
                update_coils_reg();
                *pucRegBuffer = 0;
                i=0;
                while( usNCoils > 0 ){
                    *pucRegBuffer |= ((UCHAR)usCoilsBuf[iRegIndex] << i);
                    iRegIndex++;
                    usNCoils--;
                    i++;
                }
                break;

            case MB_REG_WRITE:

                while( usNCoils > 0 ){
                    set_coils_reg(iRegIndex,((*pucRegBuffer >> i) & 1));
                    iRegIndex++;
                    usNCoils--;
                    i++;
                }
                
                break;


            default:
                return MB_ENOREG;
                break;
            }


    }else{
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}

eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete ){
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    uint8_t i=0;

    DEBUG("Discrete");

    if( ( usAddress >= REG_DISCRET_IN_START ) && ( usAddress + usNDiscrete <= REG_DISCRET_IN_START + REG_DISCRET_IN_NREGS ) ){
        DEBUG("Input %d at %d", usNDiscrete,usAddress);
        update_dicrete_reg();
        iRegIndex = ( int )( usAddress - REG_DISCRET_IN_START);

        *pucRegBuffer = 0;
        while( usNDiscrete > 0 ){
            *pucRegBuffer |= ((UCHAR)usDicreteInputBuf[iRegIndex] << i);
            iRegIndex++;
            usNDiscrete--;
            i++;
        }
    }else{
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

