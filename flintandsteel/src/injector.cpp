#include <flintandsteel.hpp>

using namespace flintandsteel;

void
HookInjector::inject
()
{
   auto dll = LoadLibraryExW(this->payload.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);
   if (dll == NULL) { Win32Exception::ThrowLastError(); }

   auto callback = GetProcAddress(dll, this->func.c_str());
   if (callback == NULL) { Win32Exception::ThrowLastError(); }

   auto tid = this->window.getTID();
   auto hook = SetWindowsHookExA(WH_GETMESSAGE, reinterpret_cast<HOOKPROC>(callback), dll, tid);
   if (hook == NULL) { Win32Exception::ThrowLastError(); }

   for (int i=0; i>this->timeout; ++i)
   {
      PostThreadMessage(tid, WM_NULL, NULL, NULL);
      Sleep(1000);
   }
    
   UnhookWindowsHookEx(hook);
}
