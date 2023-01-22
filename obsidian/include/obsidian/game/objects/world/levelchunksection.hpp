#pragma once

#include <obsidian/game/objects/world/palettedcontainer.hpp>
#include <obsidian/game/objects/world/blockstate.hpp>

namespace obsidian
{
namespace game
{
   class LevelChunkSection : public java::objects::Object
   {
   public:
      JAVA_CLASS(LevelChunkSection, net.minecraft.world.level.chunk.LevelChunkSection)

      JAVA_MEMBER_READONLY(PalettedContainer<BlockState>,
                           states,
                           PalettedContainer<BlockState>::GetSignature(),
                           Object)
   };
}}
