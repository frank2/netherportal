#pragma once

#include <obsidian/game/objects/world/stateholder.hpp>

namespace obsidian
{
namespace game
{
   class BlockState;
   class Block;
   
   class BlockBehavior : public java::objects::Object
   {
   public:
      JAVA_CLASS(BlockBehavior, net.minecraft.world.level.block.state.BlockBehavior)

      class BlockStateBase : public StateHolder<Block, BlockState>
      {
      public:
         JAVA_CLASS_NO_CONSTRUCTOR(net.minecraft.world.level.block.state.BlockBehavior$BlockStateBase)
         BlockStateBase(jobject object, bool global=false);
         operator jobject() { return this->handle; }

         JAVA_MEMBER_BOOLEAN(bool, isAir)
      };

      class Properties : public java::objects::Object
      {
      public:
         JAVA_CLASS(Properties, net.minecraft.world.level.block.state.BlockBehavior$Properties)
      };
   };
}}
