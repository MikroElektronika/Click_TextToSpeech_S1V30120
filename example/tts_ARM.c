#include "text_to_speech.h"

sbit TTS_RST at GPIOC_ODR.B2;
sbit TTS_CS at GPIOD_ODR.B13;
sbit TTS_MUTE at GPIOA_ODR.B4;
sbit TTS_RDY at GPIOD_IDR.B10;

void system_init( void );
void fatal_err( uint16_t *err );
void msg_blk( uint16_t *req, uint16_t *err );

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
    
    // DEBUG UART
    UART1_Init_Advanced( 57600, _UART_8_BIT_DATA,
                                _UART_NOPARITY,
                                _UART_ONE_STOPBIT,
                                &_GPIO_MODULE_USART1_PA9_10 );
    Delay_ms( 200 );
    
    UART1_Write_Text( "System Init Done !\r\n" );
}

void msg_blk( uint16_t *req, uint16_t *err )
{
    char txt[ 6 ];
    
    UART1_Write_Text( " MSG BLOCKED \r\n" );
    sprinti( txt, "%x\r\n", *req );
    UART1_Write_Text( txt );
    sprinti( txt, "%x\r\n", *err );
    UART1_Write_Text( txt );
}

void fatal_err( uint16_t *err )
{
    UART1_Write_Text( "Fatal Error Detected" );
    tts_init();
    tts_fatal_err_callback( fatal_err );
}

void main() 
{
    system_init();
    tts_init();
    tts_setup();
    
    tts_msg_block_callback( msg_blk );
    tts_fatal_err_callback( fatal_err );
    
    tts_config( 0x01, false, TTSV_US, 0x0100 );
    
    tts_mute();
    tts_speak( "[:nh][:dv ap 90 pr 0]" );
    Delay_ms( 2000 );
    tts_unmute();
    
    tts_speak( "Make your wish my master" );
    tts_speak( "This is world of robots" );
}
