/*****************************************************************************
* | File      	:	EPD_7in5.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2023-12-18
* | Info        :
*****************************************************************************
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_7in5_V2.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_7IN5V2_Reset(void)
{
    digitalWrite(EPD_RST_PIN, 1);
    delay_ms(20);
    digitalWrite(EPD_RST_PIN, 0);
    delay_ms(2);
    digitalWrite(EPD_RST_PIN, 1);
    delay_ms(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_7IN5V2_SendCommand(uint8_t Reg)
{
    digitalWrite(EPD_DC_PIN, 0);
    digitalWrite(EPD_CS_PIN, 0);
    SPI_WriteByte(Reg);
    digitalWrite(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_7IN5V2_SendData(uint8_t Data)
{
    digitalWrite(EPD_DC_PIN, 1);
    digitalWrite(EPD_CS_PIN, 0);
    SPI_WriteByte(Data);
    digitalWrite(EPD_CS_PIN, 1);
}

static void EPD_7IN5V2_SendData2(uint8_t *pData, uint32_t len)
{
    digitalWrite(EPD_DC_PIN, 1);
    digitalWrite(EPD_CS_PIN, 0);
    SPI_Write_nByte(pData, len);
    digitalWrite(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_7IN5V2_WaitUntilIdle(void)
{
    Serial.println("e-Paper busy\r\n");
	do{
		delay_ms(5);  
	}while(!(digitalRead(EPD_BUSY_PIN)));   
	delay_ms(5);      
    Serial.println("e-Paper busy release\r\n");
}
/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN5_V2_TurnOnDisplay(void)
{	
    EPD_7IN5V2_SendCommand(0x12);			//DISPLAY REFRESH
    delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!
    EPD_7IN5V2_WaitUntilIdle();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
uint8_t EPD_7IN5_V2_Init(void)
{
    EPD_7IN5V2_Reset();
    EPD_7IN5V2_SendCommand(0x01);			//POWER SETTING
	EPD_7IN5V2_SendData(0x07);
	EPD_7IN5V2_SendData(0x07);    //VGH=20V,VGL=-20V
	EPD_7IN5V2_SendData(0x3f);		//VDH=15V
	EPD_7IN5V2_SendData(0x3f);		//VDL=-15V

	//Enhanced display drive(Add 0x06 command)
	EPD_7IN5V2_SendCommand(0x06);			//Booster Soft Start 
	EPD_7IN5V2_SendData(0x17);
	EPD_7IN5V2_SendData(0x17);   
	EPD_7IN5V2_SendData(0x28);		
	EPD_7IN5V2_SendData(0x17);	

	EPD_7IN5V2_SendCommand(0x04); //POWER ON
	delay_ms(100); 
	EPD_7IN5V2_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

	EPD_7IN5V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_7IN5V2_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

	EPD_7IN5V2_SendCommand(0x61);        	//tres			
	EPD_7IN5V2_SendData(0x03);		//source 800
	EPD_7IN5V2_SendData(0x20);
	EPD_7IN5V2_SendData(0x01);		//gate 480
	EPD_7IN5V2_SendData(0xE0);  

	EPD_7IN5V2_SendCommand(0X15);		
	EPD_7IN5V2_SendData(0x00);		

	/*
        If the screen appears gray, use the annotated initialization command
    */
    EPD_7IN5V2_SendCommand(0X50);			
	EPD_7IN5V2_SendData(0x10);
	EPD_7IN5V2_SendData(0x07);
	// EPD_7IN5V2_SendCommand(0X50);			
	// EPD_7IN5V2_SendData(0x10);
	// EPD_7IN5V2_SendData(0x17);
    // EPD_7IN5V2_SendCommand(0X52);			
	// EPD_7IN5V2_SendData(0x03);

	EPD_7IN5V2_SendCommand(0X60);			//TCON SETTING
	EPD_7IN5V2_SendData(0x22);
	
    return 0;
}

uint8_t EPD_7IN5_V2_Init_Fast(void)
{
    EPD_7IN5V2_Reset();
    EPD_7IN5V2_SendCommand(0X00);			//PANNEL SETTING
    EPD_7IN5V2_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    /*
        If the screen appears gray, use the annotated initialization command
    */
    EPD_7IN5V2_SendCommand(0X50);			
	EPD_7IN5V2_SendData(0x10);
	EPD_7IN5V2_SendData(0x07);
	// EPD_7IN5V2_SendCommand(0X50);			
	// EPD_7IN5V2_SendData(0x10);
	// EPD_7IN5V2_SendData(0x17);
    // EPD_7IN5V2_SendCommand(0X52);			
	// EPD_7IN5V2_SendData(0x03);

    EPD_7IN5V2_SendCommand(0x04); //POWER ON
    delay_ms(100); 
	EPD_7IN5V2_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

    //Enhanced display drive(Add 0x06 command)
    EPD_7IN5V2_SendCommand(0x06);			//Booster Soft Start 
    EPD_7IN5V2_SendData (0x27);
    EPD_7IN5V2_SendData (0x27);   
    EPD_7IN5V2_SendData (0x18);		
    EPD_7IN5V2_SendData (0x17);		

    EPD_7IN5V2_SendCommand(0xE0);
    EPD_7IN5V2_SendData(0x02);
    EPD_7IN5V2_SendCommand(0xE5);
    EPD_7IN5V2_SendData(0x5A);
	
    return 0;
}

uint8_t EPD_7IN5_V2_Init_Part(void)
{
    EPD_7IN5V2_Reset();

	EPD_7IN5V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_7IN5V2_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f
	
	EPD_7IN5V2_SendCommand(0x04); //POWER ON
	delay_ms(100); 
	EPD_7IN5V2_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal
	
	EPD_7IN5V2_SendCommand(0xE0);
	EPD_7IN5V2_SendData(0x02);
	EPD_7IN5V2_SendCommand(0xE5);
	EPD_7IN5V2_SendData(0x6E);
	
    return 0;
}

/*
    The feature will only be available on screens sold after 24/10/23
*/
uint8_t EPD_7IN5_V2_Init_4Gray(void)
{
    EPD_7IN5V2_Reset();

	EPD_7IN5V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_7IN5V2_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_7IN5V2_SendCommand(0X50);			
	EPD_7IN5V2_SendData(0x10);
	EPD_7IN5V2_SendData(0x07);
	
	EPD_7IN5V2_SendCommand(0x04); //POWER ON
	delay_ms(100); 
	EPD_7IN5V2_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal
	
    EPD_7IN5V2_SendCommand(0x06);			//Booster Soft Start 
    EPD_7IN5V2_SendData (0x27);
    EPD_7IN5V2_SendData (0x27);   
    EPD_7IN5V2_SendData (0x18);		
    EPD_7IN5V2_SendData (0x17);		

	EPD_7IN5V2_SendCommand(0xE0);
	EPD_7IN5V2_SendData(0x02);
	EPD_7IN5V2_SendCommand(0xE5);
	EPD_7IN5V2_SendData(0x5F);
	
    return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5_V2_Clear(void)
{
    uint16_t Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
    uint8_t image[EPD_7IN5_V2_WIDTH / 8] = {0x00};

    uint16_t i;
    EPD_7IN5V2_SendCommand(0x10);
    for(i=0; i<Width; i++) {
        image[i] = 0xFF;
    }
    for(i=0; i<Height; i++)
    {
        EPD_7IN5V2_SendData2(image, Width);
    }

    EPD_7IN5V2_SendCommand(0x13);
    for(i=0; i<Width; i++) {
        image[i] = 0x00;
    }
    for(i=0; i<Height; i++)
    {
        EPD_7IN5V2_SendData2(image, Width);
    }
    
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_ClearBlack(void)
{
    uint16_t Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
    uint8_t image[EPD_7IN5_V2_WIDTH / 8] = {0x00};

    uint16_t i;
    EPD_7IN5V2_SendCommand(0x10);
    for(i=0; i<Width; i++) {
        image[i] = 0x00;
    }
    for(i=0; i<Height; i++)
    {
        EPD_7IN5V2_SendData2(image, Width);
    }

    EPD_7IN5V2_SendCommand(0x13);
    for(i=0; i<Width; i++) {
        image[i] = 0xFF;
    }
    for(i=0; i<Height; i++)
    {
        EPD_7IN5V2_SendData2(image, Width);
    }
    
    EPD_7IN5_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5_V2_Display(uint8_t *blackimage)
{
    uint32_t Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
	
    EPD_7IN5V2_SendCommand(0x10);
    for (uint32_t j = 0; j < Height; j++) {
        EPD_7IN5V2_SendData2((uint8_t *)(blackimage+j*Width), Width);
    }

    EPD_7IN5V2_SendCommand(0x13);
    for (uint32_t j = 0; j < Height; j++) {
        for (uint32_t i = 0; i < Width; i++) {
            blackimage[i + j * Width] = ~blackimage[i + j * Width];
        }
    }
    for (uint32_t j = 0; j < Height; j++) {
        EPD_7IN5V2_SendData2((uint8_t *)(blackimage+j*Width), Width);
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_Display_Part(uint8_t *blackimage,uint32_t x_start, uint32_t y_start, uint32_t x_end, uint32_t y_end)
{
    if(((x_start % 8 + x_end % 8 == 8) && (x_start % 8 > x_end % 8)) || (x_start % 8 + x_end % 8 == 0) || ((x_end - x_start)%8 == 0))
    {
        x_start = x_start / 8 ;
        x_end = x_end / 8;
    }
    else
    {
        x_start = x_start / 8 ;
        x_end = x_end % 8 == 0 ? x_end / 8 : x_end / 8 + 1;
    }
    uint16_t i, Width;
	Width = x_end -  x_start;
	uint16_t IMAGE_COUNTER = Width * (y_end-y_start);

    x_end -= 1;
	y_end -= 1;	

    x_start = x_start * 8;
    x_end = x_end * 8;

    EPD_7IN5V2_SendCommand(0x50);
	EPD_7IN5V2_SendData(0xA9);
	EPD_7IN5V2_SendData(0x07);

	EPD_7IN5V2_SendCommand(0x91);		//This command makes the display enter partial mode
	EPD_7IN5V2_SendCommand(0x90);		//resolution setting
	EPD_7IN5V2_SendData(x_start/256);
	EPD_7IN5V2_SendData(x_start%256);   //x-start    

	EPD_7IN5V2_SendData(x_end/256);		
	EPD_7IN5V2_SendData(x_end%256);  //x-end	

	EPD_7IN5V2_SendData(y_start/256);  //
	EPD_7IN5V2_SendData(y_start%256);   //y-start    

	EPD_7IN5V2_SendData(y_end/256);		
	EPD_7IN5V2_SendData(y_end%256);  //y-end
	EPD_7IN5V2_SendData(0x01);
    
    EPD_7IN5V2_SendCommand(0x13);
    for (uint32_t j = 0; j < IMAGE_COUNTER; j++) {
        EPD_7IN5V2_SendData(blackimage[j]);
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_Display_4Gray(const uint8_t *Image)
{
    uint32_t i,j,k;
    uint8_t temp1,temp2,temp3;

    // old  data
    EPD_7IN5V2_SendCommand(0x10);
    for(i=0; i<48000; i++) {
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0;
                if(temp2 == 0xC0)
                    temp3 |= 0x00;
                else if(temp2 == 0x00)
                    temp3 |= 0x01; 
                else if(temp2 == 0x80)
                    temp3 |= 0x01; 
                else //0x40
                    temp3 |= 0x00; 
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0) 
                    temp3 |= 0x00;
                else if(temp2 == 0x00) 
                    temp3 |= 0x01;
                else if(temp2 == 0x80)
                    temp3 |= 0x01; 
                else    //0x40
                    temp3 |= 0x00;	
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }

        }
        EPD_7IN5V2_SendData(temp3);
        // printf("%x",temp3);
    }

    EPD_7IN5V2_SendCommand(0x13);   //write RAM for black(0)/white (1)
    for(i=0; i<48000; i++) {             //5808*4  46464
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)
                    temp3 |= 0x00;//white
                else if(temp2 == 0x00)
                    temp3 |= 0x01;  //black
                else if(temp2 == 0x80)
                    temp3 |= 0x00;  //gray1
                else //0x40
                    temp3 |= 0x01; //gray2
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)  //white
                    temp3 |= 0x00;
                else if(temp2 == 0x00) //black
                    temp3 |= 0x01;
                else if(temp2 == 0x80)
                    temp3 |= 0x00; //gray1
                else    //0x40
                    temp3 |= 0x01;	//gray2
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_7IN5V2_SendData(temp3);
        // printf("%x",temp3);
    }

    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_WritePicture_4Gray(const uint8_t *Image)
{
    uint32_t i,j,k,o;
    uint8_t temp1,temp2,temp3;
    uint16_t Width, Height;
	Width = (EPD_7IN5_V2_WIDTH % 8 == 0)? (EPD_7IN5_V2_WIDTH / 8 ): (EPD_7IN5_V2_WIDTH / 8 + 1);
	Height = EPD_7IN5_V2_HEIGHT;

    EPD_7IN5V2_SendCommand(0x10);
    for(i=0; i<Height; i++) {
        for(o=0; o<Width; o++) {
            if((o < Width/2)&&(i < Height))
            {
                temp3=0;
                for(j=0; j<2; j++) {
                    temp1 = Image[(i*Width/2+o)*2+j];
                    for(k=0; k<2; k++) {
                        temp2 = temp1&0xC0;
                        if(temp2 == 0xC0)
                            temp3 |= 0x00;
                        else if(temp2 == 0x00)
                            temp3 |= 0x01; 
                        else if(temp2 == 0x80)
                            temp3 |= 0x01; 
                        else //0x40
                            temp3 |= 0x00; 
                        temp3 <<= 1;

                        temp1 <<= 2;
                        temp2 = temp1&0xC0 ;
                        if(temp2 == 0xC0) 
                            temp3 |= 0x00;
                        else if(temp2 == 0x00) 
                            temp3 |= 0x01;
                        else if(temp2 == 0x80)
                            temp3 |= 0x01; 
                        else    //0x40
                            temp3 |= 0x00;	
                        if(j!=1 || k!=1)
                            temp3 <<= 1;

                        temp1 <<= 2;
                    }

                }
                EPD_7IN5V2_SendData(temp3);
                // printf("%x",temp3);
            }
            else
            {
                EPD_7IN5V2_SendData(0x00);
            }
        }
    }


    EPD_7IN5V2_SendCommand(0x13);
    for(i=0; i<Height; i++) {
        for(o=0; o<Width; o++) {
            if((o < Width/2)&&(i < Height))
            {
                for(j=0; j<2; j++) {
                    temp1 = Image[(i*Width/2+o)*2+j];
                    for(k=0; k<2; k++) {
                        temp2 = temp1&0xC0 ;
                        if(temp2 == 0xC0)
                            temp3 |= 0x00;//white
                        else if(temp2 == 0x00)
                            temp3 |= 0x01;  //black
                        else if(temp2 == 0x80)
                            temp3 |= 0x00;  //gray1
                        else //0x40
                            temp3 |= 0x01; //gray2
                        temp3 <<= 1;

                        temp1 <<= 2;
                        temp2 = temp1&0xC0 ;
                        if(temp2 == 0xC0)  //white
                            temp3 |= 0x00;
                        else if(temp2 == 0x00) //black
                            temp3 |= 0x01;
                        else if(temp2 == 0x80)
                            temp3 |= 0x00; //gray1
                        else    //0x40
                            temp3 |= 0x01;	//gray2
                        if(j!=1 || k!=1)
                            temp3 <<= 1;

                        temp1 <<= 2;
                    }
                }
                EPD_7IN5V2_SendData(temp3);
                // printf("%x",temp3);
            }
            else
            {
                EPD_7IN5V2_SendData(0x00);
            }
        }
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5_V2_Sleep(void)
{
    EPD_7IN5V2_SendCommand(0x50);  	
    EPD_7IN5V2_SendData(0XF7);
    EPD_7IN5V2_SendCommand(0X02);  	//power off
    EPD_7IN5V2_WaitUntilIdle();
    EPD_7IN5V2_SendCommand(0X07);  	//deep sleep
    EPD_7IN5V2_SendData(0xA5);
}
