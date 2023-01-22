#pragma once

#include <obsidian/std.hpp>
#include <obsidian/thirdparty.hpp>
#include <obsidian/exception.hpp>
#include <obsidian/proguard.hpp>

namespace obsidian
{
   namespace game
   {
      std::string version()
      {
         static std::string version;
         if (version.size() != 0) { return version; }
   
         std::string commandLine = GetCommandLineA();
      
         auto arg = std::string("--version");
         auto argPos = commandLine.find(arg);
         if (argPos == std::string::npos) { throw Exception("Couldn't find version argument in commandline."); }

         auto versionPos = argPos + arg.size() + 1;
         auto whitespacePos = commandLine.find(" ", versionPos);
         version = commandLine.substr(versionPos, whitespacePos-versionPos);

         LOG(DEBUG) << "Version string found: " << version;
   
         return version;
      }

      std::string configPath()
      {
         static std::string configPath;

         if (configPath.size() != 0) { return configPath; }

         std::string envPath("%APPDATA%/.minecraft");

         std::vector<char> resolvedPath;
         std::size_t resolvedPathSize = ExpandEnvironmentStringsA(envPath.c_str(), resolvedPath.data(), resolvedPath.size());
         if (resolvedPathSize == 0) { Win32Exception::ThrowLastError(); }

         resolvedPath.resize(resolvedPathSize);
         resolvedPathSize = ExpandEnvironmentStringsA(envPath.c_str(), resolvedPath.data(), resolvedPath.size());
         if (resolvedPathSize == 0) { Win32Exception::ThrowLastError(); }

         configPath = std::string(resolvedPath.data());

         LOG(DEBUG) << "Config path built: " << configPath;
         
         return configPath;
      }

      std::string versionPath()
      {
         static std::string versionPath;  
         if (versionPath.size() != 0) { return versionPath; }
   
         auto version = game::version();
         std::stringstream configPath;
         configPath << game::configPath() << "/versions/" << version;
         versionPath = configPath.str();

         LOG(DEBUG) << "Version path built: " << versionPath;

         return versionPath;
      }

      void init()
      {
         std::stringstream mappingFilename;
         mappingFilename << versionPath() << "/client_mappings.txt";

         std::ifstream mappingFile(mappingFilename.str().c_str());

         if (!mappingFile.good())
         {
            mappingFile.close();

            LOG(DEBUG) << "No mapping file, downloading it.";

            auto config = game::config();
               
            if (!config.contains("downloads"))
            {
               throw Exception("Config contains no download section.");
            }

            if (!config["downloads"].contains("client_mappings"))
            {
               throw Exception("Downloads section does not contain client mappings.");
            }

            if (!config["downloads"]["client_mappings"].contains("url"))
            {
               throw Exception("Client mappings does not contain a URL.");
            }

            auto url = config["downloads"]["client_mappings"]["url"].get<std::string>();
            LOG(DEBUG) << "Found mapping URL: " << url;
      
            COMException::ThrowOnError(URLDownloadToFileA(NULL,
                                                          url.c_str(),
                                                          mappingFilename.str().c_str(),
                                                          0,
                                                          NULL));

            mappingFile.open(mappingFilename.str().c_str());

            if (!mappingFile.good())
            {
               throw Exception("Mapping download failed somehow.");
            }

            LOG(DEBUG) << "File downloaded to " << mappingFilename.str();
         }

         ProGuard::ParseFile(mappingFile);
      }

      json
      config
      ()
      {
         static json config = nullptr;
         if (config != nullptr) { return config; }
  
         std::stringstream configPath;
         configPath << game::versionPath() << "/" << game::version() << ".json";
   
         std::ifstream configFile(configPath.str().c_str());
         if (!configFile.good()) { throw Exception("Couldn't find config file for currently running game."); }

         LOG(DEBUG) << "Found config file: " << configPath.str();
         
         config = json::parse(configFile);
         return config;
      }
   }
}
