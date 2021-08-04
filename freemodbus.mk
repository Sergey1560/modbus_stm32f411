C_SOURCES +=  \
Lib/modbus/functions/mbfuncdiag.c \
Lib/modbus/functions/mbfuncother.c \
Lib/modbus/functions/mbutils.c \
Lib/modbus/functions/mbfuncholding.c \
Lib/modbus/functions/mbfunccoils.c \
Lib/modbus/functions/mbfuncdisc.c \
Lib/modbus/functions/mbfuncinput.c \
Lib/modbus/rtu/mbcrc.c \
Lib/modbus/rtu/mbrtu.c \
Lib/modbus/mb.c \
Lib/modbus/port/portevent.c \
Lib/modbus/port/porttimer.c \
Lib/modbus/port/portserial.c


C_INCLUDES +=  \
-ILib/modbus/functions \
-ILib/modbus/include \
-ILib/modbus/port \
-ILib/modbus/rtu
