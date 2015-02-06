#! /usr/bin/python

import Adafruit_BBIO.UART as UART
import serial
from firebase.firebase import FirebaseApplication, FirebaseAuthentication
from userConfig import config
from xbee import XBee
from datetime import datetime

### Setup and enable UART ports
#  UART1 (/dev/ttyO1): Rx -> p9_26, Tx -> P9_24
#  UART2 (/dev/ttyO2): Tx -> p9_21, Rx -> p9_22
UART.setup("UART1")

###  Initialize serial port for UART
s = serial.Serial('/dev/ttyO1', 9600)

###  Initialize XBee object
xbee = XBee(s)

### Initialize firebase client with user's config file
cf = config()
fbAuth = FirebaseAuthentication(
	cf.firebaseSecret,
	cf.firebaseEmail,
	True,
	True
	)

fb = FirebaseApplication(cf.firebaseURL, fbAuth)

###  Main code


while True:
	try: 
		packet = xbee.wait_read_frame()
		print packet
		#if packet['id'] == 'rx_io_data':


	except KeyboardInterrupt:
		break;
s.close()

# if __name__ = '__main__':
# 	main()