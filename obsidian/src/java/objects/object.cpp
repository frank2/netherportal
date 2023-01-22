#include <obsidian.hpp>

namespace obsidian
{
namespace java
{
namespace objects
{
   JAVA_METHOD_EXT(Object,
                   bool,
                   equals,
                   Function(Bool(), Object::GetSignature()),
                   Boolean)
   
   JAVA_METHOD_EXT(Object,
                   String,
                   toString,
                   Function(String::GetSignature()),
                   Object)
   
   JAVA_METHOD_EXT(Object,
                   int,
                   hashCode,
                   Function(Int()),
                   Int)

   JAVA_METHOD_EXT(Object,
                   Class,
                   getClass,
                   Function(Class::GetSignature()),
                   Object)
}}}
