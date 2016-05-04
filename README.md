![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)
#TextToSpeech click#
##By [MikroElektronika](http://www.mikroe.com)

![TextToSpeech_click](http://cdn.mikroe.com/img/click/text-to-speech/texttospeech-click.png)

---

[Product Page]( http://www.mikroe.com/click/text-to-speech/ )

[Manual Page](http://docs.mikroe.com/Text_To_Speech_click)

[Learn Page]( http://learn.mikroe.com/make-robot-speak-text-speech/ )

### Description
Text To Speech click is a mikroBUS add-on board that carries an Epson S1V30120 speech synthesis IC. 
The IC is powered by the Fonix DECtalk® v5 speech synthesis engine that can make your robot or 
portable device talk in US English, Castilian Spanish or Latin American Spanish, 
in one of nine pre-defined voices. The DECtalk engine includes a parser that gives users fine control 
over the quality, pitch and intonation of the synthesized speech. The audio is reproduced at 11.025kHz 
sampling rate. Text To Speech click board communicates with the target board microcontroller through the mikroBUS 
SPI interface (MISO, MOSI, SCK, CS) with additional functionality provided by DRDY, NRST and MUTE pins 
(in place of default mikroBUS INT, RST and AN pins) It can use either a 3.3V or 5V power supply.

___________________________________________________________________________________________________

### Example

```
#include "text_to_speech.h"
#include "text_to_speech_img.h"

sbit TTS_RST at GPIOC_ODR.B2;
sbit TTS_CS at GPIOD_ODR.B13;
sbit TTS_MUTE at GPIOA_ODR.B4;
sbit TTS_RDY at GPIOD_IDR.B10;

void msg_blk( uint16_t *req, uint16_t *err );
void fatal_err( uint16_t *err );
void system_init( void );

void msg_blk( uint16_t *req, uint16_t *err )
{

}

void fatal_err( uint16_t *err )
{
}

void system_init()
{
    GPIO_Digital_Output( &GPIOC_ODR, _GPIO_PINMASK_2 );
    GPIO_Digital_Output( &GPIOD_ODR, _GPIO_PINMASK_13 );
    GPIO_Digital_Output( &GPIOA_ODR, _GPIO_PINMASK_4 );
    GPIO_Digital_Input( &GPIOD_IDR, _GPIO_PINMASK_10 );
    Delay_ms( 200 );
    
    SPI3_Init_Advanced( _SPI_FPCLK_DIV128, _SPI_MASTER | _SPI_8_BIT |
                        _SPI_CLK_IDLE_HIGH | _SPI_SECOND_CLK_EDGE_TRANSITION |
                        _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | 
                        _SPI_SSI_1, &_GPIO_MODULE_SPI3_PC10_11_12 );

    Delay_ms( 200 );
}

void main() 
{
    system_init();
    tts_init();
    tts_msg_block_callback( msg_blk );
    tts_fatal_err_callback( fatal_err );

}
```
