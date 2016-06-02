#include "text_to_speech.h"

sbit TTS_RST at LATC1_bit;
sbit TTD_RST_DIR at TRISC1_bit;
sbit TTS_CS at LATC2_bit;
sbit TTS_CS_DIR at TRISC2_bit;
sbit TTS_MUTE at LATB8_bit;
sbit TTS_MUTE_DIR at TRISB8_bit;
sbit TTS_RDY at RE8_bit;
sbit TTS_RDY_BIT at TRISE8_bit;

void system_init( void );
void fatal_err( uint16_t *err );
void msg_blk( uint16_t *req, uint16_t *err );

void system_init()
{
    TTD_RST_DIR = 0;
    TTS_CS_DIR = 0;
    TTS_MUTE_DIR = 0;
    TTS_RDY_BIT = 1;
    Delay_ms( 200 );
                        
    SPI3_Init_Advanced( _SPI_MASTER, _SPI_8_BIT, 1, _SPI_SS_DISABLE,
                        _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, 
                        _SPI_ACTIVE_2_IDLE );

    Delay_ms( 200 );

    UART5_Init( 57600 );
    Delay_ms( 200 );

    UART5_Write_Text( "System Init Done !\r\n" );
}

void msg_blk( uint16_t *req, uint16_t *err )
{
    char txt[ 6 ];

    UART5_Write_Text( " MSG BLOCKED \r\n" );
    sprinti( txt, "%x\r\n", *req );
    UART5_Write_Text( txt );
    sprinti( txt, "%x\r\n", *err );
    UART5_Write_Text( txt );
}

void fatal_err( uint16_t *err )
{
    UART5_Write_Text( "Fatal Error Detected" );
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