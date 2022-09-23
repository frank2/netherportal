#include <flintandsteel.hpp>

using namespace flintandsteel;

Process
Process::Open
(DWORD access, BOOL inherit, DWORD pid)
{
   HANDLE handle = OpenProcess(access, inherit, pid);

   if (handle == NULL) { throw Win32Exception(); }

   return Process(handle);
}

std::optional<DWORD>
Process::FindByFileName
(std::wstring filename)
{
   std::vector<DWORD> pids;
   DWORD pidsSize = 0;

   EnumProcesses(pids.data(), 0, &pidsSize);

   if (pidsSize == 0) { throw Win32Exception(); }

   pids = std::vector<DWORD>((std::size_t)(pidsSize / sizeof(DWORD)));

   if (!EnumProcesses(pids.data(), pids.size() * sizeof(DWORD), &pidsSize)) { throw Win32Exception(); }

   std::transform(filename.begin(), filename.end(), filename.begin(), std::towlower);
 
   for (auto pid : pids)
   {
      std::optional<DWORD> foundPID = std::nullopt;
      
      try
      {
         auto proc = Process::Open(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
         auto procName = proc.getFileName();
         std::transform(procName.begin(), procName.end(), procName.begin(), std::towlower);

         if (procName == filename) { return std::optional<DWORD>(pid); }
      }
      catch (Exception &e)
      {
         continue;
      }
   }

   return std::nullopt;
}

std::optional<DWORD>
Process::FindByWindowTitle
(std::wstring title)
{
   auto window = Window::FindByTitle(title);

   if (!window.has_value()) { return std::nullopt; }

   return std::optional<DWORD>(window.value().getPID());
}

std::optional<DWORD>
Process::FindByWindowClass
(std::wstring klass)
{
   auto window = Window::FindByClass(klass);

   if (!window.has_value()) { return std::nullopt; }

   return std::optional<DWORD>(window.value().getPID());
}

std::wstring
Process::getModuleName
()
{
   if (std::wcslen(this->moduleName) == 0) {   
      HMODULE module;
      DWORD needed;
   
      if (!EnumProcessModules(this->handle, &module, sizeof(HMODULE), &needed)) { throw Win32Exception(); }
      if (GetModuleBaseNameW(this->handle, module, this->moduleName, MAX_PATH) == 0) { throw Win32Exception(); }
   }

   return std::wstring(this->moduleName);
}

std::wstring
Process::getFileName
()
{
   std::wstring moduleName = this->getModuleName();
   auto moduleNameC = moduleName.c_str();
   auto fileName = moduleNameC+moduleName.length();

   do
   {
      --fileName;

      if (*fileName == L'\\' || *fileName == L'/')
      {
         ++fileName;
         break;
      }
   } while (fileName != moduleNameC);

   return std::wstring(fileName);
}

std::optional<Window>
Process::getMainWindow
()
{
   for (auto window : Window::Enum())
   {
      if (window.getWindow(GW_OWNER).has_value() || window.isVisible()) { continue; }
      if (this->getPID() == window.getPID()) { return std::optional<Window>(window); }
   }

   return std::nullopt;
}
