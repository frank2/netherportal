#pragma once

#include <obsidian/java/objects/object.hpp>

namespace obsidian
{
namespace java
{
namespace objects
{
   template <typename T>
   class AtomicReferenceArray : public Object
   {
   public:
      JAVA_CLASS(AtomicReferenceArray, java.util.concurrent.atomic.AtomicReferenceArray)

      JAVA_METHOD(T,
                  get,
                  Function(java::Object::GetSignature().spawn(), Int().spawn()),
                  Object)

      JAVA_METHOD(T,
                  set,
                  Function(Void().spawn(), Int().spawn(), java::Object::GetSignature().spawn()),
                  Void)

      JAVA_METHOD(int,
                  length,
                  Function(Int()),
                  Int)

      T operator[](int i) { return this->get(i); }
   };
}}}
