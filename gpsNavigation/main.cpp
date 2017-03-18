/*
 * Copyright © 2014 Kosma Moczek <kosma@cloudyourcar.com>
 * This program is free software. It comes without any warranty, to the extent
 * permitted by applicable law. You can redistribute it and/or modify it under
 * the terms of the Do What The Fuck You Want To Public License, Version 2, as
 * published by Sam Hocevar. See the COPYING file for more details.
 */

#include <ctype.h>

#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <sys/types.h>
#include <sys/stat.h>

#include "minmea.h"
#include "shm/redis.h"
#include "structs.h"

#define INDENT_SPACES "  "

int configurePort(int fd) {
	struct termios options; /*структура для установки порта*/
	tcgetattr(fd, &options); /*читает пораметры порта*/

	cfsetispeed(&options, B9600); /*установка скорости порта*/
	cfsetospeed(&options, B9600); /*установка скорости порта*/

	tcsetattr(fd, TCSANOW, &options); /*сохронения параметров порта*/

	return 1;
}

int readNP(int fd, char* buf) {
	buf[0] = 0;
	do {
		if ( read(fd, buf, 1) < 1) return 0;
	} while (*buf != '$');

	do {
		buf++;
		if ( read(fd, buf, 1) < 1 ) return 0;
		if (*buf == '\n') {
			break;
		}
	} while (1);

	buf[1] = '\0';

	return 1;
}

int main(int argc, char* argv[]) {
	int fd; /* Файловый дескриптор для порта */
	char* devName;

	if (argc > 1) {
		devName = argv[1];
	} else {
		devName = "/dev/ttyUSB0";
	}

	fd = open(devName, O_RDWR | O_NOCTTY); /*'open_port()' - Открывает последовательный порт */
	if (fd == -1) {
		printf("error port\n");
		perror("open_port: Unable to open /dev/ttyUSBn - ");
		return 1;
	} else {
		configurePort(fd);
		printf("Port configured\n");
	}

	redisDataService* dserv = new redisDataService();
	dserv->connect();

    char line[MINMEA_MAX_LENGTH];
    while (readNP(fd, line) != 0) {
        //printf("%s", line);
        switch (minmea_sentence_id(line, false)) {
            case MINMEA_SENTENCE_RMC: {
                struct minmea_sentence_rmc frame;
                if (minmea_parse_rmc(&frame, line)) {
                	printf("Parsed: %s", line);
                	tel_GPSNavigation nav;

                	nav.isValid = frame.valid;
                	nav.latitude = minmea_tofloat(&frame.latitude);
                	nav.longitude = minmea_tofloat(&frame.longitude);
                	nav.speed = minmea_tofloat(&frame.speed);

                	dserv->add(TYPE_GPS_NAV, (char*)&nav, sizeof(nav));
                }
                else {
                    printf(INDENT_SPACES "$xxRMC sentence is not parsed\n");
                }
            } break;

            case MINMEA_SENTENCE_GGA: {
                struct minmea_sentence_gga frame;
                if (minmea_parse_gga(&frame, line)) {
                	//TODO
                }
                else {
                    printf(INDENT_SPACES "$xxGGA sentence is not parsed\n");
                }
            } break;

            case MINMEA_SENTENCE_GST: {
                struct minmea_sentence_gst frame;
                if (minmea_parse_gst(&frame, line)) {
                	//TODO
                }
                else {
                    printf(INDENT_SPACES "$xxGST sentence is not parsed\n");
                }
            } break;

            case MINMEA_SENTENCE_GSV: {
                struct minmea_sentence_gsv frame;
                if (minmea_parse_gsv(&frame, line)) {
                	//TODO
                }
                else {
                    printf(INDENT_SPACES "$xxGSV sentence is not parsed\n");
                }
            } break;

            case MINMEA_SENTENCE_VTG: {
               struct minmea_sentence_vtg frame;
               if (minmea_parse_vtg(&frame, line)) {
            	   //TODO
               }
               else {
                    printf(INDENT_SPACES "$xxVTG sentence is not parsed\n");
               }
            } break;

            case MINMEA_INVALID: {
                printf(INDENT_SPACES "$xxxxx sentence is not valid\n");
            } break;

            default: {
                printf(INDENT_SPACES "$xxxxx sentence is not parsed\n");
            } break;
        }
        usleep(10 * 1000);
    }

    return 0;
}

/* vim: set ts=4 sw=4 et: */
