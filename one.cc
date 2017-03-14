#include <RtAudio.h>

typedef signed short MY_TYPE;
#define SCALE 32767.0

#define BASE_RATE 0.005

unsigned int channels;
unsigned int frameCounter = 0;

int cb(void *out, void *in, unsigned int nFrames, double dt,
       RtAudioStreamStatus status, void *data) {
  MY_TYPE *buffer = (MY_TYPE *)out;
  double *lastValues = (double *)data;

  double increment;
  for (int j = 0; j < channels; j++) {
    increment = BASE_RATE * (j + 1 + (j * 0.01));
    for (int i = 0; i < nFrames; i++) {
      *buffer++ = (MY_TYPE)(lastValues[j] * SCALE * 0.5);
      lastValues[j] += increment;
      if (lastValues[j] >= 1.0)
        lastValues[j] -= 2.0;
    }
  }

  frameCounter += nFrames;
  // if ( checkCount && ( frameCounter >= nFrames ) ) 
  //   return 1;
  return 0;
}

int main(int argc, char *argv[]) {
  unsigned int bFrames, fs, device = 0, offset = 0, nFrames = 0;
  RtAudio dac;
  channels = 1;
  fs = 44100;

  double *data = (double *)calloc(channels, sizeof(double));

  bFrames = 512;
  RtAudio::StreamParameters op, ip;
  RtAudio::StreamOptions options;

  op.nChannels = 1;
  op.firstChannel = 1;
  op.deviceId = dac.getDefaultOutputDevice();

  ip.nChannels = 1;
  ip.firstChannel = 1;
  ip.deviceId = dac.getDefaultInputDevice();

  options.flags = RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;

  dac.openStream(&op, &ip, RTAUDIO_SINT16, fs, &bFrames, &cb, (void *)data,
                 &options, NULL);
  dac.startStream();

  while (1)
    ;
}