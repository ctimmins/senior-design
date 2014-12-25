#Agricultural Monitoring System

###GOAL
To quantify the microclimates within an agricultural setting. With a network of sensors, various data points from different locations can be collected and then analyzed. This system should be user friendly and provide information about the water content and temperature of grape leaves from the IR sensor and soil moisture and soil temperature from the soil sensor. The data will be stored and accessible from a web application.

###TASKS
* Build breakout board for alpha, beta, and final implementation
* PCB layout for alpha, beta, and final
* Sensor drivers for PSoC data logger 
* System integration
* Field test with UC Davis Viticulture Department
* Software implementation and analytics framework
* Network analysis and testing
* Remote server integration
* Autocad rendering for beta and final builds
* Marketing video?

###MAIN COMPONENTS
* PSoC4 - 32 bit ARM Cortex M0 based programmable system on chip a.k.a. the brains of each node
* Moisture Sensor - Non-capacitive soil moisture sensor will give a moisture profile at 6 different soil depths per node
* XBee Radio - Wireless communication system to relay data to ConnectPort
* ConnectPort - The bridge between collected data and cloud storage/web application
* Analytics Framework - TBD



