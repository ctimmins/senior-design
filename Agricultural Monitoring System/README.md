<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [System for Temperature and Evapotranspiration Monitoring (STEM)](#system-for-temperature-and-evapotranspiration-monitoring-stem)
  - [Abstract](#abstract)
  - [Marketing Presentation](#marketing-presentation)
  - [Features/Components](#featurescomponents)
      - [PSoC5](#psoc5)
      - [Soil Sensors](#soil-sensors)
      - [Infrared Sensor](#infrared-sensor)
      - [XBee Wireless Radio](#xbee-wireless-radio)
      - [Battery System](#battery-system)
      - [Low Power Consumption](#low-power-consumption)
      - [Beaglebone Black](#beaglebone-black)
      - [Data Store](#data-store)
      - [Web Application and Synchronized Data](#web-application-and-synchronized-data)
      - [Analytics Framework](#analytics-framework)
  - [Future Improvements](#future-improvements)
  - [User Manual](#user-manual)
      - [STEM](#stem)
      - [Beaglebone](#beaglebone)
      - [Firebase](#firebase)
      - [Web Application](#web-application)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

#System for Temperature and Evapotranspiration Monitoring (STEM)

##Abstract
Monitoring plant health and conserving water are two important considerations in regards to agriculture, especially given the current drought. Plants take in water through rainfall and regular watering and lose water through transpiration. If growers can quantify the amount of water that is available to the plant and how much the plant loses through transpiration, they can irrigate more efficiently. STEM accomplishes both of these goals by monitoring soil moisture and temperature at different depths in the ground around a plant.  STEM also observes the leaf temperature of the plant, which correlates to whether or not the stomata are opened or closed and thus whether or not water is evaporating from the leaves.

The board in the field sends the captured data wirelessly to a coordinator which then uploads the data to a remote database.  From this point, growers can view the data using our web application on a computer or tablet.

##Marketing Presentation
<iframe src="https://docs.google.com/presentation/d/1wbcnrjsn4lwjHY33hAu_ieI2Ulm3Yo2k4nWNngOg5mc/embed?start=false&loop=false&delayms=3000" frameborder="0" width="960" height="569" allowfullscreen="true" mozallowfullscreen="true" webkitallowfullscreen="true"></iframe>

##Features/Components
####PSoC5
32 bit ARM Cortex M0 based programmable system on chip a.k.a. the brains of each node.
####Soil Sensors
The multi-level installation of subterraneous sensors gives the user a soil moisture and temperature profile and is useful for monitoring the progression of the water-front.  In other words, our system is able to track how water flows through soil and the conditions that cause it do so.  The soil moisture sensors use transmission line techniques to acquire volumetric water content and are immune to inaccurate measurements due to varying levels of salinity.
####Infrared Sensor
The mounted infrared camera gives the user the ability to detect changes in leaf temperature as a function of time. This is used to determine when the stomates in the leaf closes in the day, which marks the point where the plant stops losing water.
####XBee Wireless Radio
XBee radios are low energy, low bandwidth radios that have a range of around 300 feet
####Battery System
The device is powered by a 2500 mAh Li-po battery. The battery can be recharged through an external solar panel or a USB phone charger. A analog to digital converter is placed to measure the health of the battery, thereby alerting the user that the battery is low.
####Low Power Consumption
Sleep timers were implemented to reduce overall power consumption. The board itself wakes up roughly every 4 seconds, but only takes and transmits measurements periodically. The frequency of message transmittance is user defined and can easily be adjusted depending on the application.
####Beaglebone Black
We used TI’s Beaglebone Black as the XBee coordinator and internet gateway. An XBee radio was mounted to the Beaglebone and Python scripts were developed to handle all communication to and from the wireless sensors. It also maintains information about multiple nodes such as battery status, and various IR configuration settings. Additional scripts were developed for uploading our data to a secure database (Firebase).
The decision to go with a linux micro computer over Digi’s ConnectPort and proprietary Device Cloud was due to the difficulty of integrating a custom user interface with Digi’s services.
####Data Store
We used Firebase to collect and manage all data.  Firebase was chosen for its simplicity in uploading and retrieving data.  It also provides an easy to use dashboard to manage, view, and download all data.
####Web Application and Synchronized Data
Our web application, along with Firebase as a backend, serves as a simple alternative to Etherios Device Cloud.  Firebase and the relevant javascript API’s allow for real time data synchronization across devices.  This means the user can see live updates of sensor data without having to refresh the page. 
####Analytics Framework
TBD

##Future Improvements

##User Manual
####STEM

####Beaglebone

####Firebase

####Web Application
