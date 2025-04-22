//
// Pin connection to Z80 Bus
// PA0-7   D0-D7 (via 74LCX245)
// PA8     MERQ
// PA9     IORQ
// PA10    WR
// PA11    RD
// PA12    BUSREQ
// PA13    BUSACK
// PA15    RESET
// PB2-9   A0-A7
// PC13    74LCX245 OE
// PC14    74LCX245 DIR (L for Data output)

// Others
// PB10
// PB11
// PB12
// PB13
// PB14
// PB15

#include "debug.h"
#include "z80boot.h"

// Reset status
void InitGPIOforRESET(void) {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;

//  Data bus for Floating input

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

//  Address bus for Floating input

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

//  Control

//  MERQ/IORQ/RD/WR/BUSACK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9| GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

//  BUSREQ & RESET
//    GPIOA->OUTDR=0;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_12 | GPIO_Pin_15);


//  245 control

    GPIOC->OUTDR|=0x6000;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    return;

}

// DMA start
void InitGPIOforDMA(void) {

    GPIO_InitTypeDef  GPIO_InitStructure;

//  Data bus for output

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

//  Address bus for Output

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

//  Control
//  MERQ/IORQ/RD/WR/BUSACK
    GPIOA->OUTDR|=0x0f00;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9| GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

// Set 245 for Output

    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    GPIO_ResetBits(GPIOC, GPIO_Pin_14);

}

// IO start
void InitGPIOforIO(void) {

    GPIO_InitTypeDef  GPIO_InitStructure;

//  Data bus for input

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

//  Address bus for input

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

//  Control
//  MERQ/IORQ/RD/WR/BUSACK
    GPIOC->OUTDR|=0x00f0;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9| GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

// Set 245 for Input

    GPIO_SetBits(GPIOC, GPIO_Pin_13);

    GPIO_SetBits(GPIOC, GPIO_Pin_14);

    GPIO_ResetBits(GPIOC, GPIO_Pin_13);

    return;

}


void WriteSingleByteDMA(uint8_t address,uint8_t data) {

    uint32_t temp;

    // Set Address

    temp=GPIOB->OUTDR;
    temp&=0xfc03;       // CLEAR PB2-9
    temp|=(address<<2);
    GPIOB->OUTDR=temp;

    // Set Data

    temp=GPIOA->OUTDR;
    temp&=0xff00;
    temp|=(data);
    GPIOA->OUTDR=temp;

    // Write Request

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);  // MERQ
    GPIO_ResetBits(GPIOA, GPIO_Pin_10); // WR

    Delay_Us(1);

    // Clear Write Request

    GPIO_SetBits(GPIOA, GPIO_Pin_8);  // MERQ
    GPIO_SetBits(GPIOA, GPIO_Pin_10); // WR

    Delay_Us(1);

    return;

}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    // Initialize

    InitGPIOforRESET();

    //  RESET
//
//    GPIO_ResetBits(GPIOA,GPIO_Pin_15);
//    Delay_Us(1);

    // Revoke RESET

    GPIO_SetBits(GPIOA,GPIO_Pin_15);

    // Send BUSREQ

    GPIO_ResetBits(GPIOA, GPIO_Pin_12);

    // Wait BUSACK

    while(1) {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)==Bit_RESET){
            break;
        }
    }

    InitGPIOforDMA();

    // start DMA

    for(int i=0;i<256;i++) {
        WriteSingleByteDMA(i, z80boot[i]);
    }

    // Revoke BUSREQ

    GPIO_SetBits(GPIOA, GPIO_Pin_12);

    Delay_Us(1);

    // RESET again

    GPIO_ResetBits(GPIOA,GPIO_Pin_15);

    Delay_Ms(1);

    GPIO_SetBits(GPIOA,GPIO_Pin_15);

    InitGPIOforIO();

    while(1)
    {
    }
}
