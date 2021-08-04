#include "main.h"


int main(void){
    eMBErrorCode eStatus;

	RCC_init();
    uptime_init();
    led_init();
    btn_init();

    if(modbus_init() != MB_ENOERR){
        ERROR("Can't init modbus");
        while(1);
    }else{
        DEBUG("Modbus init ok");
    }

    while(1){
        eStatus = eMBPoll();
        
        if(eStatus != MB_ENOERR){
            ERROR("Modbus poll error %d",eStatus);
        }
    }
}
