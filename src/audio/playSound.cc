#include <AL/al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <unistd.h>

constexpr int BUFFER_SIZE=32768;     // 32 KB buffers

using namespace std;

uint64_t loadOgg(const char fileName[],
								 vector < char > &buffer,
								 ALenum &format, ALsizei &freq) {
  int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
  int bitStream;
  long bytes;
  char array[BUFFER_SIZE];    // Local fixed size array
  FILE *f = fopen(fileName, "rb");
  vorbis_info *pInfo;
  OggVorbis_File oggFile;
  ov_open(f, &oggFile, NULL, 0);
  pInfo = ov_info(&oggFile, -1);
	//	uint64_t numSamples = ov_pcm_total(&oggFile, -1);
	uint64_t totalTime = uint64_t(ov_time_total(&oggFile, -1) * 1000000);
  format = pInfo->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
  freq = pInfo->rate;
  // decode the OGG file and put the raw audio dat into the buffer
  do {
    // Read up to a buffer's worth of decoded sound data
    bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
    // Append to end of buffer
    buffer.insert(buffer.end(), array, array + bytes);
  } while (bytes > 0);
  ov_clear(&oggFile); // release the file resources at the end
	cerr << " length: " << totalTime << '\n';
	return totalTime;
}

int main(int argc, char *argv[]) {
  ALint state;             // The state of the sound source
  ALuint bufferID;         // The OpenAL sound buffer ID
  ALuint sourceID;         // The OpenAL sound source
  ALenum format;           // The sound data format
  ALsizei freq;            // The frequency of the sound data
  vector<char> buffer;     // The sound buffer data from file

  // Initialize the OpenAL library
  alutInit(&argc, argv);

  // Create sound buffer and source
  alGenBuffers(1, &bufferID);
  alGenSources(1, &sourceID);
  
  // Set the source and listener to the same location
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	for (int i = 1; i < argc; i++) {
		uint64_t lengthMicroSecs = loadOgg(argv[i], buffer, format, freq);

		// Upload sound data to buffer
		alBufferData(bufferID, format, &buffer[0], static_cast<ALsizei>(buffer.size()), freq);

		// Attach sound buffer to source
		alSourcei(sourceID, AL_BUFFER, bufferID);
	
		alSourcePlay(sourceID);    // Finally, play the sound!!!

		// This loop checks 100 times per second,
		// it would be better to receive a callback
		// see 
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state); // Query state of source
		usleep(lengthMicroSecs);
		do {
			alGetSourcei(sourceID, AL_SOURCE_STATE, &state); // Query state of source
			cout << "at the end\n";
			usleep(20);
		} while (state != AL_STOPPED);
	}
	alDeleteBuffers(1, &bufferID);     // Clean up sound buffer and source
	alDeleteSources(1, &sourceID);
	
	alutExit();    // Clean up the OpenAL library
	return 0;
}

