#include <RtAudio.h>
#include <fftw3.h>

#define SCALE 32767.0
#define BASE_RATE 0.005

unsigned int channels;
unsigned int frameCounter = 0;

fftw_complex *in, *out;
fftw_plan p;
const int N = 1024;

int cb(void *out, void *in, unsigned int nFrames, double dt,
       RtAudioStreamStatus status, void *data) {
  signed short *buffer = (signed short *)out;
  double *lastValues = (double *)data;

  double increment;
  for (int j = 0; j < channels; j++) {
    increment = BASE_RATE * (j + 1 + (j * 0.01));
    for (int i = 0; i < nFrames; i++) {
      *buffer++ = (signed short)(lastValues[j] * SCALE * 0.5);
      lastValues[j] += increment;
      if (lastValues[j] >= 1.0)
        lastValues[j] -= 2.0;
    }
  }

  frameCounter += nFrames;
  return 0;
}

int main(int argc, char *argv[]) {

  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

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

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
}