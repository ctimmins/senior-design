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
int ReadAll(int num_sensors, char **buf) {

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
int ReadOne(int addr, char *buf) {

}

/**
 * Calculates the Check Sum
 *
 * @param res  response string to calculate CRC value
 * @return     ASCII Character representing CRC value
 */
char CalculateCheckSum(char *res) {

	int length, sum = 0;

	//find length of response
	length = strlen(res);

	//Sum all character's decimal value
	for (int i = 0; i < length; ++i) {
		sum += res[i];
	}

	return sum % 64 + 32;
}