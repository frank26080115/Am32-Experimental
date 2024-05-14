///*
// * crsf.c
// *
// *  Created on: Jun 27, 2023
// *      Author: alkaz
// */
// crc code from https://github.com/stepinside/Arduino-CRSF/tree/main
//
#include "crsf.h"
#include "targets.h"
#include "common.h"
#include "functions.h"
#include "signal.h"

uint8_t crsf_buffer[100] ={0};
uint16_t crsf_channels[16] = {0};
uint32_t invalid_crc = 0;

uint8_t crsf_crc8(const uint8_t *ptr, uint8_t len){
static const uint8_t crsf_crc8tab[256] = {
    0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
    0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
    0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
    0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
    0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
    0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
    0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
    0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
    0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
    0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
    0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
    0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
    0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
    0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
    0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
    0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9};

uint8_t crc = 0;
for (uint8_t i = 0; i < len; i++)
{
    crc = crsf_crc8tab[crc ^ *ptr++];
}
return crc;
}

void setChannels(){
	uint8_t crc = crsf_crc8(&crsf_buffer[2], crsf_buffer[1] - 1);

	if(crc == crsf_buffer[25]){

    if (crsf_buffer[1] == 24)
    {
        crsf_channels[0] = ((crsf_buffer[3] | crsf_buffer[4] << 8) & 0x07FF);
        crsf_channels[1] = ((crsf_buffer[4] >> 3 | crsf_buffer[5] << 5) & 0x07FF);
        crsf_channels[2] = ((crsf_buffer[5] >> 6 | crsf_buffer[6] << 2 | crsf_buffer[7] << 10) & 0x07FF);
        crsf_channels[3] = ((crsf_buffer[7] >> 1 | crsf_buffer[8] << 7) & 0x07FF);
        crsf_channels[4] = ((crsf_buffer[8] >> 4 | crsf_buffer[9] << 4) & 0x07FF);
        crsf_channels[5] = ((crsf_buffer[9] >> 7 | crsf_buffer[10] << 1 | crsf_buffer[11] << 9) & 0x07FF);
        crsf_channels[6] = ((crsf_buffer[11] >> 2 | crsf_buffer[12] << 6) & 0x07FF);
        crsf_channels[7] = ((crsf_buffer[12] >> 5 | crsf_buffer[13] << 3) & 0x07FF);
        crsf_channels[8] = ((crsf_buffer[14] | crsf_buffer[15] << 8) & 0x07FF);
        crsf_channels[9] = ((crsf_buffer[15] >> 3 | crsf_buffer[16] << 5) & 0x07FF);
        crsf_channels[10] = ((crsf_buffer[16] >> 6 | crsf_buffer[17] << 2 | crsf_buffer[18] << 10) & 0x07FF);
        crsf_channels[11] = ((crsf_buffer[18] >> 1 | crsf_buffer[19] << 7) & 0x07FF);
        crsf_channels[12] = ((crsf_buffer[19] >> 4 | crsf_buffer[20] << 4) & 0x07FF);
        crsf_channels[13] = ((crsf_buffer[20] >> 7 | crsf_buffer[21] << 1 | crsf_buffer[22] << 9) & 0x07FF);
        crsf_channels[14] = ((crsf_buffer[22] >> 2 | crsf_buffer[23] << 6) & 0x07FF);
        crsf_channels[15] = ((crsf_buffer[23] >> 5 | crsf_buffer[24] << 3) & 0x07FF);
    }
//
//    if(crsf_channels[4] >= 1000){
//    	RC_CAR_REVERSE = 1;
//    	comp_pwm = 0;
//    	use_sin_start = 0;
//    	brake_on_stop = 0;
//    }else{
//    	RC_CAR_REVERSE = 0;
//    	comp_pwm = 1;
//    	use_sin_start = 1;
//    	brake_on_stop = 1;
//    }
if(bi_direction){
    newinput = map(crsf_channels[crsf_input_channel], 174, 1810, 0, 2000);
    crsf_second_input = map(crsf_channels[crsf_second_input_channel], 174, 1810, 0, 2000);
}else{
    if(crsf_channels[crsf_input_channel]<= 174){
      newinput = 0;
    }else{
      newinput = map(crsf_channels[crsf_input_channel], 174, 1810, 48, 2047);
    }
    crsf_second_input = map(crsf_channels[crsf_second_input_channel], 174, 1810, 48, 2047);
}
    signaltimeout = 0;
    if((adjusted_input == 0) && !armed){
    	zero_input_count++;
    }else{
    	zero_input_count = 0;
    }
    input_ready = 1;
		signaltimeout = 0;
//    IC_TIMER_REGISTER->CCR1 = map(crsf_channels[crsf_output_PWM_channel], 172, 1810, 1000, 2000);

	}else{
		invalid_crc++;
	}
}


