#include <RtAudio.h>

typedef signed short SAMPTYPE;
typedef signed short* PSAMPTYPE;

class ddly {
public:
  ddly(int len, int maxlen):i(len), sz(maxlen) {
    _buf = new SAMPTYPE[sz];
    memset(_buf, 0, sz * sizeof(SAMPTYPE));
  }
  SAMPTYPE out(float pos) {
    int idx = (int) pos;
    int bufidx = i - idx;

    if(bufidx < 0) {
      bufidx += sz;
    }
    SAMPTYPE out = _buf[bufidx];
    return out;
  }

  void in(SAMPTYPE in0) {
    i = (i + 1) % sz;
    _buf[i] = in0;
  }   

private:
  int i, sz;
  PSAMPTYPE _buf;
};

ddly d(0xff, 0xff);

int cb(void *out, void *in, unsigned int n, double dt,
       RtAudioStreamStatus status, void *data) {
  for (int i = 0; i < n; i++) {
    SAMPTYPE in0 = *((SAMPTYPE *)in + i);
    SAMPTYPE wet = (d.out(441/2) * 0.7) + (in0 * 1.3);
    *((PSAMPTYPE)out + i) = wet;
    d.in(wet); // feedback
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

  dac.openStream(&op, &ip, RTAUDIO_SINT16, 44100, &bsize, &cb, 0, 0, 0);
  dac.startStream();

  while (1);
}