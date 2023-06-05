#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265359


/******************************
*  Magic file format strings. *
******************************/
const char fChunkID[]     = {'R', 'I', 'F', 'F'};
const char fFormat[]      = {'W', 'A', 'V', 'E'};
const char fSubchunk1ID[] = {'f', 'm', 't', ' '};
const char fSubchunk2ID[] = {'d', 'a', 't', 'a'};

/********************************
* WriteWavePCM() configuration: *
* - 2 channels,                 *
* - frequency 44100 Hz.         *
********************************/
const unsigned short N_CHANNELS = 2;
const unsigned int SAMPLE_RATE = 44100;
const unsigned short BITS_PER_BYTE = 8;

bool WriteWavePCM(short* sound, size_t pairAmount, char* fileName){
    const static unsigned int fSubchunk1Size = 16;
    const static unsigned short fAudioFormat = 1;
    const static unsigned short fBitsPerSample = 16;

    unsigned int fByteRate = SAMPLE_RATE * N_CHANNELS *
                             fBitsPerSample / BITS_PER_BYTE;

    unsigned short fBlockAlign = N_CHANNELS * fBitsPerSample / BITS_PER_BYTE;
    unsigned int fSubchunk2Size;
    unsigned int fChunkSize;

    FILE* fout;
    size_t ws;

    if (!sound || !fileName || !(fout = fopen( fileName, "w" ))) return false;

    fSubchunk2Size = pairAmount * N_CHANNELS * fBitsPerSample / BITS_PER_BYTE;
    fChunkSize = 36 + fSubchunk2Size;

    // Writing the RIFF header:
    fwrite(&fChunkID, 1, sizeof(fChunkID),      fout);
    fwrite(&fChunkSize,  sizeof(fChunkSize), 1, fout);
    fwrite(&fFormat, 1,  sizeof(fFormat),       fout);

    // "fmt" chunk:
    fwrite(&fSubchunk1ID, 1, sizeof(fSubchunk1ID),      fout);
    fwrite(&fSubchunk1Size,  sizeof(fSubchunk1Size), 1, fout);
    fwrite(&fAudioFormat,    sizeof(fAudioFormat),   1, fout);
    fwrite(&N_CHANNELS,      sizeof(N_CHANNELS),     1, fout);
    fwrite(&SAMPLE_RATE,     sizeof(SAMPLE_RATE),    1, fout);
    fwrite(&fByteRate,       sizeof(fByteRate),      1, fout);
    fwrite(&fBlockAlign,     sizeof(fBlockAlign),    1, fout);
    fwrite(&fBitsPerSample,  sizeof(fBitsPerSample), 1, fout);

    /* "data" chunk: */
    fwrite(&fSubchunk2ID, 1, sizeof(fSubchunk2ID),      fout);
    fwrite(&fSubchunk2Size,  sizeof(fSubchunk2Size), 1, fout);

    /* sound data: */
    ws = fwrite(sound, sizeof(short), pairAmount * N_CHANNELS, fout);
    fclose(fout);
    return true;
}

/************************************
* Sound data goes here! *
************************************/
const unsigned int N_SAMPLE_PAIRS = 1048576;

int main(int argc, char* argv[]){
    time_t t;
    short* sound;
    int i;
    int j;
    bool status;
    char* file_name;
    const short BIT_RANGE = 0x8000; // Keep a reasonable volume, hex is bitran

    srand((unsigned) time(&t));
    sound = (short*) malloc( sizeof(short) * N_SAMPLE_PAIRS * N_CHANNELS );

    if (!sound)
    {
        puts("Could not allocate space for the sound data.");
        return (EXIT_FAILURE);
    }

    for (i = 0, j = 0; i < N_SAMPLE_PAIRS * N_CHANNELS; i += 2, j++)
    {

	// Form:
	// Starting with dust, adjust the densities over time, find a way to modularize the code as needed, or make multiple files. Explore how to move the
	// wav encoder into its own header for clarity.
	// Move to have ranges of samples or durations of sines to get the sine sound.
	short datum1;
   	
	  
	// Keep exploring this
	int x;   
	if (j % 2 == 0 || j % 3 == 0 || j % 5 == 0) {	
		datum1 = (0.4*BIT_RANGE)*(sin(2*PI*(110)*i/SAMPLE_RATE));
	} else {
		datum1 = (0.0*BIT_RANGE)*(sin(2*PI*110*i/SAMPLE_RATE));
	}

	/*
	 else if (j % 3211 == 0) {
		datum1 = (rand()*BIT_RANGE)*(sin(2*PI*3440*i/SAMPLE_RATE));
		j++;
	} else if (j % 5121 == 0) {
		datum1 = (rand()*BIT_RANGE)*(sin(2*PI*4240*i/SAMPLE_RATE));
		j++;
	} else if (j % 7771 == 0) {
		datum1 = (rand()*BIT_RANGE)*(sin(2*PI*5140*i/SAMPLE_RATE));
		j++;
	} else if (j % 19820 == 0) {
		datum1 = (rand()*BIT_RANGE)*(sin(2*PI*(rand()*10920)*i/SAMPLE_RATE));
		j++;
	} else {
		datum1 = 0;
		j++;
	}
*/

	
	short datum2 = 0;

	/*
	if (j % 23112 > 2 && j % 23112 < 10) {
                datum2 = (rand()*BIT_RANGE)*(sin(2*PI*2340*i/SAMPLE_RATE));
        } else if (j % 4214 == 0) {
                datum2 = (rand()*BIT_RANGE)*(sin(2*PI*3440*i/SAMPLE_RATE));
        } else if (j % 7424 == 0) {
                datum2 = (rand()*BIT_RANGE)*(sin(2*PI*4240*i/SAMPLE_RATE));
        } else if (j % 9799 == 0) {
                datum2 = (rand()*BIT_RANGE)*(sin(2*PI*5140*i/SAMPLE_RATE));
        } else if (j % 21892 == 0) {
		datum2 = (rand()*BIT_RANGE)*(sin(2*PI*(rand() % 7800)*i/SAMPLE_RATE));
        } else {
		datum2 = 0;
	}
	*/

        sound[i]     = datum1; // One channel.
        sound[i + 1] = datum2; // Another channel.
    }

    file_name = argc > 1 ? argv[1] : "default.wav";
    status = WriteWavePCM(sound, N_SAMPLE_PAIRS, file_name);
    free(sound);

    if (status)
    {
        printf("Your file has been compiled to: \"%s\"\n", file_name);
    }
    else
    {
        puts( "Something seems to have gone wrong." );
        return (EXIT_FAILURE);
    }

    return 0;
}
