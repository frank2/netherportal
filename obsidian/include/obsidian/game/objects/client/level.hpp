#pragma once

#include <obsidian/game/objects/client/clientchunkcache.hpp>

namespace obsidian
{
namespace game
{
   class ClientLevel : public java::objects::Object
   {
   public:
      JAVA_CLASS(ClientLevel, net.minecraft.client.multiplayer.ClientLevel)

      JAVA_MEMBER_READONLY(ClientChunkCache,
                           chunkSource,
                           ClientChunkCache::GetSignature(),
                           Object)
   };
}}

