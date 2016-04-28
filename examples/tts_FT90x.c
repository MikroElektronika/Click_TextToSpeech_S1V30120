#include "text_to_speech.h"
#include "text_to_speech_img.h"

sbit TTS_RST at GPIO_PIN1_bit;
sbit TTS_CS at GPIO_PIN28_bit;
sbit TTS_MUTE at GPIO_PIN9_bit;
sbit TTS_RDY at GPIO_PIN3_bit;

VER_t version;

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
    uint8_t tmp[ 512 ] = { 0 };

    system_init();
    tts_init();
    tts_msg_block_callback( msg_blk );
    tts_fatal_err_callback( fatal_err );

    UART1_Write_Text( "TTS Init Done !\r\n\n" );

    if ( !tts_version_boot( &version ) )
    {
        UART1_Write_Text( "Firmware version : " );
        UART1_Write_Text( version.hwver );
        UART1_Write_Text( "\r\n\n" );
    }

    if( !tts_image_load( TTS_INIT_DATA, sizeof( TTS_INIT_DATA ) ) )
        UART1_Write_Text( "Image loading done successfully \r\n\n");

    if( !tts_image_exec() )
        UART1_Write_Text( "Image executed successfully \r\n\n" );

    if( !tts_interface_test() )
        UART1_Write_Text( "Interface test - success \r\n\n" );

    if( !tts_version_main( &version ) )
    {
        UART1_Write_Text( "Hardware version : " );
        UART1_Write_Text( version.hwver );
        UART1_Write_Text( "\r\n\n" );
        UART1_Write_Text( "Firmware version : " );
        UART1_Write_Text( version.fwver );
        UART1_Write_Text( "\r\n\n" );
    }

    if( !tts_power_default_config() )
        UART1_Write_Text( "Power configuration done !\r\n\n" );

    if( !tts_standby_enter() )
        UART1_Write_Text( "Standby mode entered !\r\n\n" );

    if( !tts_standby_exit() )
        UART1_Write_Text( "Standby mode exited !\r\n\n" );

    if( !tts_audio_default_config() )
        UART1_Write_Text( "Audio configuration done !\r\n\n" );

    if( !tts_volume_set( 0 ) )
        UART1_Write_Text( "Volume setting done !\r\n\n" );

    //if( !tts_audio_mute() )
    //    UART1_Write_Text( "Muted !\r\n\n" );

    //if( !tts_codec_configure() )
    //    UART1_Write_Text( "Codec configuration done !\r\n\n" );
    //else
    //    UART1_Write_Text( "Codec configuration failed !\r\n\n" );

    //if( !tts_codec_start( tmp, 512 ) )
    //    UART1_Write_Text( "Codec start done !\r\n\n" );
    //else
    //    UART1_Write_Text( "Codec start failed !\r\n\n" );

    if( !tts_default_config() )
        UART1_Write_Text( "TTS configured !\r\n\n" );

    // 1
    if( !tts_speak( "Stop that car" ) )
        UART1_Write_Text( "Speak done !\r\n\n" );
    else
        UART1_Write_Text( "Speak failed !\r\n\n" );

    // 2
    if( !tts_speak( "What are you doing boy" ) )
        UART1_Write_Text( "Speak done !\r\n\n" );
    else
        UART1_Write_Text( "Speak failed !\r\n\n" );

    // 3
    if( !tts_speak( "Dance on street is not good" ) )
        UART1_Write_Text( "Speak done !\r\n\n" );
    else
        UART1_Write_Text( "Speak failed !\r\n\n" );

    // 4
    if( !tts_speak( "Stop doing that or I will kick you in naughty part" ) )
        UART1_Write_Text( "Speak done !\r\n\n" );
    else
        UART1_Write_Text( "Speak failed !\r\n\n" );

    if( !tts_pause() )
        UART1_Write_Text( "Paused !\r\n\n" );
    else
        UART1_Write_Text( "Pause failed !\r\n\n" );

    if( !tts_unpause() )
        UART1_Write_Text( "Unpaused !\r\n\n" );
    else
        UART1_Write_Text( "Unpause failed !\r\n\n" );

    // 5
    if( !tts_speak( "Relax your self and all is going to be better" ) )
        UART1_Write_Text( "Speak done !\r\n\n" );
    else
        UART1_Write_Text( "Speak failed !\r\n\n" );

    UART1_Write_Text( "DONE !\r\n\n" );
}