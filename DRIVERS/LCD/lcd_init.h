#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"

#define USE_HORIZONTAL 2  //�1�7�1�7�1�7���1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�5 0�1�7�1�71�0�2�1�7�1�7�1�7�1�7 2�1�7�1�73�0�2�1�7�1�7�1�7�1�7

#ifndef SPI_SS_PIN
#define SPI_SS_PIN     9
#endif    
#ifndef SPI_MOSI_PIN
#define SPI_MOSI_PIN   10
#endif  
#ifndef SPI_SCK_PIN
#define SPI_SCK_PIN    11
#endif   
#ifndef SPI_RST_PIN
#define SPI_RST_PIN    12
#endif
#ifndef SPI_DC_PIN
#define SPI_DC_PIN     13
#endif
#ifndef SPI_BLK_PIN
#define SPI_BLK_PIN    14
#endif

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif



#define    LCD_CS_LOW       nrf_gpio_pin_clear(SPI_SS_PIN)  
#define    LCD_CS_HIGH      nrf_gpio_pin_set(SPI_SS_PIN)     
#define    LCD_RESET_LOW    nrf_gpio_pin_clear(SPI_RESET_PIN)   
#define    LCD_RESET_HIGH   nrf_gpio_pin_set(SPI_RESET_PIN)  
#define    LCD_DC_LOW       nrf_gpio_pin_clear(SPI_DC_PIN)   
#define    LCD_DC_HIGH      nrf_gpio_pin_set(SPI_DC_PIN)  
#define    LCD_BLK_LOW       nrf_gpio_pin_clear(SPI_BLK_PIN)   
#define    LCD_BLK_HIGH      nrf_gpio_pin_set(SPI_BLK_PIN)  
void lcd_spi_init(void);
void LCD_WR_DATA8(u8 dat);//8位数据
void LCD_WR_DATA(u16 dat);//16位数据
void LCD_WR_REG(u8 dat);//命令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




