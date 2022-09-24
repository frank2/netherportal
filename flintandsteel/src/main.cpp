#include <flintandsteel.hpp>
#include <argparse/argparse.hpp>

using namespace flintandsteel;
   
int
main
(int argc, char *argv[], char *envp[])
{
   argparse::ArgumentParser program("FLiNTandSTEEL");
   std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

   // main arguments
   program.add_argument("-p", "--payload")
      .required()
      .help("The DLL payload to inject.");

   program.add_argument("-t", "--technique")
      .help("The injection technique to use. Options are \"remotethread\" and \"hook\". The default is \"hook\".")
      .default_value(std::string("hook"));
      
   // shared arguments
   program.add_argument("-P", "--pid")
      .help("The process ID to inject into.")
      .default_value((DWORD)0)
      .scan<'u', DWORD>();

   program.add_argument("-w", "--window-title")
      .help("The title of the process's window to search for.")
      .default_value(std::string());

   program.add_argument("-c", "--window-class")
      .help("The class name of the process's window to search for.")
      .default_value(std::string());
      
   program.add_argument("-e", "--exe-name")
      .help("The EXE name to inject into.")
      .default_value(std::string());
      
   // hook arguments
   program.add_argument("-f", "--hook-function")
      .help("The hook callback function in the payload DLL.")
      .default_value(std::string("HookCallback"));

   program.add_argument("-T", "--timeout")
      .help("The amount of time to wait before removing the hook. A negative number results in an infinite wait time.")
      .default_value(-1)
      .scan<'i', int>();

   try
   {
      program.parse_args(argc, argv);
   }
   catch (std::runtime_error &err)
   {
      std::cerr << err.what() << std::endl;
      std::cerr << program;
      return 1;
   }

   auto payload = program.get<std::string>("--payload");
   auto technique = program.get<std::string>("--technique");
   auto pid = program.get<DWORD>("--pid");
   auto windowTitle = program.get<std::string>("--window-title");
   auto windowClass = program.get<std::string>("--window-class");
   auto exeName = program.get<std::string>("--exe-name");

   if (pid == 0 && windowTitle.size() == 0 && windowClass.size() == 0 && exeName.size() == 0)
   {
      std::cerr << "[-] No target executable argument provided. Set --pid, --window-title, --window-class or --exe-name." << std::endl;
      return 2;
   }

   if (technique != "hook" && technique != "remotethread")
   {
      std::cerr << "[-] Invalid technique: " << technique << std::endl;
      std::cerr << program;
      return 3;
   }

   try
   {
      if (technique == "remotethread")
      {
         std::cerr << "[-] \"remotethread\" currently not implemented." << std::endl;
         return 4;
      }
      else if (technique == "hook")
      {
         std::optional<Window> window = std::nullopt;
         std::optional<Process> proc = std::nullopt;

         std::cout << "[+] Applying hook technique." << std::endl;

         if (pid != 0)
         {
            std::cout << "[+] Searching for PID " << pid << std::endl;
            proc = std::optional<Process>(Process::Open(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid));
         }
         else if (exeName.size() > 0)
         {
            std::cout << "[+] Searching for EXE name " << exeName << std::endl;
            auto foundPid = Process::FindByFileName(converter.from_bytes(exeName));

            if (!foundPid.has_value())
            {
               std::cerr << "[-] Executable not found" << std::endl;
               return 5;
            }

            std::cout << "[+] Process found." << std::endl;

            proc = std::optional<Process>(Process::Open(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, foundPid.value()));
         }
         else if (windowTitle.size() > 0)
         {
            std::cout << "[+] Searching for window title \"" << windowTitle << "\"" << std::endl;
            window = Window::FindByTitle(converter.from_bytes(windowTitle));

            if (!window.has_value())
            {
               std::cerr << "[-] Window title not found" << std::endl;
               return 6;
            }
         }
         else if (windowClass.size() > 0)
         {
            std::cout << "[+] Searching for window class \"" << windowClass << "\"" << std::endl;
            window = Window::FindByClass(converter.from_bytes(windowClass));

            if (!window.has_value())
            {
               std::cerr << "[-] Window class not found" << std::endl;
               return 7;
            }
         }

         if (proc.has_value())
         {
            window = proc.value().getMainWindow();
                        
            if (!window.has_value())
            {
               std::cerr << "[-] Process \""
                         << converter.to_bytes(proc.value().getFileName())
                         << "\" has no main window." << std::endl;
               return 8;
            }
         }

         std::cout << "[+] Performing injection." << std::endl;

         HookInjector(window.value(),
                      converter.from_bytes(payload),
                      program.get<std::string>("--hook-function"),
                      program.get<int>("--timeout"))
            .inject();

         std::cout << "[+] Injection complete." << std::endl;
      }

      return 0;
   }
   catch (Exception &e)
   {
      std::cerr << "Encountered an exception: " << e.what() << std::endl;
   }
}
