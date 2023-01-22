#pragma once

#include <obsidian/std.hpp>
#include <obsidian/thirdparty.hpp>

namespace obsidian
{
   class Console
   {
   public:
      FILE *cstdin, *cstdout, *cstderr;

      Console() : cstdin(nullptr), cstdout(nullptr), cstderr(nullptr) { }
      ~Console() { this->release(); }

      void allocate() {
         if (this->cstdin != nullptr && this->cstdout != nullptr && this->cstderr != nullptr) { return; }
         if (!AllocConsole() || !SetConsoleTitle("OBSiDiAN")) { Win32Exception::ThrowLastError(); }

         freopen_s(&this->cstdin, "CONIN$", "r", stdin);
         freopen_s(&this->cstdout, "CONOUT$", "w", stdout);
         freopen_s(&this->cstderr, "CONOUT$", "w", stderr);
      }

      void release() {
         if (this->cstdin != nullptr || this->cstdout != nullptr || this->cstderr != nullptr)
         {
            if (this->cstdin != nullptr) { fclose(this->cstdin); }
            if (this->cstdout != nullptr) { fclose(this->cstdout); }
            if (this->cstderr != nullptr) { fclose(this->cstderr); }

            FreeConsole();
         }
      }
   };
}
