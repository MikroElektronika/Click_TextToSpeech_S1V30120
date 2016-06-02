#include "text_to_speech.h"

sbit TTS_RST at LATC0_bit;
sbit TTD_RST_DIR at TRISC0_bit;
sbit TTS_CS at LATE0_bit;
sbit TTS_CS_DIR at TRISE0_bit;
sbit TTS_MUTE at LATA0_bit;
sbit TTS_MUTE_DIR at TRISA0_bit;
sbit TTS_RDY at RB0_bit;
sbit TTS_RDY_BIT at TRISB0_bit;

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

    SPI1_Init_Advanced( _SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_MIDDLE,
                        _SPI_CLK_IDLE_HIGH, _SPI_LOW_2_HIGH );
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
    tts_msg_block_callback( msg_blk );
    tts_fatal_err_callback( fatal_err );
    tts_setup();

    tts_speak( "I am your robot" );
}