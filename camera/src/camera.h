/*
 * camera.h
 *
 *  Created on: 22.12.2016
 *      Author: zzuummaa
 */

#ifndef CAMERA_H_
#define CAMERA_H_

typedef void (*take_photo_callback)(char*, int);

int camera_init();

void camera_takePhoto(char* photo_name, take_photo_callback tpc);


#endif /* CAMERA_H_ */
