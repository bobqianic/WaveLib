//
// Created by bobqianic on 18/08/2023.
//

// Include necessary libraries
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <complex>
#include <cmath>
#include <functional>

#if _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#ifndef WAVLIB_PI
#define WAVLIB_PI 3.1415927410125732
#endif

// Include guards
#ifndef WAV_LIB_H
#define WAV_LIB_H

class WAVLIB {
public:
    struct FORMAT {
        struct WAV {
            unsigned short format{};      // Type of format (1 is PCM)
            unsigned short channels{};    // Number of Channels
            unsigned int sample_rate{};   // Number of Samples per second
            unsigned int data_rate{};     // (Sample Rate * BitsPerSample * Channels) / 8
            unsigned short sample_size{}; // Bits per sample
            unsigned int data_size{};     // Size of the data section
            std::vector<int> audio;       // Audio data
        };
    };

    typedef std::vector<std::complex<float>> COMPLEX_VEC;

    static bool LOAD(const std::string& filename, FORMAT::WAV& audio) {
        std::ifstream file(filename, std::ios::binary);
        bool result = LOAD::WAV(file, audio);
        if (file.is_open()) {
            file.close();
        }
        return result;
    }

#if _WIN32
    static bool LOAD(const std::wstring& filename, FORMAT::WAV& audio) {
        std::ifstream file(ONLY_WIN32::WStringToString(filename), std::ios::binary);
        bool result = LOAD::WAV(file, audio);
        if (file.is_open()) {
            file.close();
        }
        return result;
    }
#endif

    static bool DUMP(const std::string& filename, FORMAT::WAV& audio) {
        std::ofstream file(filename, std::ios::binary);
        bool result = DUMP::WAV(file, audio);
        if (file.is_open()) {
            file.close();
        }
        return result;
    }

#if _WIN32
    static bool DUMP(const std::wstring& filename, FORMAT::WAV& audio) {
        std::ofstream file(ONLY_WIN32::WStringToString(filename), std::ios::binary);
        bool result = DUMP::WAV(file, audio);
        if (file.is_open()) {
            file.close();
        }
        return result;
    }
#endif

    static bool print(const FORMAT::WAV& audio) {
        return OUTPUT::audio_info(audio);
    }

    static bool print(const COMPLEX_VEC& vec) {
        return OUTPUT::vector_info(vec);
    }

    template<typename T>
    static bool print(T value) {
        std::cout << value << std::endl;
        return true;
    }

    struct F {
        static bool WSOLA(FORMAT::WAV& audio) {

        }
    };

    struct W {
        /** Window Functions **/
        static bool Hann(COMPLEX_VEC& out, const int window_length, const bool periodic=true) {
            return WINDOW::Hann(out, window_length, periodic);
        }

        static bool Hamming(COMPLEX_VEC& out, const int window_length, const bool periodic=true, const float alpha=0.54, const float beta=0.46) {
            return WINDOW::Hamming(out, window_length, periodic, alpha, beta);
        }

        static bool Kaiser(COMPLEX_VEC& out, const int window_length, const bool periodic=true, const float beta=12.0) {
            return WINDOW::Kaiser(out, window_length, periodic, beta);
        }

        static bool Blackman(COMPLEX_VEC& out, const int window_length, const bool periodic=true) {
            return WINDOW::Blackman(out, window_length, periodic);
        }

        static bool Bartlett(COMPLEX_VEC& out, const int window_length, const bool periodic=true) {
            return WINDOW::Bartlett(out, window_length, periodic);
        }
    };

private:
    struct endian {
        static void flipEndianness(char* data, unsigned int length) {
            std::reverse(data, data + length);
        }

        static bool LittleEndian() {
            union one {
                int value;
                char byte[sizeof(int)];
            };

            one tester{};
            tester.value = 1;
            return tester.byte[0] == 1;
        }

        static bool isLittleEndian() {
            static bool system = LittleEndian();
            return system;
        }
    };

    struct integer {
        static int16_t char_2_int16(const char* data, const uint64_t length = 2, const bool LittleEndian = true) {
            bool needSwap = endian::isLittleEndian() != LittleEndian;

            if (needSwap || length < 2) {
                char buffer[2] = {0};
                std::copy(data, data + length, LittleEndian ? buffer : buffer + (2 - length));
                if (needSwap && length > 1) {
                    LittleEndian ? std::reverse(buffer, buffer + length) : std::reverse(buffer + (2 - length), buffer + 2);
                }
                return *reinterpret_cast<const int16_t*>(buffer);
            }

            return *reinterpret_cast<const int16_t*>(data);
        }

        static uint16_t char_2_uint16(const char* data, const uint64_t length = 2, const bool LittleEndian = true) {
            bool needSwap = endian::isLittleEndian() != LittleEndian;

            if (needSwap || length < 2) {
                char buffer[2] = {0};
                std::copy(data, data + length, LittleEndian ? buffer : buffer + (2 - length));
                if (needSwap && length > 1) {
                    LittleEndian ? std::reverse(buffer, buffer + length) : std::reverse(buffer + (2 - length), buffer + 2);
                }
                return *reinterpret_cast<const uint16_t*>(buffer);
            }

            return *reinterpret_cast<const uint16_t*>(data);
        }

        static int32_t char_2_int32(const char* data, const uint64_t length = 4, const bool LittleEndian = true) {
            bool needSwap = endian::isLittleEndian() != LittleEndian;

            if (length == 2) {
                return static_cast<int32_t>(char_2_int16(data, 2, LittleEndian));
            }

            if (needSwap || length < 4) {
                char buffer[4] = {0};
                std::copy(data, data + length, LittleEndian ? buffer : buffer + (4 - length));
                if (needSwap && length > 1) {
                    LittleEndian ? std::reverse(buffer, buffer + length) : std::reverse(buffer + (4 - length), buffer + 4);
                }
                return *reinterpret_cast<const int32_t*>(buffer);
            }

            return *reinterpret_cast<const int32_t*>(data);
        }

        static uint32_t char_2_uint32(const char* data, const uint64_t length = 4, const bool LittleEndian = true) {
            bool needSwap = endian::isLittleEndian() != LittleEndian;

            if (length == 2) {
                return static_cast<uint32_t>(char_2_uint16(data, 2, LittleEndian));
            }

            if (needSwap || length < 4) {
                char buffer[4] = {0};
                std::copy(data, data + length, LittleEndian ? buffer : buffer + (4 - length));
                if (needSwap && length > 1) {
                    LittleEndian ? std::reverse(buffer, buffer + length) : std::reverse(buffer + (4 - length), buffer + 4);
                }
                return *reinterpret_cast<const uint32_t*>(buffer);
            }

            return *reinterpret_cast<const uint32_t*>(data);
        }

        static int64_t char_2_int64(const char* data, const uint64_t length = 8, const bool LittleEndian = true) {
            bool needSwap = endian::isLittleEndian() != LittleEndian;

            if (length == 2) {
                return static_cast<int64_t>(char_2_int16(data, 2, LittleEndian));
            } else if (length == 4) {
                return static_cast<int64_t>(char_2_int32(data, 4, LittleEndian));
            }

            if (needSwap || length < 8) {
                char buffer[8] = {0};
                std::copy(data, data + length, LittleEndian ? buffer : buffer + (8 - length));
                if (needSwap && length > 1) {
                    LittleEndian ? std::reverse(buffer, buffer + length) : std::reverse(buffer + (8 - length), buffer + 8);
                }
                return *reinterpret_cast<const int64_t*>(buffer);
            }

            return *reinterpret_cast<const int64_t*>(data);
        }

        static uint64_t char_2_uint64(const char* data, const uint64_t length = 8, const bool LittleEndian = true) {
            bool needSwap = endian::isLittleEndian() != LittleEndian;

            if (length == 2) {
                return static_cast<uint64_t>(char_2_uint16(data, 2, LittleEndian));
            } else if (length == 4) {
                return static_cast<uint64_t>(char_2_uint32(data, 4, LittleEndian));
            }

            if (needSwap || length < 8) {
                char buffer[8] = {0};
                std::copy(data, data + length, LittleEndian ? buffer : buffer + (8 - length));
                if (needSwap && length > 1) {
                    LittleEndian ? std::reverse(buffer, buffer + length) : std::reverse(buffer + (8 - length), buffer + 8);
                }
                return *reinterpret_cast<const uint64_t*>(buffer);
            }

            return *reinterpret_cast<const uint64_t*>(data);
        }
    };

    struct character {
        /*Convert integer to char array*/
        template<typename integer>
        static void int2Char(const integer& data, char* out, const bool LittleEndian = true) {
            const char* p = reinterpret_cast<const char*>(&data);
            std::copy(p, p + sizeof(decltype(data)), out);
            if (endian::isLittleEndian() != LittleEndian) {
                endian::flipEndianness(out, sizeof(decltype(data)));
            }
        }
    };

    struct DATA {
        static std::string read(std::ifstream& file, uint64_t length) {
            if (!file.is_open()) {
                return "";
            }
            std::string buffer(length, '\0'); // Preallocate the string with the given length
            file.read(&buffer[0], static_cast<int64_t>(length));
            return buffer;
        }

        static bool write(std::ofstream& file, const std::string& data) {
            if (!file.is_open()) {
                return false;
            }
            file.write(data.c_str(), static_cast<int64_t>(data.size()));
            return true;
        }

        template<typename integer>
        static bool write(std::ofstream& file, const integer& data) {
            if (!file.is_open()) {
                return false;
            }
            char buffer[sizeof(decltype(data))];
            character::int2Char(data, buffer);
            file.write(buffer, sizeof(decltype(data)));
            return true;
        }
    };

#if _WIN32
    struct ONLY_WIN32 {
        static std::string WStringToString(const std::wstring& wstr){
            int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
            std::string strTo(sizeNeeded, 0);
            WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], sizeNeeded, NULL, NULL);
            return strTo;
        }
    };
#endif

    struct LOAD {
        static bool WAV(std::ifstream& file, FORMAT::WAV& audio) {
            if (!file.is_open()) {
                return false;
            }

            // Read RIFF header
            if (DATA::read(file, 4) != "RIFF") {
                return false;
            }

            file.seekg(4, std::ios::cur);

            // Read File-Type header
            if (DATA::read(file, 4) != "WAVE") {
                return false;
            }

            file.seekg(8, std::ios::cur);

            // Read format
            audio.format = integer::char_2_int16(DATA::read(file, 2).c_str());

            // Read channel
            audio.channels = integer::char_2_int16(DATA::read(file, 2).c_str());

            // Read sample rate
            audio.sample_rate = integer::char_2_int32(DATA::read(file, 4).c_str());

            // Read data rate
            audio.data_rate = integer::char_2_int32(DATA::read(file, 4).c_str());

            file.seekg(2, std::ios::cur);

            // Read sample size
            audio.sample_size = integer::char_2_int16(DATA::read(file, 2).c_str());
            if (audio.sample_size < 8 || audio.sample_size > 32) {
                return false;
            }

            // Read data size
            while (!file.eof()) {
                if (DATA::read(file, 4) == "data") {
                    audio.data_size = integer::char_2_int32(DATA::read(file, 4).c_str());
                    break;
                }
                file.seekg(-3, std::ios::cur);
            }

            int sample_byte = audio.sample_size / 8;
            audio.audio.reserve(audio.data_size / sample_byte);
            std::unique_ptr<char[]> buffer(new char [1024 * 1024 * 4]);

            for (uint64_t i = 0; i < audio.audio.size(); i+= 1024 * 1024 * 4) {
                file.read(buffer.get(), 1024 * 1024 * 4);
                std::streamsize bytes_read = file.gcount();
                for (uint64_t j = 0; j < bytes_read; j+=sample_byte) {
                    audio.audio.push_back(integer::char_2_int32(buffer.get(), sample_byte));
                }
            }

            return true;
        }
    };

    struct DUMP {
        static bool WAV(std::ofstream& file, FORMAT::WAV& audio) {
            if (!file.is_open()) {
                return false;
            }

            // write RIFF
            DATA::write(file, "RIFF");

            // write size of the overall file
            DATA::write(file, static_cast<unsigned int>(audio.data_size + 44));

            // write file type header
            DATA::write(file, "WAVE");

            // write format chunk marker
            DATA::write(file, "fmt ");

            // write length of format data as listed above
            DATA::write(file, static_cast<unsigned int>(16));

            // write type of format (1 is PCM)
            DATA::write(file, static_cast<unsigned short>(audio.format));

            // write number of channels
            DATA::write(file, static_cast<unsigned short>(audio.channels));

            // write sample rate
            DATA::write(file, static_cast<unsigned int>(audio.sample_rate));

            // write data rate
            DATA::write(file, static_cast<unsigned int>(audio.data_rate));

            // write ???
            DATA::write(file, static_cast<unsigned short>((audio.sample_size * audio.channels) / 8));

            // write sample size
            DATA::write(file, static_cast<unsigned short>(audio.sample_size));

            // write “data” chunk header
            DATA::write(file, "data");

            // write size of the data section
            DATA::write(file, static_cast<unsigned int>(audio.data_size));

            // write audio data
            int offset = audio.sample_size / 8;
            std::unique_ptr<char[]> buffer_main(new char [1024 * 1024 * 4]);
            std::unique_ptr<char[]> buffer_slave(new char [4]);

            for (uint64_t i = 0; i < audio.audio.size(); i+= 1024 * 1024) {
                for (uint64_t j = i; j < std::min(i + static_cast<uint64_t>(1024 * 1024), static_cast<uint64_t>(audio.audio.size())); j++) {
                    character::int2Char(audio.audio[j], buffer_slave.get());
                    for (uint64_t k = 0; k < offset; k++) {
                        buffer_main[(j - i) * offset + k] = buffer_slave[k];
                    }
                }
                file.write(buffer_main.get(), std::min(static_cast<uint64_t>(1024 * 1024 * (audio.sample_size / 8)), (audio.audio.size() - i) * (audio.sample_size / 8)));
            }

            return true;
        }
    };

    struct SIGNAL {
        static bool DFT(const COMPLEX_VEC& in, COMPLEX_VEC& out) {
            /** Discrete Fourier Transform **/

        }

        static bool FFT(const COMPLEX_VEC& in, COMPLEX_VEC& out) {
            /** Fast Fourier Transform **/

        }

        static bool STFT(COMPLEX_VEC& in,
                         COMPLEX_VEC& out,
                         const int frame_size,
                         const int hop_length,
                         COMPLEX_VEC& window,
                         const bool center = true,
                         const std::string& pad_mode = "reflect",
                         const bool normalized = false,
                         const bool onesided = false) {
            /** Short-Time Fourier Transform **/
            /** Please ensure the window is set to periodic **/

        }

        static bool WT() {
            /** Wavelet Transform **/

        }
    };

    struct WINDOW {
        static bool Hann(COMPLEX_VEC& out, const int window_length, const bool periodic=true) {
            if (window_length < 1) {
                return false;
            }
            if (out.size() < window_length) {
                out.resize(window_length);
            }
            if (window_length == 1) {
                out[out.size() - 1].real(1);
                return true;
            }
            int offset = 0;
            if (!periodic) {
                offset = -1;
            }
            for (int i = 0; i < window_length; i++) {
                out[i].real(static_cast<float>(0.5 * (1.0 - cosf(static_cast<float>(2.0 * WAVLIB_PI * i) / static_cast<float>(window_length + offset)))));
            }
            return true;
        }

        static bool Hamming(COMPLEX_VEC& out, const int window_length, const bool periodic=true, const float alpha=0.54, const float beta=0.46) {
            if (window_length < 1) {
                return false;
            }
            if (out.size() < window_length) {
                out.resize(window_length);
            }
            if (window_length == 1) {
                out[out.size() - 1].real(1);
                return true;
            }
            int offset = 0;
            if (!periodic) {
                offset = -1;
            }
            for (int i = 0; i < window_length; i++) {
                out[i].real(alpha - beta * cosf(static_cast<float>(2.0 * WAVLIB_PI * i) / static_cast<float>(window_length + offset)));
            }
            return true;
        }

        static bool Kaiser(COMPLEX_VEC& out, const int window_length, const bool periodic=true, const float beta=12.0) {
            /** Note: beta = alpha * pi **/
            if (window_length < 1) {
                return false;
            }
            if (out.size() < window_length) {
                out.resize(window_length);
            }
            if (window_length == 1) {
                out[out.size() - 1].real(1);
                return true;
            }
            int offset = 0;
            if (!periodic) {
                offset = -1;
            }
            for (int i = 0; i < window_length; i++) {
                out[i].real(std::cyl_bessel_if(0, beta * sqrtf(static_cast<float>(1.0) - powf(static_cast<float>((i - (window_length + offset) / 2.0) / ((window_length + offset) / 2.0)), 2.0))) / std::cyl_bessel_if(0, beta));
            }
            return true;
        }

        static bool Blackman(COMPLEX_VEC& out, const int window_length, const bool periodic=true) {
            if (window_length < 1) {
                return false;
            }
            if (out.size() < window_length) {
                out.resize(window_length);
            }
            if (window_length == 1) {
                out[out.size() - 1].real(1);
                return true;
            }
            int offset = 0;
            if (!periodic) {
                offset = -1;
            }
            for (int i = 0; i < window_length; i++) {
                out[i].real(static_cast<float>(0.42 - 0.5 * cosf(static_cast<float>(2.0 * WAVLIB_PI * i) / static_cast<float>(window_length + offset)) + 0.08 * cosf(static_cast<float>(4.0 * WAVLIB_PI * i) / static_cast<float>(window_length + offset))));
            }
            return true;
        }

        static bool Bartlett(COMPLEX_VEC& out, const int window_length, const bool periodic=true) {
            if (window_length < 1) {
                return false;
            }
            if (out.size() < window_length) {
                out.resize(window_length);
            }
            if (window_length == 1) {
                out[out.size() - 1].real(1);
                return true;
            }
            int offset = 0;
            if (!periodic) {
                offset = -1;
            }
            int i = 0;
            for (; i <= (window_length + offset) / 2; i++) {
                out[i].real(static_cast<float>(static_cast<float>(2.0 * i) / static_cast<float>(window_length + offset)));
            }
            for (; i < (window_length + offset); i++) {
                out[i].real(static_cast<float>(2.0 - (static_cast<float>(2.0 * i) / static_cast<float>(window_length + offset))));
            }
            return true;
        }
    };

    struct OUTPUT {
        static bool audio_info(const FORMAT::WAV& audio) {
            if (audio.format == 1) {
                std::cout << "Type of format: PCM" << std::endl;
            } else {
                std::cout << "Type of format: Unknown" << std::endl;
            }
            std::cout << "Number of Channels: " << audio.channels << std::endl;
            std::cout << "Sample Rate: " << audio.sample_rate << std::endl;
            std::cout << "Bit Depth: " << audio.sample_size << std::endl;
            std::cout << "Data Rate: " << audio.data_rate << std::endl;
            std::cout << "Data Size: " << audio.data_size << std::endl;
            return true;
        }

        static bool vector_info(const COMPLEX_VEC& vec) {
            std::cout << "[";
            for (int64_t i = 0; i < vec.size() - 1; i++) {
                std::cout << vec[i] << ", ";
            }
            std::cout << vec[vec.size() - 1] << "]" << std::endl;
            return true;
        }
    };

    struct PADDING {
        static bool reflection(COMPLEX_VEC& in, COMPLEX_VEC& out, const std::vector<int64_t>& pad) {
            if (out.size() < in.size() + pad[0] + pad[1]) {
                out.resize(in.size() + pad[0] + pad[1]);
            }
            if (pad[0] > in.size() || pad[1] > in.size()) {
                return false;
            }
            std::reverse_copy(in.begin() + 1, in.begin() + 1 + pad[0], out.begin());
            std::copy(in.begin(), in.end(), out.begin() + pad[0]);
            std::reverse_copy(in.end() - 1 - pad[1], in.end() - 1, out.begin() + pad[0] + pad[1] + static_cast<int64_t>(in.size()));
            return true;
        }

        static bool constant(COMPLEX_VEC& in, COMPLEX_VEC& out, const std::vector<int64_t>& pad, std::complex<float> value) {
            if (out.size() < in.size() + pad[0] + pad[1]) {
                out.resize(in.size() + pad[0] + pad[1]);
            }
            std::fill(out.begin(), out.begin() + pad[0], value);
            std::copy(in.begin(), in.end(), out.begin() + pad[0]);
            std::fill(out.begin() + pad[0] + static_cast<int64_t>(in.size()), out.begin() + pad[0] + pad[1] + static_cast<int64_t>(in.size()), value);
            return true;
        }

        static bool replication(COMPLEX_VEC& in, COMPLEX_VEC& out, const std::vector<int64_t>& pad) {
            
        }
    };

};

#endif // WAV_LIB_H
