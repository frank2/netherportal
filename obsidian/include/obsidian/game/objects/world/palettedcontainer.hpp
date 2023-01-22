#pragma once

#include <obsidian/java/objects/object.hpp>

namespace obsidian
{
namespace game
{
   template <typename T>
   class PalettedContainer : public java::objects::Object
   {
   public:
      JAVA_CLASS(PalettedContainer, net.minecraft.world.level.chunk.PalettedContainer)

      JAVA_METHOD(void,
                  acquire,
                  Function(Void()),
                  Void)

      JAVA_METHOD(void,
                  release,
                  Function(Void()),
                  Void)

      JAVA_METHOD(void,
                  set,
                  Function(Void(), Int(), Int(), Int(), java::objects::Object::GetSignature()),
                  Void)

      JAVA_METHOD(T,
                  get,
                  Function(java::objects::Object::GetSignature(), Int(), Int(), Int()),
                  Object)
   };
}}
