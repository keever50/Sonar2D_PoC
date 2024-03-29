/*Coded by Kevin Witteveen*/

#pragma once
#include <Arduino.h>
#include <LittleFS.h>

/*Contains characters RIFF as 32bit hex*/
#define WAV_RIFF_ID     0x46464952
/*Contains characters WAVE as 32bit hex*/
#define WAV_WAVE_ID     0x45564157
/*Contains characters 'fmt ' as 32bit hex*/
#define WAV_FMT_ID      0x20746D66

/*
The WAV in PCM audio format structure.
Contains the header of the file and pointer to allocated memory.
*/
typedef struct
{
    uint32_t chunk_size;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_size;
    uint8_t *data;
}wav_pcm_s;

/*PCM*/

/*
Prints out basic information about the wav in pcm format to serial.
Serial.begin(xxx); must have been called before.
*/
void wav_print_pcm_info(wav_pcm_s *wav);
/*
Takes an WAV file with the PCM audio format and writes its content into *wav, a wav_pcm_s struct.
This allocates ALL DATA into memory at once. (Call wav_unload_pcb to free)
Returns a number when an error has occured.
*/
int wav_load_pcm(File file, wav_pcm_s *wav);
/*Does the same as wav_load_pcb, but does NOT load data into RAM.*/
int wav_load_pcm_info(File file, wav_pcm_s *wav);

/*
Frees all the allocated memory
*/
int wav_unload_pcm(wav_pcm_s *wav);


/*Audio stream (Seperate this later)*/
class Audio_Stream : public Stream
{

};

/*Wav PCM stream*/
class Wav_PCM_Stream : public Audio_Stream
{
public:
    virtual int available() override;
    virtual int read() override;
    virtual int peek() override;
    virtual void flush() override;
    void seek( int pos );
    virtual size_t write(uint8_t) override;

    /*File should be static and alive!*/
    int begin(File *file);

    int get_sample_rate();


    wav_pcm_s _wav;
    File *_file;
    int _file_data_start_pos;
    int _file_data_end_pos;
};