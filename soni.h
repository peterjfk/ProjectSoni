#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>
#define PI 3.1415

typedef unsigned char BYTE;
typedef uint16_t WORD;

//Wav file header information
typedef struct WAVHeader{
    char riff_header[4];
    int wav_size;           //total file size
    char wave_header[4];
    char fmt_header[4];
    int fmt_chunk_size;
    short audio_format;
    short num_channels;
    int sample_rate;    //the number of samples per second
    int byte_rate;  
    short sample_alighment;
    short bit_depth; //number of bytes per sample
    char data_header[4]; 
    int data_bytes; //number of bytes in data

}WAVHeader;

//calculate N point DFT using the recursive geortzel algorithm
double complex goertzel(int16_t * sa,int lsa,int k){   //pass sample array address and the frequency index probed
    double complex X;    //DFT 
    int n; double complex yk;
    
    X=0;    
    
    for (n=0;n<lsa; n++){
        double complex ss=((double) sa[n])/INT16_MAX;
        X=X*cexp(-I*((2*PI)/lsa)*k)+ ss;
    }
    

    return X; //N point DFT at frequency w=(2*pi*k/N)


}


