#pragma once

#include <string>

#ifdef _MSC_VER
typedef unsigned __int8  uint8_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif


class SHA3
{
public:
  enum Bits { Bits224 = 224, Bits256 = 256, Bits384 = 384, Bits512 = 512 };

  explicit SHA3(Bits bits = Bits256);

  std::string operator()(const void* data, size_t numBytes);
  std::string operator()(const std::string& text);

  void add(const void* data, size_t numBytes);

  std::string getHash();

  void reset();

private:

  void processBlock(const void* data);
  void processBuffer();

  enum { StateSize    = 1600 / (8 * 8),
         MaxBlockSize =  200 - 2 * (224 / 8) };

  uint64_t m_hash[StateSize];
  uint64_t m_numBytes;
  size_t   m_blockSize;
  size_t   m_bufferSize;
  uint8_t  m_buffer[MaxBlockSize];
  Bits     m_bits;
};
