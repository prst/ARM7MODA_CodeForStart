#  ----------------------------------------------------------------------------
#          ATMEL Microcontroller Software Support
#  ----------------------------------------------------------------------------
#  Copyright (c) 2009, Atmel Corporation
#
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  - Redistributions of source code must retain the above copyright notice,
#  this list of conditions and the disclaimer below.
#
#  Atmel's name may not be used to endorse or promote products derived from
#  this software without specific prior written permission. 
#
#  DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
#  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
#  DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
#  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#  ----------------------------------------------------------------------------


# CIDR addresses
# 0x400E0740 for AT91SAM3U
# 0xFFFFEE40 for AT91SAM9263 and AT91CAP9
# 0xFFFFF240 for other devices
set cidr_addresses [list 0xfffff240 0xffffee40]
set sam3_cidr_addresses [list 0x400e0740]



# When reading the chip ID on the device, SAM-BA masks the 5 LS bits (device version)
# and then compares it with the following array. So the chip IDs written here must
# have the version field at 0.
array set devices {
    0x28000960 at91sam3u4
    0x28100960 at91sam3u4
    0x170a0940 at91sam7a3
    0x260a0940 at91sam7a3
    0x27330540 at91sam7l64
    0x27330740 at91sam7l128
    0x27280340 at91sam7s321
    0x27080340 at91sam7s32
    0x27090540 at91sam7s64
    0x270a0740 at91sam7s128
    0x270c0740 at91sam7s128
    0x270b0940 at91sam7s256
    0x270d0940 at91sam7s256
    0x270b0a40 at91sam7s512
    0x27280340 at91sam7se32
    0x272a0940 at91sam7se256
    0x272a0a40 at91sam7se512
    0x27650240 at91sam7sl16
    0x27680340 at91sam7sl32
    0x27680540 at91sam7sl64
    0x275a0740 at91sam7x128
    0x275b0940 at91sam7x256
    0x275c0a40 at91sam7x512
    0x271a0740 at91sam7xc128
    0x271b0940 at91sam7xc256
    0x271c0a40 at91sam7xc512
    0x019803a0 at91sam9260
    0x019703a0 at91sam9261
    0x019607a0 at91sam9263
    0x819903a0 at91sam9g10
    0x019905a0 at91sam9g20
    0x819b05a0 at91sam9g45
    0x819b05a0 at91sam9m10
    0x019b03a0 at91sam9rl64
    0x329993a0 at91sam9xe128
    0x329973a0 at91sam9xe128
    0x329a93a0 at91sam9xe256
    0x329a73a0 at91sam9xe512
    0x329a53a0 at91sam9xe512
    0x329aa3a0 at91sam9xe512
    0x83770940 at91cap7
    0x039a03a0 at91cap9
}
