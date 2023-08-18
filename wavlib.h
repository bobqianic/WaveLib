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

#if _WIN32
#define NOMINMAX
#include <windows.h>
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
        std::ifstream file(WINDOWS_ONLY::WStringToString(filename), std::ios::binary);
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
        std::ofstream file(WINDOWS_ONLY::WStringToString(filename), std::ios::binary);
        bool result = DUMP::WAV(file, audio);
        if (file.is_open()) {
            file.close();
        }
        return result;
    }
#endif

    static bool print_info(FORMAT::WAV& audio) {
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
        static short char2Short(const char* data, const bool LittleEndian = true) {
            if (endian::isLittleEndian() != LittleEndian) {
                char buffer[2];
                std::copy(data, data + 2, buffer);
                endian::flipEndianness(buffer, 2);
                return *reinterpret_cast<const short*>(buffer);
            }
            return *reinterpret_cast<const short*>(data);
        }

        static unsigned short char2uShort(const char* data, const bool LittleEndian = true) {
            if (endian::isLittleEndian() != LittleEndian) {
                char buffer[2];
                std::copy(data, data + 2, buffer);
                endian::flipEndianness(buffer, 2);
                return *reinterpret_cast<const unsigned short*>(buffer);
            }
            return *reinterpret_cast<const unsigned short*>(data);
        }

        static int char2Int(const char* data, const bool LittleEndian = true) {
            if (endian::isLittleEndian() != LittleEndian) {
                char buffer[4];
                std::copy(data, data + 4, buffer);
                endian::flipEndianness(buffer, 4);
                return *reinterpret_cast<const int*>(buffer);
            }
            return *reinterpret_cast<const int*>(data);
        }

        static unsigned int char2uInt(const char* data, const bool LittleEndian = true) {
            if (endian::isLittleEndian() != LittleEndian) {
                char buffer[4];
                std::copy(data, data + 4, buffer);
                endian::flipEndianness(buffer, 4);
                return *reinterpret_cast<const unsigned int*>(buffer);
            }
            return *reinterpret_cast<const unsigned int*>(data);
        }

        static long long char2LongLong(const char* data, const bool LittleEndian = true) {
            if (endian::isLittleEndian() != LittleEndian) {
                char buffer[8];
                std::copy(data, data + 8, buffer);
                endian::flipEndianness(buffer, 8);
                return *reinterpret_cast<const long long*>(buffer);
            }
            return *reinterpret_cast<const long long*>(data);
        }

        static unsigned long long char2uLongLong(const char* data, const bool LittleEndian = true) {
            if (endian::isLittleEndian() != LittleEndian) {
                char buffer[8];
                std::copy(data, data + 8, buffer);
                endian::flipEndianness(buffer, 8);
                return *reinterpret_cast<const unsigned long long*>(buffer);
            }
            return *reinterpret_cast<const unsigned long long*>(data);
        }
    };

    struct character {
        static void short2Char(const short& data, char* out, const bool LittleEndian = true) {
            const char* p = reinterpret_cast<const char*>(&data);
            std::copy(p, p + 2, out);
            if (endian::isLittleEndian() != LittleEndian) {
                endian::flipEndianness(out, 2);
            }
        }

        static void short2Char(const unsigned short& data, char* out, const bool LittleEndian = true) {
            const char* p = reinterpret_cast<const char*>(&data);
            std::copy(p, p + 2, out);
            if (endian::isLittleEndian() != LittleEndian) {
                endian::flipEndianness(out, 2);
            }
        }

        static void int2Char(const int& data, char* out, const bool LittleEndian = true) {
            const char* p = reinterpret_cast<const char*>(&data);
            std::copy(p, p + 4, out);
            if (endian::isLittleEndian() != LittleEndian) {
                endian::flipEndianness(out, 4);
            }
        }

        static void int2Char(const unsigned int& data, char* out, const bool LittleEndian = true) {
            const char* p = reinterpret_cast<const char*>(&data);
            std::copy(p, p + 4, out);
            if (endian::isLittleEndian() != LittleEndian) {
                endian::flipEndianness(out, 4);
            }
        }

        static void longlong2Char(const long long& data, char* out, const bool LittleEndian = true) {
            const char* p = reinterpret_cast<const char*>(&data);
            std::copy(p, p + 8, out);
            if (endian::isLittleEndian() != LittleEndian) {
                endian::flipEndianness(out, 8);
            }
        }

        static void longlong2Char(const unsigned long long& data, char* out, const bool LittleEndian = true) {
            const char* p = reinterpret_cast<const char*>(&data);
            std::copy(p, p + 8, out);
            if (endian::isLittleEndian() != LittleEndian) {
                endian::flipEndianness(out, 8);
            }
        }
    };

    struct DATA {
        static std::string read(std::ifstream& file, unsigned long long length) {
            if (!file.is_open()) {
                return "";
            }
            std::string buffer(length, '\0'); // Preallocate the string with the given length
            file.read(&buffer[0], static_cast<long long>(length));
            return buffer;
        }

        static bool write(std::ofstream& file, const std::string& data) {
            if (!file.is_open()) {
                return false;
            }
            file.write(data.c_str(), static_cast<long long>(data.size()));
            return true;
        }

        static bool write(std::ofstream& file, const short& data) {
            if (!file.is_open()) {
                return false;
            }
            char buffer[2];
            character::short2Char(data, buffer);
            file.write(buffer, 2);
            return true;
        }

        static bool write(std::ofstream& file, const unsigned short& data) {
            if (!file.is_open()) {
                return false;
            }
            char buffer[2];
            character::short2Char(data, buffer);
            file.write(buffer, 2);
            return true;
        }

        static bool write(std::ofstream& file, const int& data) {
            if (!file.is_open()) {
                return false;
            }
            char buffer[4];
            character::int2Char(data, buffer);
            file.write(buffer, 4);
            return true;
        }

        static bool write(std::ofstream& file, const unsigned int& data) {
            if (!file.is_open()) {
                return false;
            }
            char buffer[4];
            character::int2Char(data, buffer);
            file.write(buffer, 4);
            return true;
        }

        static bool write(std::ofstream& file, const long long& data) {
            if (!file.is_open()) {
                return false;
            }
            char buffer[8];
            character::longlong2Char(data, buffer);
            file.write(buffer, 8);
            return true;
        }

        static bool write(std::ofstream& file, const unsigned long long& data) {
            if (!file.is_open()) {
                return false;
            }
            char buffer[8];
            character::longlong2Char(data, buffer);
            file.write(buffer, 8);
            return true;
        }
    };

#if _WIN32
    struct WINDOWS_ONLY {
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
            audio.format = integer::char2Short(DATA::read(file, 2).c_str());

            // Read channel
            audio.channels = integer::char2Short(DATA::read(file, 2).c_str());

            // Read sample rate
            audio.sample_rate = integer::char2Int(DATA::read(file, 4).c_str());

            // Read data rate
            audio.data_rate = integer::char2Int(DATA::read(file, 4).c_str());

            file.seekg(2, std::ios::cur);

            // Read sample size
            audio.sample_size = integer::char2Short(DATA::read(file, 2).c_str());
            if (audio.sample_size < 8 || audio.sample_size > 32) {
                return false;
            }

            // Read data size
            while (!file.eof()) {
                if (DATA::read(file, 4) == "data") {
                    audio.data_size = integer::char2Int(DATA::read(file, 4).c_str());
                    break;
                }
                file.seekg(-3, std::ios::cur);
            }

            int offset = audio.sample_size / 8;
            std::unique_ptr<char[]> audio_buffer_main(new char[1024 * 1024 * offset]);
            std::unique_ptr<char[]> audio_buffer_slave(new char[4]);
            audio.audio.reserve(audio.data_size / offset);

            // read file and convert char[] to vector<int> (supports 8 bits, 16 bits, 24 bits and 32 bits)
            for (unsigned long long i = 0; i < audio.data_size; i += 1024 * 1024 * offset) {
                unsigned int buffer_size = std::min(static_cast<unsigned long long>(1024 * 1024 * offset), static_cast<unsigned long long>(audio.data_size) - i);
                file.read(audio_buffer_main.get(), buffer_size);
                for (unsigned int j = 0; j < buffer_size; j += offset){
                    std::copy(audio_buffer_main.get() + j, audio_buffer_main.get() + j + offset, audio_buffer_slave.get());
                    if ((audio_buffer_slave[offset - 1] & static_cast<char>(-128))) {
                        std::fill(audio_buffer_slave.get() + offset, audio_buffer_slave.get() + 4, static_cast<char>(-1));
                    } else {
                        std::fill(audio_buffer_slave.get() + offset, audio_buffer_slave.get() + 4, static_cast<char>(0));
                    }
                    audio.audio.push_back(integer::char2Int(audio_buffer_slave.get()));
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
            std::unique_ptr<char[]> buffer_main(new char [1024 * 1024 * offset]);
            std::unique_ptr<char[]> buffer_slave(new char [4]);

            for (unsigned long long i = 0; i < audio.audio.size(); i+= 1024 * 1024) {
                for (unsigned long long j = i; j < std::min(i + static_cast<unsigned long long>(1024 * 1024), static_cast<unsigned long long>(audio.audio.size())); j++) {
                    character::int2Char(audio.audio[j], buffer_slave.get());
                    for (unsigned long long k = 0; k < offset; k++) {
                        buffer_main[(j - i) * offset + k] = buffer_slave[k];
                    }
                }
                file.write(buffer_main.get(), std::min(static_cast<unsigned long long>(1024 * 1024 * (audio.sample_size / 8)), (audio.audio.size() - i) * (audio.sample_size / 8)));
            }

            return true;
        }
    };

};

#endif // WAV_LIB_H
