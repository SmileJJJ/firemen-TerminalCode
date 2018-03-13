/**
  ******************************************************************************
  * �ļ�����: stm_flash.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: �ڲ�Falsh��дʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm_flash.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
#if STM32_FLASH_SIZE < 256
  #define STM_SECTOR_SIZE  1024 //�ֽ�
#else 
  #define STM_SECTOR_SIZE         2048
#endif

#if STM32_FLASH_WREN        //���ʹ����д 
static uint16_t STMFLASH_BUF [ STM_SECTOR_SIZE / 2 ];//�����2K�ֽ�
#endif

/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ��ȡָ����ַ�İ���(16λ����)
  * �������: faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
  * �� �� ֵ: ����ֵ:��Ӧ����.
  * ˵    ������
  */
uint16_t STMFLASH_ReadHalfWord ( uint32_t faddr )
{
        return *(__IO uint16_t*)faddr; 
}

#if STM32_FLASH_WREN        //���ʹ����д   
/**
  * ��������: ������д��
  * �������: WriteAddr:��ʼ��ַ
  *           pBuffer:����ָ��
  *           NumToWrite:����(16λ)��
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Write_NoCheck ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )   
{                                           
        uint16_t i;        
        
        for(i=0;i<NumToWrite;i++)
        {
                HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,WriteAddr,pBuffer[i]);
          WriteAddr+=2;                                    //��ַ����2.
        }  
} 

/**
  * ��������: ��ָ����ַ��ʼд��ָ�����ȵ�����
  * �������: WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
  *           pBuffer:����ָ��
  *           NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Write ( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite )        
{
        uint16_t secoff;           //������ƫ�Ƶ�ַ(16λ�ּ���)
        uint16_t secremain; //������ʣ���ַ(16λ�ּ���)           
         uint16_t i;    
        uint32_t secpos;           //������ַ
        uint32_t offaddr;   //ȥ��0X08000000��ĵ�ַ
        
  uint32_t PageError = 0;

    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    
    f.NbPages = 1;
        if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
        
        HAL_FLASH_Unlock();                                                //����
        
        offaddr=WriteAddr-FLASH_BASE;                //ʵ��ƫ�Ƶ�ַ.
        secpos=offaddr/STM_SECTOR_SIZE;                        //������ַ  0~127 for STM32F103RBT6
        secoff=(offaddr%STM_SECTOR_SIZE)/2;                //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
        secremain=STM_SECTOR_SIZE/2-secoff;                //����ʣ��ռ��С   
        if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
        
        while(1) 
        {        
                STMFLASH_Read(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
                for(i=0;i<secremain;i++)//У������
                {
                        if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����            
                }
                if(i<secremain)//��Ҫ����
                {
									  f.PageAddress = secpos*STM_SECTOR_SIZE+FLASH_BASE;
                        HAL_FLASHEx_Erase(&f,&PageError);//�����������
									
                        for(i=0;i<secremain;i++)//����
                        {
                                STMFLASH_BUF[i+secoff]=pBuffer[i];          
                        }
                        STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
                }else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.                                    
                if(NumToWrite==secremain)break;//д�������
                else//д��δ����
                {
                        secpos++;                                //������ַ��1
                        secoff=0;                                //ƫ��λ��Ϊ0          
                           pBuffer+=secremain;          //ָ��ƫ��
                        WriteAddr+=secremain;        //д��ַƫ��           
                           NumToWrite-=secremain;        //�ֽ�(16λ)���ݼ�
                        if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
                        else secremain=NumToWrite;//��һ����������д����
                }         
        };        
        HAL_FLASH_Lock();//����
}
#endif

/**
  * ��������: ��ָ����ַ��ʼ����ָ�����ȵ�����
  * �������: ReadAddr:��ʼ��ַ
  *           pBuffer:����ָ��
  *           NumToRead:����(16λ)��
  * �� �� ֵ: ��
  * ˵    ������
  */
void STMFLASH_Read ( uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead )           
{
        uint16_t i;
        
        for(i=0;i<NumToRead;i++)
        {
                pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
                ReadAddr+=2;//ƫ��2���ֽ�.        
        }
}

/**
  * ��������: ���ڲ�flashд�����ݲ���
  * �������: WriteAddr:��ʼ��ַ
  *           WriteData:Ҫд�������
  * �� �� ֵ: ��
  * ˵    ������
  */
void Test_Write( uint32_t WriteAddr, uint16_t WriteData )           
{
        STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/



//uint32_t writeFlashData = 0x55555555;
//uint32_t addr = 0x08007000;

//void writeFlashTest(void)
//{    uint32_t PageError = 0;

//    FLASH_EraseInitTypeDef f;
//    f.TypeErase = FLASH_TYPEERASE_PAGES;
//    f.PageAddress = addr;
//    f.NbPages = 1;
// 
//  HAL_FLASH_Unlock();

// 
//    HAL_FLASHEx_Erase(&f, &PageError);
// 
//    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, writeFlashData);
// 
//  HAL_FLASH_Lock();
//}

// 