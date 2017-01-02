/*
 * camera.h
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#ifndef CAMERA_H_
#define CAMERA_H_

/**
 * Macros for getting status of finalizing taking photo
 * process.
 *
 * return 1 - success taking photo process finalizing
 * 		  0 - there are error
 */
# define GET_EXIT_STATUS(status) WIFEXITED(status)

/**
 * Signature of callback function that called after taking photo contains:
 * char* - fileName
 * int   - exit status of taking photo process
 */
typedef void (*take_photo_callback)(char*, int);

int camera_init();

void camera_takePhoto(char* photo_name, take_photo_callback tpc);


#endif /* CAMERA_H_ */
