#pragma once

#include <obsidian/java/sig.hpp>

namespace obsidian
{
   class ProGuard
   {
   public:
      struct ObjectMember
      {
         std::string signature;
         std::string name;
         
         struct Hash
         {
            std::size_t operator()(const ObjectMember& member) const {
               return std::hash<std::string>()(member.signature) ^ std::hash<std::string>()(member.name);
            }
         };
         
         bool operator==(const ObjectMember &other) const {
            return this->signature == other.signature && this->name == other.name;
         }
      };

      using ObjectMemberMapping = std::unordered_map<
         ObjectMember,
         std::string,
         ObjectMember::Hash>;

      struct ObjectMap
      {
         std::string objectMapping;
         ObjectMemberMapping memberMappings;
      };

      using ObjectMappings = std::map<std::string, ObjectMap>;
      
      ObjectMappings data;

      ProGuard() {}

   public:
      static ProGuard* Instance()
      {
         static std::unique_ptr<ProGuard> ptr = std::make_unique<ProGuard>();

         return ptr.get();
      }

   public:
      static void ParseFile(std::ifstream &mappingFile)
      {
         auto ptr = Instance();
         std::string line;
         std::string lastObject;
            
         LOG(DEBUG) << "Parsing mapping file, this may take a moment...";
            
         while (std::getline(mappingFile, line))
         {
            std::stringstream lineStream;
            lineStream << line;

            std::string typeOrComment;
            lineStream >> typeOrComment;

            if (typeOrComment[0] == '#') { /* LOG(DEBUG) << "Found comment."; */ continue; }

            std::string nameOrArrow;
            lineStream >> nameOrArrow;

            if (nameOrArrow == "->")
            {
               std::string typeName, arrow, mapping;
               typeName = typeOrComment;
               arrow = nameOrArrow;
               lineStream >> mapping;

               auto colonPos = mapping.find(':');
               if (colonPos != std::string::npos) { mapping = mapping.substr(0, colonPos); }

               //LOG(DEBUG) << "Found object declaration: "
               //           << typeName << " -> " << mapping;
                  
               auto blankMap = ObjectMap{mapping,ObjectMemberMapping()};
               ptr->data.insert(std::make_pair(typeName, blankMap));

               lastObject = typeName;
            }
            else if (typeOrComment.find(':') != std::string::npos) {
               std::string linesAndTypeName, nameAndArgs, arrow, mapping;
               linesAndTypeName = typeOrComment;
               nameAndArgs = nameOrArrow;
               lineStream >> arrow >> mapping; 

               auto colonPos = linesAndTypeName.find(':');
               colonPos = linesAndTypeName.find(':', colonPos+1);
               auto typePos = colonPos+1;
               auto typeName = linesAndTypeName.substr(typePos, linesAndTypeName.size()-typePos);

               auto argPos = nameAndArgs.find('(')+1;
               auto funcName = nameAndArgs.substr(0, argPos-1);
               auto args = nameAndArgs.substr(argPos, nameAndArgs.size()-argPos-1);

               //LOG(DEBUG) << "Found function: "
               //           << typeName << " " << funcName
               //           << "(" << args << ") -> " << mapping;

               auto mapData = ptr->data.find(lastObject);
               
               if (mapData == ptr->data.end()) {
                  std::stringstream ss;
                  ss << "Failed to find object: " << lastObject;
                  throw Exception(ss.str());
               }

               auto typeSig = java::sig::Function::FromJavaString(typeName, args)->toString();

               //LOG(DEBUG) << "New function signature: " << typeSig << " -> " << match[2];
                  
               auto key = ObjectMember{typeSig,funcName};
               mapData->second.memberMappings.insert(std::make_pair(key, mapping));
            }
            else {
               std::string typeName, memberName, arrow, mapping;
               typeName = typeOrComment;
               memberName = nameOrArrow;
               lineStream >> arrow >> mapping;

               //LOG(DEBUG) << "Found member: " << typeName << " " << memberName << " -> " << mapping;

               auto mapData = ptr->data.find(lastObject);

               if (mapData == ptr->data.end()) {
                  std::stringstream ss;
                  ss << "Failed to find object: " << lastObject;
                  throw Exception(ss.str());
               }

               auto typeSig = java::sig::Signature::FromJavaString(typeName)->toString();
                  
               //LOG(DEBUG) << "New type signature: " << typeSig << " -> " << match[2];
                  
               auto key = ObjectMember{typeSig,memberName};
               mapData->second.memberMappings.insert(std::make_pair(key, mapping));
            }
         }
            
         mappingFile.close();
      }
            
      static std::string ResolveObject(std::string object)
      {
         auto instance = ProGuard::Instance();
         auto search = instance->data.find(object);

         if (search == instance->data.end()) {
            LOG(DEBUG) << "Object not found, returning object name.";
            return object;
         }
         else {
            LOG(DEBUG) << "Object mapping found for " << object << ": " << search->second.objectMapping;
            return search->second.objectMapping;
         }
      }
      static std::string ResolveMember(std::string object, std::string signature, std::string name)
      {
         auto instance = ProGuard::Instance();
         auto search = instance->data.find(object);

         if (search == instance->data.end()) {
            LOG(DEBUG) << "Object " << object << " not found, returning member name.";
            return name;
         }

         auto mappings = search->second;
         auto key = ObjectMember{signature, name};
         auto memberSearch = mappings.memberMappings.find(key);

         if (memberSearch == mappings.memberMappings.end()) {
            LOG(DEBUG) << "Object " << name << " with signature \"" << signature << "\" not found, returning member name.";
            return name;
         }
         else {
            LOG(DEBUG) << "Object mapping for " << object << "::" << name << " found: " << memberSearch->second;
            return memberSearch->second;
         }
      }
      static java::sig::SpawnedSig MapSignature(java::sig::SpawnedSig sig)
      {
         auto sigType = sig->sigType();

         if (sigType == "Object")
         {
            auto castSig = std::reinterpret_pointer_cast<java::sig::Object>(sig);
            auto newClass = ProGuard::ResolveObject(castSig->baseNotation());

            return java::sig::Object(newClass).spawn();
         }
         else if (sigType == "Array")
         {
            auto castSig = std::reinterpret_pointer_cast<java::sig::Array>(sig);
            auto recastBase = MapSignature(castSig->arrayType);

            return java::sig::Array(recastBase).spawn();
         }
         else if (sigType == "Function")
         {
            auto castSig = std::reinterpret_pointer_cast<java::sig::Function>(sig);
            auto recastReturn = MapSignature(castSig->returnType);
            std::vector<java::sig::SpawnedSig> arguments;

            for (auto arg : castSig->arguments) { arguments.push_back(MapSignature(arg)); }

            return java::sig::Function(recastReturn, arguments).spawn();
         }
         else { return sig; }
      }
   };
}
