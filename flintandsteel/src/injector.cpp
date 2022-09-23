#include <flintandsteel.hpp>

using namespace flintandsteel;

void
HookInjector::inject
()
{
   auto dll = LoadLibraryW(this->payload.c_str());
   if (dll == NULL) { Win32Exception::ThrowLastError(); }

   auto callback = GetProcAddress(dll, this->func.c_str());
   if (callback == NULL) { Win32Exception::ThrowLastError(); }

   auto tid = this->window.getTID();
   auto hook = SetWindowsHookExA(WH_GETMESSAGE, reinterpret_cast<HOOKPROC>(callback), dll, tid);
   if (hook == NULL) { Win32Exception::ThrowLastError(); }

   for (std::size_t i=0; i<10; ++i)
   {
      PostThreadMessage(tid, WM_NULL, NULL, NULL);
      Sleep(500);
   }

   UnhookWindowsHookEx(hook);
}
