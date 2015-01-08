/** Utility and Helper functions
 *
 */

#include <project.h>

/** 
 *  Reads data from all sensors
 *  
 *  @param  num_sensor  number of sensors to read
 *  @param  buf         buffer to hold data
 *  @return             error code  
 */
int read_all(int num_sensors, char **buf) {

	for (int i = 0; i < num_sensors; ++i) {
		buf[i] = read_one(i, buf[i]);
	}
}

/**
 * Reads data from one sensor
 *
 * @param  addr  address of sensor to read (0-5)
 * @param  buf   bufer to hold data from sensor measurement
 * @return       status code  
 */

int read_one