//
// james anderson, dsp sketch one, realtime bytebeat
//

#include <RtAudio.h>

int t;
int bb(void *out, void *in, unsigned int n, double dt,
       RtAudioStreamStatus status, void *data) {
  for (int i = 0; i < n; i++) {
    *((signed short *)out + i) = ((t << 1) ^ ((t << 1) + (t >> 7) & t >> 12)) |
                                 t >> (4 - (1 ^ 7 & (t >> 15))) | t >> 7;
    t++;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  RtAudio dac;
  RtAudio::StreamParameters op, ip;

  op.nChannels = 1;
  op.firstChannel = 1;
  op.deviceId = dac.getDefaultOutputDevice();

  ip.nChannels = 1;
  ip.firstChannel = 1;
  ip.deviceId = dac.getDefaultInputDevice();

  unsigned int bsize = 512;
  dac.openStream(&op, &ip, RTAUDIO_SINT16, 44100, &bsize, &bb, 0, 0, 0);
  dac.startStream();

  while (1)
    ;
}