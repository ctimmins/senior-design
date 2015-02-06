"""
parse.py

By Chad Timmins, 2015
chadtimmins@gmail.com

Provides parsing utility functions which parses received XBee API messages
and returns a formatted data structure that can be used to retrieve
data and configuration options.

Received Messages Format: 
	[0x00][LSB][SrcAddrHigh][SrcAddrLow][junk][0x00][data][...][CheckSum]

"""

def parseXbeeMsg(msg):
	# get 16-bit source address
	src = (msg[3] << 8) | msg[4]
	mlen = msg[1]
	cksm = msg[-1]
	data = msg[6:-1]

	pkg = {
		"SrcAddr": src,
		"MsgLen": mlen,
		"CheckSum": cksm,
		"Data": data
	}

	return pkg


