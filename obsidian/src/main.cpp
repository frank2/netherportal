#include <obsidian.hpp>

INITIALIZE_EASYLOGGINGPP

using namespace obsidian;

void
mainThread
(LPVOID module)
{
   Console console;

   try
   {
      using namespace java::sig;
      // initialize the logger
      el::Configurations defaultConf;
      defaultConf.setToDefault();
      defaultConf.set(el::Level::Debug,
                      el::ConfigurationType::Format,
                      "[%datetime] [%level] [%func] %msg");
      defaultConf.set(el::Level::Info,
                      el::ConfigurationType::Format,
                      "[%datetime] [%level] [%func] %msg");
      defaultConf.set(el::Level::Warning,
                      el::ConfigurationType::Format,
                      "[%datetime] [%level] [%func] %msg");
      defaultConf.set(el::Level::Error,
                      el::ConfigurationType::Format,
                      "[%datetime] [%level] [%func] %msg");
      defaultConf.set(el::Level::Fatal,
                      el::ConfigurationType::Format,
                      "[%datetime] [%level] [%func] %msg");

      el::Loggers::reconfigureLogger("default", defaultConf);

      // initialize the console
      console.allocate();

      // initialize the jni environment
      java::init();
      LOG(DEBUG) << "JNI initialized.";

      // main routine code
      game::init();
      LOG(DEBUG) << "Game initialized.";
      
      auto member = ProGuard::ResolveMember("net.minecraft.client.Minecraft",
                                            Object("net.minecraft.client.Minecraft"),
                                            "instance");
      LOG(DEBUG) << "ProGuard member name for net.minecraft.client.Minecraft::instance: " << member;

      auto mappedObject = ProGuard::MapSignature(Object("net.minecraft.client.Minecraft").spawn());

      LOG(DEBUG) << "Mapped signature for net.minecraft.client.Minecraft: " << mappedObject->toString();

      auto minecraft = game::Minecraft::get_instance();

      LOG(DEBUG) << "Minecraft instance: 0x" << std::hex << minecraft;

      /*
      auto funcSig = Function::FromJavaString("void", "com.mojang.blaze3d.pipeline.RenderPipeline,float")->toString();
      LOG(DEBUG) << "Function signature: " << funcSig;
      */
                     
      while (1)
      {
         LOG(DEBUG) << "I live!";
         Sleep(1000);
      }

      // termination code
      CoUninitialize();
      java::release();
   }
   catch (std::exception &e)
   {
      LOG(ERROR) << "Fatal exception: " << e.what();
   }
   
   Sleep(60000);
   return;
}

BOOL WINAPI
DllMain
(HINSTANCE module, DWORD reason, LPVOID reserved)
{
   switch (reason)
   {
   case DLL_PROCESS_ATTACH:
      CreateThread(NULL,
                   8192,
                   reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread),
                   (LPVOID)module,
                   0,
                   NULL);
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
