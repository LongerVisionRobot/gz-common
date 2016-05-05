/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef _IGNITION_COMMON_UTIL_HH_
#define _IGNITION_COMMON_UTIL_HH_

#include <boost/uuid/sha1.hpp>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <memory>
#include <string>
#include <future>

/////////////////////////////////////////////////
// Defines

/// \brief Seconds in one nano second.
#define IGN_NANO_TO_SEC 0.0000000001

/// \brief Nano seconds in one second.
#define IGN_SEC_TO_NANO 1000000000

/// \brief Nano seconds in one millisecond.
#define IGN_MS_TO_NANO 1000000

/// \brief Nano seconds in one microsecond.
#define IGN_US_TO_NANO 1000

/// \brief Speed of light.
#define IGN_SPEED_OF_LIGHT = 299792458.0

/// \brief Sleep for the specifed number of seconds
#define ign_sleep_s(_s) (std::this_thread::sleep_for(\
                         std::chrono::seconds(_s)))

/// \brief Sleep for the specifed number of microseconds
#define ign_sleep_us(_us) (std::this_thread::sleep_for(\
                           std::chrono::microseconds(_us)))

/// \brief Sleep for the specifed number of milliseconds
#define ign_sleep_ms(_ms) (std::this_thread::sleep_for(\
                           std::chrono::milliseconds(_ms)))

/// \brief Sleep for the specifed number of nanoseconds
#define ign_sleep_ns(_ns) (std::this_thread::sleep_for(\
                           std::chrono::nanoseconds(_ns)))

/// \brief Get the system time.
#define ign_system_time() (std::chrono::system_clock::now())

/// \brief Get the system time in seconds since epoch.
#define ign_system_time_s() (std::chrono::duration_cast<std::chrono::seconds>(\
      std::chrono::system_clock::now().time_since_epoch()).count())

/// \brief Get the system time in microseconds since epoch.
#define ign_system_time_us() (std::chrono::duration_cast<\
    std::chrono::microseconds>(\
      std::chrono::system_clock::now().time_since_epoch()).count())

/// \brief Get the system time in milliseconds since epoch.
#define ign_system_time_ms() (std::chrono::duration_cast<\
    std::chrono::milliseconds>(\
      std::chrono::system_clock::now().time_since_epoch()).count())

/// \brief Get the system time in nanoseconds since epoch.
#define ign_system_time_ns() (std::chrono::duration_cast<\
    std::chrono::nanoseconds>(\
      std::chrono::system_clock::now().time_since_epoch()).count())

/// \brief This macro defines the standard way of launching an exception
/// inside ignition.
#define ign_assert(_expr, _msg) assert((_msg, _expr))

#if defined(__GNUC__)
#define IGN_DEPRECATED(version) __attribute__((deprecated))
#define IGN_FORCEINLINE __attribute__((always_inline))
#elif defined(MSVC)
#define IGN_DEPRECATED(version) ()
#define IGN_FORCEINLINE __forceinline
#else
#define IGN_DEPRECATED(version) ()
#define IGN_FORCEINLINE
#endif

/// \def IGNITION_VISIBLE
/// Use to represent "symbol visible" if supported

/// \def IGNITION_HIDDEN
/// Use to represent "symbol hidden" if supported
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define IGNITION_VISIBLE __attribute__ ((dllexport))
    #else
      #define IGNITION_VISIBLE __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define IGNITION_VISIBLE __attribute__ ((dllimport))
    #else
      #define IGNITION_VISIBLE __declspec(dllimport)
    #endif
  #endif
  #define IGNITION_HIDDEN
#else
  #if __GNUC__ >= 4
    #define IGNITION_VISIBLE __attribute__ ((visibility ("default")))
    #define IGNITION_HIDDEN  __attribute__ ((visibility ("hidden")))
  #else
    #define IGNITION_VISIBLE
    #define IGNITION_HIDDEN
  #endif
#endif

/// \brief Forward declarations for the common classes
namespace ignition
{
  namespace common
  {
    /// \brief Enumeration of the transform types
    enum NodeTransformType {TRANSLATE, ROTATE, SCALE, MATRIX};

    /// \brief A runtime error.
    typedef std::runtime_error exception;

    /// \brief Get the wall time as an ISO string: YYYY-MM-DDTHH:MM:SS.NS
    /// \return The current wall time as an ISO string.
    IGNITION_VISIBLE
    std::string SystemTimeISO();

    /// \brief add path sufix to common::SystemPaths
    /// \param[in] _suffix The suffix to add.
    IGNITION_VISIBLE
    void addSearchPathSuffix(const std::string &_suffix);

    /// \brief search for file in common::SystemPaths
    /// \param[in] _file Name of the file to find.
    /// \return The path containing the file.
    IGNITION_VISIBLE
    std::string findFile(const std::string &_file);

    /// \brief search for file in common::SystemPaths
    /// \param[in] _file Name of the file to find.
    /// \param[in] _searchLocalPath True to search in the current working
    /// directory.
    /// \return The path containing the file.
    IGNITION_VISIBLE
    std::string findFile(const std::string &_file,
                         bool _searchLocalPath);

    /// \brief search for a file in common::SystemPaths
    /// \param[in] _file the file name to look for.
    /// \return The path containing the file.
    IGNITION_VISIBLE
    std::string findFilePath(const std::string &_file);

    /// \brief Compute the SHA1 hash of an array of bytes.
    /// \param[in] _buffer Input sequence. The permitted data types for this
    /// function are std::string and any STL container.
    /// \return The string representation (40 character) of the SHA1 hash.
    template<typename T>
    IGNITION_VISIBLE
    std::string getSha1(const T &_buffer);
  }

}

///////////////////////////////////////////////
// Implementation of get_sha1
template<typename T>
std::string ignition::common::getSha1(const T &_buffer)
{
  boost::uuids::detail::sha1 sha1;
  unsigned int hash[5];
  std::stringstream stream;

  if (_buffer.size() == 0)
  {
    sha1.process_bytes(NULL, 0);
  }
  else
  {
    sha1.process_bytes(&(_buffer[0]), _buffer.size() * sizeof(_buffer[0]));
  }

  sha1.get_digest(hash);

  for (std::size_t i = 0; i < sizeof(hash) / sizeof(hash[0]); ++i)
  {
    stream << std::setfill('0')
      << std::setw(sizeof(hash[0]) * 2)
      << std::hex
      << hash[i];
  }

  return stream.str();
}
#endif
