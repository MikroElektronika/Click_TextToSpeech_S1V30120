#include "text_to_speech.h"

sbit TTS_RST at PORTA6_bit;
sbit TTD_RST_DIR at DDA6_bit;
sbit TTS_CS at PORTA5_bit;
sbit TTS_CS_DIR at DDA5_bit;
sbit TTS_MUTE at PORTA7_bit;
sbit TTS_MUTE_DIR at DDA7_bit;
sbit TTS_RDY at PIND2_bit;
sbit TTS_RDY_BIT at DDD2_bit;

void system_init()
{
    TTD_RST_DIR = 1;
    TTS_CS_DIR = 1;
    TTS_MUTE_DIR = 1;
    TTS_RDY_BIT = 0;
    Delay_ms( 200 );

    SPI1_Init_Advanced( _SPI_MASTER, _SPI_FCY_DIV8, _SPI_CLK_HI_TRAILING );
    Delay_ms( 200 );

    UART1_Init( 57600 );
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

    tts_speak( "I am your robot" );
}