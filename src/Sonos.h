#ifndef H_SONOS
#define H_SONOS

#include <AL/al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include <vector>

#define SONOS_BUFFER_SIZE     32768       // 32 KB buffers

using namespace std;


struct Sonos
{
    /*** Member variables ***/
    ALint m_state;                  // The state of the sound source
    ALuint m_bufferID;              // The OpenAL sound buffer ID
    ALuint m_sourceID;              // The OpenAL sound source
    ALenum m_format;                // The sound data m_format
    ALsizei m_freq;                 // The frequency of the sound data
    vector<char> m_buffer;          // The sound buffer data from file

    /*** Local functions ***/
    int Initialize();
    int Destroy();

    // Typical call would look something like Sonos::PlayOggFile(soundFilePath, m_buffer, m_format, m_freq);
    void PlayOggFile(const char *filename, vector<char> &buffer, ALenum &format, ALsizei &frequency);
};

int Sonos::Initialize()
{
    // Initialize the OpenAL library: You can pass special initialization arguments here
    alutInit(0, 0);     /* alutInit(&argc, argv); */

    // Create the sound buffer and source
    alGenBuffers(1, &m_bufferID);
    alGenSources(1, &m_sourceID);

    // Set the source and listener to the same 3D position
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(m_sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
    return 1;
}

int Sonos::Destroy()
{
    // Clean up the sound buffer and source and clean up the OpenAL library
    alDeleteBuffers(1, &m_bufferID);
    alDeleteSources(1, &m_sourceID);
    alutExit();
    return 1;
}

void Sonos::PlayOggFile(const char *fileName, vector<char> &buffer, ALenum &format, ALsizei &frequency)
{
    int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[SONOS_BUFFER_SIZE];                // Local fixed size array
    FILE *f;

    // Attempt to read the local Ogg file...
    f = fopen(fileName, "rb");
    if (f == 0)
    {
        cerr << "Cannot open " << fileName << " for reading..." << endl;
        exit(-1);
    }

    vorbis_info *pInfo;
    OggVorbis_File oggFile;

    // Try opening the given file
    if (ov_open(f, &oggFile, NULL, 0) != 0)
        {
        cerr << "Error opening " << fileName << " for decoding..." << endl;
        exit(-1);
        }
    // end if

    // Get some information about the OGG file
    pInfo = ov_info(&oggFile, -1);

    // Check the number of channels... always use 16-bit samples
    if (pInfo->channels == 1)
        m_format = AL_FORMAT_MONO16;
    else
        m_format = AL_FORMAT_STEREO16;

    // The frequency of the sampling rate
    m_freq = pInfo->rate;

    // Keep reading until all is read
    do
    {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read(&oggFile, array, SONOS_BUFFER_SIZE, endian, 2, 1, &bitStream);

        if (bytes < 0)
        {
            ov_clear(&oggFile);
            cerr << "Error decoding " << fileName << "..." << endl;
            exit(-1);
        }

        // Append to end of buffer
        buffer.insert(buffer.end(), array, array + bytes);
    }
    while (bytes > 0);

    // Clean up!
    ov_clear(&oggFile);

    // Upload sound data to buffer
    alBufferData(m_bufferID, m_format, &m_buffer[0], static_cast<ALsizei>(m_buffer.size()), m_freq);

    // Attach sound buffer to source
    alSourcei(m_sourceID, AL_BUFFER, m_bufferID);

    // Finally, play the sound!!!
    alSourcePlay(m_sourceID);

    // This is an example loop to shut down Sonos once the device is not playing anymore
    //do
    //{
    //    // Query the m_state of the souce
    //    alGetSourcei(m_sourceID, AL_SOURCE_STATE, &m_state);
    //}
    //while (m_state != AL_STOPPED);
}

#endif /* H_SONOS */
