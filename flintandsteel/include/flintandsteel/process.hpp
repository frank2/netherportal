#pragma once

#include "flintandsteel.hpp"

namespace flintandsteel
{
   class Process
   {
      HANDLE handle;
      DWORD pid;
      wchar_t moduleName[MAX_PATH];

   public:
      Process(HANDLE handle) : handle(handle), pid(0) {
         ZeroMemory(this->moduleName, sizeof(this->moduleName));
      }
      
      static Process Open(DWORD access, BOOL inherit, DWORD pid);
      static std::optional<DWORD> FindByFileName(std::wstring filename);
      static std::optional<DWORD> FindByWindowTitle(std::wstring title);
      static std::optional<DWORD> FindByWindowClass(std::wstring klass);

      operator HANDLE() const { return this->handle; }

      HANDLE getHandle() { return this->handle; }
      DWORD getPID() { if (this->pid == 0) { this->pid = GetProcessId(this->handle); } return this->pid; }
      std::wstring getModuleName();
      std::wstring getFileName();
      std::optional<Window> getMainWindow();
   };
}
