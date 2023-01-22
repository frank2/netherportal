#include <obsidian.hpp>

using namespace obsidian;
using namespace java::sig;

SpawnedSig Signature::FromJavaString(std::string javaString) {
   if (javaString == "boolean") { return Boolean().spawn(); }
   else if (javaString == "byte") { return Byte().spawn(); }
   else if (javaString == "char") { return Char().spawn(); }
   else if (javaString == "short") { return Short().spawn(); }
   else if (javaString == "int") { return Int().spawn(); }
   else if (javaString == "long") { return Long().spawn(); }
   else if (javaString == "float") { return Float().spawn(); }
   else if (javaString == "double") { return Double().spawn(); }
   else if (javaString == "void") { return Void().spawn(); }
   else if (javaString.find("[]") != std::string::npos) {
      std::size_t bracketPos = javaString.find("[]");
      std::size_t depth = 0;
      auto depthPos = bracketPos;
         
      while (depthPos != std::string::npos)
      {
         depth += 1;
         depthPos = javaString.find("[]", depthPos+2);
      }

      std::string baseTypeString = javaString.substr(0, bracketPos);
      auto baseType = Signature::FromJavaString(baseTypeString);

      while (depth > 0)
      {
         --depth;
         baseType = Array(baseType).spawn();
      }

      return baseType;
   }
   else { return Object(javaString).spawn(); }
}

template <typename T>
SpawnedSig Signature::FromType()
{
   auto typedata = typeid(T);

   if (typedata.name() == "v") { return Void().spawn(); }
   else if (typedata.name() == "b") { return Boolean().spawn(); }
   else if (typedata.name() == "h") { return Byte().spawn(); }
   else if (typedata.name() == "c") { return Char().spawn(); }
   else if (typedata.name() == "s") { return Short().spawn(); }
   else if (typedata.name() == "i") { return Int().spawn(); }
   else if (typedata.name() == "l") { return Long().spawn(); }
   else if (typedata.name() == "f") { return Float().spawn(); }
   else if (typedata.name() == "d") { return Double().spawn(); }
   else if (std::is_base_of(java::objects::Object, std::remove_pointer<T>::type)::value) {
      return std::remove_pointer<T>::type::GetSignature().spawn();
   }
   else { throw Exception("Failed to resolve type into a Java signature."); }
}
