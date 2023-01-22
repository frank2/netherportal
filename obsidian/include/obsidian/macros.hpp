#pragma once

#define TOKEN_MERGE_IMPL(x, y) x##y
#define TOKEN_MERGE(x, y) TOKEN_MERGE_IMPL(x, y)

#define DECLARE_PROPERTY(type, name) __declspec(property(get = propGet_##name, put = propSet_##name)) type name
#define DECLARE_READONLY_PROPERTY(type, name) __declspec(property(get = propGet_##name)) type name
#define PROPERTY_GET(type, name) type propGet_##name()
#define PROPERTY_GET_EXT(className, type, name) type className::propGet_##name()
#define PROPERTY_SET(type, name) void propSet_##name(type value)
#define PROPERTY_SET_EXT(className, type, name) void className::propSet_##name(type value)

#define JAVA_CLASS_NAKED_HEADER(JavaClass)\
   static jclass GetClass()

#define JAVA_CLASS_NAKED(JavaClass) \
   static jclass GetClass() \
   {\
      using namespace obsidian;\
      using namespace obsidian::java;\
      using namespace obsidian::java::sig;\
      \
      static jclass objClass = nullptr;\
      \
      if (objClass == nullptr)\
      {\
         auto mapped = ProGuard::ResolveObject(#JavaClass);                 \
         auto sigNotation = sig::Object(mapped).sigNotation();\
         LOG(DEBUG) << #JavaClass << " mapped to " << sigNotation;   \
         objClass = java::env->FindClass(sigNotation.c_str()); \
         if (objClass == nullptr) { throw Exception("Java class not found: " #JavaClass); } \
      }\
      \
      return objClass;\
   }\
   static std::string GetClassString() {\
      static std::string classStr = #JavaClass;\
      \
      return classStr;\
   }

#define JAVA_CLASS_CONSTRUCTOR(ClassName) \
   ClassName(jobject handle, bool global=false) : obsidian::java::objects::Object(handle, global) {}\
   operator jobject() { return this->handle; }

#define JAVA_CLASS_SIGNATURE(JavaClass) \
   static obsidian::java::sig::Object GetSignature() {\
      static obsidian::java::sig::Object obj(#JavaClass);\
      \
      return obj;\
   }

#define JAVA_CLASS(ClassName, JavaClass) \
   JAVA_CLASS_NAKED(JavaClass)\
   JAVA_CLASS_SIGNATURE(JavaClass)\
   JAVA_CLASS_CONSTRUCTOR(ClassName)

#define JAVA_CLASS_NO_CONSTRUCTOR(JavaClass) \
   JAVA_CLASS_NAKED(JavaClass)\
   JAVA_CLASS_SIGNATURE(JavaClass)

#define JAVA_CLASS_NO_SIGNATURE(ClassName, JavaClass) \
   JAVA_CLASS_NAKED(JavaClass)\
   JAVA_CLASS_CONSTRUCTOR(ClassName)

#define JAVA_METHOD_HEADER(returnType, name)   \
   template <typename... Args>\
   returnType name(Args&&... args)

#define JAVA_METHOD_CODE(returnType, name, signature, methodType) \
   {\
      using namespace obsidian;\
      using namespace obsidian::java;\
      using namespace obsidian::java::sig;      \
      \
      static jmethodID methodID = nullptr;      \
      \
      if (methodID == nullptr)                  \
      {                                         \
         jclass objClass = GetClass();                            \
         auto methodSig = (signature).spawn();                         \
         auto mapping = ProGuard::ResolveMember(GetClassString(), methodSig->toString(), #name);\
         auto mappedSig = ProGuard::MapSignature(methodSig);\
         methodID = java::env->GetMethodID(objClass, mapping.c_str(), mappedSig->toString().c_str()); \
         if (methodID == nullptr) {\
            std::stringstream errStream;\
            errStream << "Method " << GetClassString() << "::" << #name\
                      << " with mapping " << mapping\
                      << " and signature " << mappedSig->toString() << " not found.";\
            throw Exception(errStream.str());\
         }\
      }                                                                 \
      static_cast<returnType>(java::env->TOKEN_MERGE(Call, methodType ## Method)(*this, methodID, std::forward<Args>(args)...)); \
   }

#define JAVA_METHOD(returnType, name, signature, methodType) \
   JAVA_METHOD_HEADER(returnType, name)\
   JAVA_METHOD_CODE(returnType, name, signature, methodType)

#define JAVA_METHOD_EXT(className, returnType, name, signature, methodType)  \
   template <typename... Args>\
   returnType className::name(Args&&... args)\
   JAVA_METHOD_CODE(returnType, name, signature, methodType)

#define STATIC_JAVA_METHOD_HEADER(returnType, name) \
   template <typename... Args>                      \
   static returnType name(Args&&... args)

#define STATIC_JAVA_METHOD_CODE(returnType, name, signature, methodType) \
   {\
      using namespace obsidian;\
      using namespace obsidian::java;\
      using namespace obsidian::java::sig;      \
      \
      static jmethodID methodID = nullptr;      \
      \
      if (methodID == nullptr)                  \
      {                                         \
         jclass objClass = GetClass();                            \
         auto methodSig = (signature).spawn();                         \
         auto mapping = ProGuard::ResolveMember(GetClassString(), methodSig->toString(), #name);\
         auto mappedSig = ProGuard::MapSignature(methodSig);\
         methodID = java::env->GetStaticMethodID(objClass, mapping.c_str(), mappedSig->toString().c_str()); \
         if (methodID == nullptr) {\
            std::stringstream errStream;\
            errStream << "Method " << GetClassString() << "::" << #name\
                      << " with mapping " << mapping\
                      << " and signature " << mappedSig->toString() << " not found.";\
            throw Exception(errStream.str());\
         }\
      }                                                                 \
      static_cast<returnType>(java::env->TOKEN_MERGE(CallStatic, methodType ## Method)(GetClass(), methodID, std::forward<Args>(args)...)); \
   }

#define STATIC_JAVA_METHOD(returnType, name, signature, methodType) \
   STATIC_JAVA_METHOD_HEADER(returnType, name)\
   STATIC_JAVA_METHOD_CODE(returnType, name, signature, methodType)

#define STATIC_JAVA_METHOD_EXT(className, returnType, name, signature, methodType) \
   template <typename... Args>                      \
   static returnType className::name(Args&&... args)\
   STATIC_JAVA_METHOD_CODE(returnType, name, signature, methodType)

#define JAVA_MEMBER_GET_HEADER(returnType, name) PROPERTY_GET(returnType, name)
#define JAVA_MEMBER_GET_CODE(returnType, name, signature, memberType) \
   {\
      using namespace obsidian;\
      using namespace obsidian::java;\
      using namespace obsidian::java::sig;\
      \
      static jfieldID fieldID = nullptr;\
                                        \
      if (fieldID == nullptr)           \
      {                                         \
         auto memberSig = (signature).spawn();              \
         auto mapping = ProGuard::ResolveMember(GetClassString(), memberSig->toString(), #name); \
         auto mappedSig = ProGuard::MapSignature(memberSig);            \
         fieldID = java::env->GetFieldID(GetClass(), mapping.c_str(), mappedSig->toString().c_str()); \
         if (fieldID == nullptr) {\
            std::stringstream errStream;\
            errStream << "Field " << GetClassString() << "::" << #name\
                      << " with mapping " << mapping\
                      << " and signature " << mappedSig->toString() << " not found.";\
            throw Exception(errStream.str());\
         }\
      }\
      \
      return static_cast<returnType>(java::env->TOKEN_MERGE(Get, memberType ## Field)(*this, fieldID)); \
   }
#define JAVA_MEMBER_GET(returnType, name, signature, memberType) \
   JAVA_MEMBER_GET_HEADER(returnType, name)\
   JAVA_MEMBER_GET_CODE(returnType, name, signature, memberType)

#define JAVA_MEMBER_GET_EXT(className, returnType, name, signature, memberType) \
   PROPERTY_GET_EXT(className, returnType, name)\
   JAVA_MEMBER_GET_CODE(returnType, name, signature, memberType)

#define JAVA_MEMBER_SET_HEADER(returnType, name) PROPERTY_SET(returnType, name)
#define JAVA_MEMBER_SET_CODE(returnType, name, signature, memberType, nativeType) \
   {\
      using namespace obsidian;\
      using namespace obsidian::java;\
      using namespace obsidian::java::sig;\
      \
      static jfieldID fieldID = nullptr;        \
                                        \
      if (fieldID == nullptr)           \
      {                                         \
         auto memberSig = (signature).spawn();              \
         auto mapping = ProGuard::ResolveMember(GetClassString(), memberSig->toString(), #name); \
         auto mappedSig = ProGuard::MapSignature(memberSig);            \
         fieldID = java::env->GetFieldID(GetClass(), mapping.c_str(), mappedSig->toString().c_str()); \
         if (fieldID == nullptr) {\
            std::stringstream errStream;\
            errStream << "Field " << GetClassString() << "::" << #name\
                      << " with mapping " << mapping\
                      << " and signature " << mappedSig->toString() << " not found.";\
            throw Exception(errStream.str());\
         }\
      }\
      \
      java::env->TOKEN_MERGE(Set, memberType ## Field)(*this, fieldID, (nativeType)value); \
   }

#define JAVA_MEMBER_SET(returnType, name, signature, memberType, nativeType) \
   JAVA_MEMBER_SET_HEADER(returnType, name)\
   JAVA_MEMBER_SET_CODE(returnType, name, signature, memberType, nativeType)

#define JAVA_MEMBER_SET_EXT(className, returnType, name, signature, memberType, nativeType) \
   PROPERTY_SET_EXT(className, returnType, name)\
   JAVA_MEMBER_SET_CODE(returnType, name, signature, memberType, nativeType)
   
#define JAVA_MEMBER(returnType, name, signature, memberType, nativeType) \
   DECLARE_PROPERTY(returnType, name);\
   JAVA_MEMBER_GET(returnType, name, signature, memberType)\
   JAVA_MEMBER_SET(returnType, name, signature, memberType, nativeType)

#define JAVA_MEMBER_EXT(className, returnType, name, signature, memberType, nativeType) \
   JAVA_MEMBER_GET_EXT(className, returnType, name, signature, memberType)\
   JAVA_MEMBER_SET_EXT(className, returnType, name, signature, memberType, nativeType)

#define JAVA_MEMBER_HEADER(returnType, name)   \
   DECLARE_PROPERTY(returnType, name);\
   JAVA_MEMBER_GET_HEADER(returnType, name);\
   JAVA_MEMBER_SET_HEADER(returnType, name);

#define JAVA_MEMBER_BOOLEAN(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Boolean(), Boolean, jboolean)
#define JAVA_MEMBER_BYTE(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Byte(), Byte, jbyte)
#define JAVA_MEMBER_CHAR(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Char(), Char, jchar)
#define JAVA_MEMBER_SHORT(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Short(), Short, jshort)
#define JAVA_MEMBER_INT(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Int(), Int, jint)
#define JAVA_MEMBER_LONG(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Long(), Long, jlong)
#define JAVA_MEMBER_FLOAT(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Float(), Float, jfloat)
#define JAVA_MEMBER_DOUBLE(returnType, name) JAVA_MEMBER(returnType, name, obsidian::java::sig::Double(), Double, jdouble)
#define JAVA_MEMBER_OBJECT(returnType, name) JAVA_MEMBER(returnType, name, (returnType)::GetSignature(), Object, jobject)

#define JAVA_MEMBER_READONLY(returnType, name, signature, memberType) \
   DECLARE_READONLY_PROPERTY(returnType, name);\
   JAVA_MEMBER_GET(returnType, name, signature, memberType)

#define JAVA_MEMBER_READONLY_HEADER(returnType, name) \
   DECLARE_READONLY_PROPERTY(returnType, name);\
   JAVA_MEMBER_GET_HEADER(returnType, name);\

#define JAVA_MEMBER_READONLY_EXT(className, returnType, name, signature, memberType) \
   JAVA_MEMBER_GET_EXT(className, returnType, name, signature, memberType)

#define STATIC_JAVA_MEMBER_GET_HEADER(returnType, name) \
   static returnType get_##name()

#define STATIC_JAVA_MEMBER_GET_CODE(returnType, name, signature, memberType) \
   {                                            \
      using namespace obsidian;\
      using namespace obsidian::java;\
      using namespace obsidian::java::sig;\
      \
      static jfieldID fieldID = nullptr;  \
                                        \
      if (fieldID == nullptr)           \
      {                                         \
         auto memberSig = (signature).spawn();              \
         auto mapping = ProGuard::ResolveMember(GetClassString(), memberSig->toString(), #name); \
         auto mappedSig = ProGuard::MapSignature(memberSig);            \
         fieldID = java::env->GetStaticFieldID(GetClass(), mapping.c_str(), mappedSig->toString().c_str()); \
         if (fieldID == nullptr) {\
            std::stringstream errStream;\
            errStream << "Field " << GetClassString() << "::" << #name\
                      << " with mapping " << mapping\
                      << " and signature " << mappedSig->toString() << " not found.";\
            throw Exception(errStream.str());\
         }\
      }\
      \
      return static_cast<returnType>(java::env->TOKEN_MERGE(GetStatic, memberType ## Field)(GetClass(), fieldID)); \
   }
       
#define STATIC_JAVA_MEMBER_GET(returnType, name, signature, memberType) \
   STATIC_JAVA_MEMBER_GET_HEADER(returnType, name)\
   STATIC_JAVA_MEMBER_GET_CODE(returnType, name, signature, memberType)

#define STATIC_JAVA_MEMBER_GET_EXT(className, returnType, name, signature, memberType) \
   static returnType className::get_##name()\
   STATIC_JAVA_MEMBER_GET_CODE(returnType, name, signature, memberType)

#define STATIC_JAVA_MEMBER_SET_HEADER(returnType, name) \
   static void set_##name(returnType value)

#define STATIC_JAVA_MEMBER_SET_CODE(returnType, name, signature, memberType, nativeType) \
   {\
      using namespace obsidian;\
      using namespace obsidian::java::sig;\
      \
      static jfieldID fieldID = nullptr;  \
                                        \
      if (fieldID == nullptr)           \
      {                                         \
         auto memberSig = (signature).spawn();              \
         auto mapping = ProGuard::ResolveMember(GetClassString(), memberSig->toString(), #name); \
         auto mappedSig = ProGuard::MapSignature(memberSig);            \
         fieldID = java::env->GetStaticFieldID(GetClass(), mapping.c_str(), mappedSig->toString().c_str()); \
         if (fieldID == nullptr) {\
            std::stringstream errStream;\
            errStream << "Field " << GetClassString() << "::" << #name\
                      << " with mapping " << mapping\
                      << " and signature " << mappedSig->toString() << " not found.";\
            throw Exception(errStream.str());\
         }\
      }\
      \
      java::env->TOKEN_MERGE(SetStatic, memberType ## Field)(GetClass(), fieldID, (nativeType)value); \
   }

#define STATIC_JAVA_MEMBER_SET(returnType, name, signature, memberType, nativeType) \
   STATIC_JAVA_MEMBER_SET_HEADER(returnType, name)\
   STATIC_JAVA_MEMBER_SET_CODE(returnType, name, signature, memberType, nativeType)

#define STATIC_JAVA_MEMBER_SET_EXT(className, returnType, name, signature, memberType, nativeType) \
   static void className::set_##name(returnType value)\
   STATIC_JAVA_MEMBER_SET_CODE(returnType, name, signature, memberType, nativeType)
   
#define STATIC_JAVA_MEMBER(returnType, name, signature, memberType, nativeType) \
   STATIC_JAVA_MEMBER_GET(returnType, name, signature, memberType)\
   STATIC_JAVA_MEMBER_SET(returnType, name, signature, memberType, nativeType)

#define STATIC_JAVA_MEMBER_HEADER(returnType, name)\
   STATIC_JAVA_MEMBER_GET_HEADER(returnType, name);   \
   STATIC_JAVA_MEMBER_SET_HEADER(returnType, name);

#define STATIC_JAVA_MEMBER_EXT(className, returnType, name, signature, memberType, nativeType) \
   STATIC_JAVA_MEMBER_GET_EXT(className, returnType, name, signature, memberType, nativeType); \
   STATIC_JAVA_MEMBER_SET_EXT(className, returnType, name, signature, memberType, nativeType);

#define STATIC_JAVA_MEMBER_BOOLEAN(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Boolean(), Boolean, jboolean)
#define STATIC_JAVA_MEMBER_BYTE(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Byte(), Byte, jbyte)
#define STATIC_JAVA_MEMBER_CHAR(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Char(), Char, jchar)
#define STATIC_JAVA_MEMBER_SHORT(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Short(), Short, jshort)
#define STATIC_JAVA_MEMBER_INT(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Int(), Int, jint)
#define STATIC_JAVA_MEMBER_LONG(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Long(), Long, jlong)
#define STATIC_JAVA_MEMBER_FLOAT(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Float(), Float, jfloat)
#define STATIC_JAVA_MEMBER_DOUBLE(returnType, name) STATIC_JAVA_MEMBER(returnType, name, obsidian::java::sig::Double(), Double, jdouble)
#define STATIC_JAVA_MEMBER_OBJECT(returnType, name) STATIC_JAVA_MEMBER(returnType, name, (returnType)::GetSignature(), Object, jobject)

#define STATIC_JAVA_MEMBER_READONLY(returnType, name, signature, memberType) \
   STATIC_JAVA_MEMBER_GET(returnType, name, signature, memberType)
   
#define STATIC_JAVA_MEMBER_READONLY_EXT(className, returnType, name, signature, memberType) \
   STATIC_JAVA_MEMBER_GET_EXT(className, returnType, name, signature, memberType)

