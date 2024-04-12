/*** Information about I2C 
 * USART sends the least significient bit first
 *
 * 
 * to configure :
 * 	Master feature:-
 * 		I2C speed mode:  standard / fast
 * 		I2c clock speed:    100Khz
 * 	Slave features:-
 * 		i can't confirm if they are nessacery for v 1.0.0
 * ...
 * 		pins of I2c1 on 401RC are (PB7 SDA), (PB6 SCL)
 * 		4.7 kohm
 * 
*/







/**************************CR1***********************************/
/*
START: Start generation
This bit is set and cleared by software and cleared by hardware when start is sent or PE=0.
*In Master Mode:
0: No Start generation
1: Repeated start generation

*In Slave mode:
0: No Start generation
1: Start generation when the bus is free
*/

/*
STOP: Stop generation
The bit is set and cleared by software, cleared by hardware when a Stop condition is
detected, set by hardware when a timeout error is detected.
In Master Mode:
0: No Stop generation.
1: Stop generation after the current byte transfer or after the current Start condition is sent.

In Slave mode:
0: No Stop generation.
1: Release the SCL and SDA lines after the current byte transfer.
*/

/*
ACK: Acknowledge enable
This bit is set and cleared by software and cleared by hardware when PE=0.
0: No acknowledge returned
1: Acknowledge returned after a byte is received (matched address or data)
Bit 9
*/




/******************************CR2***********************************/
/*
FREQ[5:0]: Peripheral clock frequency
0b000000: Not allowed
0b000001: Not allowed
0b000010: 2 MHz
...
0b110010: 50 MHz
Higher than 0b100100: Not allowed
*/

/*****Own address register 1 (I2C_OAR1)*****/
/*
Bit 15 ADDMODE Addressing mode (slave mode)
0: 7-bit slave address (10-bit address not acknowledged)
1: 10-bit slave address (7-bit address not acknowledged)
*/




/****
 * I2C regs
 * 
 * RC1: ha7tag meno
 * 	1. bit0		Peripheral enable(PE)				I2C enable
 * 	2. bit8		start generation
 * 	3. bit9		stop generation
 * 	4. bit10	acknowlege enable		
 * 
 * RC2: ha7tago 34an a enble different interrupts and
 * 	1. FREQ[0:5]		Peripheral clock frequency		  The minimum allowed frequency is 2 MHz, the maximum frequency is limited by the maximum APB1 frequency and cannot exceed 50 MHz (peripheral intrinsic maximum limit).
 * 
 * OAR1:
 * 	1. bit15		(slave mode) adderssing mode		0: 7-bit slave address
 * 
 * 
 * 
 * TRISE: ha7tago 34an azabt el Rise time lazm ykon (Fclk/1Mhz +1)
 * 	1.TRISE[5:0]: 	Maximum rise time in Fm/Sm mode (Master mode)
*/





















#include "rcc.h"
#include "i2c.h"
#include "i2c_regs.h"
#include <stdint.h>
#include "common_macros.h"








/** @defgroup I2C_duty_cycle_in_fast_mode I2C duty cycle in fast mode
  * @{
  */
#define I2C_DUTYCYCLE_2                 0x00000000U
#define I2C_DUTYCYCLE_16_9              I2C_CCR_DUTY




#define I2C_MIN_PCLK_FREQ(__PCLK__, __SPEED__)             (((__SPEED__) <= 100000U) ? ((__PCLK__) < I2C_MIN_PCLK_FREQ_STANDARD) : ((__PCLK__) < I2C_MIN_PCLK_FREQ_FAST))
#define I2C_CCR_CALCULATION(__PCLK__, __SPEED__, __COEFF__)     (((((__PCLK__) - 1U)/((__SPEED__) * (__COEFF__))) + 1U) & I2C_CCR_CCR)
#define I2C_FREQRANGE(__PCLK__)                            ((__PCLK__)/1000000U)
#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000U) ? ((__FREQRANGE__) + 1U) : ((((__FREQRANGE__) * 300U) / 1000U) + 1U))
#define I2C_SPEED_STANDARD(__PCLK__, __SPEED__)            ((I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 2U) < 4U)? 4U:I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 2U))
#define I2C_SPEED_FAST(__PCLK__, __SPEED__, __DUTYCYCLE__) (((__DUTYCYCLE__) == I2C_DUTYCYCLE_2)? I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 3U) : (I2C_CCR_CALCULATION((__PCLK__), (__SPEED__), 25U) | I2C_DUTYCYCLE_16_9))


#define I2C_SPEED(__PCLK__, __SPEED__, __DUTYCYCLE__)      (((__SPEED__) <= 100000U)? (I2C_SPEED_STANDARD((__PCLK__), (__SPEED__))) : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__)) & I2C_CCR_CCR) == 0U)? 1U : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__))) | I2C_CCR_FS))













/*master*/

/***stm implmentation***/
// #define I2C_7BIT_ADD_WRITE(__ADDRESS__)                    ((uint8_t)((__ADDRESS__) & (uint8_t)(~I2C_OAR1_ADD0)))
// #define I2C_7BIT_ADD_READ(__ADDRESS__)                     ((uint8_t)((__ADDRESS__) | I2C_OAR1_ADD0))

#define I2C_7BIT_ADD_WRITE(__ADDRESS__)                    ((uint8_t)((__ADDRESS__<<1) & (uint8_t)(~I2C_OAR1_ADD0)))
#define I2C_7BIT_ADD_READ(__ADDRESS__)                     ((uint8_t)((__ADDRESS__<<1) | I2C_OAR1_ADD0))


















#define Pclock 16000000

#define i2cinitclockspeed	100000
#define i2cDefaultFMDutyCycle	0
#define i2cStandardMode			0x0000

//freqrange (Mhz)
#define FREQ_range 16


#define CCR_Value_StandardMode(__Fpclk, __Fsclk)  ((__Fpclk/(2*__Fsclk)) & 0x0fff)







void I2C_Init(uint8 I2Cmodule, I2C_InitTypeDef* configPtr){

	RCC_Enable(RCC_I2C1);





/*---------------------------- I2C CR1 & CR2 Configuration --------------------*/
    // Disable I2C peripheral to configure it
	CLEAR_BYMASK(I2C1->CR1, I2C_CR1_PE);

	/*************/
	/*software Reset I2C*/
	//   I2C1->CR1 |= I2C_CR1_SWRST;
	//   I2C1->CR1 &= ~I2C_CR1_SWRST;
	/*************/

    // Frequency range: Set peripheral clock frequency (16 MHz in this example) /* 6 bits for the frequency*/
	CLEAR_AND_SET_BYMASKS(I2C1->CR2, I2C_CR2_FREQ, 16UL);



/*---------------------------- I2C TRISE Configuration --------------------*/
	/* Configure I2C Rise Time (Fclk/1Mhz)+1*/
	CLEAR_AND_SET_BYMASKS(I2C1->TRISE, I2C_TRISE_TRISE,(FREQ_range+1));
	// CLEAR_AND_SET_BYMASKS(I2C1->TRISE, I2C_TRISE_TRISE, I2C_RISE_TIME(FREQ_range, configPtr->ClockSpeed));



/*---------------------------- I2C CCR Configuration ----------------------*/
  /* Configure I2Cx speed modes: set Sm   Standard mode  */
	I2C1->CCR = 0;
  	CLEAR_AND_SET_BYMASKS(I2C1->CCR, 
							(I2C_CCR_FS | I2C_CCR_DUTY | I2C_CCR_CCR), 
							i2cStandardMode| CCR_Value_StandardMode(Pclock, configPtr->ClockSpeed));
    // I2C1->CCR |= 210;         // Set clock control register (CCR) value for desired SCL frequency
	/*CCR = Fpclk/(2*Fsclk) */


/*---------------------------- I2C OAR1 Configuration ---------------------*/
  /* Configure I2C Own Address and 7bit addressing mode */
	CLEAR_AND_SET_BYMASKS(I2C1->OAR1, 
							(I2C_OAR1_ADDMODE | I2C_OAR1_ADD8_9 | I2C_OAR1_ADD1_7 | I2C_OAR1_ADD0), 
							(configPtr->AddressingMode | ((configPtr->OwnAddress1)<<1)));






	// Enable the I2C peripheral
    I2C1->CR1 |= I2C_CR1_PE;
}











void I2C_Master_Transmit(uint8 I2Cmodule, uint16_t DevAddress, uint8_t *pData, uint8_t length){
	
	
	
	// wait while busy (while there is communication on the bus)
	while((( I2C1->SR2 & I2C_SR2_BUSY) >> I2C_SR2_BUSY_Pos));
	
	// Enable the I2C peripheral (it should be : if not enable then enable)
    I2C1->CR1 |= I2C_CR1_PE;
	
	
	/* Disable Pos */
    CLEAR_BIT(I2C1->CR1, I2C_CR1_POS_Pos);



/*********************(master) request write (Frame Head)***************************/
	//generate start
	SET_BIT(I2C1->CR1, I2C_CR1_START_Pos);

	//wait until its start is fully generated 
	while( ( I2C1->SR1 & I2C_SR1_SB) == 0 );			//master mode flag
	//if you passed that while then you are in MASETER mode

	// while ((I2C1->SR2 & I2C_SR2_MSL_Msk) == 1)


	// if (I2C1->AddressingMode == I2C_ADDRESSINGMODE_7BIT)
	// {
		/* Send slave address */
		I2C1->DR = I2C_7BIT_ADD_WRITE(DevAddress);
		// I2C1->DR = I2C_7BIT_ADD_READ(DevAddress);
	// }




	/* Halt if  the address isn't yet sent  and   you are still a master(didn't lose arbitration) */
	// wait untill ADDR flag is 1  which mean that address is transmitted successfully
	// Address sent (Master)
	// 0: No end of address transmission
	// 1: End of address transmission
	while( (( I2C1->SR1 & I2C_SR1_ADDR) == 0) /*&&  ((I2C1->SR2 & I2C_SR2_MSL_Msk) == 1)*/);
	

/********************************************************************/


	// Clear ADDR flag
	{
		volatile uint32_t tmpreg = 0x00U;
		tmpreg = I2C1->SR1;
		tmpreg = I2C1->SR2;
		(void)tmpreg;
	}






	//Bit 7 TxE: Data register empty (transmitters)   // doest rise after the address phase
	//0: Data register not empty
	//1: Data register empty
	// while((( I2C1->SR1 & I2C_SR1_TXE) >> I2C_SR1_TXE_Pos) == 0);



// Send data
	for (uint8_t i = 0; i < (length); i++){
		

        // Wait until data register is empty
        while (!(I2C1->SR1 & I2C_SR1_TXE));
        

		// Send data
        I2C1->DR = pData[i];



		// i guess again i guess check for ack
		while ((( I2C1->SR1 & I2C_SR1_BTF) >> I2C_SR1_BTF_Pos) == 0);
		
    }




//TXE means the data register is empty and SR1. BTF means that both the data register and the shift register are empty.
	// Bit 2 BTF: Byte transfer finished
	// 0: Data byte transfer not done
	// 1: Data byte transfer succeeded
	// while ((( I2C1->SR1 & I2C_SR1_BTF) >> I2C_SR1_BTF_Pos) == 0);



	/* Generate Stop */
    SET_BIT(I2C1->CR1, I2C_CR1_STOP_Pos);



}






void I2C_Slave_Receive(uint8 I2Cmodule, uint8_t *pData, uint8_t length){

	/* Disable Pos */
    CLEAR_BIT(I2C1->CR1, I2C_CR1_POS_Pos);

	/* Enable Address Acknowledge    (to Send ack after recieveing(not sure))*/
    SET_BIT(I2C1->CR1, I2C_CR1_ACK_Pos);

	/** wait untill ADDR flag is 1  which mean that address is matched(slave)
	* Address matched (Slave)
	* 0: Address mismatched or not received.
	* 1: Received address matched. */
	while((( I2C1->SR1 & I2C_SR1_ADDR) >> I2C_SR1_ADDR_Pos) == 0);

	// Clear ADDR flag by reading SR1 and SR2 {} to resolve a confilt of recreating uint tmpreg
	{
		volatile uint32_t tmpreg = 0x00U;
		tmpreg = I2C1->SR1;
		tmpreg = I2C1->SR2;
		(void)tmpreg;
	}


	// Recieve data
    for (uint8_t i = 0; i < length; i++) {
        // Wait until data register is empty
        while (!(I2C1->SR1 & I2C_SR1_RXNE));
        // Send data
        pData[i] = (uint8_t)I2C1->DR;

    }

	// Wait until all data is transmitted
    // while (!(I2C1->SR1 & I2C_SR1_BTF));


	/* Wait until STOP flag is set(detected) */
	while ( (I2C1->SR1 & I2C_SR1_STOPF) == 0 );


	// //clear all the flags to prepare for next Receive
	// I2C1->SR1 = 0x0000;
	// /* Clear STOP flag */
	// while ((I2C1->SR1 & I2C_SR1_STOPF) != 0 )
	{
		volatile uint32_t tmpreg = 0x00U;
		tmpreg = I2C1->SR1;
		I2C1->CR1 = I2C_CR1_PE;
		(void)tmpreg;
	}

	//you need to clear the SR1 and SR2
	//you need to check on errors BERR


	/* Disable Address Acknowledge */
    CLEAR_BIT(I2C1->CR1, I2C_CR1_ACK_Pos);
}







void I2C_start(void)
{
    // /* 
	//  * Clear the TWINT flag before sending the start bit TWINT=1
	//  * send the start bit by TWSTA=1
	//  * Enable TWI Module TWEN=1 
	//  */
    // TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    // /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    // while(BIT_IS_CLEAR(TWCR,TWINT));



	//generate start
	SET_BIT(I2C1->CR1, I2C_CR1_START_Pos);

	//wait until its really generated (افضل واقف طول ما هو بصفر)
	while((( I2C1->SR1 & I2C_SR1_SB) >> I2C_SR1_SB_Pos) == 0);
}
void I2C_stop(void)
{
    // /* 
	//  * Clear the TWINT flag before sending the stop bit TWINT=1
	//  * send the stop bit by TWSTO=1
	//  * Enable TWI Module TWEN=1 
	//  */
	// TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);


	/* Generate Stop */
    SET_BIT(I2C1->CR1, I2C_CR1_STOP_Pos);
}
// Function to send data over I2C
void I2C_SendData(uint8_t address, uint8_t* data, uint8_t length) {
    // Wait until I2C is not busy
    while (I2C1->SR2 & I2C_SR2_BUSY);

    // Send start condition
    I2C1->CR1 |= I2C_CR1_START;

    // Wait until start condition is sent
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Send address with write bit
    I2C1->DR = address << 1;

    // Wait until address is sent
    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    // Clear ADDR flag
    (void)I2C1->SR2;

    // Send data
    for (uint8_t i = 0; i < length; i++) {
        // Wait until data register is empty
        while (!(I2C1->SR1 & I2C_SR1_TXE));
        // Send data
        I2C1->DR = data[i];
    }

    // Wait until all data is transmitted
    while (!(I2C1->SR1 & I2C_SR1_BTF));

    // Send stop condition
    I2C1->CR1 |= I2C_CR1_STOP;
}


