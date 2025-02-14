/******************************************************************************
Title:      HVSP Fuse Doctor
Hardware:   ATmega8 or any AVR with at least 8 IO pins
License:    GNU General Public License
Author:     Iliya Miroslavov Iliev (i.miroslavov@gmail.com)

DESCRIPTION:
Restore the fuse bits of a any AVR to factory defaults
using High-voltage Serial Programming (HVSP).

*******************************************************************************/
#include <inttypes.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "main.h"
#include "mcus.h"

static inline void clockit(void)
{
    HVSP_SCI_OUT |= (1 << HVSP_SCI);

    // The minimum period for the Serial Clock Input (SCI) during High-voltage Serial Programming is 220ns. Waiting 1us just in case.
    _delay_us(1);

    HVSP_SCI_OUT &= ~(1 << HVSP_SCI);
}

/*
* Send instruction and data byte to the target and read one byte from it
*/
static uint8_t transferByte(uint8_t data, uint8_t instruction)
{
    uint8_t byteRead = 0;

    // Set first bit to zero
    ZEROINPUTS();
    clockit();

    for (uint8_t i = 0; i < 8; i++)
    {
        // Read one bit form SDO
        byteRead <<= 1; // Clear LSB by shifting 1 bit
        if (0 != (HVSP_SDO_IN & (1 << HVSP_SDO)))
        {
            byteRead |= 1;
        }

        // Output next data bit on SDI
        if (data & 0x80)
        {
            HVSP_SDI_OUT |= (1 << HVSP_SDI);
        }
        else
        {
            HVSP_SDI_OUT &= ~(1 << HVSP_SDI);
        }

        // Output next instruction bit on SII
        if (instruction & 0x80)
        {
            HVSP_SII_OUT |= (1 << HVSP_SII);
        }
        else
        {
            HVSP_SII_OUT &= ~(1 << HVSP_SII);
        }

        clockit();

        // Prepare for processing next bit
        data <<= 1;
        instruction <<= 1;
    }

    // Set last two bits to zero
    ZEROINPUTS();
    clockit();
    clockit();

    return byteRead;
}

static void pollSDO(void)
{
    // Wait until SDO goes high
    while (0 == (HVSP_SDO_IN & _BV(HVSP_SDO)));
}

static uint8_t getSignature(uint8_t code)
{
    transferByte(0x08, 0x4C);
    transferByte(code, 0x0C);
    transferByte(0x00, 0x68);

    return transferByte(0x00, 0x6C);
}

static void writeFuseBits(uint8_t code, uint8_t A, uint8_t B)
{
    transferByte(0x40, 0x4C);
    transferByte(code, 0x2C);
    transferByte(0x00, A);
    transferByte(0x00, B);

    pollSDO();

    transferByte(0x00, 0x4C);

    pollSDO();
}

#define WRITE_FUSE_BIT_LOW(code)  writeFuseBits(code, 0x64, 0x6C)
#define WRITE_FUSE_BIT_HIGH(code) writeFuseBits(code, 0x74, 0x7C)
#define WRITE_FUSE_BIT_EXT(code)  writeFuseBits(code, 0x66, 0x6E)

static uint8_t readFuseBits(uint8_t A, uint8_t B)
{
    transferByte(0x04, 0x4C);
    transferByte(0x00, A);

    return transferByte(0x00, B);
}

#define READ_FUSE_BIT_LOW()  readFuseBits(0x68, 0x6C)
#define READ_FUSE_BIT_HIGH() readFuseBits(0x7A, 0x7E)
#define READ_FUSE_BIT_EXT()  readFuseBits(0x6A, 0x6E)

int main(void)
{
    /*
    The following algorithm puts the device in High-voltage Serial Programming mode:
    (see Atmel data sheet for "High Voltage Serial Programming")

    1. Set Prog_enable pins (SDI, SII, SDO, SCI)  0 , RESET pin and VCC to 0V.
    2. Apply 5V between VCC and GND. Ensure that VCC reaches at least 1.8V within the next 20 microseconds.
    3. Wait 20 - 60 us, and apply 11.5 - 12.5V to RESET.
    4. Keep the Prog_enable pins unchanged for at least 10 us after the High-voltage has been
       applied to ensure the Prog_enable Signature has been latched.
    5. Release the Prog_enable[2] (SDO pin) to avoid drive contention on the Prog_enable[2]/SDO pin.
    6. Wait at least 300 us before giving any serial instructions on SDI/SII.
    7. Exit Programming mode by power the device down or by bringing RESET pin to 0V.
    */

    // Set LED pin as output and initially off
    LED_DDR  = (1 << LED_PIN);
    LED_OUT |= (1 << LED_PIN);

    // Turn on internal pull-up for push button
    SWITCH_DDR &= ~(1 << SWITCH_PIN);
    SWITCH_OUT |= (1 << SWITCH_PIN);

    while (1)
    {
        bool success = false;

        // Set the LED initially off
        LED_OUT |= (1 << LED_PIN);

        // Start when push button is pushed
        if (0 == (SWITCH_IN & (1 << SWITCH_PIN)))
        {
            // 1. Set all pins as output
            HVSP_SCI_DDR  |= (1 << HVSP_SCI);
            HVSP_SII_DDR  |= (1 << HVSP_SII);
            HVSP_SDI_DDR  |= (1 << HVSP_SDI);
            HVSP_SDO_DDR  |= (1 << HVSP_SDO);
            HVSP_RST_DDR  |= (1 << HVSP_RST);
            HVSP_VCC_DDR  |= (1 << HVSP_VCC);

            // Set pins to 0, RST to 1 because of the inverting NPN transistor
            HVSP_SCI_OUT &= ~(1 << HVSP_SCI);
            HVSP_SII_OUT &= ~(1 << HVSP_SII);
            HVSP_SDI_OUT &= ~(1 << HVSP_SDI);
            HVSP_SDO_OUT &= ~(1 << HVSP_SDO);
            HVSP_RST_OUT |=  (1 << HVSP_RST);
            HVSP_VCC_OUT &= ~(1 << HVSP_VCC);

            // 2. Apply VCC 5V to target
            HVSP_VCC_OUT |= (1 << HVSP_VCC);

            // 3. Wait 20-60us before applying 12V to RESET
            _delay_us(60);
            HVSP_RST_OUT &= ~(1 << HVSP_RST);

            // 4. Keep the Prog_enable pins unchanged for at least 10us after the high-voltage has been applied
            _delay_us(20);

            // 5. Switch Prog_enable[2] (SDO pin) to input
            HVSP_SDO_DDR  &= ~(1 << HVSP_SDO);

            // 6. Wait >=300us before sending any data on SDI/SII
            _delay_us(300);

            for (uint8_t device = 0; 0 != pgm_read_byte(&targetMCU[device].signature[0]); device++)
            {
                if (getSignature(0) == pgm_read_byte(&targetMCU[device].signature[0]) &&
                    getSignature(1) == pgm_read_byte(&targetMCU[device].signature[1]) &&
                    getSignature(2) == pgm_read_byte(&targetMCU[device].signature[2]))
                {
                    // Get default fuse bits for selected target
                    uint8_t fuseLowBits      = pgm_read_byte(&targetMCU[device].fuseLowBits);
                    uint8_t fuseHighBits     = pgm_read_byte(&targetMCU[device].fuseHighBits);
                    uint8_t fuseExtendedBits = pgm_read_byte(&targetMCU[device].fuseExtendedBits);

                    // Write default fuse bits
                    WRITE_FUSE_BIT_LOW(fuseLowBits);
                    WRITE_FUSE_BIT_HIGH(fuseHighBits);

                    if (0 != fuseExtendedBits)
                    {
                        WRITE_FUSE_BIT_EXT(fuseExtendedBits);
                    }

                    // Verify results
                    if ((fuseLowBits == READ_FUSE_BIT_LOW()) &&
                        (fuseHighBits == READ_FUSE_BIT_HIGH()) &&
                        ((0 ==fuseExtendedBits) || (fuseExtendedBits == READ_FUSE_BIT_EXT())))
                    {
                         success = true;
                    }

                    break;
                }
            }

            // 7. Exit Programming mode by setting all pins to 0 and then power off the device
            HVSP_SCI_OUT &= ~(1 << HVSP_SCI);
            HVSP_SII_OUT &= ~(1 << HVSP_SII);
            HVSP_SDI_OUT &= ~(1 << HVSP_SDI);
            HVSP_RST_OUT |=  (1 << HVSP_RST);
            _delay_us(10);
            HVSP_VCC_OUT &= ~(1 << HVSP_VCC);

            // Turn on the LED for 4 sec if the fuse restore was successful or flash the LED on error
            for (int i = 0; i < 16; i++)
            {
                if (success)
                {
                    LED_OUT &= ~(1 << LED_PIN);
                }
                else
                {
                    LED_OUT ^= (1 << LED_PIN);
                }

                _delay_ms(250);
            }
        }
    }
}
