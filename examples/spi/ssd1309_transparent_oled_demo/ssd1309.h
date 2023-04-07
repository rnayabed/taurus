/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1309
 */

#ifndef __SSD1309_H__
#define __SSD1309_H__

#include <stddef.h>
#include"stdlib.h"
#include"gpio.h"



#define GPIO_PIN_RESET  0
#define GPIO_PIN_SET    1

#define PI 3.14


#include "ssd1309_conf.h"
/*
#if defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#else
#error "SSD1309 library was tested only on STM32F0, STM32F1, STM32F3, STM32F4, STM32F7, STM32L0, STM32L1, STM32L4, STM32H7, STM32G0, STM32G4 MCU families. Please modify ssd1309.h if you know what you are doing. Also please send a pull request if it turns out the library works on other MCU's as well!"
#endif
*/
#ifdef SSD1309_X_OFFSET
#define SSD1309_X_OFFSET_LOWER (SSD1309_X_OFFSET & 0x0F)
#define SSD1309_X_OFFSET_UPPER ((SSD1309_X_OFFSET >> 4) & 0x07)
#else
#define SSD1309_X_OFFSET_LOWER 0
#define SSD1309_X_OFFSET_UPPER 0
#endif

#include "ssd1309_fonts.h"

/* vvv I2C config vvv 

#ifndef SSD1309_I2C_PORT
#define SSD1309_I2C_PORT        hi2c1
#endif

#ifndef SSD1309_I2C_ADDR
#define SSD1309_I2C_ADDR        (0x3C << 1)
#endif

 ^^^ I2C config ^^^ */

/* vvv SPI config vvv */

#ifndef SSD1309_SPI_PORT
#define SSD1309_SPI_PORT        hspi2
#endif


#ifndef SSD1309_CS_Pin
#define SSD1309_CS_Pin          GPIO_PIN_12
#endif

#ifndef SSD1309_DC_Pin
#define SSD1309_DC_Pin          GPIO_0
#endif

#ifndef SSD1309_Reset_Pin
#define SSD1309_Reset_Pin       GPIO_1
#endif

/* ^^^ SPI config ^^^ */

#if defined(SSD1309_USE_I2C)
extern I2C_HandleTypeDef SSD1309_I2C_PORT;
#elif defined(SSD1309_USE_SPI)
//extern SPI_HandleTypeDef SSD1309_SPI_PORT;
#else
#error "You should define SSD1309_USE_SPI or SSD1309_USE_I2C macro!"
#endif

// SSD1309 OLED height in pixels
#ifndef SSD1309_HEIGHT
#define SSD1309_HEIGHT          64
#endif

// SSD1309 width in pixels
#ifndef SSD1309_WIDTH
#define SSD1309_WIDTH           128
#endif

#ifndef SSD1309_BUFFER_SIZE
#define SSD1309_BUFFER_SIZE   (( SSD1309_WIDTH * SSD1309_HEIGHT )/ 8)
#endif

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1309_COLOR;

typedef enum {
    SSD1309_OK = 0x00,
    SSD1309_ERR = 0x01  // Generic error.
} SSD1309_Error_t;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} SSD1309_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} SSD1309_VERTEX;

// Procedure definitions
void ssd1309_Init(void);
void ssd1309_Fill(SSD1309_COLOR color);
void ssd1309_UpdateScreen(void);
void ssd1309_DrawPixel(uint8_t x, uint8_t y, SSD1309_COLOR color);
char ssd1309_WriteChar(char ch, FontDef Font, SSD1309_COLOR color);
char ssd1309_WriteString(char* str, FontDef Font, SSD1309_COLOR color);
void ssd1309_SetCursor(uint8_t x, uint8_t y);
void ssd1309_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1309_COLOR color);
void ssd1309_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1309_COLOR color);
void ssd1309_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1309_COLOR color);
void ssd1309_Polyline(const SSD1309_VERTEX *par_vertex, uint16_t par_size, SSD1309_COLOR color);
void ssd1309_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1309_COLOR color);
void ssd1309_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1309_COLOR color);
/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void ssd1309_SetContrast(const uint8_t value);
/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void ssd1309_SetDisplayOn(const uint8_t on);
/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t ssd1309_GetDisplayOn();

// Low-level procedures
void ssd1309_Reset(void);
void ssd1309_WriteCommand(uint8_t byte);
void ssd1309_WriteData(uint8_t* buffer, size_t buff_size);
SSD1309_Error_t ssd1309_FillBuffer(uint8_t* buf, uint32_t len);



#endif // __SSD1309_H__
