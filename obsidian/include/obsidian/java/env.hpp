#pragma once

#include <obsidian/std.hpp>
#include <obsidian/thirdparty.hpp>

namespace obsidian
{
   namespace java
   {
      extern JavaVM *jvm;
      extern JNIEnv *env;

      void init();
      void release();
   }
}

