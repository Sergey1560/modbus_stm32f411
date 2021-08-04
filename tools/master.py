#import logging
#logging.basicConfig()
#log = logging.getLogger()
#log.setLevel(logging.DEBUG)

import time
import pymodbus
from pymodbus.pdu import ModbusRequest
from pymodbus.client.sync import ModbusSerialClient as ModbusClient
from pymodbus.transaction import ModbusRtuFramer

import sys
import termios
import atexit
from select import select

port = '/dev/ttyUSB0'
baudrate = 38400


class KBHit:

    def __init__(self):
        '''Creates a KBHit object that you can call to do various keyboard things.
        '''
        # Save the terminal settings
        self.fd = sys.stdin.fileno()
        self.new_term = termios.tcgetattr(self.fd)
        self.old_term = termios.tcgetattr(self.fd)

        # New terminal setting unbuffered
        self.new_term[3] = (self.new_term[3] & ~termios.ICANON & ~termios.ECHO)
        termios.tcsetattr(self.fd, termios.TCSAFLUSH, self.new_term)

        # Support normal-terminal reset at exit
        atexit.register(self.set_normal_term)


    def set_normal_term(self):
        ''' Resets to normal terminal.  On Windows this is a no-op.
        '''
        termios.tcsetattr(self.fd, termios.TCSAFLUSH, self.old_term)


    def getch(self):
        ''' Returns a keyboard character after kbhit() has been called.
            Should not be called in the same program as getarrow().
        '''

        s = ''
        return sys.stdin.read(1)


    def kbhit(self):
        ''' Returns True if keyboard character was hit, False otherwise.
        '''
        dr,dw,de = select([sys.stdin], [], [], 0)
        return dr != []


client = ModbusClient(
  method = 'rtu'
  ,port=port
  ,baudrate=baudrate
  ,parity='N'
  ,timeout=1
  )

kb = KBHit()

connection = client.connect()

line=''

print("Press l to toggle led, ESC or Ctrl+C to exit")

try:
    while True:

        registers  = client.read_input_registers(999,2,unit=10)# start_address, count, slave_id
        discrete  = client.read_discrete_inputs(1999,2,unit=10)

        if kb.kbhit():
            c = kb.getch()
            
            if ord(c) == 27: # ESC
                print("\n")
                break
            
            if ord(c) == 108: # l
                if discrete.bits[0]:
                    value = 0
                else:
                    value = 1
                write  = client.write_coil(2999,value,unit=10)# address = 1, value to set = 425, slave ID = 1



        line = "Uptime :"+str(registers.registers[0])+" sec "
        line = line + "CPU :" + str(registers.registers[1]) + "Mhz "

        if discrete.bits[0]:
            line += "LED : ON "
        else:
            line += "LED : OFF "

        if discrete.bits[1]:
            line += "BTN : Released "
        else:
            line += "BTN : Pressed "

        print(line, end="\r", flush=True)
        time.sleep(1) 

except KeyboardInterrupt:
    print('\n')

