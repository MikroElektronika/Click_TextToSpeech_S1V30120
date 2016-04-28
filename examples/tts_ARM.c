#include "text_to_speech.h"
#include "text_to_speech_img.h"

sbit TTS_RST at GPIOC_ODR.B2;
sbit TTS_CS at GPIOD_ODR.B13;
sbit TTS_MUTE at GPIOA_ODR.B4;
sbit TTS_RDY at GPIOD_IDR.B10;

VER_t version;

void system_init( void );
void err_cb( uint16_t *err );
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

void tts_setup()
{
    tts_msg_block_callback( msg_blk );
    tts_fatal_err_callback( fatal_err );
    tts_err_callback( err_cb );

    UART1_Write_Text( "TTS Init Done !\r\n\n" );

    if ( !tts_version_boot( &version ) )
    {
        UART1_Write_Text( "Firmware version : " );
        UART1_Write_Text( version.hwver );
        UART1_Write_Text( "\r\n\n" );
    }

    if( !tts_image_load( ( uint8_t* )TTS_INIT_DATA, sizeof( TTS_INIT_DATA ) ) )
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

    if( !tts_audio_default_config() )
        UART1_Write_Text( "Audio configuration done !\r\n\n" );

    if( !tts_volume_set( 0x20 ) )
        UART1_Write_Text( "Volume setting done !\r\n\n" );

    if( !tts_default_config() )
        UART1_Write_Text( "TTS configured !\r\n\n" );
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

void err_cb( uint16_t *err )
{
    char txt[ 6 ];

    UART1_Write_Text( " ERR \r\n" );
    sprinti( txt, "%x\r\n", *err );
    UART1_Write_Text( txt );
}

void fatal_err( uint16_t *err )
{
    char txt[ 6 ];
    
    UART1_Write_Text( "Fatal Error Detected" );
    sprinti( txt, "%x\r\n", *err );
    UART1_Write_Text( txt );
    tts_init();
    tts_fatal_err_callback( fatal_err );
}

void main() 
{
    system_init();
    tts_init();
    tts_setup();

    /* Singing the birthday song */
    tts_speak( "[hxae<300,10>piy<300,10> brr<600,12>th<100>dey<600,10> tuw<600,15> yu<1200,14>_<120>]\
                [hxae<300,10>piy<300,10> brr<600,12>th<100>dey<600,10> tuw<600,17> yu<1200,15>_<120>]\
                [hxae<300,10>piy<300,10>brr<600,22>th<100>dey<600,19>dih<600,15>rdeh<600,14>ktao<600,12>k_<120>_<120>]\
                [hxae<300,20>piy<300,20> brr<600,19>th<100>dey<600,15> tuw<600,17> yu<1200,15>] " );
}
    tts_speak( "[NAO<1200,18>NAO<600,22>NAO<600,25>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>HXEY<600,27>JHUW<1100,25>D<100>_<300>] \
                [NAO<1200,18>NAO<600,22>NAO<600,25>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>HXEY<600,27>JHUW<1100,25>D<100>_<300>] \
                [NAO<1200,18>NAO<600,22>NAO<600,25>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>HXEY<600,27>JHUW<1100,25>D<100>_<300>] \
                [NAO<1200,18>NAO<600,22>NAO<600,25>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>HXEY<600,27>JHUW<1100,25>D<100>_<300>] \
                [NAO<1200,18>NAO<600,22>NAO<600,25>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>HXEY<600,27>JHUW<1100,25>D<100>_<300>] \
                [NAO<1200,18>NAO<600,22>NAO<600,25>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>HXEY<600,27>JHUW<1100,25>D<100>_<300>] \
                [NAO<1200,18>NAO<600,22>NAO<600,25>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>NAO<150,32>NAO<150,30>NAO<300,32>NAO<1200,30>_<600>HXEY<600,27>JHUW<1100,25>D<100>_<300>] " );
}
