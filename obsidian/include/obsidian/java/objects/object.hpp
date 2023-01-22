#pragma once

#include <obsidian/std.hpp>
#include <obsidian/thirdparty.hpp>

#include <obsidian/macros.hpp>
#include <obsidian/exception.hpp>
#include <obsidian/proguard.hpp>

namespace obsidian
{
namespace java
{
namespace objects
{
   class String;
   class Class;
   
   class Object
   {
   protected:
      jobject handle;
      bool global;
      
   public:
      JAVA_CLASS_NO_CONSTRUCTOR(java.lang.Object)
      Object(jobject handle, bool global=false) : handle(handle), global(global) {}
      operator jobject() { return this->handle; }

      JAVA_METHOD_HEADER(bool, equals);
      JAVA_METHOD_HEADER(String, toString);
      JAVA_METHOD_HEADER(int, hashCode);
      JAVA_METHOD_HEADER(Class, getClass);

      virtual void release() {
         if (this->global) { this->releaseGlobal(); }
         else { this->releaseLocal(); }
      }

      void releaseLocal() {
         java::env->DeleteLocalRef(*this);
      }

      void releaseGlobal() {
         java::env->DeleteGlobalRef(*this);
      }
   };
}}}
