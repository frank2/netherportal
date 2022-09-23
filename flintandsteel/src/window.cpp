#include <flintandsteel.hpp>

using namespace flintandsteel;

std::vector<Window>
Window::Enum
()
{
    std::vector<Window> windows;

    EnumWindows(
        [](HWND hwnd, LPARAM param) -> BOOL {
            auto windows = (std::vector<HWND> *)param;
            windows->push_back(Window(hwnd));
            return TRUE;
        },
        (LPARAM)&windows
    );

    return windows;
}

std::optional<Window>
Window::Find
(std::wstring klass, std::wstring title)
{
   LPCWSTR klassC = (klass.size() == 0) ? nullptr : klass.c_str();
   LPCWSTR titleC = (title.size() == 0) ? nullptr : title.c_str();
   HWND hwnd = FindWindowW(klassC, titleC);

   return (hwnd == NULL) ? std::nullopt : std::optional<Window>(Window(hwnd));
}

std::optional<Window>
Window::FindByClass
(std::wstring klass)
{
   return Window::Find(klass, L"");
}

std::optional<Window>
Window::FindByTitle
(std::wstring title)
{
   return Window::Find(L"", title);
}

std::wstring
Window::getText
()
{
   auto length = GetWindowTextLengthW(this->handle);

   if (length == 0) { Win32Exception::ThrowLastError(); }
   
   std::vector<wchar_t> buffer(length+1);
   
   if (GetWindowTextW(this->handle, buffer.data(), length+1) == 0) { Win32Exception::ThrowLastError(); }

   return std::wstring(buffer.data());
}
