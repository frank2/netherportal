#pragma once

#include <obsidian/java/sig.hpp>
#include <obsidian/java/objects/object.hpp>

namespace obsidian
{
namespace java
{
namespace objects
{
   // arrays are treated in a special way in the JNI environment.
   template <typename T>
   class Array : public Object
   {
   public:
      struct Element;
      
      class PrimitiveArray : public std::enable_shared_from_this<PrimitiveArray>
      {
      public:
         Array<T> &parent;
         std::any primitive;

         PrimitiveArray(Array<T> &parent) : parent(parent) {
            auto sig = Array<T>::GetSignature();
            auto sigType = sig.arrayType->sigType();

            if (sigType == "Boolean") {
               this->primitive = java::env->GetBooleanArrayElements(reinterpret_cast<jbooleanArray>(parent.handle), 0);
            }
            else if (sigType == "Byte") {
               this->primitive = java::env->GetByteArrayElements(reinterpret_cast<jbyteArray>(parent.handle), 0);
            }
            else if (sigType == "Char") {
               this->primitive = java::env->GetCharArrayElements(reinterpret_cast<jcharArray>(parent.handle), 0);
            }
            else if (sigType == "Short") {
               this->primitive = java::env->GetShortArrayElements(reinterpret_cast<jshortArray>(parent.handle), 0);
            }
            else if (sigType == "Int") {
               this->primitive = java::env->GetIntArrayElements(reinterpret_cast<jintArray>(parent.handle), 0);
            }
            else if (sigType == "Long") {
               this->primitive = java::env->GetLongArrayElements(reinterpret_cast<jlongArray>(parent.handle), 0);
            }
            else if (sigType == "Float") {
               this->primitive = java::env->GetFloatArrayElements(reinterpret_cast<jfloatArray>(parent.handle), 0);
            }
            else if (sigType == "Double") {
               this->primitive = java::env->GetDoubleArrayElements(reinterpret_cast<jdoubleArray>(parent.handle), 0);
            }
            else if (sigType == "Object" || sigType == "Array") {
               this->primitive = java::env->GetObjectArrayElements(reinterpret_cast<jobjectArray>(parent.handle), 0);
            }
         }
         PrimitiveArray(PrimitiveArray &prim) : parent(prim.parent), primitive(prim.primitive) {}
         ~PrimitiveArray() {
            if (this->primitive.type() == typeid(jboolean *)) {
               java::env->ReleaseBooleanArrayElements(reinterpret_cast<jbooleanArray>(parent.handle),
                                                      std::any_cast<jboolean *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jbyte *)) {
               java::env->ReleaseByteArrayElements(reinterpret_cast<jbyteArray>(parent.handle),
                                                      std::any_cast<jbyte *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jchar *)) {
               java::env->ReleaseCharArrayElements(reinterpret_cast<jcharArray>(parent.handle),
                                                      std::any_cast<jchar *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jshort *)) {
               java::env->ReleaseShortArrayElements(reinterpret_cast<jshortArray>(parent.handle),
                                                      std::any_cast<jshort *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jint *)) {
               java::env->ReleaseIntArrayElements(reinterpret_cast<jintArray>(parent.handle),
                                                      std::any_cast<jint *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jlong *)) {
               java::env->ReleaseLongArrayElements(reinterpret_cast<jlongArray>(parent.handle),
                                                      std::any_cast<jlong *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jfloat *)) {
               java::env->ReleaseFloatArrayElements(reinterpret_cast<jfloatArray>(parent.handle),
                                                      std::any_cast<jfloat *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jdouble *)) {
               java::env->ReleaseDoubleArrayElements(reinterpret_cast<jdoubleArray>(parent.handle),
                                                      std::any_cast<jdouble *>(this->primitive),
                                                      0);
            }
            else if (this->primitive.type() == typeid(jobject *)) {
               java::env->ReleaseObjectArrayElements(reinterpret_cast<jobjectArray>(parent.handle),
                                                      std::any_cast<jobject *>(this->primitive),
                                                      0);
            }
            else { throw Exception(std::string("Encountered unknown type ID: ") + this->primitive.type().name()); }
         }

         std::shared_ptr<Element> get(std::size_t i) {
            if (i >= this->parent.length) { throw Exception("Array index out of bounds."); }

            if (this->primitive.type() == typeid(jboolean *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jboolean *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jbyte *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jbyte *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jchar *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jchar *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jshort *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jshort *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jint *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jint *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jlong *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jlong *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jfloat *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jfloat *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jdouble *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jdouble *>(this->primitive)[i]);
            }
            else if (this->primitive.type() == typeid(jobject *)) {
               return std::make_shared<Element>(this->shared_from_this(),
                                                &std::any_cast<jobject *>(this->primitive)[i]);
            }
            else { throw Exception(std::string("Encountered unknown type ID: " + this->primitive.type().name())); }
         }

         std::shared_ptr<Element> operator[](std::size_t i) { return this->get(i); }
      };

      class Element : std::enable_shared_from_this<Element>
      {
      public:
         std::shared_ptr<PrimitiveArray> parent;
         std::any primitive;

         Element(std::shared_ptr<PrimitiveArray> parent, jboolean *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jbyte *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jchar *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jshort *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jint *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jlong *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jfloat *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jdouble *primitive)
            : parent(parent),
              primitive(primitive) {}
         Element(std::shared_ptr<PrimitiveArray> parent, jobject *primitive)
            : parent(parent),
              primitive(primitive) {}

         T get() {
            auto prim = this->primitive;

            if (prim.type() == typeid(jboolean *)) {
               return static_cast<T>(*std::any_cast<jboolean *>(prim));
            }
            else if (prim.type() == typeid(jbyte *)) {
               return static_cast<T>(*std::any_cast<jbyte *>(prim));
            }
            else if (prim.type() == typeid(jchar *)) {
               return static_cast<T>(*std::any_cast<jchar *>(prim));
            }
            else if (prim.type() == typeid(jshort *)) {
               return static_cast<T>(*std::any_cast<jshort *>(prim));
            }
            else if (prim.type() == typeid(jint *)) {
               return static_cast<T>(*std::any_cast<jint *>(prim));
            }
            else if (prim.type() == typeid(jlong *)) {
               return static_cast<T>(*std::any_cast<jlong *>(prim));
            }
            else if (prim.type() == typeid(jfloat *)) {
               return static_cast<T>(*std::any_cast<jfloat *>(prim));
            }
            else if (prim.type() == typeid(jdouble *)) {
               return static_cast<T>(*std::any_cast<jdouble *>(prim));
            }
            else if (prim.type() == typeid(jobject *)) {
               return static_cast<T>(*std::any_cast<jobject *>(prim));
            }
            else { throw Exception(std::string("Encountered unknown type ID: ") + prim.type().name()); }
         }
         void set(T &value) {
            auto prim = this->primitive;

            if (prim.type() == typeid(jboolean *)) {
               *std::any_cast<jboolean *>(prim) = static_cast<jboolean>(value);
            }
            else if (prim.type() == typeid(jbyte *)) {
               *std::any_cast<jbyte *>(prim) = static_cast<jbyte>(value);
            }
            else if (prim.type() == typeid(jchar *)) {
               *std::any_cast<jchar *>(prim) = static_cast<jchar>(value);
            }
            else if (prim.type() == typeid(jshort *)) {
               *std::any_cast<jshort *>(prim) = static_cast<jshort>(value);
            }
            else if (prim.type() == typeid(jint *)) {
               *std::any_cast<jint *>(prim) = static_cast<jint>(value);
            }
            else if (prim.type() == typeid(jlong *)) {
               *std::any_cast<jlong *>(prim) = static_cast<jlong>(value);
            }
            else if (prim.type() == typeid(jfloat *)) {
               *std::any_cast<jfloat *>(prim) = static_cast<jfloat>(value);
            }
            else if (prim.type() == typeid(jdouble *)) {
               *std::any_cast<jdouble *>(prim) = static_cast<jdouble>(value);
            }
            else if (prim.type() == typeid(jobject *)) {
               *std::any_cast<jobject *>(prim) = static_cast<jobject>(value);
            }
            else { throw Exception(std::string("Encountered unknown type ID: " + prim.type().name())); }
         }

         T operator->() { return this->get(); }
         T operator*() { return this->get(); }
      };

   protected:
      std::weak_ptr<PrimitiveArray> primitive;
      
      std::shared_ptr<PrimitiveArray> getPrimitive() {
         if (this->primitive.expired()) { this->primitive = std::make_shared<PrimitiveArray>(*this); }

         return this->primitive.lock();
      }

   public:
      // as of older versions of Java, an array is just a special Object, not its own class
      JAVA_CLASS_NO_SIGNATURE(Array, java.lang.Object)
      static sig::Array GetSignature()
      {
         // this can't be a static variable due to the dynamic nature of the signature
         return sig::Array(Signature::FromType<T>());
      }
      operator jarray(){ return reinterpret_cast<jarray>(this->handle); }

      DECLARE_READONLY_PROPERTY(std::size_t, length);
      PROPERTY_GET(std::size_t, length) { return reinterpret_cast<std::size_t>(java::env->GetArrayLength(*this)); }

      std::shared_ptr<Element> operator[](std::size_t i) {
         return (*this->getPrimitive())[i];
      }
   };
}}}
