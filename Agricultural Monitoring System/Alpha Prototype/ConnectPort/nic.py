
# http://www.digi.com/wiki/developer/index.php/XBee_Extensions_to_the_Python_socket_API

# XBee communication imports
import xbee
import socket
from socket import *

# Create a socket through which the ConnectPort communicates with XBee
sd = socket(AF_XBEE, SOCK_DGRAM, XBS_PROT_TRANSPORT);
sd.bind(("",0x00,0,0));

# Store local time in var
import time
var = time.localtime(time.time());

# Store length # of bytes in buffer_in and the XBee source address in src_addr
# NOTE: if there are more bytes on the receive buffer than length, extra bytes will be discarded
# NOTE: socket is blocking by default
buffer_in, src_addr = sd.recvfrom(255);
# To make the socket non-blocking:
sd.setblocking(0);
# To return to blocking:
sd.setblocking(1);

# Send character 'g' to XBee with address src_addr
sd.sendto('g',0,'00:13:a2:00:40:c2:5c:7b!');



