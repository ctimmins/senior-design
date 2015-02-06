"""
setup.py 

By Chad Timmins, 2015
chadtimmins@gmail.com

Helps initialize and enable UART2 on Beaglebone Black
"""

import Adafruit_BBIO.UART as UART

def initUart(port="UART2"):
	UART.setup(port)
