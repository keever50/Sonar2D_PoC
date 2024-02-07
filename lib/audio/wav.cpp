#include <Arduino.h>
#include <wav.h>
#include <LittleFS.h>
#include <logging.h>

inline int32_t charptr_to_int32(char* ptr)
{
    return *(int32_t*)ptr;
}

void wav_print_pcm_info(wav_pcm_s *wav)
{
    Serial.printf("Data size: %d bytes\n", wav->data_size);
    Serial.printf("Sample rate: %d\n", wav->sample_rate);
    Serial.printf("Channels: %d\n", wav->channels);
    Serial.printf("Bit depth: %d\n", wav->bits_per_sample);

    Serial.printf("Duration: %f seconds\n", ((float)wav->data_size/(float)wav->sample_rate)/wav->channels);

}

int wav_load_pcm(File file, wav_pcm_s *wav)
{

    /*RIFF*/
    uint32_t chunk_id;
    uint32_t chunk_size;
    uint32_t riff_format;

    /*Check chunk ID if RIFF*/
    file.readBytes((char*)&chunk_id,4);
    if(chunk_id!=WAV_RIFF_ID)
    {
        LOG_ERROR("WAV not loaded : Invalid chunk ID. Expected RIFF");
        Serial.println(chunk_id);
        return 1;
    }
    file.readBytes((char*)&chunk_size,4);
    /*Check riff format for WAVE*/
    file.readBytes((char*)&riff_format,4);
    if(riff_format!=WAV_WAVE_ID)
    {
        LOG_ERROR("WAV not loaded : Invalid format. Expected WAV");
        return 1;
    }

    /*WAVE*/
    uint32_t subchunk_id;
    uint32_t subchunk_size;
    uint16_t audio_format;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;

    /*Check sub chunk ID for 'fmt '*/
    file.readBytes((char*)&subchunk_id,4);
    if(subchunk_id!=WAV_FMT_ID) 
    {
        LOG_ERROR("WAV not loaded : Invalid sub chunk ID. Expected FMT");
        return 1;        
    }
    file.readBytes((char*)&subchunk_size,4);
    /*Check if PCM*/
    file.readBytes((char*)&audio_format,2);
    if(audio_format!=1) 
    {
        LOG_ERROR("WAV not loaded : Unexpected audio format. Expected PCM");
        return 1;        
    }    
    file.readBytes((char*)&channels,2);
    file.readBytes((char*)&sample_rate,4);
    file.readBytes((char*)&byte_rate,4);
    file.readBytes((char*)&block_align,2);
    file.readBytes((char*)&bits_per_sample,2);

    uint32_t subchunk_id2;
    uint32_t data_size;
    file.readBytes((char*)&subchunk_id2,4);
    file.readBytes((char*)&data_size,4);

    /*DATA*/
    char *data = (char*)malloc(data_size);
    file.readBytes(data, data_size);

    /*Fill in wav_pcb_s struct*/
    wav->chunk_size;
    wav->channels=channels;
    wav->sample_rate=sample_rate;
    wav->byte_rate=byte_rate;
    wav->block_align=block_align;
    wav->bits_per_sample=bits_per_sample;
    wav->data_size=data_size;
    wav->data=data;

    return 0;
}
int wav_unload_pcm(wav_pcm_s *wav)
{
    free(wav->data);
    return 0;
}