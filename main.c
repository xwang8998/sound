#include <stdint.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <stdlib.h>

#include "wave.h"
#include <stdio.h>

#include <linux/soundcard.h>

#define Audio_Device "/dev/dsp"

//不同的声卡有着不同的播放参数，这些参数可以使用file命令获得

#define Sample_Size 24 //there're two kinds of bits,8 bits and 16 bits

#define Sample_Rate 48000 //sampling rate

#define ALL_PRINTF 1
#define BUFNUM 8002400
int play_sound(char *filename)

{

    struct stat stat_buf;

    unsigned char *buf = NULL;

    int handler,fd;

    int result;

    int arg,status;

   FILE *g;

    //打开声音文件，将文件读入内存

    fd=open(filename,O_RDONLY);

    if(fd<0) 
			
     return -1;

    if(fstat(fd,&stat_buf))

    {

        close(fd);

        return -2;

    }

    if(!stat_buf.st_size)

    {

        close(fd);

        return -3;

   }

   buf=malloc(stat_buf.st_size);

   if(!buf)

   {

       close(fd);

       return -4;

   }

   if(read(fd,buf,stat_buf.st_size)<0)

   {

       free(buf);

       close(fd);

       return -5;

   }

	g=fopen("pcm.txt","w");
for(int i=0;i<4096;i++)
{
	printf("pcm_data = %d\n",*(buf+i));
	fprintf(g,"%d\n",*(buf+i));
}	
	fclose(g);
   //打开声卡设备，并设置声卡播放参数，这些参数必须与声音文件参数一致

   handler=open(Audio_Device,O_WRONLY);

   if(handler==-1)

   {

       printf("open Audio_Device fail");

       return -1;

   }

 

   arg=Sample_Rate;

   status=ioctl(handler,SOUND_PCM_WRITE_RATE,&arg);

   if(status==-1)

   {

       printf("error from SOUND_PCM_WRITE_RATE ioctl");

       return -1;

   }

   arg=Sample_Size;

   status=ioctl(handler,SOUND_PCM_WRITE_BITS,&arg);

   if(status==-1)

   {

       printf("error from SOUND_PCM_WRITE_BITS ioctl");

       return -1;

   }

  //向端口写入，播放

   result=write(handler,buf,stat_buf.st_size);

   if(result==-1)

   {

       printf("Fail to play the sound!");

       return -1;

   }

  

   free(buf);

   close(fd);

   close(handler);

  

    return result;

}


int main(int argc,char *argv[])
{

	int date;
	int ldata,rdata;
	int t=0;	
	char  buffer[BUFNUM]={0};
	int  buf_l[30]={0};
	int  buf_r[30]={0};
	int32_t bdata;
	FILE *s;
   printf("\r\nsound test!!!\r\n");
  date = play_sound("/home/wangxiao8998/sound/2448_1k_0dBFS_30sec.WAV");
printf("date =%d\n",date);
   
	//fp =fopen("/home/wangxiao8998/sound/2448_1k_0dBFS_30sec.WAV","rb");
	FILE *fp = NULL;
    
     Wav wav;
     RIFF_t riff;
     FMT_t fmt;
     Data_t data;
 
     //fp = fopen("/home/wangxiao8998/sound/TheWaitingGame.wav", "rb");
	printf("##########################");
	printf("##%s", argv[1]);
     fp = fopen(argv[1], "rb");
     if (!fp) {
         printf("can't open audio file\n");
         exit(1);
     }
 
     fread(&wav, 1, sizeof(wav), fp);
	printf("sizeof(wav)=%ld\n",sizeof(wav));     
     riff = wav.riff;
     fmt = wav.fmt;
     data = wav.data;
 
     printf("ChunkID \t%c%c%c%c\n", riff.ChunkID[0], riff.ChunkID[1], riff.ChunkID[2], riff.ChunkID[3]);
    printf("ChunkSize \t%d\n", riff.ChunkSize);
     printf("Format \t\t%c%c%c%c\n", riff.Format[0], riff.Format[1], riff.Format[2], riff.Format[3]);
     
     printf("\n");
     
     printf("Subchunk1ID \t%c%c%c%c\n", fmt.Subchunk1ID[0], fmt.Subchunk1ID[1], fmt.Subchunk1ID[2], fmt.Subchunk1ID[3]);
     printf("Subchunk1Size \t%d\n", fmt.Subchunk1Size);
     printf("AudioFormat \t%d\n", fmt.AudioFormat);
     printf("NumChannels \t%d\n", fmt.NumChannels);
     printf("SampleRate \t%d\n", fmt.SampleRate);
     printf("ByteRate \t%d\n", fmt.ByteRate);
     printf("BlockAlign \t%d\n", fmt.BlockAlign);
     printf("BitsPerSample \t%d\n", fmt.BitsPerSample);
     
     printf("\n");
 
     printf("blockID \t%c%c%c%c\n", data.Subchunk2ID[0], data.Subchunk2ID[1], data.Subchunk2ID[2], data.Subchunk2ID[3]);
     printf("blockSize \t%d\n", data.Subchunk2Size);
     
     printf("\n");
    
     printf("duration \t%d\n", data.Subchunk2Size / fmt.ByteRate);
	s=fopen("ppp.pcm","w");
#if ALL_PRINTF==1
	for(unsigned int k=0;k<BUFNUM;k++)
	{
 		fread(&buffer[0],sizeof(char),fmt.BlockAlign,fp);
	//	fprintf(s,"%d\n",*(buffer+k));
	//	ldata=(((buffer[2]&0xff)<<16)|((buffer[1]&0xff)<<8)|(buffer[0]&0xff))&0xffffff;
	//	printf("**(%d) ldata[%x]= %x-%x-%x rdata=%x-%x-%x\n",k,ldata,buffer[0]&0xff,buffer[1]&0xff,buffer[2]&0xff,buffer[3]&0xff,buffer[4]&0xff,buffer[5]&0xff);
		if(fmt.BlockAlign==4)
		{

			ldata=(((buffer[1]&0xff)<<8)|(buffer[0]&0xff))&0xffff;
			rdata=(((buffer[3]&0xff)<<8)|(buffer[2]&0xff))&0xffff;
		}
		else if(fmt.BlockAlign==6)
		{

			ldata=(((buffer[2]&0xff)<<16)|((buffer[1]&0xff)<<8)|(buffer[0]&0xff))&0xffffff;
			rdata=(((buffer[5]&0xff)<<16)|((buffer[4]&0xff)<<8)|(buffer[3]&0xff))&0xffffff;
		}
		else;

		
	//	printf("**(%d) ldata[%x]= %x-%x rdata=%x-%x\n",k,ldata,buffer[0]&0xff,buffer[1]&0xff,buffer[2]&0xff,buffer[3]&0xff);
		for(int i=29;i>=1;i--)
		{
			buf_l[i]=buf_l[i-1];
			buf_r[i]=buf_r[i-1];
		}		
			buf_l[0]=ldata;
			buf_r[0]=rdata;
		if((buf_l[0]==buf_l[1])
		  && (buf_l[1]==buf_l[2])	
		  && (buf_l[2]==buf_l[3])	
		  && (buf_l[3]==buf_l[4])	
		  && (buf_l[4]==buf_l[5])	
		  && (buf_l[5]==buf_l[6])	
		  && (buf_l[6]==buf_l[7])	
		  && (buf_l[7]==buf_l[8])	
		  && (buf_l[8]==buf_l[9])	
		  && (buf_l[9]==buf_l[10])	
		  && (buf_l[10]==buf_l[11])	
		  && (buf_l[11]==buf_l[12])	
		  && (buf_l[12]==buf_l[13])	
		  && (buf_l[13]==buf_l[14])	
		  && (buf_l[14]==buf_l[15])	
		  && (buf_l[15]==buf_l[16])	
		  && (buf_l[16]==buf_l[17])	
		  && (buf_l[17]==buf_l[18])	
		  && (buf_l[18]==buf_l[19])	
		  && (buf_l[19]==buf_l[20])	
		  && (buf_l[20]==buf_l[21])	
		  && (buf_l[21]==buf_l[22])	
		  && (buf_l[22]==buf_l[23])	
		  && (buf_l[23]==buf_l[24])	
		  && (buf_l[24]==buf_l[25])	
		  && (buf_l[25]==buf_l[26])	
		  && (buf_l[26]==buf_l[27])	
		  && (buf_l[27]==buf_l[28])	
		  && (buf_l[28]==buf_l[29]))	
			printf("LLLL[%d] %d-%d ldata same at[%.4f] ldata=%x\n",k,fmt.BlockAlign,fmt.BitsPerSample,k*0.1/842310,buf_l[0]);
		else if((buf_r[0]==buf_r[1])
		  && (buf_r[1]==buf_r[2])	
		  && (buf_r[2]==buf_r[3])	
		  && (buf_r[3]==buf_r[4])	
		  && (buf_r[4]==buf_r[5])	
		  && (buf_r[5]==buf_r[6])	
		  && (buf_r[6]==buf_r[7])	
		  && (buf_r[7]==buf_r[8])	
		  && (buf_r[8]==buf_r[9])	
		  && (buf_r[9]==buf_r[10])	
		  && (buf_r[10]==buf_r[11])	
		  && (buf_r[11]==buf_r[12])	
		  && (buf_r[12]==buf_r[13])	
		  && (buf_r[13]==buf_r[14])	
		  && (buf_r[14]==buf_r[15])	
		  && (buf_r[15]==buf_r[16])	
		  && (buf_r[16]==buf_r[17])	
		  && (buf_r[17]==buf_r[18])	
		  && (buf_r[18]==buf_r[19])	
		  && (buf_r[19]==buf_r[20])	
		  && (buf_r[20]==buf_r[21])	
		  && (buf_r[21]==buf_r[22])	
		  && (buf_r[22]==buf_r[23])	
		  && (buf_r[23]==buf_r[24])	
		  && (buf_r[24]==buf_r[25])	
		  && (buf_r[25]==buf_r[26])	
		  && (buf_r[26]==buf_r[27])	
		  && (buf_r[27]==buf_r[28])	
		  && (buf_r[28]==buf_r[29]))	
			
			printf("RRRR[%d] %d-%d rdata same at[%.4f] rdata=%x\n",k,fmt.BlockAlign,fmt.    BitsPerSample,k*0.1/842310,buf_r[0]);
		else ;
		
	}	
#else
	while(t<BUFNUM){
		bdata = (buffer[t+2]<<16)|(buffer[t+1]<<8)|(buffer[t]);
	//	printf("%d\n",bdata);
	//	fprintf(s,"%X\n",buffer[t]&0xff);
	//	printf("%x\n",buffer[t]&0xff);
		fwrite(&buffer[0],sizeof(char),1,s);
		fread(&buffer[0],sizeof(char),1,fp);
		printf("[%d]%x-%x-%x\n",t,buffer[t],buffer[t+1]<<8,buffer[t+2]<<16);
	//	fprintf(s,"%d\n",*(buffer+t+1));
	//	fprintf(s,"%d\n",*(buffer+t+2));
	//	fprintf(s,"%d\n",*(buffer+t+3));
		t=t+1;
	}
#endif		
	fclose(s);
	fclose(fp);	
   exit(0);

}
