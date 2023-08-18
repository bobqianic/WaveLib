# WaveLib
### A simple audio library implemented in pure C++.
## Features
- [ ] `LOAD` & `DUMP` audio (eg. `.wav` `.mp3`)
- [x] Print audio info
- [ ] Audio processing algo (eg. `WSOLA`)

## Demo
```cpp
#include "wavlib.h"

int main() {
    WAVLIB::FORMAT::WAV audio;
    WAVLIB::LOAD(R"(C:\Users\a13.wav)", audio);
    WAVLIB::print_info(audio);
}
```

```console
Type of format: PCM
Number of Channels: 1
Sample Rate: 16000
Bit Depth: 16
Data Rate: 32000
Data Size: 5717716
```
