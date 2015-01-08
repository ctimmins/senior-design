#Data Logger for Moisture Sensors

##SDI-12 Communication
###Overview
SDI-12 is a standards-based protocol for interfacing sensors to data loggers and data acquisition equipment. Multiple sensors with unique addresses can share a common 3-wire bus (power, ground, and data). Two-way communication between the sensor and logger are possible by sharing the data line for transmit and receive as defined by the standard. Sensor measurements are triggered by protocol command.
###Advantages
* Unique and complex self calibration algorithms can be done in microprocessor- based sensors.
* Sensors can be interchanged without reprogramming the data recorder with calibration or other information.
* Power is supplied to sensors through the interface.

##DDI-Serial Communication
###Overview
The DDI-Serial communications protocol is ideal for systems that have dedicated serial signaling lines for each sensor or use a multiplexer to handle multiple sensors. The serial communications are compatible with many TTL serial implementations that support active-high logic levels using 0-3.6 volts signal levels. When the sensor is first powered, it automatically makes measurements of the integrated transducers then outputs a response over the data line. Systems using this protocol control the sensor excitation to initiate data transfers from the sensor.
###Advantages
* Very easy to interface PSoC to sensor

##Generalized Schematic of Data Logger System
![alt-tag](https://raw.githubusercontent.com/ctimmins/senior-design/master/Agricultural%20Monitoring%20System/Alpha%20Prototype/Data%20Logger/images/DDI_Schematic.png)