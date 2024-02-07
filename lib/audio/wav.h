#pragma once
#include <Arduino.h>
#include <LittleFS.h>

/*Contains characters RIFF as 32bit hex*/
#define WAV_RIFF_ID 0x46464952
/*Contains characters WAVE as 32bit hex*/
#define WAV_WAVE_ID 0x45564157
/*Contains characters 'fmt ' as 32bit hex*/
#define WAV_FMT_ID 0x20746D66

typedef struct
{
    uint32_t chunk_size;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_size;
    char *data;
}wav_pcm_s;

/*PCM*/
void wav_print_pcm_info(wav_pcm_s *wav);
int wav_load_pcm(File file, wav_pcm_s *wav);
int wav_unload_pcm(wav_pcm_s *wav);