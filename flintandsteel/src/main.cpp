#include <flintandsteel.hpp>

using namespace flintandsteel;

int
error
(int errorCode, char *message)
{
   return errorCode;
}
   
int
wmain
(int argc, wchar_t *argv[], wchar_t *envp[])
{
   if (argc < 3) {
      std::wcerr << L"Error: not enough arguments." << std::endl
                 << L"Usage: " << argv[0] << L" \"<window title>\" \"<payload DLL>\" [\"<export name>\"]" << std::endl;
      return 1;
   }

   auto windowTitle = argv[1];
   auto payload = argv[2];
   wchar_t *exportC = (argc > 3) ? argv[3] : L"HookCallback";
   std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
   std::string func = conv.to_bytes(exportC);

   try
   {
      std::wcout << "[+] Searching for window with title \"" << windowTitle << "\"" << std::endl;

      auto window = Window::FindByTitle(windowTitle);

      if (!window.has_value())
      {
         std::cerr << "Error: couldn't find window." << std::endl;
         return 3;
      }

      std::cout << "[+] Performing injection on process that owns the window." << std::endl;

      HookInjector(window.value(), payload, func).inject();

      std::cout << "[+] Injection performed successfully." << std::endl;
      return 0;
   }
   catch (Exception &e)
   {
      std::cerr << "Encountered an error: " << e.what() << std::endl;
      return 2;
   }
}
