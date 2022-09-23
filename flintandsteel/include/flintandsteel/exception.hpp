#pragma once

#include "flintandsteel.hpp"

namespace flintandsteel
{
   class Exception : public std::exception
   {
   public:
      std::string error;

      Exception() : std::exception() {}
      Exception(std::string error) : error(error), std::exception() {}   
      Exception(std::wstring error) : std::exception() { this->saveErrorAsUTF8(error); }

      const char *what() const noexcept {
         return this->error.c_str();
      }

   protected:
      void saveErrorAsUTF8(std::wstring error) {
         std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
         this->error = conv.to_bytes(error);
      }
   };

   class Win32Exception : public Exception
   {
   public:
      DWORD code;

      Win32Exception(DWORD code) : code(code), Exception() {
         LPWSTR buffer = nullptr;
         std::wstringstream stream;

         FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&buffer,
            0,
            NULL
         );
         
         stream << L"Win32 Error 0x" 
                << std::hex << std::uppercase << this->code << std::nouppercase 
                << ": " << buffer;
         LocalFree(buffer);

         this->saveErrorAsUTF8(stream.str());
      }
      Win32Exception() : Win32Exception(GetLastError()) {}

      static void ThrowLastError() { if (GetLastError() != 0) { throw Win32Exception(); } }
   };
}
