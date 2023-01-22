#pragma once

#include <obsidian/game/objects/client/level.hpp>

namespace obsidian
{
namespace game
{
   class ClientLevel;
   
   class Minecraft : public java::objects::Object
   {
   public:
      JAVA_CLASS(Minecraft, net.minecraft.client.Minecraft)

      STATIC_JAVA_MEMBER_READONLY(Minecraft,
                                  instance,
                                  Minecraft::GetSignature(),
                                  Object)

      JAVA_MEMBER_READONLY(ClientLevel,
                           level,
                           ClientLevel::GetSignature(),
                           Object)

   };
}}
