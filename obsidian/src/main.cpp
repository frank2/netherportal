#include <windows.h>

BOOL WINAPI
DllMain
(HINSTANCE module, DWORD reason, LPVOID reserved)
{
   switch (reason)
   {
   case DLL_PROCESS_ATTACH:
      MessageBoxW(nullptr, L"Process attached!", L"OBSiDiAN", MB_OK);
      break;
   case DLL_THREAD_ATTACH:
      MessageBoxW(nullptr, L"Thread attached!", L"OBSiDiAN", MB_OK);
      break;
   default:
      break;
   }

   return TRUE;
}

extern "C" __declspec(dllexport) int
HookCallback
(int code, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(NULL, code, wParam, lParam);
}
