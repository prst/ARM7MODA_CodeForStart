/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "../common/applet.h"
#include <board.h>
#include <pio/pio.h>
#include <utility/trace.h>
#include <utility/math.h>
#include <twi/twi.h>
#include <drivers/async/async.h>
#include <drivers/twi/twid.h>

//------------------------------------------------------------------------------
//         Local definitions
//------------------------------------------------------------------------------

/// TWI peripheral redefinition if needed
#if !defined(AT91C_BASE_TWI) && defined(AT91C_BASE_TWI0)
    #define AT91C_BASE_TWI      AT91C_BASE_TWI0
    #define AT91C_ID_TWI        AT91C_ID_TWI0
    #define PINS_TWI            PINS_TWI0
#endif

/// TWI clock frequency in Hz.
#define TWCK            400000

/// Slave address of AT24C chips.
#define AT24C_ADDRESS   0x50

#if defined (at91sam7s) || defined (at91sam7x) || defined (at91sam7l) || defined (at91sam7a3)
/// Build a tiny applet for devices with small SRAM size
#define TINY_APPPLET
/// Stack size in SRAM
#define STACK_SIZE 0x100

#else
/// Read write buffer size in page number 
#define BUFFER_NB_PAGE 64
#endif

//------------------------------------------------------------------------------
//         Local structures
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Structure for storing parameters for each command that can be performed by
/// the applet.
//------------------------------------------------------------------------------
struct _Mailbox {

    /// Command send to the monitor to be executed.
    unsigned int command;
    /// Returned status, updated at the end of the monitor execution.
    unsigned int status;

    /// Input Arguments in the argument area
    union {

        /// Input arguments for the Init command.
        struct {

            /// Communication link used.
            unsigned int comType;
            /// Trace level.
            unsigned int traceLevel;
            /// atmel AT24 serial EEPROM index.
            unsigned int at24DeviceIndex;

        } inputInit;

        /// Output arguments for the Init command.
        struct {

            /// Memory size.
            unsigned int memorySize;
            /// Buffer address.
            unsigned int bufferAddress;
            /// Buffer size.
            unsigned int bufferSize;

        } outputInit;

        /// Input arguments for the Write command.
        struct {

            /// Buffer address.
            unsigned int bufferAddr;
            /// Buffer size.
            unsigned int bufferSize;
            /// Memory offset.
            unsigned int memoryOffset;

        } inputWrite;

        /// Output arguments for the Write command.
        struct {

            /// Bytes written.
            unsigned int bytesWritten; 

        } outputWrite;

        /// Input arguments for the Read command.
        struct {

            /// Buffer address.
            unsigned int bufferAddr;
            /// Buffer size.
            unsigned int bufferSize;
            /// Memory offset.
            unsigned int memoryOffset;

        } inputRead;

        /// Output arguments for the Read command.
        struct {

            /// Bytes read.
            unsigned int bytesRead;

        } outputRead;

        /// Input arguments for the Full Erase command.
        // NONE

        /// Output arguments for the Full Erase command.
        // NONE
        
        /// Input arguments for the Buffer Erase command.
        // NONE

        /// Output arguments for the Buffer Erase command.
        // NONE
    } argument;    
};


//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Describes a serial EEPROM device parameters.
//------------------------------------------------------------------------------
typedef struct _At24Desc {

    /// Device string name.
    const char *name;
    /// Size of device in bytes.
    unsigned int size;
    /// Size of one page in bytes.
    unsigned int pageSize;
} At24Desc;


//------------------------------------------------------------------------------
//         Global variables
//------------------------------------------------------------------------------

/// End of program space (code + data).
extern unsigned int end;
    
    
//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Size of one page in the dataflash, in bytes.
static unsigned int pageSize;
/// Size of the buffer used for read/write operations in bytes.
static unsigned int bufferSize;

/// Pio pins to configure.
static const Pin pinTwi[] = {PINS_TWI};

/// TWI driver instance.
static Twid twid;

/// Array of recognized serial EEPROM chips.
static const At24Desc at24Devices[] = {
    {  "AT24C01",    128,   8},
    {  "AT24C02",    256,   8},
    {  "AT24C04",    512,  16},
    {  "AT24C08",   1024,  16},
    {  "AT24C16",   2048,  16},
    {  "AT24C32",   4096,  32},
    {  "AT24C64",   8192,  32},
    { "AT24C128",  16384,  64},
    { "AT24C256",  32768,  64},
    { "AT24C512",  65536, 128},
    {"AT24C1024", 131072, 256}
};

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Applet main entry. This function decodes received command and executes it.
/// \param argc  always 1
/// \param argv  Address of the argument area.
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    struct _Mailbox *pMailbox = (struct _Mailbox *) argv;
    // atmel eeprom device index.
    unsigned char at24DeviceIndex;
    
    unsigned int bytesToWrite, bytesToRead, bufferAddr, memoryOffset, packetSize;
    
    volatile unsigned int i;
    
    // index on read/write buffer
    unsigned char *pBuffer;
    
    // Configure the DBGU
    TRACE_CONFIGURE_ISP(DBGU_STANDARD, 115200, BOARD_MCK);

    // Configure pins 
     PIO_Configure(pinTwi, PIO_LISTSIZE(pinTwi));
    
    // ----------------------------------------------------------
    // INIT: 
    // ----------------------------------------------------------
    if (pMailbox->command == APPLET_CMD_INIT) {

#if (DYN_TRACES == 1)
        traceLevel = pMailbox->argument.inputInit.traceLevel;
#endif
        
        TRACE_INFO("-- EEPROM ISP Applet %s --\n\r", SAM_BA_APPLETS_VERSION);
        TRACE_INFO("-- %s\n\r", BOARD_NAME);
        TRACE_INFO("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

        AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TWI;
        TWI_ConfigureMaster(AT91C_BASE_TWI, TWCK, BOARD_MCK);
        TWID_Initialize(&twid, AT91C_BASE_TWI);
        at24DeviceIndex = pMailbox->argument.inputInit.at24DeviceIndex;
        
        // Get device parameters
        pageSize = at24Devices[at24DeviceIndex].pageSize;
        
#ifdef TINY_APPPLET        
       bufferSize = AT91C_ISRAM_SIZE                 // sram size
              - ( ((unsigned int) &end) - AT91C_ISRAM ) // program size (romcode, code+data)
           - STACK_SIZE;                             // stack size (if same area of applet code)
#else        
        bufferSize = BUFFER_NB_PAGE * pageSize;
#endif        
        bufferSize -= bufferSize % pageSize;  // integer number of pages can be contained in each buffer
        pMailbox->argument.outputInit.bufferAddress = (unsigned int) &end;
        pMailbox->argument.outputInit.bufferSize = bufferSize;
        pMailbox->argument.outputInit.memorySize = at24Devices[at24DeviceIndex].size;
        TRACE_INFO("\t%s pageSize : 0x%x bufferAddr : 0x%x\n\r",
               at24Devices[at24DeviceIndex].name, pageSize, pMailbox->argument.outputInit.bufferAddress);
        pMailbox->status = APPLET_SUCCESS;                  
    }

    // ----------------------------------------------------------
    // WRITE: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_WRITE) {

        memoryOffset = pMailbox->argument.inputWrite.memoryOffset;
        bufferAddr   = pMailbox->argument.inputWrite.bufferAddr;
        bytesToWrite = pMailbox->argument.inputWrite.bufferSize;
        TRACE_INFO("WRITE at offset: 0x%x buffer at : 0x%x of: 0x%x Bytes\n\r",
               memoryOffset, bufferAddr, bytesToWrite);

        pBuffer = (unsigned char *) bufferAddr;
        if ((memoryOffset % pageSize) != 0) {
            
            packetSize  = min (pageSize - (memoryOffset % pageSize), bytesToWrite) ;
            // Write the page contents
            TWID_Write(&twid, AT24C_ADDRESS, memoryOffset, 2,  pBuffer, packetSize, 0);
            
            // Wait at least 10 ms
            for (i=0; i < 100000; i++);
            
            bytesToWrite -= packetSize;
            pBuffer += packetSize;
            memoryOffset += packetSize;
        }

        // If it remains more than one page to write
        while (bytesToWrite >= pageSize) {
            // Write the page contents 
            TWID_Write(&twid, AT24C_ADDRESS, memoryOffset, 2, pBuffer, pageSize, 0);
            
            // Wait at least 10 ms
            for (i=0; i < 100000; i++);
            pBuffer += pageSize;
            memoryOffset += pageSize;
            bytesToWrite -= pageSize;
        }

        // Write remaining data
        if (bytesToWrite > 0) {
            // Write the page contents
            TWID_Write(&twid, AT24C_ADDRESS, memoryOffset, 2, pBuffer, bytesToWrite, 0);
            // Wait at least 10 ms
            for (i=0; i < 100000; i++);            
            // No more bytes to write
            bytesToWrite = 0;
        }
        
        
        TRACE_INFO("WRITE return byte written : 0x%x Bytes\n\r",
               pMailbox->argument.inputWrite.bufferSize - bytesToWrite);

        pMailbox->argument.outputWrite.bytesWritten = pMailbox->argument.inputWrite.bufferSize - bytesToWrite;
        pMailbox->status = APPLET_SUCCESS;
    }

    // ----------------------------------------------------------
    // READ: 
    // ----------------------------------------------------------
    else if (pMailbox->command == APPLET_CMD_READ) {

        memoryOffset = pMailbox->argument.inputRead.memoryOffset;
        bufferAddr   = pMailbox->argument.inputRead.bufferAddr;
        bytesToRead  = pMailbox->argument.inputRead.bufferSize;

        TRACE_INFO("READ at offset: 0x%x buffer at : 0x%x of: 0x%x Bytes\n\r",
               memoryOffset, bufferAddr, bytesToRead);

        pBuffer = (unsigned char *) bufferAddr;
        // Read the contents
        TWID_Read(&twid, AT24C_ADDRESS, memoryOffset, 2, pBuffer, bytesToRead, 0);
        TRACE_INFO("READ return byte read : 0x%x Bytes\n\r",
               pMailbox->argument.inputRead.bufferSize);
        pMailbox->argument.outputRead.bytesRead = pMailbox->argument.inputRead.bufferSize ;
        pMailbox->status = APPLET_SUCCESS;
    }

    // Acknowledge the end of command
    TRACE_INFO("\tEnd of applet (command : %x --- status : %x)\n\r", pMailbox->command, pMailbox->status);

    // Notify the host application of the end of the command processing
    pMailbox->command = ~(pMailbox->command);

    return 0;
}

