#include <obsidian.hpp>

using namespace obsidian;

JavaVM *java::jvm = nullptr;
JNIEnv *java::env = nullptr;

void
java::init
()
{
   if (java::jvm != nullptr && java::env != nullptr) { return; }
         
   jsize vms;

   JNIException::ThrowOnError(JNI_GetCreatedJavaVMs(&java::jvm, 1, &vms), "Failed to acquire running Java VM.");
   LOG(DEBUG) << "Got JVM: 0x" << std::hex << java::jvm;
         
   jint result = jvm->GetEnv(reinterpret_cast<void**>(&java::env), JNI_VERSION_1_6);

   if (result == JNI_EDETACHED)
   {
      JNIException::ThrowOnError(java::jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), NULL),
                                 "JVM is detached and couldn't reattach.");
   }
   else
   {
      JNIException::ThrowOnError(result, "Failed to get JVM environment.");
   }

   LOG(DEBUG) << "Got JVM env: 0x" << std::hex << env;
}

void
java::release
()
{
   if (java::jvm == nullptr || java::env == nullptr) { return; }

   java::jvm->DetachCurrentThread();

   java::jvm = nullptr;
   java::env = nullptr;
}
   
