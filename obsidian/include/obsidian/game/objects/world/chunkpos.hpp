#pragma once

#include <obsidian.hpp>

namespace obsidian
{
namespace game
{
   class ChunkPos : public java::objects::Object
   {
   public:
      JAVA_CLASS(net.minecraft.world.level.ChunkPos)

      JAVA_MEMBER_INT(int, x)
      JAVA_MEMBER_INT(int, z)
   };
}}
