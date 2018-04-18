//******************************************************************************
//  MSP430F20xx Demo - I2C Master Transmitter and Receiver for bq27541 EVM
//
//  Description: MSP430 I2C Master communicates with a bq27541EVM (I2C Slave)
//  using an I2C Master software driver.  Whenever master device transmits data
//  to the slave device, the data is read back for verification purposes.
//  If the data is incorrect, the program will toggle the P1.0 LED forever.
//  The various LED's on the bq27541EVM are enabled/disabled to demonstrate
//  I2C communications.
//
//  ACLK = n/a, MCLK = SMCLK = Calibrated 1MHz
//
//                   Slave                      Master
//                  bq27541                  MSP430F23x0
//             -----------------   /|\    -----------------
//            |                 |   |    |              XIN|-
//            |                 |   |    |                 |
//            |                 |   |    |             XOUT|-
//            |                 |  10K   |                 |
//            |                 |   |    |                 |
//            |                 |   |    |             P1.0|---> LED
//            |              SDA|---|--->|SDA              |
//            |              SCL|<--|----|SCL              |
//
//  Note: Internal pull-ups may be used in this example for SDA & SCL
//        bq27541 7-bit Slave address = 1010101b
//
//  R. Wu
//  Texas Instruments Inc.
//  March 2010
//  Built with IAR Embedded Workbench Version: 4.21A
//******************************************************************************
#include <stdlib.h>
#include "gd32f1x0.h"
#include "bq27541.h"                                // Device-specific header

#define _I2C_USCI                           // OPTION #1: Use MSP430 USCI module
//#define _I2C_BITBANG                        // OPTION #2: Use Standard GPIOs

#define ATRATE_MA            -100           // USER CONFIG: AtRate setting (mA)
#define I2CSLAVEADDR         0x55           // 7-bit slave address
#define BUFFERSIZE             32           // # of bytes for Tx & Rx buffers

// extern UINT8 Message[RANDMESGNUMBYTES]; // random message sent to the bq device
// extern UINT8 Key[SECRETKEYNUMBYTES]; // secret key - should match contents of bq 
// extern UINT32 Digest_32[5]; // Result of SHA1/HMAC obtained by MCU stored here
// extern UINT32 H[5];

uint8_t DeviceID[10];           // Stores the Device ID data
uint8_t Digest[10];               // SHA1 response from the bq27541
uint8_t Key[16];
uint32_t Digest_32[5];
uint32_t H[5];

unsigned char TxData[BUFFERSIZE];           // Stores data bytes to be TX'd
unsigned char RxData[BUFFERSIZE];           // Stores data bytes that are RX'd
unsigned int  temperature;                  // Stores temperature
unsigned int  voltage;                      // Stores voltage
  signed int  atrate;                       // Stores AtRate
unsigned int  artte;                        // Stores AtRate Time to Empty
unsigned int  soc;                          // Stores State of Charge
unsigned int  dcap;                         // Stores Design Capacity
unsigned int  dnamelen;                     // Stores Device Name Length

unsigned int transBytes2UnsignedInt(unsigned char msb, unsigned char lsb);
void MSP430_bq27541_read(unsigned char cmd, unsigned int bytes);
void MSP430_bq27541_cmdWrite(unsigned char cmd, unsigned char data);
void MSP430_bq27541_blockWrite(unsigned char *buffer, unsigned int length);
void MSP430_bq27541_error(void);



unsigned int transBytes2UnsignedInt(unsigned char msb, unsigned char lsb)
{
  unsigned int tmp;
  
  tmp = ((msb << 8) & 0xFF00);
  return ((unsigned int)(tmp + lsb) & 0x0000FFFF);  
}


// bq27541 ¶Á¼Ä´æÆ÷
void MSP430_bq27541_read(unsigned char cmd, unsigned int bytes)
{
  unsigned char tx[1];
  tx[0] = cmd;
  
}


// bq27541   Ð´¼Ä´æÆ÷
void MSP430_bq27541_cmdWrite(unsigned char cmd, unsigned char data)
{
  unsigned char tx[2];
  
  tx[0] = cmd;
  tx[1] = data;

}




// bq27541   Ð´Êý¾Ý
void MSP430_bq27541_blockWrite(unsigned char *buffer, unsigned int length)
{
}


//bq27541 ´íÎóÐÅÏ¢
void MSP430_bq27541_error(void)
{
  while (1)                                 // Loop forever
  {

  }
}

void bq27541(void)
{ 
  unsigned int i, bytes;
  unsigned int sum = 0;
  unsigned char checksum;
  unsigned char read;
  unsigned char msb, lsb;



  // Read Temperature (units = 0.1K)
  MSP430_bq27541_read(bq27541CMD_TEMP_LSB, 2);
  temperature = transBytes2UnsignedInt(RxData[1], RxData[0]);
    
  // Read Voltage (units = mV)
  MSP430_bq27541_read(bq27541CMD_VOLT_LSB, 2);
  voltage = transBytes2UnsignedInt(RxData[1], RxData[0]);
  
  // Set AtRate (units = mA)
  atrate = ATRATE_MA;
  msb = ((atrate >> 8) & 0x00FF);
  lsb = (atrate & 0x00FF);
  bytes = 0;
  TxData[bytes++] = bq27541CMD_AR_LSB;
  TxData[bytes++] = lsb;
  TxData[bytes++] = msb;
  MSP430_bq27541_blockWrite(TxData, bytes);

  // Verify AtRate was set correctly (units = mA)
  MSP430_bq27541_read(bq27541CMD_AR_LSB, 2);
  if ((RxData[1] != msb) || (RxData[0] != lsb))
  {
    MSP430_bq27541_error();                 // Signal error condition occurred
  }
  
  // Read AtRate Time to Empty (units = Minutes)
  MSP430_bq27541_read(bq27541CMD_ARTTE_LSB, 2);
  artte = transBytes2UnsignedInt(RxData[1], RxData[0]);
  
  // Read State of Charge (units = %)
  MSP430_bq27541_read(bq27541CMD_SOC_LSB, 2);
  soc = transBytes2UnsignedInt(RxData[1], RxData[0]);

  // Read Design Capacity (units = mAH)
  MSP430_bq27541_read(bq27541CMD_DCAP_LSB, 2);
  dcap = transBytes2UnsignedInt(RxData[1], RxData[0]);
  
  // Read Device Name Length
  MSP430_bq27541_read(bq27541CMD_DNAMELEN, 1);
  dnamelen = RxData[0];
  
  // Read Device Name (Rx buffer should end up with ASCII chars for 'bq27541')
  MSP430_bq27541_read(bq27541CMD_DNAME, 7);
  
  // Write & read back 32 bytes of data in Manufacturer Info Block A
  for (i = 0; i < BUFFERSIZE; i++)          
  {
    TxData[i] = i;                          // Initialize data to be written
  }
  MSP430_bq27541_cmdWrite(bq27541CMD_DFDCNTL, 0);// BlockDataControl() = 0x00
  MSP430_bq27541_cmdWrite(bq27541CMD_DFCLS, 58);// Write the subclass value
  MSP430_bq27541_cmdWrite(bq27541CMD_DFBLK, 0);// Select offset within the flash  
  for (i = 0; i < BUFFERSIZE; i++)          // Compute the checksum of the block
  {
    sum += TxData[i];                       // Calculate the sum of the values  
  }
  checksum = (0xFF - (sum & 0x00FF));       // Compute checksum based on the sum
  MSP430_bq27541_cmdWrite(bq27541CMD_DFDCKS, checksum); // Write checksum value
  for (i = 0; i < BUFFERSIZE; i++)          // Write 32 bytes to Info Block A
  {
    MSP430_bq27541_cmdWrite((bq27541CMD_ADF+i), TxData[i]);  
  }
  MSP430_bq27541_read(bq27541CMD_ADF, 32);  // Read the contents of the block
  for (i = 0; i < BUFFERSIZE; i++)          // Check if writes were successful
  {
    if (TxData[i] != RxData[i])             // Tx & Rx data values match?
    {
      MSP430_bq27541_error();               // Signal error condition occurred
    }
  }

  // TODO: Insert the private 128-bit key that is stored in the bq27541
  // Key[15..8] = K1 (highest 64 bits of the key)
  // Key[ 7..0] = K0 (lowest 64 bits of the key)
  // In this example 0x0123456789ABCDEFFEDCBA9876543210 is used since a fresh
  // unprogrammed bq27541 device has this as its default for the 128-bit key.
  Key[15] = 0x01;
  Key[14] = 0x23;
  Key[13] = 0x45;
  Key[12] = 0x67;
  Key[11] = 0x89;
  Key[10] = 0xAB;
  Key[ 9] = 0xCD;
  Key[ 8] = 0xEF;
  Key[ 7] = 0xFE;
  Key[ 6] = 0xDC;
  Key[ 5] = 0xBA;
  Key[ 4] = 0x98;
  Key[ 3] = 0x76;
  Key[ 2] = 0x54;
  Key[ 1] = 0x32;
  Key[ 0] = 0x10;
  
  // Perform my own SHA1 (Host side)
 // read = (uint8_t)TI_getRandomIntegerFromVLO();// Use instability of VLO for rand
//   srand(read);                              // Plant seed based on random value 
//   for (i = 0; i < RANDMESGNUMBYTES; i++)    // Initialize random challenge bytes
//   {
//     Message[i] = rand();                    // Generate 1 random challenge byte
//   }
//   SHA1_authenticate();                      // Execute SHA-1/HMAC algorithm
  
  // Authenticate the bq27541
  MSP430_bq27541_cmdWrite(bq27541CMD_DFDCNTL, 1); // BlockDataControl() = 0x01
  // Write block of random challenge to bq27541 (starting at location ADF)
  bytes = 0;
  TxData[bytes++] = bq27541CMD_ADF;
//   for (i = 1; i <= RANDMESGNUMBYTES; i++)
//   {
//     TxData[bytes++] = Message[i-1];  
//   }
//   MSP430_bq27541_blockWrite(TxData, bytes);
  
  // Write checksum for the challenge to the bq27541
//   sum = 0;
//   for (i = 0; i < RANDMESGNUMBYTES; i++)    // Compute the checksum of the block
//   {
//     sum += Message[i];                      // Calculate the sum of the values  
//   }
//   checksum = (0xFF - (sum & 0x00FF));       // Compute checksum based on the sum
  MSP430_bq27541_cmdWrite(bq27541CMD_ACKSDFD, checksum);
  // Read back the digest from the bq27541
  //MSP430_bq27541_read(bq27541CMD_ADF, RANDMESGNUMBYTES);// Read digest contents
  
  // The 20 bytes of the digest returned by the bq27541 is arranged in 32-bit
  // words so that it can be compared with the results computed by the MCU
  Digest_32[4] = (uint32_t)(RxData[ 0])*0x00000001 +
                 (uint32_t)(RxData[ 1])*0x00000100 +
                 (uint32_t)(RxData[ 2])*0x00010000 +
                 (uint32_t)(RxData[ 3])*0x01000000;
  Digest_32[3] = (uint32_t)(RxData[ 4])*0x00000001 +
                 (uint32_t)(RxData[ 5])*0x00000100 +
                 (uint32_t)(RxData[ 6])*0x00010000 +
                 (uint32_t)(RxData[ 7])*0x01000000; 
  Digest_32[2] = (uint32_t)(RxData[ 8])*0x00000001 +
                 (uint32_t)(RxData[ 9])*0x00000100 +
                 (uint32_t)(RxData[10])*0x00010000 +
                 (uint32_t)(RxData[11])*0x01000000;
  Digest_32[1] = (uint32_t)(RxData[12])*0x00000001 +
                 (uint32_t)(RxData[13])*0x00000100 +
                 (uint32_t)(RxData[14])*0x00010000 +
                 (uint32_t)(RxData[15])*0x01000000;
  Digest_32[0] = (uint32_t)(RxData[16])*0x00000001 +
                 (uint32_t)(RxData[17])*0x00000100 +
                 (uint32_t)(RxData[18])*0x00010000 +
                 (uint32_t)(RxData[19])*0x01000000;

  // The results produced by the MCU and bq27541 must match for success
  if ( (Digest_32[0] == H[0]) && (Digest_32[1] == H[1]) &&
       (Digest_32[2] == H[2]) && (Digest_32[3] == H[3]) &&
       (Digest_32[4] == H[4]) )
  {
    // Set P1.0 LED on MSP430 EVM to signal that command sequence was successful
  }
  else
  {
    MSP430_bq27541_error();                 // Error condition
  }
 
}
