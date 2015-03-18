<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [System for Temperature and Evapotranspiration Monitoring (STEM)](#system-for-temperature-and-evapotranspiration-monitoring-stem)
  - [ABSTRACT](#abstract)
  - [Features/Components](#featurescomponents)
    - [PSoC4**](#psoc4)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

#System for Temperature and Evapotranspiration Monitoring (STEM)

##ABSTRACT
Monitoring plant health and conserving water are two important considerations in regards to agriculture, especially given the current drought. Plants take in water through rainfall and regular watering and lose water through transpiration. If growers can quantify the amount of water that is available to the plant and how much the plant loses through transpiration, they can irrigate more efficiently. STEM accomplishes both of these goals by monitoring soil moisture and temperature at different depths in the ground around a plant.  STEM also observes the leaf temperature of the plant, which correlates to whether or not the stomata are opened or closed and thus whether or not water is evaporating from the leaves.

The board in the field sends the captured data wirelessly to a coordinator which then uploads the data to a remote database.  From this point, growers can view the data using our web application on a computer or tablet.

##Features/Components
###PSoC4
32 bit ARM Cortex M0 based programmable system on chip a.k.a. the brains of each node
* **Moisture Sensor** - Non-capacitive soil moisture sensor will give a moisture profile at 4 different soil depths per node
* **Temperature Sensor** - Will give temperature profile of soil with at least 2 different depths
* **XBee Radio** - Wireless communication system to relay data to ConnectPort
* **Beaglebone Black** - The bridge between collected data and cloud storage/web application
* **Infrared Sensor** - Gives IR profile of transpiring leaves to infer water/sugar content through rate of change in temperature
* **Analytics Framework** - TBD



