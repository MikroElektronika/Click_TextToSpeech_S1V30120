#include "text_to_speech.h"
#include "text_to_speech_img.h"

sbit TTS_RST      at LATC1_bit;
sbit TTS_CS       at LATC2_bit;
sbit TTS_MUTE     at LATB8_bit;
sbit TTS_RDY      at RE8_bit;
sbit TTS_RST_Dir  at TRISC1_bit;
sbit TTS_CS_Dir   at TRISC2_bit;
sbit TTS_MUTE_Dir at TRISB8_bit;
sbit TTS_RDY_Dir  at TRISE8_bit;

VER_t version;

void system_init( void );
void fatal_err( uint16_t *err );
void msg_blk( uint16_t *req, uint16_t *err );

void system_init()
{
    AD1PCFG = 0xFFFF;
    
    TTS_RST_Dir  = 0;
    TTS_CS_Dir   = 0;
    TTS_MUTE_Dir = 0;
    TTS_RDY_Dir  = 1;
    Delay_ms( 200 );

    SPI3_Init_Advanced( _SPI_MASTER, _SPI_8_BIT, 64, _SPI_SS_DISABLE,
                        _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH,
                        _SPI_ACTIVE_2_IDLE );
    Delay_ms( 200 );

    // DEBUG UART
    UART2_Init( 57600 );
    Delay_ms( 200 );

    UART2_Write_Text( "System Init Done !\r\n" );

}
void tts_setup()
{
    tts_msg_block_callback( msg_blk );
    tts_fatal_err_callback( fatal_err );
    UART2_Write_Text( "TTS Init Done !\r\n\n" );

    if ( !tts_version_boot( &version ) )
    {
        UART2_Write_Text( "Firmware version : " );
        UART2_Write_Text( version.hwver );
        UART2_Write_Text( "\r\n\n" );
    }

    if( !tts_image_load( ( uint8_t* )TTS_INIT_DATA, sizeof( TTS_INIT_DATA ) ) )
        UART2_Write_Text( "Image loading done successfully \r\n\n");

    if( !tts_image_exec() )
        UART2_Write_Text( "Image executed successfully \r\n\n" );

    if( !tts_interface_test() )
        UART2_Write_Text( "Interface test - success \r\n\n" );

    if( !tts_version_main( &version ) )
    {
        UART2_Write_Text( "Hardware version : " );
        UART2_Write_Text( version.hwver );
        UART2_Write_Text( "\r\n\n" );
        UART2_Write_Text( "Firmware version : " );
        UART2_Write_Text( version.fwver );
        UART2_Write_Text( "\r\n\n" );
    }

    if( !tts_power_default_config() )
        UART2_Write_Text( "Power configuration done !\r\n\n" );

    if( !tts_standby_enter() )
        UART2_Write_Text( "Standby mode entered !\r\n\n" );

    if( !tts_standby_exit() )
        UART2_Write_Text( "Standby mode exited !\r\n\n" );

    if( !tts_audio_default_config() )
        UART2_Write_Text( "Audio configuration done !\r\n\n" );

    if( !tts_volume_set( 0 ) )
        UART2_Write_Text( "Volume setting done !\r\n\n" );

    if( !tts_default_config() )
        UART2_Write_Text( "TTS configured !\r\n\n" );
}

void msg_blk( uint16_t *req, uint16_t *err )
{
    char txt[ 6 ];

    UART2_Write_Text( " MSG BLOCKED \r\n" );
    sprinti( txt, "%x\r\n", *req );
    UART2_Write_Text( txt );
    sprinti( txt, "%x\r\n", *err );
    UART2_Write_Text( txt );
}

void fatal_err( uint16_t *err )
{
    UART2_Write_Text( "Fatal Error Detected" );
    tts_init();
    tts_fatal_err_callback( fatal_err );
}

void main()
{
    system_init();
    tts_init();
    tts_setup();

   // 1
    tts_speak( "I am your robot" );
    tts_speak( "I am serving to you my master" );

}
