#pragma once

#include <obsidian/std.hpp>
#include <obsidian/thirdparty.hpp>

namespace obsidian
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

   class COMException : public Exception
   {
   public:
      HRESULT code;

      COMException(HRESULT code) : code(code), Exception() {
         _com_error err(this->code);
         auto comError = std::string(err.ErrorMessage());
         std::stringstream stream;

         stream << "COM Exception 0x" << std::hex << this->code << ": " << comError;
         this->error = stream.str();
      }
      
      static void ThrowOnError(HRESULT code) { if (FAILED(code)) { throw COMException(code); } }
   };

   class JNIException : public Exception
   {
   public:
      jint code;

      JNIException(jint code) : code(code) {
         std::stringstream stream;
         stream << "JNI Exception: code " << (int)code;

         this->error = stream.str();
      }
      JNIException(jint code, std::string message) : code(code) {
         std::stringstream stream;
         stream << "JNI Exception, code " << (int)code << ": " << message;

         this->error = stream.str();
      }
      JNIException(jint code, std::wstring message) : code(code) {
         std::wstringstream stream;
         stream << L"JNI Exception, code " << (int)code << ": " << message;

         this->saveErrorAsUTF8(stream.str());
      }

      static void ThrowOnError(jint code) { if (code != JNI_OK) { throw JNIException(code); } }
      static void ThrowOnError(jint code, std::string message) { if (code != JNI_OK) { throw JNIException(code, message); } }
      static void ThrowOnError(jint code, std::wstring message) { if (code != JNI_OK) { throw JNIException(code, message); } }
   };
}
