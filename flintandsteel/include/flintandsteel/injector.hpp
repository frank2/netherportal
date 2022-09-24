#pragma once

#include <flintandsteel.hpp>

namespace flintandsteel
{
   class Injector
   {
   public:
      Injector() {}
      virtual void inject() {}
   };

   class HookInjector : public Injector
   {
      Window window;
      std::wstring payload;
      std::string func;
      int timeout;
      
   public:
      HookInjector(Window window, std::wstring payload, std::string func, int timeout=5)
         : Injector(),
           window(window),
           payload(payload),
           func(func),
           timeout(timeout) {}
      void inject();
   };

   /*
   class MemoryInjector : public Injector
   {
      DWORD pid;

   public:
      MemoryInjector(DWORD pid) : Injector(), pid(pid) {}
      void inject();
   };
   */
}
