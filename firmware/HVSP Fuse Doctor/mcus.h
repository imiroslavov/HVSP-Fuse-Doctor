#ifndef MCUS_H_
#define MCUS_H_

static const TARGET_MCU_INFO_t PROGMEM targetMCU[] =
{
    {   // ATtiny13
        .signature        = { SIGNATURE_0, 0x90, 0x07 },
        .fuseLowBits      = 0x6A,
        .fuseHighBits     = 0xFF,
        .fuseExtendedBits = 0x00,
    },
    {   // ATtiny24
        .signature        = { SIGNATURE_0, 0x91, 0x0B },
        .fuseLowBits      = 0x62,
        .fuseHighBits     = 0xDF,
        .fuseExtendedBits = 0xFF,
    },
    {   // ATtiny44
        .signature        = { SIGNATURE_0, 0x92, 0x07 },
        .fuseLowBits      = 0x62,
        .fuseHighBits     = 0xDF,
        .fuseExtendedBits = 0xFF,
    },
    {   // ATtiny84
        .signature        = { SIGNATURE_0, 0x93, 0x0C },
        .fuseLowBits      = 0x62,
        .fuseHighBits     = 0xDF,
        .fuseExtendedBits = 0xFF,
    },
    {   // ATtiny25
        .signature        = { SIGNATURE_0, 0x91, 0x08 },
        .fuseLowBits      = 0x62,
        .fuseHighBits     = 0xDF,
        .fuseExtendedBits = 0xFF,
    },
    {   // ATtiny45
        .signature        = { SIGNATURE_0, 0x92, 0x06 },
        .fuseLowBits      = 0x62,
        .fuseHighBits     = 0xDF,
        .fuseExtendedBits = 0xFF,
    },
    {   // ATtiny85
        .signature        = { SIGNATURE_0, 0x93, 0x0B },
        .fuseLowBits      = 0x62,
        .fuseHighBits     = 0xDF,
        .fuseExtendedBits = 0xFF,
    },
    {   // ATtiny2313
        .signature        = { SIGNATURE_0, 0x91, 0x0A },
        .fuseLowBits      = 0x64,
        .fuseHighBits     = 0xDF,
        .fuseExtendedBits = 0xFF,
    },
    // Mark end of the list
    { { 0,0,0 }, 0, 0, 0 },
};

#endif /* MCUS_H_ */