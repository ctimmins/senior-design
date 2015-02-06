#!/usr/bin/python
from xbee.helpers import initUart, parseXbeeMsg
from xbee.xbee import XBee
from time import sleep

# This is a test

if __name__ == '__main__':

	### Initialize UART2
	#      Rx->P9_22
	#      Tx->P9_21 
	initUart("UART2")

	xbee = XBee('/dev/ttyO2')

	while True:
		try:
			while xbee.serial.inWaiting() == 0:
				sleep(0.25)
			msg = xbee.Receive()
			data = parseXbeeMsg(msg)

			print "src: %s" % data["SrcAddr"]
			print "message length: %s" % data["MsgLen"] 
			print "Chksm: %s" % data["CheckSum"]
			print "Message: %s" % data["data"] 
			
			xbee.SendStr(str(data), src)
		except KeyboardInterrupt:
			break

	xbee.serial.close()