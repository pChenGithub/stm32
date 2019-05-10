#include "allinclude.h"
#include "stm32l0xx_hal.h"

uint16_t W25QXX_TYPE = W25Q128;     //默认是W25Q128
uint8_t W25QXX_BUFFER[512]; 
#define delay_us(x) HAL_Delay(x/1000)


#define PB_L(x) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_##x, GPIO_PIN_RESET)
#define PB_H(x) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_##x, GPIO_PIN_SET)

SPI_HandleTypeDef SpiHandle;

uint8_t SPI1_ReadWriteByte(uint8_t pTxData)  { 

		uint8_t pRxData =0; 
	//HAL_UART_Transmit(&huart1, "cmd \n", 5, 5000);
HAL_SPI_TransmitReceive(&SpiHandle, &pTxData, &pRxData, 1, 5000); 
	
	return pRxData;
}

void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector 
                       
/*
SCLK PB3
MISO PB4
MOSI PB5
CS   PB14
*/


//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
 
	__HAL_RCC_GPIOB_CLK_ENABLE();


      //GPIOB14
  GPIO_InitStructure.Pin =GPIO_PIN_14;//PB14
  GPIO_InitStructure.Mode =GPIO_MODE_OUTPUT_PP;//输出
  GPIO_InitStructure.Pull =GPIO_PULLUP;//?
  GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_VERY_HIGH;//100MHz
  //初始化
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	PB_H(14);

	#if 0	
	GPIO_InitStructure.Pin =GPIO_PIN_3;//PB14
  GPIO_InitStructure.Mode =GPIO_MODE_OUTPUT_PP;//输出
  GPIO_InitStructure.Pull =GPIO_NOPULL;//?
  GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_VERY_HIGH;//100MHz
  //初始化
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin =GPIO_PIN_4;//PB14
  GPIO_InitStructure.Mode =GPIO_MODE_OUTPUT_PP;//输出
  GPIO_InitStructure.Pull =GPIO_NOPULL;//?
  GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_VERY_HIGH;//100MHz
  //初始化
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin =GPIO_PIN_5;//PB14
  GPIO_InitStructure.Mode =GPIO_MODE_OUTPUT_PP;//输出
  GPIO_InitStructure.Pull =GPIO_NOPULL;//?
  GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_VERY_HIGH;//100MHz
  //初始化
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif
	
	SpiHandle.Instance               = SPI1;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial     = 7;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	
  SpiHandle.Init.Mode = SPI_MODE_MASTER;


  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
///* test */	
	uint8_t aTxBuffer = 0x90;
	uint8_t aRxBuffer;


	PB_H(14);
	delay_us(1000);
	PB_L(14);
	delay_us(1000);
	PB_H(14);
	delay_us(1000);
	PB_L(14);
	delay_us(1000);
	PB_H(14);
	delay_us(1000);
	PB_L(14);

//	
//	/*
	HAL_SPI_TransmitReceive(&SpiHandle, &aTxBuffer, &aRxBuffer, 1, 5000);
//	
	aTxBuffer = 0x00;
	HAL_SPI_TransmitReceive(&SpiHandle, &aTxBuffer, &aRxBuffer, 1, 5000);
	HAL_SPI_TransmitReceive(&SpiHandle, &aTxBuffer, &aRxBuffer, 1, 5000);
	HAL_SPI_TransmitReceive(&SpiHandle, &aTxBuffer, &aRxBuffer, 1, 5000);
	HAL_SPI_TransmitReceive(&SpiHandle, &aTxBuffer, &aRxBuffer, 1, 5000);
	HAL_SPI_TransmitReceive(&SpiHandle, &aTxBuffer, &aRxBuffer, 1, 5000);
//	
//// 
////       GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;//PG7
////  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
//// 
////       GPIO_SetBits(GPIOG,GPIO_Pin_7);//PG7输出1,防止NRF干扰SPI FLASH的通信 
////                                //SPI FLASH不选中
////       SPI1_Init();                                   //初始化SPI
////       SPI1_SetSpeed(SPI_BaudRatePrescaler_4);           //设置为21M时钟,高速模式 

HAL_UART_Transmit(&huart1, (uint8_t*)&W25QXX_TYPE, 2, 5000);
       W25QXX_TYPE=W25QXX_ReadID();      //读取FLASH ID.
//			



HAL_UART_Transmit(&huart1, (uint8_t*)&W25QXX_TYPE, 2, 5000);

//HAL_UART_Transmit(&huart1, "1 \n", 3, 5000);
//HAL_UART_Transmit(&huart1, "11 \n", 4, 5000);
			 uint8_t pBuffer[24] ={0};
			 uint8_t pBufferW[24] = {6,4,3,4,5,6,7,8,9,10};
//			 
//HAL_UART_Transmit(&huart1, "12345\n", 7, 5000);
			 //HAL_UART_Transmit(&huart1, "2 \n", 3, 5000);
			 W25QXX_Erase_Sector(0x000000);
			 
			 //HAL_UART_Transmit(&huart1, "3\n", 3, 5000);
			 W25QXX_Write_Page(pBufferW, 0x000000, 2);
			 //
			 //HAL_UART_Transmit(&huart1, "4\n", 3, 5000);
			 W25QXX_Read(pBuffer,0x000000,2);
			 
			 //HAL_UART_Transmit(&huart1, "5 \n", 3, 5000);
			 W25QXX_Write_Page(pBuffer, 0x000002, 3);
//	
//			 
			 Lcd_display_num( 1, 3, 0);
			 Lcd_display_num( 2, pBuffer[1], 0);
			 Lcd_display_num( 3, pBuffer[2], 0);
			 //pBuffer[0] = 6;
			 //pBuffer[2] = 2;
//			 
			 pBuffer[0] += 48;
			 pBuffer[1] += 48;
			 pBuffer[2] += 48;
			 pBuffer[3] = '\n';
			 
			 HAL_UART_Transmit(&huart1, pBuffer, 4, 5000);
			 
			 HAL_UART_Transmit(&huart1, "okok\n", 6, 5000);
			 
//			 
//			 //HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,0);
//			 
		 PB_H(14);
	delay_us(1000);
	PB_L(14);
	delay_us(1000);
		 PB_H(14);
		 delay_us(1000);
	PB_L(14);
	delay_us(1000);
		 PB_H(14);
		 delay_us(1000);
	PB_L(14);
	
	/* test end */	
}  
 
//读取W25QXX的状态寄存器
//BIT7  6  5 4 3  2 1 0
//SPR RV  TBBP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t W25QXX_ReadSR(void) 
{  
       uint8_t byte=0; 
       PB_L(14);                          //使能器件 
       SPI1_ReadWriteByte(W25X_ReadStatusReg);  //发送读取状态寄存器命令  
       byte=SPI1_ReadWriteByte(0Xff);      //读取一个字节  
       PB_H(14);                          //取消片选   
       return byte; 
} 
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit7,5,4,3,2)可以写!!!
void W25QXX_Write_SR(uint8_t sr) 
{ 
        PB_L(14);                         //使能器件 
       SPI1_ReadWriteByte(W25X_WriteStatusReg); //发送写取状态寄存器命令  
       SPI1_ReadWriteByte(sr);             //写入一个字节  
        PB_H(14);                         //取消片选           
} 
//W25QXX写使能 
//将WEL置位 
void W25QXX_Write_Enable(void) 
{
      PB_L(14);                           //使能器件 
  SPI1_ReadWriteByte(W25X_WriteEnable);    //发送写使能  
     PB_H(14);                            //取消片选           
} 
//W25QXX写禁止 
//将WEL清零  
void W25QXX_Write_Disable(void) 
{  
    PB_L(14);                             //使能器件 
  SPI1_ReadWriteByte(W25X_WriteDisable);   //发送写禁止指令    
PB_H(14);	
}              
//读取芯片ID
//返回值如下:                                    
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16  
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64
//0XEF17,表示芯片型号为W25Q128    
uint16_t W25QXX_ReadID(void)
{
       uint16_t Temp = 0;    
        PB_L(14);                                   
       SPI1_ReadWriteByte(0x90);//发送读取ID命令   
       SPI1_ReadWriteByte(0x00);     
       SPI1_ReadWriteByte(0x00);     
       SPI1_ReadWriteByte(0x00);                             
       Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
       Temp|=SPI1_ReadWriteByte(0xFF);      
        PB_H(14);                                   
       return Temp;
}             
//读取SPIFLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead) 
{ 
      uint16_t i;                                                                                        
    PB_L(14);                            //使能器件 
  SPI1_ReadWriteByte(W25X_ReadData);       //发送读取命令 
   SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>16));  //发送24bit地址  
  SPI1_ReadWriteByte((uint8_t)((ReadAddr)>>8)); 
  SPI1_ReadWriteByte((uint8_t)ReadAddr); 
  for(i=0;i<NumByteToRead;i++)
       { 
       pBuffer[i]=SPI1_ReadWriteByte(0XFF);  //循环读数  
  }
                                                       
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!  
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
      uint16_t i;  
  W25QXX_Write_Enable();                //SET WEL 
      
	PB_L(14);  //使能器件 
  SPI1_ReadWriteByte(W25X_PageProgram);    //发送写页命令 
   SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>16)); //发送24bit地址  
   SPI1_ReadWriteByte((uint8_t)((WriteAddr)>>8)); 
  SPI1_ReadWriteByte((uint8_t)WriteAddr); 
   for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//循环写数  
    PB_H(14);                             //取消片选 
       W25QXX_Wait_Busy();                                        //等待写入结束
} 
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECKOK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite) 
{                                       
       uint16_t pageremain;    
       pageremain=256-WriteAddr%256; //单页剩余的字节数                      
       if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
       while(1)
       {       
                 W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
                 if(NumByteToWrite==pageremain)break;//写入结束了
             else //NumByteToWrite>pageremain
                 {
                          pBuffer+=pageremain;
                          WriteAddr+=pageremain;      
 
                          NumByteToWrite-=pageremain;                        //减去已经写入了的字节数
                          if(NumByteToWrite>256)pageremain=256;//一次可以写入256个字节
                          else pageremain = NumByteToWrite;      //不够256个字节了
                 }
       };       
} 
//写SPIFLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)                                                  
//NumByteToWrite:要写入的字节数(最大65535) 
          
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite) 
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;       
	uint16_t i;  
	uint8_t *W25QXX_BUF;   
	W25QXX_BUF = W25QXX_BUFFER;     
	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小 
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
      if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
       while(1) 
       {      
                 W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
                 for(i=0;i<secremain;i++)//校验数据
                 {
                          if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除     
                 }
                 if(i<secremain)//需要擦除
                 {
                          W25QXX_Erase_Sector(secpos);//擦除这个扇区
                          for(i=0;i<secremain;i++)   //复制
                          {
                                   W25QXX_BUF[i+secoff]=pBuffer[i];         
                          }
                          W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  
 
                 }else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.                                       
                 if(NumByteToWrite==secremain)break;//写入结束了
                 else//写入未结束
                 {
                          secpos++;//扇区地址增1
                          secoff=0;//偏移位置为0      
 
                    pBuffer+=secremain;  //指针偏移
                          WriteAddr+=secremain;//写地址偏移  
						NumByteToWrite-=secremain;                                  //字节数递减
                          if(NumByteToWrite>4096)secremain=4096;  //下一个扇区还是写不完
                          else secremain=NumByteToWrite;                 //下一个扇区可以写完了
                 }      
       };     
}
//擦除整个芯片              
//等待时间超长...
void W25QXX_Erase_Chip(void) 
{                                 
  W25QXX_Write_Enable();                //SET WEL 
  W25QXX_Wait_Busy(); 
   PB_L(14);                            //使能器件 
  SPI1_ReadWriteByte(W25X_ChipErase);      //发送片擦除命令  
      PB_H(14);                           //取消片选           
       W25QXX_Wait_Busy();                                    //等待芯片擦除结束
} 
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr) 
{  
       //监视falsh擦除情况,测试用 
      printf("fe:%x\r\n",Dst_Addr);   
      Dst_Addr*=4096;
  W25QXX_Write_Enable();              //SET WEL    
//HAL_UART_Transmit(&huart1, "20 \n", 4, 5000);	
  W25QXX_Wait_Busy(); 
	//HAL_UART_Transmit(&huart1, "30 \n", 4, 5000);	
      PB_L(14);                         //使能器件 
  SPI1_ReadWriteByte(W25X_SectorErase);    //发送扇区擦除指令
//HAL_UART_Transmit(&huart1, "40 \n", 4, 5000);		
   SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>16));  //发送24bit地址  
  //HAL_UART_Transmit(&huart1, "50 \n", 4, 5000);	 
	SPI1_ReadWriteByte((uint8_t)((Dst_Addr)>>8)); 
  SPI1_ReadWriteByte((uint8_t)Dst_Addr);  
      PB_H(14);                           //取消片选           
  W25QXX_Wait_Busy();                                    //等待擦除完成
}  
//等待空闲
void W25QXX_Wait_Busy(void) 
{ 
       while((W25QXX_ReadSR()&0x01)==0x01); // 等待BUSY位清空
}  
//进入掉电模式
void W25QXX_PowerDown(void) 
{ 
  PB_L(14);                             //使能器件 
  SPI1_ReadWriteByte(W25X_PowerDown);      //发送掉电命令  
    PB_H(14);                             //取消片选           
  delay_us(3);                             //等待TPD  
} 
//唤醒
void W25QXX_WAKEUP(void) 
{  
       PB_L(14);                        //使能器件 
   SPI1_ReadWriteByte(W25X_ReleasePowerDown); // send W25X_PowerDown command 0xAB   
     PB_H(14);                            //取消片选           
  delay_us(3);                             //等待TRES1
} 
