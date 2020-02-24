/** 10.2 Bei Erkennen bestimmer RFID Tag's Türöffner betätigen.
         Anwendung: Zutrittskontrolle
*/
#include "mbed.h"
#include "MFRC522.h"

// NFC/RFID Reader (SPI)
// NFC/RFID Reader (SPI)
MFRC522    rfidReader( MBED_CONF_IOTKIT_RFID_MOSI, MBED_CONF_IOTKIT_RFID_MISO, MBED_CONF_IOTKIT_RFID_SCLK, MBED_CONF_IOTKIT_RFID_SS, MBED_CONF_IOTKIT_RFID_RST );

DigitalOut led1( MBED_CONF_IOTKIT_LED4 );
DigitalOut mosfet( MBED_CONF_IOTKIT_MOSFET1 );

// erlaubte RFID Tag's
char ids[3] [4] = {
    { 0x14, 0x63, 0x19, 0x1d },
    { 0x14, 0x63, 0x19, 0x1e },
    { 0x14, 0x63, 0x19, 0x1f },
};

int main(void)
{
    printf( "Init RFID \n" );
    // Init. RC522 Chip
    rfidReader.PCD_Init();
    printf( "bereit\n" );

    while (true) 
    {
        led1 = 0;

        // Look for new cards
        if ( rfidReader.PICC_IsNewCardPresent())
            if ( rfidReader.PICC_ReadCardSerial()) 
            {
                led1   = 1;

                // Print Card UID (2-stellig mit Vornullen, Hexadecimal)
                printf("Card UID: ");
                for ( int i = 0; i < rfidReader.uid.size; i++ )
                    printf("%02X:", rfidReader.uid.uidByte[i]);
                printf("\n");

                // alle ids durchlaufen (r = ids, c = position)
                int ok = true;
                for ( int r = 0; r < 3; r++ ) 
                {
                    for ( int c = 0; c < 4; c++ ) 
                    {
                    	ok = true;
                        if  ( rfidReader.uid.uidByte[c] != ids[r] [c] ) 
                        {
                            ok = false;
                            break;
                        }
                    }
                    if	( ok )
                    	break;
                }
				// RFID Tag's erkannt?
				if  ( ok )
				{
					mosfet = 1;
					wait_ms(1000);
					mosfet = 0;
					break;
				}
            }
        wait ( 0.2f );
    }
}
