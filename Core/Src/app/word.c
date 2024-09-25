#include "main.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "word.h"
#include "../hal/base.h"
#include "../lib/W25Qxx.h"
#include "../lib/OLED.h"

uint8_t KEY1_State = 0, KEY2_State = 0, KEY3_State = 0, KEY4_State = 0, KEY5_State = 0;
uint8_t KEY1_LastState = 0, KEY2_LastState = 0, KEY3_LastState = 0, KEY4_LastState = 0, KEY5_LastState = 0;
uint8_t KEY1_Count = 0, KEY2_Count = 0, KEY3_Count = 0, KEY4_Count = 0, KEY5_Count = 0;
uint8_t KEY1_Down = 0, KEY2_Down = 0, KEY3_Down = 0, KEY4_Down = 0, KEY5_Down = 0;

// 0 对比度 1 年 2 月 3 日 4 词书 5 每日复习数量 6 从第一天至今天数 7 每日新词数量 8 9学习总数 10 11新学总数
uint8_t config_data[32] = {0};

uint8_t months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint8_t condition = 0;
// extern char word[][15];
// extern char trans[][256];
uint16_t index1[] = {};
uint16_t index2[] = {};
// 定义菜单结构体
typedef struct Menu {
    char* text; // 当前菜单名称
    struct Menu* parentMenu; // 上一级菜单对象
    struct Menu** sonMenus; // 下一级菜单列表
    int sonCount; // 下一级菜单数量
    void (*func)(void); // 点击该菜单时调用的函数
} Menu;

struct menuManager{
  Menu* current;
  uint16_t index;
};

void testDebug(void){
  char v[32]={0};
  sprintf(v,"stream %d %d %d %d %d",KEY1_State,KEY2_State,KEY3_State,KEY4_State,KEY5_State);
  print1(v);
}

void contrast_setting(void){
  condition=1;
  OLED_DrawFilledRectangle(0,16,128,48, OLED_COLOR_REVERSED);
  OLED_DrawRectangle(0, 30, 128, 4, OLED_COLOR_NORMAL);
  uint8_t ori_config=config_data[0];
  if(KEY1_Down==1){
    if(config_data[0]>0x10)config_data[0]-=10;
  }
  if(KEY2_Down==1){
    if(config_data[0]<0xFF)config_data[0]+=10;
  }
  OLED_DrawFilledRectangle(1,30,(config_data[0]-0x10)*126/(0xFF-0x10),4, OLED_COLOR_NORMAL);
  if(ori_config!=config_data[0]){
    OLED_SendCmd(0x81);
    OLED_SendCmd(config_data[0]);
  }
  OLED_ShowFrame();
}

uint8_t temp_index_day=0;
void day_setting(void){
  condition=2;
  char buf[32]={0};
  uint8_t day=months[config_data[2]-1];
  if((config_data[1]%4==0)&&(config_data[2]==2))day=29;
  if(config_data[3]>day)config_data[3]=day;
  if(KEY3_Down==1){
    temp_index_day++;
    if(temp_index_day>5)temp_index_day=0;
  }
  OLED_NewFrame();
  uint8_t RecW = 8;
  uint8_t RecH = 2;
  uint8_t RecX = 0;
  uint8_t RecY = 40;
  if(temp_index_day==0){
    RecX=24;
  }
  if(temp_index_day==1){
    RecX=32;
  }
  if(temp_index_day==2){
    RecX=55;
  }
  if(temp_index_day==3){
    RecX=63;
  }
  if(temp_index_day==4){
    RecX=86;
  }
  if(temp_index_day==5){
    RecX=94;
  }
  if(KEY1_Down==1){ 
    // 年份，取值0-99
    if(temp_index_day==0){
      if(config_data[1]<90)config_data[1]+=10;
      else config_data[1]=99;
    }
    if(temp_index_day==1){
      if(config_data[1]<99)config_data[1]+=1;
    }
    //月份，取值1-12
    if(temp_index_day==2){
      if(config_data[2]<10)config_data[2]+=10;
      else config_data[2]=12;
    }
    if(temp_index_day==3){
      if(config_data[2]<12)config_data[2]+=1;
    }
    //日期，动态取值
    if(temp_index_day==4){
      if(config_data[3]<day/10*10)config_data[3]+=10;
      else config_data[3]=day;
    }
    if(temp_index_day==5){
      if(config_data[3]<day)config_data[3]+=1;
    }
  }
  if(KEY2_Down==1){ 
    // 年份，取值0-99
    if(temp_index_day==0){
      if(config_data[1]>=10)config_data[1]-=10;
      else config_data[1]=0;
    }
    if(temp_index_day==1){
      if(config_data[1]>=1)config_data[1]-=1;
    }
    //月份，取值1-12
    if(temp_index_day==2){
      if(config_data[2]>10)config_data[2]-=10;
      else config_data[2]=1;
    }
    if(temp_index_day==3){
      if(config_data[2]>1)config_data[2]-=1;
    }
    //日期，动态取值
    if(temp_index_day==4){
      if(config_data[3]>10)config_data[3]-=10;
      else config_data[3]=1;
    }
    if(temp_index_day==5){
      if(config_data[3]>1)config_data[3]-=1;
    }
  }
  OLED_DrawFilledRectangle(RecX,RecY,RecW,RecH, OLED_COLOR_NORMAL);
  sprintf(buf, "20%0*d年%0*d月%0*d日", 2,config_data[1], 2,config_data[2], 2,config_data[3]);
  OLED_PrintString(8, 23, buf, 'B', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}

void keyTest(void){
  char buf[32]={0};
  OLED_NewFrame();
  sprintf(buf, "KEY1:%d cnt:%d", KEY1_State, KEY1_Count);
  OLED_PrintASCIIString(0, 0, buf, &afont8x6, OLED_COLOR_NORMAL);
  sprintf(buf, "KEY2:%d cnt:%d", KEY2_State, KEY2_Count);
  OLED_PrintASCIIString(0, 12, buf, &afont8x6, OLED_COLOR_NORMAL);
  sprintf(buf, "KEY3:%d cnt:%d", KEY3_State, KEY3_Count);
  OLED_PrintASCIIString(0, 24, buf, &afont8x6, OLED_COLOR_NORMAL);
  sprintf(buf, "KEY4:%d cnt:%d", KEY4_State, KEY4_Count);
  OLED_PrintASCIIString(0, 36, buf, &afont8x6, OLED_COLOR_NORMAL);
  sprintf(buf, "KEY5:%d cnt:%d", KEY5_State, KEY5_Count);
  OLED_PrintASCIIString(0, 48, buf, &afont8x6, OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}

void funcionRecite(void){
}


uint16_t word_wheel=0;
void drawWord(uint16_t index){
  if(condition==0){
    word_wheel=0;
  }
  if(condition!=7)condition=3;
  unsigned char buff[256]={0};
  
	OLED_NewFrame();
  BSP_W25Qx_Read(buff,(20480+index)<<8,256);
  OLED_PrintString(0, -word_wheel+16, buff, 'A', OLED_COLOR_NORMAL);
  OLED_DrawFilledRectangle(0, 0, 128, 16, OLED_COLOR_REVERSED);
  BSP_W25Qx_Read(buff,(12288+index)<<8,256);
  OLED_PrintString(0, 0, buff, 'B', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  if(KEY3_State==1){
    if(word_wheel-2>=0)word_wheel-=2;
  }
  if(KEY5_State==1){
    if(word_wheel+2<=256)word_wheel+=2;
  }
}

uint16_t temp_Core_index=0;
void funcionCore(void){
  if(condition==0){
    temp_Core_index=0;
  }
  if(KEY1_Down==1){
    condition=0;
    if(temp_Core_index-1>=0)temp_Core_index--;
    else temp_Core_index=741;
  }
  if(KEY2_Down==1){
    condition=0;
    if(temp_Core_index+1<=741)temp_Core_index++;
    else temp_Core_index=0;
  }
  drawWord(index1[temp_Core_index]);
}

void funcionHighFrequency(void){
  if(condition==0){
    temp_Core_index=0;
  }
  if(KEY1_Down==1){
    condition=0;
    if(temp_Core_index-1>=0)temp_Core_index--;
    else temp_Core_index=1787;
  }
  if(KEY2_Down==1){
    condition=0;
    if(temp_Core_index+1<=1787)temp_Core_index++;
    else temp_Core_index=0;
  }
  drawWord(index2[temp_Core_index]);
}

uint16_t temp_all_index=0;
void funcionAllWord(void){
  if(condition==0){
    temp_all_index=0;
  }
  if(KEY1_Down==1){
    condition=0;
    if(temp_all_index-1>=0)temp_all_index--;
    else temp_all_index=6006;
  }
  if(KEY2_Down==1){
    condition=0;
    if(temp_all_index+1<6006)temp_all_index++;
    else temp_all_index=0;
  }
  drawWord(temp_all_index);
}


void writeWordData(uint8_t *data, uint16_t index){
  //先读出整个扇区的数据
  uint8_t buff[16*3]={0};
  uint16_t shanQu_index=index/16;
  shanQu_index*=16;
  for(int i=0;i<16;i++){
    BSP_W25Qx_Read(buff+i*3,(26488+shanQu_index+i)<<8,3);
  }
  //写入数据
  for(int i=0;i<3;i++){
    buff[(index-shanQu_index)*3+i]=data[i];
  }
  // char buf[20];
  // sprintf(buf, "%d %d %d", data[0],data[1],data[2]);
  // print1(buf);
  // HAL_Delay(1000);
  //擦除扇区
  BSP_W25Qx_Erase_Block((26488+shanQu_index)<<8);
  //写入数据
  for(int i=0;i<16;i++){
    BSP_W25Qx_Write(buff+i*3,(26488+shanQu_index+i)<<8,3);
  }
}

void findWord(char *word, uint16_t index){
  //判断word里有没有“?”
  for(int i=0;i<strlen(word);i++){
    if(word[i]=='?'){
      OLED_PrintString(0, 16, "单词输入有误", 'A', OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      return;
    }
  }

  //将所有字母转为小写
  for(int i=0;i<strlen(word);i++){
    if(word[i]>='A'&&word[i]<='Z'){
      word[i]=word[i]-'A'+'a';
    }
  }
  //二分查找单词
  uint16_t l=12398,r=12288+6007;
  while(l<r){
    uint16_t mid=(l+r)/2;
    char buf[100]={0};
    BSP_W25Qx_Read(buf,mid<<8,100);
    OLED_NewFrame();
    uint8_t mx_len=strlen(word);
    for(int i=0;i<100;i++){
      if(buf[i]==0xFF){
        break;
      }
      if(i>mx_len)mx_len=i;
    }
    if(memcmp(buf,word,mx_len)==0){
      word_wheel=0;
      while(1){
        drawWord(mid-12288);
        if(KEY4_State==1){
          return;
        }
      }
      return;
    }
    if(memcmp(buf,word,mx_len)>0){
      r=mid;
    }
    else{
      l=mid+1;
    }
  }
  while(1){
    OLED_NewFrame();
    OLED_PrintString(0, 16, "单词未收录或拼写有误", 'A', OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    if(KEY4_State==1){
      return;
    }
  }
  // OLED_PrintString(0, 16, word, 'A', OLED_COLOR_NORMAL);
  // OLED_ShowFrame();
}

void funcionSearch(void){
  char letter[26]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char morse[26][5]={
    ".-", //A
    "-...", //B
    "-.-.", //C
    "-..", //D
    ".", //E
    "..-.", //F
    "--.", //G
    "....", //H
    "..", //I
    ".---", //J
    "-.-", //K
    ".-..", //L
    "--", //M
    "-.", //N
    "---", //O
    ".--.", //P
    "--.-", //Q
    ".-.", //R
    "...", //S
    "-", //T
    "..-", //U
    "...-", //V
    ".--", //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
  };
  OLED_NewFrame();
  static uint8_t wheel=0;
  static char word[100]={0};
  static uint8_t word_index=0;
  static uint8_t temp_index=0;
  //将word用空格分割，转换为字母，如果单个字母转换错误，显示为“?”，最后未分割的字符直接显示为摩斯电码
  if(KEY4_Down==1){
    if(word_index-1>=0)word[word_index-1]=0;
    if(word_index>0){
      word_index--;
      KEY4_Down=0;
    }
  }
  char word2[10]={0};
  char word3[100]={0};
  uint8_t word2_index=0;
  uint8_t word3_index=0;
  for(int i=0;i<word_index;i++){
    if(word[i]==' '){
      if(word2_index==0){
        findWord(word3,word3_index-1);
        return;
      }
      uint8_t flag=0;
      for(int j=word3_index;j<word3_index+word2_index;j++){
        word3[j]=0;
      }
      for(int j=0;j<26;j++){
        if(strcmp(morse[j],word2)==0){
          word3[word3_index++]=letter[j];
          flag=1;
          break;
        }
      }
      if(!flag)word3[word3_index++]='?';
      for(int j=0;j<word2_index;j++)word2[j]=0;
      word2_index=0;
    }
    else{
      word2[word2_index++]=word[i];
      word3[word3_index+word2_index-1]=word2[word2_index-1];
      
    }
  }


  OLED_PrintString(0, 0, word3, 'A', OLED_COLOR_NORMAL);


  if(KEY5_Down==1){
    wheel+=1;
  }
  for(int i=0;i<5;i++){
    char buf[20];
    sprintf(buf, "%c:%s", letter[i+wheel%2*15], morse[i+wheel%2*15]);
    OLED_PrintASCIIString(0, 16+i*9, buf, &afont8x6, OLED_COLOR_NORMAL);
  }
  for(int i=0;i<5;i++){
    char buf[20];
    sprintf(buf, "%c:%s", letter[i+5+wheel%2*15], morse[i+5+wheel%2*15]);
    OLED_PrintASCIIString(42, 16+i*9, buf, &afont8x6, OLED_COLOR_NORMAL);
  }
  for(int i=0;i<5;i++){
    if(i+10+wheel%2*15>=26)break;
    char buf[20];
    sprintf(buf, "%c:%s", letter[i+10+wheel%2*15], morse[i+10+wheel%2*15]);
    OLED_PrintASCIIString(84, 16+i*9, buf, &afont8x6, OLED_COLOR_NORMAL);
  }

  if(KEY1_Down==1){
    if(word_index<99 && word_index-temp_index+1<10){
      word[word_index++]='.';
    }
  }
  if(KEY2_Down==1){
    if(word_index<99 && word_index-temp_index+1<10){
      word[word_index++]='-';
    }
  }
  if(KEY3_Down==1){
    if(word_index<99){
      word[word_index++]=' ';
      temp_index=word_index;
    }
  }

  //OLED_PrintString(0, 0, word, 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}



void funcionStudy(void){
  if(condition==0){
    condition=1;
    word_wheel=0;
  }
  if(condition==1){
    if(word_wheel==0){
      char buff[256]={0};
    }
  }
  OLED_ShowFrame();
}

uint16_t index3=0;
uint16_t l_index=0, r_index=0;
void funcionStudyStart(void){
  static uint8_t study_words[256*2];
  if(condition==0){
    BSP_W25Qx_Read(study_words,(32496<<8),256);
    BSP_W25Qx_Read(study_words+256,(32496<<8)+256,256);
	  index3=0;
    l_index=0;
    r_index=0;
    // for(int i=0;i<256;i++){
    //   char buf[20];
    //   sprintf(buf, "%d", (study_words[i*2+1]<<8)+study_words[i*2]);
    //   print1(buf);
    // }
    while(r_index<256){
      if(!((study_words[r_index*2+1]<<8)+study_words[r_index*2]==0 || (study_words[r_index*2+1]<<8)+study_words[r_index*2]==0xFFFF)){
        study_words[l_index*2]=study_words[r_index*2];
        study_words[l_index*2+1]=study_words[r_index*2+1];
        if(l_index!=r_index){
          study_words[r_index*2]=0;
          study_words[r_index*2+1]=0;
        }
        l_index++;
      }
      r_index++;
    }
    condition=6;
  }
  if(l_index==0){
      char buff[128]="今日单词记忆完成!";
      OLED_NewFrame();
      OLED_PrintString(0, 16, buff, 'A', OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      HAL_Delay(1000);
      KEY4_Down=1;
      return;
  }
  if(condition==6){   //只显示单词，不显示中文
    unsigned char buff[256]={0};
    OLED_NewFrame();
    BSP_W25Qx_Read(buff,(12288+(study_words[1]<<8)+study_words[0])<<8,256);
    OLED_PrintString(0, 0, buff, 'B', OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    if(KEY1_Down==1 || KEY2_Down==1){
      condition=7;
      word_wheel=0;
    }
    return;
  }
  OLED_NewFrame();
  if(KEY1_Down==1){   //本单词认识
    uint8_t word_data[32]={0};  //0学习进度 1上次记忆天数 2本次需要记忆次数
    BSP_W25Qx_Read(word_data,((study_words[1]<<8)+study_words[0]+26488)<<8,3);
    condition=6;
    if(word_data[2]==0)word_data[2]=0xFF;
    // char buf[20];
    // sprintf(buf, "%d %d %d", word_data[0],word_data[1],word_data[2]);
    // print1(buf);
    // HAL_Delay(1000);
    if(word_data[2]==0xFF){   //今天该词一遍过
      if(word_data[0]==1)word_data[0]=10;
      else if(word_data[0]==0xFF){
        word_data[0]=30;
      }
      else word_data[0]+=10;
      if(word_data[1]==0xFF){   //新学单词
        if(config_data[10]==0xFF && config_data[11]==0xFF){
          config_data[10]=0;
          config_data[11]=0;
        }
        if(config_data[11]==0xFF){
          config_data[10]++;
          config_data[11]=0;
        }
        else config_data[11]++;
      }
      if(config_data[8]==0xFF && config_data[9]==0xFF){
        config_data[8]=0;
        config_data[9]=0;
      }
      if(config_data[9]==0xFF){
        config_data[8]++;
        config_data[9]=0;
      }
      else config_data[9]++;
      BSP_W25Qx_Erase_Block(8192<<8);
      BSP_W25Qx_Write(config_data,8192<<8,32);
      word_data[1]=config_data[6];
      writeWordData(word_data,(study_words[1]<<8)+study_words[0]);
      for(uint8_t i=0;i<l_index;i++){   //删除该单词，并将后面的单词往前移动
        study_words[i*2]=study_words[i*2+2];
        study_words[i*2+1]=study_words[i*2+3];
      }
      l_index--;
    }
    else{   //今天该词没过
      word_data[2]--;
      writeWordData(word_data,(study_words[1]<<8)+study_words[0]);
      uint8_t newPosition=0;
      if(word_data[2]==3)newPosition=3;
      if(word_data[2]==2)newPosition=7;
      if(word_data[2]==1)newPosition=15;
      if(word_data[2]==0){  //本单词今天记忆完成
        if(word_data[1]==0xFF){   //新学单词
          if(config_data[10]==0xFF && config_data[11]==0xFF){
            config_data[10]=0;
            config_data[11]=0;
          }
          if(config_data[11]==0xFF){
            config_data[10]++;
            config_data[11]=0;
          }
          else config_data[11]++;
        }
        if(config_data[8]==0xFF && config_data[9]==0xFF){
          config_data[8]=0;
          config_data[9]=0;
        }
        if(config_data[9]==0xFF){
          config_data[8]++;
          config_data[9]=0;
        }
        else config_data[9]++;
        BSP_W25Qx_Erase_Block(8192<<8);
        BSP_W25Qx_Write(config_data,8192<<8,32);
        for(uint8_t i=0;i<l_index;i++){   //删除该单词，并将后面的单词往前移动
          study_words[i*2]=study_words[i*2+2];
          study_words[i*2+1]=study_words[i*2+3];
        }
        l_index--;
      }
      else{   //本单词今天仍需记忆
        uint16_t temp_word=(study_words[1]<<8)+study_words[0];
        newPosition=l_index>newPosition?newPosition:l_index;
        for(uint8_t i=0;i<newPosition;i++){   //将本单词放到记忆队列的指定位置
          study_words[i*2]=study_words[i*2+2];
          study_words[i*2+1]=study_words[i*2+3];
        }
        study_words[(newPosition-1)*2]=temp_word&0xFF;
        study_words[(newPosition-1)*2+1]=temp_word>>8;
      }
    }
  }
  else if(KEY2_Down==1){   //本单词不认识
    uint8_t word_data[32]={0};  //0学习进度 1上次记忆天数 2本次需要记忆次数
    BSP_W25Qx_Read(word_data,((study_words[1]<<8)+study_words[0]+26488)<<8,2);
    condition=6;
    word_data[0]=1;
    word_data[2]=3;
    writeWordData(word_data,(study_words[1]<<8)+study_words[0]);
    uint8_t newPosition=3;
    uint16_t temp_word=(study_words[1]<<8)+study_words[0];
    newPosition=l_index>newPosition?newPosition:l_index;
    for(uint8_t i=0;i<newPosition;i++){   //将本单词放到记忆队列的指定位置
      study_words[i*2]=study_words[i*2+2];
      study_words[i*2+1]=study_words[i*2+3];
    }
    study_words[(newPosition-1)*2]=temp_word&0xFF;
    study_words[(newPosition-1)*2+1]=temp_word>>8;
  }
  if(KEY1_Down==1||KEY2_Down==1){   //实时保存study_words
    BSP_W25Qx_Erase_Block((32496<<8)+256);
    BSP_W25Qx_Write(study_words,(32496<<8),256);
    BSP_W25Qx_Write(study_words+256,(32496<<8)+256,256);
  }
  char buf[32]={0};
  // sprintf(buf, "%d %d", index3,(study_words[index3*2+1]<<8)+study_words[index3*2]);
  // print1(buf);
  if(condition==7)drawWord((study_words[1]<<8)+study_words[0]);
}


void funcionStudyNewDay(void){
  if(config_data[4]==0xFF){
    char buff[128]="你还未选择词书";
    OLED_NewFrame();
    OLED_PrintString(0, 16, buff, 'A', OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    return;
  }
  if(config_data[5]==0xFF){
    char buff[128]="你还未选择每日复习数量";
    OLED_NewFrame();
    OLED_PrintString(0, 16, buff, 'A', OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    return;
  }
  if(config_data[7]==0xFF){
    char buff[128]="你还未选择每日新学数量";
    OLED_NewFrame();
    OLED_PrintString(0, 16, buff, 'A', OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    return;
  }
  condition=5;
  if(config_data[6]==0xFF)config_data[6]=0;
  config_data[6]++;
  uint8_t day=months[config_data[2]-1];
  if((config_data[1]%4==0)&&(config_data[2]==2))day=29;
  if(++config_data[3]>day){
    config_data[3]=1;
    if(++config_data[2]>12){
      config_data[2]=1;
      config_data[1]++;
    }
  }
  uint16_t words_index[100]={0};
  uint8_t words_evaluate[100]={0};
  uint16_t* book;
  uint16_t num;
  if(config_data[4]==1){
    book=index1;
    num=742;
  }
  else if(config_data[4]==2){
    book=index2;
    num=1788;
  }
  else if(config_data[4]==3){
    book=NULL;
    num=6007;
  }
  uint16_t new_words[100]={0};
  uint8_t new_words_num=0;
  // 获取该词书每个单词的学习进度,并按照进度排序
  for(int i=0;i<num;i++){
    uint16_t index1=0;
    if(book!=NULL)index1=book[i];
    else index1=i;
    uint8_t word_data[32]={0};  //0学习进度 1上次记忆天数 2本次需要记忆次数
    BSP_W25Qx_Read(word_data,(index1+26488)<<8,32);
    if(word_data[0]==0xFF){   //该单词尚未安排学习
      if(new_words_num<config_data[7]){
        new_words[new_words_num++]=index1;
      }
      continue;
    }
    else if(word_data[0]==100){ //该单词已经学习完成
      continue;
    }
    else{
      uint16_t space_day;
      if(config_data[6]-word_data[1]<0)space_day=256+config_data[6]-word_data[1];
      else space_day=config_data[6]-word_data[1];
      uint8_t rational_day;
      if(word_data[0]==1)rational_day=1;
      if(word_data[0]==10)rational_day=2;
      if(word_data[0]==20)rational_day=3;
      if(word_data[0]==30)rational_day=4;
      if(word_data[0]==40)rational_day=5;
      if(word_data[0]==50)rational_day=7;
      if(word_data[0]==60)rational_day=14;
      if(word_data[0]==70)rational_day=30;
      if(word_data[0]==80)rational_day=60;
      if(word_data[0]==90)rational_day=90;
      if(space_day<rational_day)continue;
      uint16_t evaluateScore=(space_day-rational_day)*90/rational_day;
      if(words_evaluate[99]>=evaluateScore)continue;
      uint8_t i=99;
      while(i>0 && words_evaluate[i-1]<evaluateScore){
        words_evaluate[i]=words_evaluate[i-1];
        words_index[i]=words_index[i-1];
        i--;
      }
      words_evaluate[i]=evaluateScore;
      words_index[i]=index1;
    }
  }
  BSP_W25Qx_Erase_Block(32496<<8);
  uint8_t review_words_num=0;
  for(int i=0;i<config_data[5];i++){
    if (words_index[i]==0)break;
    review_words_num++;
  }
  BSP_W25Qx_Write((uint8_t *)words_index,(32496<<8),config_data[5]*2);
  BSP_W25Qx_Write((uint8_t *)new_words,(32496<<8)+256,new_words_num*2);
  char buff1[128];
  sprintf(buff1,"新学%d个单词,复习%d个单词",new_words_num,review_words_num);

  OLED_NewFrame();
  OLED_PrintString(0, 16, buff1, 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  HAL_Delay(1000);
  KEY4_Down=1;
  condition = 2;
}

void funcionClearData(void){
  OLED_NewFrame();
  OLED_PrintString(0, 16, "数据清除中", 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  uint32_t cnt=26488;
  for(int i=0;i<32;i++){
    config_data[i]=0xFF;
  }
  while(cnt<26488+6007){
    if((cnt)%16==0){
      BSP_W25Qx_Erase_Block(cnt<<8);
    }
    cnt++;
  }
  OLED_NewFrame();
  OLED_PrintString(0, 16, "数据清除完成", 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  HAL_Delay(1000);
  condition=2;
  KEY4_Down=1;
}

void funcionStudyData(void){
  OLED_NewFrame();
  OLED_PrintString(0, 0, "学习统计", 'B', OLED_COLOR_NORMAL);
  char buff1[50]={0};
  if(config_data[11]==0xFF && config_data[10]==0xFF){
    config_data[11]=0;
    config_data[10]=0;
  }
  if(config_data[9]==0xFF && config_data[8]==0xFF){
    config_data[9]=0;
    config_data[8]=0;
  }
  if(config_data[6]==0xFF){
    config_data[6]=0;
  }
  sprintf(buff1, "学习单词总数:%d", config_data[11]+(config_data[10]<<8));
  OLED_PrintString(0, 16, buff1, 'A', OLED_COLOR_NORMAL);
  sprintf(buff1, "复习单词总数:%d", config_data[9]+(config_data[8]<<8));
  OLED_PrintString(0, 16+15, buff1, 'A', OLED_COLOR_NORMAL);
  sprintf(buff1, "学习总天数:%d", config_data[6]);
  OLED_PrintString(0, 16+15*2, buff1, 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}


void funcionStudySettingSelectCore(void){
  config_data[4]=1;
  char buff1[128]="词书切换成功";
  OLED_NewFrame();
  OLED_PrintString(0, 16, buff1, 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  HAL_Delay(1000);
  KEY4_Down=1;
  condition = 2;
}

void funcionStudySettingSelectHighFrequency(void){
  config_data[4]=2;
  char buff1[128]="词书切换成功";
  OLED_NewFrame();
  OLED_PrintString(0, 16, buff1, 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  HAL_Delay(1000);
  KEY4_Down=1;
  condition = 2;  
}

void funcionStudySettingSelectAllWord(void){
  config_data[4]=3;
  char buff1[128]="词书切换成功";
  OLED_NewFrame();
  OLED_PrintString(0, 16, buff1, 'A', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  HAL_Delay(1000);
  KEY4_Down=1;
  condition = 2;
}

void funcionStudySettingWordNumReview(void){
  condition = 2;
  if (config_data[5]==0xFF) config_data[5]=10;
  if(KEY1_Down==1){
    if(config_data[5]+10<=100)config_data[5]+=10;
  }
  if(KEY2_Down==1){
    if(config_data[5]-10>=10)config_data[5]-=10;
  }
  OLED_NewFrame();
  OLED_PrintString(0, 0, "复习单词数", 'B', OLED_COLOR_NORMAL);
  char buff1[10]={0};
  sprintf(buff1, "%d", config_data[5]);
  OLED_PrintString(0, 16, buff1, 'B', OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}

void funcionStudySettingWordNumNew(void){
  condition = 2;
  if (config_data[7]==0xFF) config_data[7]=10;
  if(KEY1_Down==1){
    if(config_data[7]+10<=100)config_data[7]+=10;
  }
  if(KEY2_Down==1){
    if(config_data[7]-10>=10)config_data[7]-=10;
  }
  OLED_NewFrame();
  OLED_PrintString(0, 0, "新学单词数", 'B', OLED_COLOR_NORMAL);
  char buff1[10]={0};
  sprintf(buff1, "%d", config_data[7]);
  OLED_PrintString(0, 16, buff1, 'B', OLED_COLOR_NORMAL);
  OLED_ShowFrame();  
}

Menu mainMenu = {"主菜单", &mainMenu, NULL, 0, NULL};
Menu recite = {"背词", &mainMenu, NULL, 0, NULL};
Menu study_start = {"开始学习", &recite, NULL, 0, funcionStudyStart};
Menu study_newDay = {"下一天", &recite, NULL, 0, funcionStudyNewDay};
Menu study_data = {"学习统计", &recite, NULL, 0, funcionStudyData};
Menu study_setting = {"学习设置", &recite, NULL, 0, NULL};
Menu study_setting_select = {"选择词书", &study_setting, NULL, 0, NULL};
Menu study_setting_select_core = {"核心单词750", &study_setting_select, NULL, 0, funcionStudySettingSelectCore};
Menu study_setting_select_highFrequency = {"高频单词1800", &study_setting_select, NULL, 0, funcionStudySettingSelectHighFrequency};
Menu study_setting_select_allWord = {"全部单词6000", &study_setting_select, NULL, 0, funcionStudySettingSelectAllWord};
Menu study_setting_wordNum = {"每日单词数", &study_setting, NULL, 0, NULL};
Menu study_setting_ClearData = {"清空学习数据", &study_setting, NULL, 0, funcionClearData};
Menu study_setting_wordNum_review = {"复习数量", &study_setting_wordNum, NULL, 0, funcionStudySettingWordNumReview};
Menu study_setting_wordNum_new = {"新学数量", &study_setting_wordNum, NULL, 0, funcionStudySettingWordNumNew};
Menu dictionary = {"词书", &mainMenu, NULL, 0, NULL};
Menu core = {"核心单词750", &dictionary, NULL, 0, funcionCore};
Menu highFrequency= {"高频单词1800", &dictionary, NULL, 0, funcionHighFrequency};
Menu allWord = {"全部单词6000", &dictionary, NULL, 0, funcionAllWord};
Menu search = {"单词查找", &dictionary, NULL, 0, funcionSearch};
Menu testMenu = {"测试菜单", &mainMenu, NULL, 0, NULL};
Menu test_key = {"按键测试", &testMenu, NULL, 0, keyTest};
Menu test_menu = {"多级菜单测试", &testMenu, NULL, 0, NULL};
Menu test_debug = {"debug测试", &testMenu, NULL, 0, testDebug};
Menu test_menu_1 = {"多级菜单1", &test_menu, NULL, 0, NULL};
Menu test_menu_2 = {"多级菜单2", &test_menu, NULL, 0, NULL};
Menu test_menu_3 = {"多级菜单3", &test_menu, NULL, 0, NULL};
Menu test_menu_4 = {"多级菜单4", &test_menu, NULL, 0, NULL};
Menu test_menu_5 = {"多级菜单5", &test_menu, NULL, 0, NULL};
Menu test_menu_6 = {"多级菜单6", &test_menu, NULL, 0, NULL};
Menu test_menu_7 = {"多级菜单7", &test_menu, NULL, 0, NULL};
Menu test_menu_8 = {"多级菜单8", &test_menu, NULL, 0, NULL};
Menu test_menu_1_1 = {"多级菜单1-1", &test_menu_1, NULL, 0, NULL};
Menu test_menu_1_2 = {"多级菜单1-2", &test_menu_1, NULL, 0, NULL};
Menu test_menu_1_1_1 = {"多级菜单1-1-1", &test_menu_1_1, NULL, 0, NULL};
Menu test_menu_1_1_2 = {"多级菜单1-1-2", &test_menu_1_1, NULL, 0, NULL};
Menu settingMenu = {"设置", &mainMenu, NULL, 0, NULL};
Menu setting_contrast = {"亮度调节", &settingMenu, NULL, 0, contrast_setting};
Menu setting_day = {"日期设置", &settingMenu, NULL, 0, day_setting};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main4Word(void)
{
  BSP_W25Qx_Init();
  BSP_W25Qx_Read(config_data,8192<<8,32);
  HAL_TIM_Base_Start_IT(&htim2);
  OLED_Init();
  OLED_SendCmd(0x81);
  OLED_SendCmd(config_data[0]);
  if(config_data[1]==0xFF)config_data[1]=24;
  if(config_data[2]==0xFF)config_data[2]=8;
  if(config_data[3]==0xFF)config_data[3]=22;

  // //主机代码，多台机子拷贝数据使用
  // //将w25q64数据拷贝给从机
  // uint16_t idx=0;
  // while(idx<26488){
  //   uint8_t trans_data[256]={0};
  //   uint8_t receive_data[2]={0};
  //   //发送数据地址
  //   trans_data[0]=idx>>8;
  //   trans_data[1]=idx&0xFF;
  //   HAL_Delay(1);
  //   HAL_UART_Transmit(&huart1,trans_data,2,HAL_MAX_DELAY);
  //   HAL_UART_Receive(&huart1,receive_data,2,HAL_MAX_DELAY);
  //   //校验从机应答
  //   if(receive_data[0]!=trans_data[0]||receive_data[1]!=trans_data[1]){
  //     break;
  //   }
  //   BSP_W25Qx_Read(trans_data,idx<<8,256);
  //   HAL_Delay(1);
  //   HAL_UART_Transmit(&huart1,trans_data,256,HAL_MAX_DELAY);
  //   //接受从机返回应答
  //   HAL_UART_Receive(&huart1,trans_data,1,HAL_MAX_DELAY);
  //   if(idx%100==0){
  //     char buf[20]={0};
  //     sprintf(buf, "transmit:%d", idx);
  //     print1(buf);
  //   }
  //   idx++;
  // }



  // 全片擦除
  // for(int i=0;i<0x7FFF-16;i+=16){
  //   BSP_W25Qx_Erase_Block(i<<8);
  //   char buf[20]={0};
  //   sprintf(buf, "擦除:%d", i);
  //   print1(buf);
  // }


  //以下为烧录示例，具体按需求更改
  // 将字模数据写入到w25q64中

  // //12号字模烧录,修改x值为第x次烧录（初始为0）
  // uint8_t x=0;
  // for(int y=0;y<250;y++){
  //   BSP_W25Qx_Write((uint8_t *)(font12x12.chars)+(y*28),(y+250*x)<<8,28);
  //   sprintf(buf, "%d:0x%02X,0x%02X,0x%02X", y,*(uint8_t *)(font12x12.chars)+(y*28)+0,*(uint8_t *)(font12x12.chars)+(y*28)+1,*(uint8_t *)(font12x12.chars)+(y*28)+2);
  //   print1(buf);
  // }

  // // //12号字最后一次烧录，只有20个
  // uint8_t x=14;
  // for(int y=0;y<20;y++){
  //   BSP_W25Qx_Write((uint8_t *)(font12x12.chars)+(y*28),(y+250*x)<<8,28);
  //   sprintf(buf, "%d:0x%02X,0x%02X,0x%02X", y,*(uint8_t *)(font12x12.chars)+(y*28)+0,*(uint8_t *)(font12x12.chars)+(y*28)+1,*(uint8_t *)(font12x12.chars)+(y*28)+2);
  //   print1(buf);
  // }

  //15号字模烧录,修改x值为第x次烧录（初始为0）
  // uint8_t x=0;
  // for(int y=0;y<250;y++){
  //   BSP_W25Qx_Write((uint8_t *)(font15x15.chars)+(y*34),(y+250*x+4096)<<8,34);
  //   sprintf(buf, "%d:0x%02X,0x%02X,0x%02X", y,*(uint8_t *)(font15x15.chars)+(y*34)+0,*(uint8_t *)(font15x15.chars)+(y*34)+1,*(uint8_t *)(font15x15.chars)+(y*34)+2);
  //   print1(buf);
  // }

  // //15号字最后一次烧录，只有20个
  // uint8_t x=14;
  // for(int y=0;y<20;y++){
  //   BSP_W25Qx_Write((uint8_t *)(font15x15.chars)+(y*34),(y+250*x+4096)<<8,34);
  //   sprintf(buf, "%d:0x%02X,0x%02X,0x%02X", y,*(uint8_t *)(font15x15.chars)+(y*34)+0,*(uint8_t *)(font15x15.chars)+(y*34)+1,*(uint8_t *)(font15x15.chars)+(y*34)+2);
  //   print1(buf);
  // }


  // extern uint8_t xx;
  // extern uint8_t word[][15];
  // for(int y=0;y<700;y++){
  //   BSP_W25Qx_Write((uint8_t *)word+(y*15),(y+700*xx+12288)<<8,15);
  // }
  // for(int y=0;y<28;y++){
  //   BSP_W25Qx_Write((uint8_t *)trans+(y*256),(y+65*xx+20480)<<8,256);
  //   sprintf(buf, "%d:0x%02X,0x%02X,0x%02X", y,*(uint8_t *)trans+(y*256)+0,*(uint8_t *)trans+(y*256)+1,*(uint8_t *)trans+(y*256)+2);
  //   print1(buf);
  // }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //数据统计
  // uint32_t cnt=26488,cnt1=0;
  // uint8_t z[2]={0};
  // while(cnt<26488+6007){
  //   BSP_W25Qx_Read(z,cnt<<8,1);
  //   if(z[0]!=0xFF){
  //     cnt1++;
  //     sprintf(buf, "%d:0x%02X 0x%02X", cnt,z[0],z[1]);
  //     print1(buf);
  //   }
  //   // else{
  //   // }
  //   // if((cnt)%16==0){
  //   //   BSP_W25Qx_Erase_Block(cnt<<8);
  //   // }
  //   cnt++;
  // }
  // sprintf(buf, "%d", cnt1,z[0]);
  // print1(buf);


  /*
    w25q64内容：
    0-4095页 12号汉字字模
    4096-8191页 15号汉字字模
    8192页 配置命令
    12288页 单词
    20480页 汉译
    26488页 单词记忆水平
    32496页 今日单词索引
  */

  //每级菜单参数包括：菜单名称，是否有下一级菜单，下一级菜单列表，上一级菜单，菜单执行的操作

  //词书
  dictionary.sonCount = 4;
  dictionary.sonMenus = malloc(sizeof(Menu*) * dictionary.sonCount);
  dictionary.sonMenus[0] = &core;
  dictionary.sonMenus[1] = &highFrequency;
  dictionary.sonMenus[2] = &allWord;
  dictionary.sonMenus[3] = &search;

  //背词
  recite.sonCount = 4;
  recite.sonMenus = malloc(sizeof(Menu*) * recite.sonCount);
  recite.sonMenus[0] = &study_start;
  recite.sonMenus[1] = &study_newDay;
  recite.sonMenus[2] = &study_data;
  recite.sonMenus[3] = &study_setting;

  //主菜单
  mainMenu.sonCount = 4;
  mainMenu.sonMenus = malloc(sizeof(Menu*) * mainMenu.sonCount);
  mainMenu.sonMenus[0] = &recite;
  mainMenu.sonMenus[1] = &dictionary;
  mainMenu.sonMenus[2] = &settingMenu;
  mainMenu.sonMenus[3] = &testMenu;
  

  //学习设置
  study_setting.sonCount = 3;
  study_setting.sonMenus = malloc(sizeof(Menu*) * study_setting.sonCount);
  study_setting.sonMenus[0] = &study_setting_select;
  study_setting.sonMenus[1] = &study_setting_wordNum;
  study_setting.sonMenus[2] = &study_setting_ClearData;


  //选择词书
  study_setting_select.sonCount = 3;
  study_setting_select.sonMenus = malloc(sizeof(Menu*) * study_setting_select.sonCount);
  study_setting_select.sonMenus[0] = &study_setting_select_core;
  study_setting_select.sonMenus[1] = &study_setting_select_highFrequency;
  study_setting_select.sonMenus[2] = &study_setting_select_allWord;

  //每日单词数
  study_setting_wordNum.sonCount = 2;
  study_setting_wordNum.sonMenus = malloc(sizeof(Menu*) * study_setting_wordNum.sonCount);
  study_setting_wordNum.sonMenus[0] = &study_setting_wordNum_new;
  study_setting_wordNum.sonMenus[1] = &study_setting_wordNum_review;



  testMenu.sonCount = 3;
  testMenu.sonMenus = malloc(sizeof(Menu*) * testMenu.sonCount);
  testMenu.sonMenus[0] = &test_key;
  testMenu.sonMenus[1] = &test_menu;
  testMenu.sonMenus[2] = &test_debug;
  
  test_menu.sonCount = 8;
  test_menu.sonMenus = malloc(sizeof(Menu*) * test_menu.sonCount);
  test_menu.sonMenus[0] = &test_menu_1;
  test_menu.sonMenus[1] = &test_menu_2;
  test_menu.sonMenus[2] = &test_menu_3;
  test_menu.sonMenus[3] = &test_menu_4;
  test_menu.sonMenus[4] = &test_menu_5;
  test_menu.sonMenus[5] = &test_menu_6;
  test_menu.sonMenus[6] = &test_menu_7;
  test_menu.sonMenus[7] = &test_menu_8;
  
  test_menu_1.sonCount = 2;
  test_menu_1.sonMenus = malloc(sizeof(Menu*) * test_menu_1.sonCount);
  test_menu_1.sonMenus[0] = &test_menu_1_1;
  test_menu_1.sonMenus[1] = &test_menu_1_2;
  
  test_menu_1_1.sonCount = 2;
  test_menu_1_1.sonMenus = malloc(sizeof(Menu*) * test_menu_1_1.sonCount);
  test_menu_1_1.sonMenus[0] = &test_menu_1_1_1;
  test_menu_1_1.sonMenus[1] = &test_menu_1_1_2;
  
  settingMenu.sonCount = 2;
  settingMenu.sonMenus = malloc(sizeof(Menu*) * settingMenu.sonCount);   
  settingMenu.sonMenus[0] = &setting_contrast;
  settingMenu.sonMenus[1] = &setting_day;
  


  // 菜单管理器，包含参数：当前菜单，当前索引
  struct menuManager manager = {
      .current = &mainMenu,
      .index = 0
  };
  // 程序状态
  // 0：位于菜单界面
  uint16_t wheel = 0;
  float sensitivity = 0.5;
	while (1)
	{
    print("update");
    // sprintf(buf, "%d", cnt1);
    // print1(buf);
    // continue;
    KEY1_Down=0;
    KEY2_Down=0;
    KEY3_Down=0;
    KEY4_Down=0;
    KEY5_Down=0;
    if(KEY1_State==1 && KEY1_LastState==0){
      KEY1_Count++;
      KEY1_Down=1;
    }
    KEY1_LastState = KEY1_State;
    if(KEY2_State==1 && KEY2_LastState==0){
      KEY2_Count++;
      KEY2_Down=1;
    }
    KEY2_LastState = KEY2_State;
    if(KEY3_State==1 && KEY3_LastState==0){
      KEY3_Count++;
      KEY3_Down=1;
    }
    KEY3_LastState = KEY3_State;
    if(KEY4_State==1 && KEY4_LastState==0){
      KEY4_Count++;
      KEY4_Down=1;
    }
    KEY4_LastState = KEY4_State;
    if(KEY5_State==1 && KEY5_LastState==0){
      KEY5_Count++;
      KEY5_Down=1;
    }
    //从机代码
  // uint16_t idx=0;
  // while(1){
  //   uint8_t receive_data[256]={0};
  //   uint8_t ind[2]={0};
  //   //接受主机数据地址
  //   HAL_UART_Receive(&huart1,ind,2,HAL_MAX_DELAY);
  //   HAL_Delay(1);
  //   HAL_UART_Transmit(&huart1,ind,2,HAL_MAX_DELAY);
  //   HAL_UART_Receive(&huart1,receive_data,256,HAL_MAX_DELAY);
  //   BSP_W25Qx_Write(receive_data,(ind[0]<<8|ind[1])<<8,256);
  //   //发送应答
  //   HAL_Delay(1);
  //   HAL_UART_Transmit(&huart1,receive_data,1,HAL_MAX_DELAY);
  //   if(((ind[0]<<8)|ind[1])%100==0){
  //     char buf[20]={0};
  //     sprintf(buf, "%d,%d,%d,%d", (ind[0]<<8|ind[1]),receive_data[0],receive_data[1],receive_data[2]);
  //     print1(buf);
  //   }
  //   if(((ind[0]<<8)|ind[1])==26487){
  //     break;
  //   }
  // }

  // char buff[20]="拷贝完成";
  // OLED_NewFrame();
  // OLED_PrintString(0, 16, buff, 'A', OLED_COLOR_NORMAL);
  // OLED_ShowFrame();
  // HAL_Delay(1000);
    KEY5_LastState = KEY5_State;
    uint16_t targetWheel = manager.index * 14;
    if(wheel+48<targetWheel+14){    //当前窗口无法覆盖光标选中项，窗口平滑下拉
      float temp=((targetWheel+14-wheel-48)*sensitivity);
      if(temp<1.0)temp=1.0;
      wheel+=(uint16_t)temp;
    }
    if(wheel>targetWheel){    //当前窗口无法覆盖光标选中项，窗口平滑上拉
      float temp=((wheel-targetWheel)*sensitivity);
      if(temp<1.0)temp=1.0;
      wheel-=(uint16_t)temp;
    }
    if(manager.current->func != NULL){
      manager.current->func(); // 执行当前菜单的函数
    }
    else{
      OLED_NewFrame();
      if(manager.current->sonCount==0){
        OLED_PrintString(0, 16, "没有下级菜单,要么是因为我没做,要么是卡bug了,试试返回重进", 'A', OLED_COLOR_NORMAL);
      }
      else{
        for(uint16_t i=0;i<manager.current->sonCount;i++){
          if(i==manager.index){
            OLED_PrintString(0, i*14+16-wheel, manager.current->sonMenus[i]->text, 'A', OLED_COLOR_REVERSED);
          }
          else{
            OLED_PrintString(0, i*14+16-wheel, manager.current->sonMenus[i]->text, 'A', OLED_COLOR_NORMAL);
          }
        }
      }
      OLED_DrawFilledRectangle(0, 0, 128, 16, OLED_COLOR_REVERSED);
      if(manager.current==&mainMenu){
        char buf[20];
        sprintf(buf,"%d月%d日",config_data[2],config_data[3]);
        OLED_PrintString(0,0,buf,'B',OLED_COLOR_NORMAL);
      }
      else OLED_PrintString(0, 0, manager.current->text, 'B', OLED_COLOR_NORMAL);
      OLED_ShowFrame();
    }
    // drawWord(index1[1]);
    //按键操作
    if(KEY1_Down==1){  //上拉
      if(manager.index==0)manager.index=manager.current->sonCount;
      manager.index--;
    }
    if(KEY2_Down==1){   //下拉
      manager.index++;
      if(manager.index>=manager.current->sonCount)manager.index=0;
    }
    if(KEY3_Down==1){ //进入下一级菜单
      if(manager.current->sonCount==0){
        continue;
      }
      manager.current = manager.current->sonMenus[manager.index];
      manager.index = 0;
      wheel = 0;
    }
    if(KEY4_Down==1){ //返回上一级菜单
      if(condition == 5)continue; //状态五禁用返回键
      manager.current = manager.current->parentMenu;
      manager.index = 0;
      wheel = 0;
      if(condition == 1 || condition == 2){   //保存亮度/时间数据;
        BSP_W25Qx_Erase_Block(8192<<8);
        BSP_W25Qx_Write(config_data,8192<<8,32);
      }
      condition = 0;
    }
	}
}

void TRoutine4Word() {
  if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_SET){
    KEY1_State=1;
  }else{
    KEY1_State=0;
  }
  if(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_SET){
    KEY2_State=1;
  }else{
    KEY2_State=0;
  }
  if(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_SET){
    KEY3_State=1;
  }else{
    KEY3_State=0;
  }
  if(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_SET){
    KEY4_State=1;
  }else{
    KEY4_State=0;
  }
}
