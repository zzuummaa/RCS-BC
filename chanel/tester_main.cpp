/*
 * tester_main.cpp
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: Cntgfy
 */

#include <unistd.h>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include <raspicam/raspicam.h>
#include "ppm2bmp/ppm2bmp.h"

using namespace std;

const char PPM_TITLE_FORMAT[] = "P6\n%d %d\n%d\n";

/*int main(int argc, char *argv[]) {
	// make sure args count ok
	if (argc < 3) {
		printf("Usage example: ppm2bmp.exe in.ppm out.bmp\n");
		return errParamError;
	}

	FILE *fIn = fopen(argv[1], "r");
	if (fIn == NULL) {
		printf("Can't open file '%s'\n", argv[1]);
		return errOpenBmpError;
	}

	int buffSize = 0xffffff;
	BYTE *buff = (BYTE*)malloc(buffSize);
	if (1 > fread(buff, 1, buffSize, fIn)){
		return errOpenBmpError;
	};
	fclose(fIn);

	int bmpSize;
	BYTE *bmp = convert(buff, &bmpSize);

	FILE *fOut = fopen(argv[2], "w");
	if (fOut == NULL) {
		return errWriteBmpError;
	}

	if (bmpSize != fwrite(bmp, 1, bmpSize, fOut)) {
		return errWriteBmpError;
	}

	fclose(fOut);

	return 0;
}*/

int main ( int argc,char **argv ) {

    /*raspicam::RaspiCam Camera; //Cmaera object

    //Open camera

    cout<<"Opening Camera..."<<endl;

    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}

    //wait a while until camera stabilizes

    cout<<"Sleeping for 3 secs"<<endl;

    sleep(3);

    //capture

    Camera.grab();

    //allocate memory

    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];

    //extract the image in rgb format

    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//get camera image

    //save

    std::ofstream outFile ( "raspicam_image.ppm",std::ios::binary );

    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";

    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );

    cout<<"Image saved at raspicam_image.ppm"<<endl;

    //free resrources

    delete data;*/

    return 0;

}
