import Adafruit_BBIO.UART as UART
import serial
from xbee import XBee

UART.setup("UART2")
ser = serial.Serial(port = "/dev/ttyO2", baudrate=9600)
ser.open()
#xbee = XBee(ser)

while True:
	try:
		while ser.inWaiting() > 0:
			rec = hex(ord(ser.read(1)))
			if rec == '0x7e':
				print ("Start bit recieved")
			#print ("bytes in buffer: " + str(ser.inWaiting()))
		
	except KeyboardInterrupt:
		break
ser.close()
