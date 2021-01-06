//Sender main file
//modify prototype in audacity for various audio properties such as:
//sampling frequency, number of channels, bit depth and etc


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soni.h"
#include <math.h>

int main()
{

    FILE *fp; //prototype WAV file pointer
    FILE *ft;   //transmitted WAV file pointer
    FILE *fd;
    WAVHeader wh;  //WAV header struct

    fp=fopen("prototype.wav","rb"); //read bytes mode
    fd=fopen("testdoc.txt","rb"); //read bytes mode
    ft=fopen("transmit.wav","wb");  //write bytes mode
    fread(&wh,1,sizeof(wh),fp);     //read typical file header info
    //determine required file size
    fseek(fd, 0, SEEK_END); // seek to end of file
    int size = ftell(fd); // get current file pointer
    rewind(fd); // beginning of file


    int16_t s;  //modulated sample value
    BYTE n;     //modulating signal sample values
    int16_t * sptr=&s;
    BYTE * nptr=&n;

    float sf=wh.sample_rate; //sampling frequency
    int fsig=2000; //modulating frequency
    int kf=4000;    //modulation factor
    int fpad=400;   //padding frequency
  
    int i=0;    //ith sample
    int k=0;    //kth bit
    int l=0;    //lth byte in message
    int m=0;    //mth bit in byte
    float t=0;      //time variable
    int K1=0;       //frequency probed X(k), k in DFT, zeros frequency
    int K2=0;       //ones frequency
    int bs=(int) (sf/fsig); //# of samples in block
    int nf=50*bs;  //number of periods per bit
    int wb=5;    //number of padding bytes
    wh.data_bytes=(wh.bit_depth/8)*nf*size*8;
    //wh.wav_size=wh.data_bytes+sizeof(wh);
    fwrite(&wh,1,sizeof(wh),ft);    //write header info
    
  
    //iterate all bytes in message
    while (l<size){

        if (l>=wb){
        fread(nptr,1,1,fd);
        }
     
    //iterate all bits in bytes of message   
        for (m=7; m>=0; m--){
        int j=0; //samples in block
     
    //iterate all samples in a period/frame
     while(j<nf){
            t=i/sf; //calculate t in seconds
            if (l<wb){
            *sptr=INT16_MAX*cos(2.0*PI*fpad*t); //modulated signal amplitude, y(t)=cos((Wc+Kf*x(t))*t)
            fwrite(sptr,1,2,ft);        //write info to transmitting file
            
            }
            else
            {

            *sptr=INT16_MAX*cos((2.0*PI*fsig+2.0*PI*kf*(0b00000001 & (*nptr >> m)))*t); //modulated signal amplitude, y(t)=cos((Wc+Kf*x(t))*t)
            fwrite(sptr,1,2,ft);        //write info to transmitting file
           
            }
            
  
            i++;  
            j++;
            
        }

        }
        l++;
    }
    
   
    //close all files streams
    fclose(fp); 
    fclose(ft);
    fclose(fd);

}
