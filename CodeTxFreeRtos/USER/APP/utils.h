#ifndef _UTILS_H
#define _UTILS_H

#include "main.h"


/** 
  * @brief  uchar��float���͵�ת�� 
  */
typedef union {
	uint8_t uchar_data[4];
	float   float_data;
}uchar_and_float_t;

#endif

