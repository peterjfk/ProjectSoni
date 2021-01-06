//Receiver main file
//Decodes binary encoded information in received.wav file and writes it to output.txt file
//ensure all WAV file properties match with the sender for correct decoding
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "soni.h"
#include <math.h>

int main()
{
    FILE * fr;  //received audio file pointer
    FILE * fo;  //output message file pointer
    WAVHeader H; //header object
    fr=fopen("transmit.wav","rb");  //read WAV file in binary mode
    fo=fopen("output.txt","wb");  //write to text file in binary mode
    fread(&H,1,sizeof(H),fr);   //read the WAV header info, advance pointer to the samples
    
    uint16_t s; //modulated message samples
    uint16_t * sptr=&s; //data pointer

    int kf=4000;  //modulation factor
    int fsig=2000;  //modulation frequency
    int sf=H.sample_rate;   //sampling frequency
    int bs=(int) (sf/fsig); //samples per fundamental period
    int nf=50*bs; //samples per period
    int K1=round(fmod(fsig*nf/sf,nf));  //frequency fsig (Hz) index     --zeros
    int K2=round(fmod((fsig+kf)*nf/sf,nf)); //frequency fsig+kf (Hz) index --ones

    int i=0; //ith frame
    int j=0;  //jth sample
    int m=0;  //mth decoded bit

    // masks to extract bit information from bytes
    BYTE bmask1=0b00000001;
    BYTE bmask0=0b00000000;

    BYTE bt; //Byte info from the samples
    BYTE * bptr=&bt; //address
    int ns=H.data_bytes/((H.bit_depth/8)*8); //number of samples
 

    //iterate all periods/frames within the samples of the WAV file with binary encoded information
    while (i<ns/nf){
      *bptr=0;
      //find all bits for a byte of information
      for(m=7; m>=0;m--){
      j=0;
      int16_t ps[nf]; //array to store the N samples in a period for a DFT
      //iterate all N samples in a period
      while(j<nf){
        //read all the sample values and add them to sample array
        fread(sptr,1,2,fr);
        ps[j]=*sptr;
        j++;
      }
    //perform DFT to obtain the bits corresponding to N samples using the geortzel algorithmn
     double X1=cabs(goertzel(ps,nf,K1));  //zeros frequency DFT magnitude
     double X2=cabs(goertzel(ps,nf,K2));  //ones frequency -//-
    
    //compare magnitudes to determine its a zero or a one
     if (X2/X1>1.0){
       *bptr=(bmask1<<m) | *bptr;  //assign 1 for the value for the mth bit of the byte
       //printf("X1=%lf X2=%lf\n",X1,X2);
     }
     else
     {
       *bptr=(bmask0<<m) | *bptr; //aasign 0 value for the mth bi
     }
     
     }
     fwrite(bptr,1,1,fo); //write the decoded byte information to the output file
     i++;
    }

    //close received file and decoded file streams
    fclose(fo);
    fclose(fr);

    return 0;
}

