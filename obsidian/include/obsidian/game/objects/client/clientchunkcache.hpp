#pragma once

#include <obsidian/java/objects/atomicreferencearray.hpp>
#include <obsidian/game/objects/world/levelchunk.hpp>

namespace obsidian
{
namespace game
{
   class ClientChunkCache : public java::objects::Object
   {
   public:
      class Storage : public java::objects::Object
      {
      public:
         JAVA_CLASS(Storage, net.minecraft.client.multiplayer.ClientChunkCache$Storage)

         JAVA_MEMBER_READONLY(java::objects::AtomicReferenceArray<LevelChunk>,
                              chunks,
                              java::objects::AtomicReferenceArray<LevelChunk>::GetSignature(),
                              Object)
      };

      JAVA_CLASS(ClientChunkCache, net.minecraft.client.multiplayer.ClientChunkCache)

      JAVA_MEMBER_READONLY(ClientChunkCache::Storage,
                           chunkSource,
                           ClientChunkCache::Storage::GetSignature(),
                           Object)

      JAVA_METHOD(LevelChunk,
                  getChunk,
                  Function(LevelChunk::GetSignature(), Int()),
                  Object)
   };
}}

