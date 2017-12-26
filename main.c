#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
struct WAV_Format {
    uint32_t ChunkID;   /* "RIFF" */
    uint32_t ChunkSize; /* 36 + Subchunk2Size */
    uint32_t Format;    /* "WAVE" */

    /* sub-chunk "fmt" */
    uint32_t Subchunk1ID;   /* "fmt " */
    uint32_t Subchunk1Size; /* 16 for PCM */
    uint16_t AudioFormat;   /* PCM = 1*/
    uint16_t NumChannels;   /* Mono = 1, Stereo = 2, etc. */
    uint32_t SampleRate;    /* 8000, 44100, etc. */
    uint32_t ByteRate;  /* = SampleRate * NumChannels * BitsPerSample/8 */
    uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */
    uint16_t BitsPerSample; /* 8bits, 16bits, etc. */

    /* sub-chunk "data" */
    uint32_t Subchunk2ID;   /* "data" */
    uint32_t Subchunk2Size; /* data size */
};

int main(void)
{
    FILE *fp = NULL;
    FILE *wp = NULL;
    struct WAV_Format wav;
    DIR *dir=NULL;
    struct dirent *pdirent=NULL;
    dir = opendir("./");
    char name[50];
    int lename=0;
    char buf[20];
    memset(name,0,50);
    printf("rename 's/.wax/.wav/' * :rename cmd\n");
    printf("note:Not repeatable\n");
    if( dir == NULL )
    {
        printf("open dir fail.\n");
    }

    while( (pdirent = readdir( dir )) != NULL)
    {


        if(strcmp(pdirent->d_name,"audio_fix")==0||strcmp(pdirent->d_name,".")==0||strcmp(pdirent->d_name,"..")==0)
        {
            continue;
        }
        else
        {

            fp = fopen(pdirent->d_name, "rb");
            if (!fp)
            {
                printf("can't open audio file\n");
                exit(1);
            }
            strcpy(name,pdirent->d_name);
            lename = strlen(pdirent->d_name);

            name[lename-1]='x';
            name[lename]='\0';

            wp = fopen(name, "w+");
            if (!wp)
            {
                printf("can't open audio file\n");
                exit(1);
            }


            printf("***********************\n");

            printf("%s->%s\n",pdirent->d_name,name);

            printf("***********************\n");
            fread(&wav, 1, sizeof(struct WAV_Format), fp);
            fwrite(&wav, 1, sizeof(struct WAV_Format), wp);
            printf("ChunkID \t%x\n", wav.ChunkID);
            printf("ChunkSize \t%d\n", wav.ChunkSize);
            printf("Format \t\t%x\n", wav.Format);
            printf("Subchunk1ID \t%x\n", wav.Subchunk1ID);
            printf("Subchunk1Size \t%d\n", wav.Subchunk1Size);
            printf("AudioFormat \t%d\n", wav.AudioFormat);
            printf("NumChannels \t%d\n", wav.NumChannels);
            printf("SampleRate \t%d\n", wav.SampleRate);
            printf("ByteRate \t%d\n", wav.ByteRate);
            printf("BlockAlign \t%d\n", wav.BlockAlign);
            printf("BitsPerSample \t%d\n", wav.BitsPerSample);
            printf("Subchunk2ID \t%x\n", wav.Subchunk2ID);
            printf("Subchunk2Size \t%d\n", wav.Subchunk2Size);
            while(wav.Subchunk2Size!=0)
            {
                fread(buf,1,1,fp);
                fwrite(buf,1,1,wp);
                wav.Subchunk2Size--;
            }

            fclose(fp);
            fclose(wp);
        }

     }
    closedir(dir);
    system("mkdir temp");
    system("mv  *.wax ./temp");
    return 0;
}

