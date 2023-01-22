#pragma once

#include <obsidian/java/objects/object.hpp>
#include <obsidian/java/objects/string.hpp>

namespace obsidian
{
namespace java
{
namespace objects
{
   class Class : public Object
   {
   public:
      JAVA_CLASS(Class, java.lang.Class);

      JAVA_METHOD(String,
                  getName,
                  Function(String::GetSignature()),
                  Object)
   };
}}}

      
