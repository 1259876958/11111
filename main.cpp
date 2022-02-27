#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<graphics.h>
#include<math.h>
#include"tools.hpp"
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <math.h>
#include "13kfft.c" 
#pragma comment(lib, "winmm.lib")
#define DATASIZE 4096
#define VOLUMESIZE 37
struct MusicNote
{
	int noteName;
	double frequence;
}note[VOLUMESIZE] = {
	{-116,46.249},{-115,51.913},{-114,55.0},{-113, 61.735},{-112, 69.296},{-111, 77.782},{-110, 82.407},
	{-109, 92.499},{-108, 103.826},{-107, 110.0},{-106, 123.471},{-105, 138.591},{-104, 155.563},
	{-103, 164.814},{-102, 184.997},{-101, 207.652},{-100, 220.0},{-99, 246.942},{-98, 277.183},
	{-97, 311.127},{-96, 329.629}, {-95, 369.994}, {-94, 415.305}, {-93, 440.0},{-92, 493.883},
	{105, 554.365}, {106, 622.254}, {107, 659.255}, {108, 739.989},{109, 830.609},{110, 880.0},
	{111, 987.767},{112, 1108.731}, {113, 1244.598}, {114, 1318.520}, {115, 1479.978},{116, 1661.219},
};
enum{WID=1475,HEI=729};
void record()
{
    WAVEFORMATEX waveform;
    HWAVEIN hWaveIn = 0;

    LPWAVEHDR pWaveHdr = NULL;
    PBYTE pBuffer = NULL;
    MMRESULT error = 0;
    double i = 400000000;
    FILE* fp = NULL;
    char directory[50] = "d:\\volume.pcm";

    waveform.wFormatTag = WAVE_FORMAT_PCM;
    waveform.nChannels = 1;
    waveform.nSamplesPerSec = 8000;
    waveform.wBitsPerSample = 16;
    waveform.nBlockAlign = 2;
    waveform.nAvgBytesPerSec = 16000;
    waveform.cbSize = 0;

    pBuffer = (PBYTE)malloc(10 * waveform.nAvgBytesPerSec);
    *pBuffer = 0;
    pWaveHdr = (PWAVEHDR)malloc(sizeof(WAVEHDR));
    pWaveHdr->lpData = (LPSTR)pBuffer;
    pWaveHdr->dwBufferLength = 10 * waveform.nAvgBytesPerSec;
    pWaveHdr->dwBytesRecorded = 0;
    pWaveHdr->dwUser = 0;
    pWaveHdr->dwFlags = 0;
    pWaveHdr->dwLoops = 1;
    pWaveHdr->lpNext = NULL;
    pWaveHdr->reserved = 0;

    error = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, 0, 0, CALLBACK_NULL);
    error = waveInPrepareHeader(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
    error = waveInAddBuffer(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
    error = waveInStart(hWaveIn);
    while (i > 0)
    {
        i--;
    }

    waveInStop(hWaveIn);
    waveInReset(hWaveIn);

    fp = fopen(directory, "wb");

    fwrite(pBuffer, pWaveHdr->dwBytesRecorded, 1, fp);
    fclose(fp);

    free(pWaveHdr);
    pWaveHdr = NULL;
    free(pBuffer);
    pBuffer = NULL;
}

void getValue(double* data)
{
    int i;
    int length;
    signed short int datatmp = 0;
    signed short int dataChoose = 600;
    FILE* fp = NULL;
    char directory[50] = "d:\\volume.pcm";

    fp = fopen(directory, "rb");

    if (fp == NULL)
    {
        printf("[getValue]open volume.pcm error!\n");
        exit(0);
    }

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    for (i = 0; i < 8000; i++)
    {
        fread(&datatmp, 1, sizeof(datatmp), fp);
    }

    for (i = 0; i < length; i += 2)
    {
        fread(&datatmp, 1, sizeof(datatmp), fp);

        if (datatmp > dataChoose)
        {
            break;
        }
    }

    for (i = 0; i < DATASIZE; i++)
    {
        fread(&datatmp, 1, sizeof(datatmp), fp);
        *(data + i) = datatmp;
    }
}

void getFFTEnergy(double* data)
{
    double pr[DATASIZE], pi[DATASIZE], fr[DATASIZE], fi[DATASIZE];
    int i = 0;

    for (i = 0; i < DATASIZE; i++)
    {
        pr[i] = *(data + i);
        pi[i] = 0.0;
    }

    kfft(pr, pi, DATASIZE, 12, fr, fi, 0, 1);

    for (i = 0; i < DATASIZE; i++)
    {
        *(data + i) = pr[i];
    }
}

double getFrequence(double* data)
{
    int locate = -1;
    int i;
    double frequence;
    double max;

    max = *data;

    for (i = 0; i < (DATASIZE / 2); i++)
    {
        if (*(data + i) > max)
        {
            max = *(data + i);
            locate = i;
        }
    }
    if (locate < 0)
        return -1.5;
    frequence = locate * (8000.0 / DATASIZE);
    return frequence;
}
int showVolume(double fre)
{
    int i = 0;
    double xLeft = 0.0;
    double xRight = 0.0;
    if (fre <= note[0].frequence || fre >= note[36].frequence)
        return 0;
    else {
        for (i = 0; i < (VOLUMESIZE - 1); i++)
        {
            if (note[i].frequence<fre && note[i + 1].frequence>fre)
            {
                xLeft = fre - note[i].frequence;
                xRight = note[i + 1].frequence - fre;

                if (xLeft < xRight) {
                    if (i >= 12 && i <= 24) {
                        return note[i].noteName * 999 - 196;
                    }
                    else
                        return note[i].noteName;
                }
                else {
                    if (i + 1 >= 12 && i + 1 <= 24) {
                        return note[i + 1].noteName * 999 - 196;
                    }
                    else
                        return note[i + 1].noteName;
                }
            }
        }
    }
}
int main() {
    double frequence;
    double pcmData[DATASIZE];
	char r;
	initgraph(WID, HEI, 1);
	cleardevice();
	IMAGE aa;
	loadimage(&aa, "./1.jpg", WID, HEI);
	DWORD* paa = GetImageBuffer(&aa);
	putimage(0, 0, &aa);
	int** bb = HB(&aa);
	int* c = XX(&aa, bb);
	int** dd,i,j;
	dd = (int**)malloc(sizeof(int*) * 999);
	for (i = 0; i < 999; ++i)
		dd[i] = (int*)malloc(sizeof(int) * 6);
	for (i = 0; i < 999; ++i) {
		for (j = 0; j < 6; ++j)
			dd[i][j] = 0;
	}
	HH(WID, HEI, bb, c, dd);
	int p=0,q,q1,q2,p0;
	while (1) {
        printf("recording...\n");
        do {
            record();
            getValue(pcmData);
            getFFTEnergy(pcmData);
            frequence = getFrequence(pcmData);
        } while (frequence == -1.5);
        printf("%lf ", frequence);
        q = showVolume(frequence);
        printf("%d\n", q);
        if (q >= -999)
            p = BB(dd, p, q);
        else{
            q1 = (0 - q) % 1000;
            q2 = (q + q1)/1000;
            p0 = BB(dd, p, q1);
            if (p0 == -1)
                p = BB(dd, p, q2);
            else
                p = p0;
        }
		if (p == -1)
			break;
		BX(&aa, dd);
		cleardevice();
		putimage(0, 0, &aa);
	}
	return 0;
}