#ifndef MAIN_H_
#define MAIN_H_

/*
* Port/Pin defines, adapt to your selected AVR and circuit
*/
#define HVSP_SDI_OUT PORTC
#define HVSP_SDI_DDR DDRC
#define HVSP_SDI     PC0 // Serial Data Input on target

#define HVSP_SII_OUT PORTC
#define HVSP_SII_DDR DDRC
#define HVSP_SII     PC1 // Serial Instruction Input on target

#define HVSP_SDO_OUT PORTC
#define HVSP_SDO_IN  PINC
#define HVSP_SDO_DDR DDRC
#define HVSP_SDO     PC2 // Serial Data Output on target

#define HVSP_SCI_OUT PORTC
#define HVSP_SCI_DDR DDRC
#define HVSP_SCI     PC3 // Serial Clock Input on target

#define HVSP_VCC_OUT PORTC
#define HVSP_VCC_DDR DDRC
#define HVSP_VCC     PC4 // VCC to target

#define HVSP_RST_OUT PORTC
#define HVSP_RST_DDR DDRC
#define HVSP_RST     PC5 // +12V to reset pin on target (inverted through NPN transistor)

#define LED_OUT      PORTB
#define LED_DDR      DDRB
#define LED_PIN      PB0 // LED to show result

#define SWITCH_OUT   PORTB
#define SWITCH_DDR   DDRB
#define SWITCH_IN    PINB
#define SWITCH_PIN   PB1 // Push button to start fuse restore

#define ZEROINPUTS() HVSP_SDI_OUT &= ~(1 << HVSP_SDI); \
                     HVSP_SII_OUT &= ~(1 << HVSP_SII)

/*
* Structure to hold signature and default fuse bits of target to be programmed
* See MCU data sheet or avr-libc include files iotnxx.h for the values of LFUSE_DEFAULT, HFUSE_DEFAULT, EFUSE_DEFAULT
* or use Atmel AVR Fuse Calculator: http://www.engbedded.com/fusecalc/
*/
typedef struct TARGET_MCU_INFO_struct {
    uint8_t signature[3];
    uint8_t fuseLowBits;
    uint8_t fuseHighBits;
    uint8_t fuseExtendedBits;
} TARGET_MCU_INFO_t;

#endif /* MAIN_H_ */