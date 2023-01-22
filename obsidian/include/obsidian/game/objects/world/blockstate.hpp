#pragma once

#include <obsidian/game/objects/world/blockbehavior.hpp>

namespace obsidian
{
namespace game
{
   class BlockState : public BlockBehavior::BlockStateBase
   {
   public:
      JAVA_CLASS(BlockState, net.minecraft.world.level.block.state.BlockState)
   };
}}
      
