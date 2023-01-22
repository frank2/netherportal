#pragma once

#include <obsidian/java/objects/object.hpp>

namespace obsidian
{
namespace java
{
namespace objects
{
   class String : public Object
   {
   public:
      JAVA_CLASS(String, java.lang.String)

      std::string getStdString()
      {
         auto stringData = java::env->GetStringUTFChars(reinterpret_cast<jstring>(this->handle), 0);
         std::string stdString(stringData);

         java::env->ReleaseStringUTFChars(reinterpret_cast<jstring>(this->handle), stringData);

         return stdString;
      }
   };
}}}
