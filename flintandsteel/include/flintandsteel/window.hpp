#pragma once

#include <flintandsteel.hpp>

namespace flintandsteel
{
   class Window
   {
      HWND handle;
      DWORD pid;
      DWORD tid;

   public:
      Window(HWND handle) : handle(handle), pid(0), tid(0) {}

      static std::vector<Window> Enum();
      static std::optional<Window> Find(std::wstring klass, std::wstring title);
      static std::optional<Window> FindByClass(std::wstring klass);
      static std::optional<Window> FindByTitle(std::wstring title);

      operator HWND() const { return this->handle; }

      DWORD getPID() { if (this->pid == 0) { GetWindowThreadProcessId(this->handle, &this->pid); } return this->pid; }
      DWORD getTID() { if (this->tid == 0) { this->tid = GetWindowThreadProcessId(this->handle, &this->pid); } return this->tid; }
      std::optional<Window> getWindow(UINT cmd) {
         auto hwnd = GetWindow(this->handle, cmd);
         return (hwnd == NULL) ? std::nullopt : std::optional<Window>(Window(hwnd));
      }
      std::wstring getText();

      bool isVisible() { return (bool)IsWindowVisible(this->handle); }
   };
}
