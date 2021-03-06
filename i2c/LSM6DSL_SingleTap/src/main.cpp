/**
 * Pedometer
 * @see https://github.com/stm32duino/LSM6DSL/tree/master/examples
 */

#include "mbed.h"
#include "OLEDDisplay.h"
#include "LSM6DSLSensor.h"

// UI
OLEDDisplay oled( MBED_CONF_IOTKIT_OLED_RST, MBED_CONF_IOTKIT_OLED_SDA, MBED_CONF_IOTKIT_OLED_SCL );

static DevI2C devI2c( MBED_CONF_IOTKIT_I2C_SDA, MBED_CONF_IOTKIT_I2C_SCL );
static LSM6DSLSensor acc_gyro( &devI2c, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW ); // low address
uint16_t tap_count = 0;

int main()
{
    uint8_t id;
    LSM6DSL_Event_Status_t status;

    oled.clear();
    oled.printf( "Schlagen\n" );

    acc_gyro.init(NULL);
    acc_gyro.enable_x();    
    acc_gyro.enable_g();
    acc_gyro.enable_single_tap_detection();

    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    while (true) {
        acc_gyro.get_event_status( &status );
        if  ( status.TapStatus ) {
            tap_count++;
            oled.cursor( 1, 0 );
            oled.printf( "tap %6d\n", tap_count );
            printf( "tap %6d\n", tap_count );
        }
        wait( 0.1f );
    }
}