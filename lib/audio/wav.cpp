/*Coded by Kevin Witteveen*/

#include <Arduino.h>
#include <wav.h>
#include <LittleFS.h>
#include <logging.h>

inline int32_t charptr_to_int32(uint8_t* ptr)
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
    file.read((uint8_t*)&chunk_id,4);
    if(chunk_id!=WAV_RIFF_ID)
    {
        LOG_ERROR("WAV not loaded : Invalid chunk ID. Expected RIFF");
        Serial.println(chunk_id);
        return 1;
    }
    file.read((uint8_t*)&chunk_size,4);
    /*Check riff format for WAVE*/
    file.read((uint8_t*)&riff_format,4);
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
    file.read((uint8_t*)&subchunk_id,4);
    if(subchunk_id!=WAV_FMT_ID) 
    {
        LOG_ERROR("WAV not loaded : Invalid sub chunk ID. Expected FMT");
        return 1;        
    }
    file.read((uint8_t*)&subchunk_size,4);
    /*Check if PCM*/
    file.read((uint8_t*)&audio_format,2);
    if(audio_format!=1) 
    {
        LOG_ERROR("WAV not loaded : Unexpected audio format. Expected PCM");
        return 1;        
    }    
    file.read((uint8_t*)&channels,2);
    file.read((uint8_t*)&sample_rate,4);
    file.read((uint8_t*)&byte_rate,4);
    file.read((uint8_t*)&block_align,2);
    file.read((uint8_t*)&bits_per_sample,2);

    /*Data info*/

    uint32_t subchunk_id2;
    uint32_t data_size;
    file.read((uint8_t*)&subchunk_id2,4);
    file.read((uint8_t*)&data_size,4);

    /*DATA*/
    uint8_t *data = (uint8_t*)malloc(data_size);
    if(data == nullptr)
    {
        LOG_CRITICAL("WAV not loaded : No memory!");
        return 1;    
    }
    file.read(data, data_size);

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

int wav_load_pcm_info(File file, wav_pcm_s *wav)
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

    /*Data info*/

    uint32_t subchunk_id2;
    uint32_t data_size;
    file.readBytes((char*)&subchunk_id2,4);
    file.readBytes((char*)&data_size,4);

    /*Fill in wav_pcb_s struct*/
    wav->chunk_size;
    wav->channels=channels;
    wav->sample_rate=sample_rate;
    wav->byte_rate=byte_rate;
    wav->block_align=block_align;
    wav->bits_per_sample=bits_per_sample;
    wav->data_size=data_size;

    return 0;   
}

int wav_unload_pcm(wav_pcm_s *wav)
{
    if(wav->data==nullptr)
    {
        LOG_CRITICAL("WAV not unloaded : got null pointer");
        return 1;
    };
    free(wav->data);
    return 0;
}

int Wav_PCM_Stream::read()
{
    int data = _file->read();
    if(_file->position()>=_file_data_end_pos)
    {
        _file->seek(_file_data_start_pos);
    }

    return data;
}

int Wav_PCM_Stream::available()
{
    return 0;
}
int Wav_PCM_Stream::peek()
{
    return 0;
}
void Wav_PCM_Stream::flush()
{

}
size_t Wav_PCM_Stream::write(uint8_t)
{
    return 0;
}

/*LIMIT THIS! When pos goes too far (two times too far), it corrupts.*/
void Wav_PCM_Stream::seek(int pos)
{
    int setpos = pos;
    if(pos>_file_data_end_pos)
    {
        setpos = _file_data_start_pos+(pos-_file_data_end_pos);
    }

    if(pos<_file_data_start_pos)
    {
        setpos = _file_data_end_pos-(_file_data_start_pos-pos);  
    }

    _file->seek(setpos);
    //Serial.println(setpos);

}

int Wav_PCM_Stream::begin(File *file)
{
    /*Check existance of the file and check if it is actually a file*/
    if(file==nullptr || !file->isFile())  return 1;
    _file = file;

    /*Load wav file*/
    int err = wav_load_pcm_info(*_file, &_wav);
    if(err) return err;

    _file_data_start_pos = _file->position();
    if(_file_data_start_pos<0) return 1;
    _file_data_end_pos = _file_data_start_pos+_wav.data_size;

    Serial.println(_file_data_start_pos);
    Serial.println(_file_data_end_pos);

    return 0;
}

int Wav_PCM_Stream::get_sample_rate()
{
    return _wav.sample_rate;
}

