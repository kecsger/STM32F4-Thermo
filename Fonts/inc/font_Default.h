/*
 * ili9341_font.h
 *
 *  Created on: 2015 okt. 30
 *      Author: kecsger12
 */

#ifndef INC_FONT_DEFAULT_H_
#define INC_FONT_DEFAULT_H_


/**
 * @defgroup TM_FONTS
 * @brief    Fonts library for all my LCD libraries
 * @{
 *
 * Default fonts library. It is used in all LCD based libraries.
 *
 * \par Supported fonts
 *
 * Currently, these fonts are supported:
 *  - 7 x 10 pixels
 *  - 11 x 18 pixels
 *  - 16 x 26 pixels
 *
 * \par Changelog
 */

#include "stm32f4xx.h"
#include "string.h"

#include "font_def.h"


/**
 * @brief  7 x 10 pixels font size structure
 */
TM_FontDef_t TM_Font_7x10;

/**
 * @brief  11 x 18 pixels font size structure
 */
TM_FontDef_t TM_Font_11x18;

/**
 * @brief  16 x 26 pixels font size structure
 */
TM_FontDef_t TM_Font_16x26;

/**
 * @brief  Calculates string length and height in units of pixels depending on string and font used
 * @param  *str: String to be checked for length and height
 * @param  *SizeStruct: Pointer to empty @ref TM_FONTS_SIZE_t structure where informations will be saved
 * @param  *Font: Pointer to @ref TM_FontDef_t font used for calculations
 * @retval Pointer to string used for length and height
 */
char* TM_FONTS_GetStringSize(char* str, TM_FONTS_SIZE_t* SizeStruct, TM_FontDef_t* Font);


#endif /* INC_FONT_DEFAULT_H_ */
