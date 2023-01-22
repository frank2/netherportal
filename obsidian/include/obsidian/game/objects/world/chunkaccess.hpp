#pragma once

#include <obsidian/java/objects/array.hpp>
#include <obsidian/game/objects/world/levelchunksection.hpp>

namespace obsidian
{
namespace game
{
   class ChunkAccess : public java::objects::Object
   {
   public:
      JAVA_CLASS(ChunkAccess, net.minecraft.world.level.chunk.ChunkAccess)

      JAVA_MEMBER_READONLY(java::objects::Array<LevelChunkSection>,
                           sections,
                           Array(LevelChunkSection::GetSignature()),
                           Object)

      JAVA_METHOD(ChunkPos,
                  getPos,
                  Function(ChunkPos::GetSignature()),
                  Object)
