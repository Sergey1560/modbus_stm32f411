# Modbus RTU example for STM32F411

This is an example of working with the Modbus RTU protocol using STM32F411 dev board from [https://github.com/WeActTC/MiniSTM32F4x1](WeAct Studio)

The library [FreeMODBUS](https://www.embedded-experts.at/en/freemodbus/about/) v1.6 is used to work with Modbus protcol.

## Peripherals used

* USART2 - for serial
* TIM2 - for modbus 50us timer
* TIM3 - for 1s, "uptime" timer

All hardware library settings are located in the folder Lib/modbus/port

## Example description

* device address - 0x0A
* 2 16-bit inputs registers at address 1000: reg[0] for uptime in seconds, reg[1] for system core clock in Mhz
* 2 discrete inputs at address 2000: 0 - led state (PC13), 1 - button state (PA0)
* 1 Coil for change led state at address 3000

There is a [python example](tools/master.py) for testing.

You can also check using mbpoll:

Get 2 input registers from 1000:

```
mbpoll /dev/ttyUSB0 -b 38400 -P none -r 1000 -c 2 -a 0x0A -t 3
```

Answer:

```
mbpoll /dev/ttyUSB0 -r 1000 -c 2 -b 38400 -P none -a 0x0A -t 3
mbpoll 1.0-0 - FieldTalk(tm) Modbus(R) Master Simulator
Copyright © 2015-2019 Pascal JEAN, https://github.com/epsilonrt/mbpoll
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; type 'mbpoll -w' for details.

Protocol configuration: Modbus RTU
Slave configuration...: address = [10]
                        start reference = 1000, count = 2
Communication.........: /dev/ttyUSB0,      38400-8N1 
                        t/o 1.00 s, poll rate 1000 ms
Data type.............: 16-bit register, input register table

-- Polling slave 10... Ctrl-C to stop)
[1000]: 	25
[1001]: 	100

```

Get 2 discrete input from address 2000:

```
mbpoll /dev/ttyUSB0 -b 38400 -P none -r 2000 -c 2 -a 0x0A -t 1
```

Enable led:

```
mbpoll /dev/ttyUSB0 -b 38400 -P none -r 3000 -a 0x0A -t 0 1
```

Disable led:

```
mbpoll /dev/ttyUSB0 -b 38400 -P none -r 3000 -a 0x0A -t 0 0
```
