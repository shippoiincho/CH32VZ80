//
// Pin connection to Z80 Bus
// PA0-7   D0-D7 (via 74LCX245)
// PA8     MERQ
// PA9     IORQ
// PA10    WR
// PA11    RD
// PA12    BUSREQ
// PA13    BUSACK
// PA14    M1
// PA15    RESET
// PB2-9   A0-A7
// PC13    74LCX245 DIR (L for Data output)
// PC14    74LCX245 OE

// Others
// PB10  WAIT
// PB11
// PB12
// PB13  (SPI SCK)
// PB14  (SPI MISO)
// PB15  (SPI MOSI)

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

//  MERQ/IORQ/RD/WR/BUSACK/M1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9| GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
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

//  WAIT
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOB,GPIO_Pin_10);


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
//   GPIOA->OUTDR=0xd000;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |GPIO_Pin_9| GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

// Set 245 for Input

    GPIO_SetBits(GPIOC, GPIO_Pin_14);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    GPIO_ResetBits(GPIOC, GPIO_Pin_14);

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

static inline void DataBusForOutput(void) {

//    GPIO_InitTypeDef  GPIO_InitStructure;

//  Data bus for output

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

     GPIOA->CFGLR=0x33333333;

     GPIOC->BSHR = GPIO_Pin_14;
     GPIOC->BCR = GPIO_Pin_13|GPIO_Pin_14;

//     GPIO_SetBits(GPIOC, GPIO_Pin_14);
//     GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//     GPIO_ResetBits(GPIOC, GPIO_Pin_14);

    return;
}

static inline void WaitForIORead(void) {

        uint32_t indata;

        // Inactive WAIT

        GPIOB->CFGHR=0x44444344;  // MUST MODIFY FOR PB11-15
        GPIOB->BSHR=GPIO_Pin_10;

    while(1) {
        indata=GPIOA->INDR;
        if((indata&0x0200)!=0) {
            break;
        }
    }

//  Data bus for input

    GPIOA->CFGLR=0x44444444;

// WAIT for input

    GPIOB->CFGHR=0x44444444;// MUST MODIFY FOR PB11-1

    // 245 for Input

    GPIOC->BSHR = GPIO_Pin_13|GPIO_Pin_14;
    GPIOC->BCR = GPIO_Pin_14;

    return;
}

static inline void WaitForIOWrite(void) {

        uint32_t indata;

        // Inactive WAIT

        GPIOB->CFGHR=0x44444344;// MUST MODIFY FOR PB11-1
        GPIOB->BSHR=GPIO_Pin_10;

    while(1) {
        indata=GPIOA->INDR;
        if((indata&0x0200)!=0) {
            break;
        }
    }

// WAIT for input

    GPIOB->CFGHR=0x44444444;// MUST MODIFY FOR PB11-1

    return;
}

int main(void)
{

    uint32_t indata,outdata,ioport,codesize;
    static uint32_t code_ptr;

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

    codesize=sizeof(z80prog);
//    codesize=0x1fff;

    while(1)
    {

        // Wait for IO Access

//        ioport=GPIOB->INDR;
//        ioport>>=2;
//        ioport&=0xff;

        indata=GPIOA->INDR;

        if((indata&0x0200)!=0) {
            continue;
        }

        // IO Read

        if((indata&0x0800)==0) {

            ioport=GPIOB->INDR;
            ioport>>=2;
            ioport&=0xff;

            switch(ioport) {

            case 0x40:

               DataBusForOutput();

//               outdata=GPIOA->OUTDR;
//               outdata&=0xff00;

               switch(code_ptr) {
               case 0:
                   outdata=(codesize&0xff);
                   break;
               case 1:
                   outdata=((codesize>>8)&0xff);
                   break;
               default:
                   outdata=z80prog[code_ptr-2];
               }

               GPIOA->BCR=0xff;
               GPIOA->BSHR=outdata;

               code_ptr++;

               WaitForIORead();
               break;

            default:

                WaitForIORead();
                break;

            }

            continue;
        }


        // IO Write

        if((indata&0x0400)==0) {

            ioport=GPIOB->INDR;
            ioport>>=2;
            ioport&=0xff;

            switch(ioport) {

            case 0x40:
                // reset code pointer;
                code_ptr=0;
                break;

            default:

                break;

            }

            WaitForIOWrite();
            continue;
        }

        // Interrupt
        if((indata&0x4000)==0) {
            WaitForIOWrite();
        }


    }
}
