#include "text_to_speech.h"

sbit TTS_RST at GPIO_PIN1_bit;
sbit TTS_CS at GPIO_PIN28_bit;
sbit TTS_MUTE at GPIO_PIN9_bit;
sbit TTS_RDY at GPIO_PIN3_bit;

void system_init( void );
void fatal_err( uint16_t *err );
void msg_blk( uint16_t *req, uint16_t *err );

void system_init()
{
    GPIO_Digital_Output( &GPIO_PORT_00_07, _GPIO_PINMASK_1 );
    GPIO_Digital_Output( &GPIO_PORT_24_31, _GPIO_PINMASK_4 );
    GPIO_Digital_Output( &GPIO_PORT_08_15, _GPIO_PINMASK_1 );
    GPIO_Digital_Input( &GPIO_PORT_00_07, _GPIO_PINMASK_3 );
    
    
    Delay_ms( 200 );

    SPIM1_Init_Advanced( _SPI_MASTER_CLK_RATIO_128, 
                         _SPI_CFG_PHASE_CAPTURE_RISING |
                         _SPI_CFG_POLARITY_IDLE_HIGH |
                         _SPI_CFG_SS_AUTO_DISABLE |
                         _SPI_CFG_FIFO_DISABLE,
                         _SPI_SS_LINE_NONE );
    Delay_ms( 200 );

    // DEBUG UART
    UART1_Init( 115200 );
    Delay_ms( 200 );

    UART1_Write_Text( "System Init Done !\r\n" );
}

void msg_blk( uint16_t *req, uint16_t *err )
{
    char txt[ 6 ];

    UART1_Write_Text( " MSG BLOCK \r\n" );
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
    tts_msg_block_callback( msg_blk );
    tts_fatal_err_callback( fatal_err );
    tts_setup();

    tts_speak( "I am your robot" );
}
