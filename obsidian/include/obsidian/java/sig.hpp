#pragma once

#include <obsidian/std.hpp>
#include <obsidian/thirdparty.hpp>

namespace obsidian
{
namespace java
{
namespace sig
{
   class Signature;
   using SpawnedSig = std::shared_ptr<Signature>;

   template <typename T, typename... Args>
   std::shared_ptr<T> spawn(Args... args) { return std::make_shared<T>(args...); }

   class Signature
   {
   public:
      std::string base;

      Signature() {}
      Signature(std::string base) : base(base) {}

      operator std::string() { return this->toString(); }

      virtual std::string sigType() { return "Signature"; }
      virtual std::string toString() { return this->base; }
      virtual SpawnedSig spawn() { return sig::spawn<Signature>(this->base); }

      static SpawnedSig FromJavaString(std::string javaString);
      template <typename T>
      static SpawnedSig FromType();
   };

   class Boolean : public Signature {
   public:
      Boolean() : Signature("Z") {}
      std::string sigType() { return "Boolean"; }
   };
   class Byte : public Signature {
   public:
      Byte() : Signature("B") {}
      std::string sigType() { return "Byte"; }
   };
   class Char : public Signature {
   public:
      Char() : Signature("C") {}
      std::string sigType() { return "Char"; }
   };
   class Short : public Signature {
   public:
      Short() : Signature("S") {}
      std::string sigType() { return "Short"; }
   };
   class Int : public Signature {
   public:
      Int() : Signature("I") {}
      std::string sigType() { return "Int"; }
   };
   class Long : public Signature {
   public:
      Long() : Signature("J") {}
      std::string sigType() { return "Long"; }
   };
   class Float : public Signature {
   public:
      Float() : Signature("F") {}
      std::string sigType() { return "Float"; }
   };
   class Double : public Signature {
   public:
      Double() : Signature("D") {}
      std::string sigType() { return "Double"; }
   };
   class Void : public Signature {
   public:
      Void() : Signature("V") {}
      std::string sigType() { return "Void"; }
   };

   class Object : public Signature
   {
   public:
      std::string path;

      Object(std::string path) : path(path), Signature("L") {}
      std::string sigType() { return "Object"; }
      std::string toString() {
         std::stringstream ss;
         ss << this->base << this->sigNotation() << ";";

         return ss.str();
      }
      SpawnedSig spawn() { return sig::spawn<Object>(this->path); }

      std::string baseNotation() { return this->path; }
      std::string sigNotation() {
         std::string result = this->path;
         std::transform(result.begin(),
                        result.end(),
                        result.begin(),
                        [](char c) { if (c == '.') { return '/'; } else { return c; } });

         return result;
      }
   };

   class Array : public Signature
   {
   public:
      SpawnedSig arrayType;

      Array(SpawnedSig arrayType) : arrayType(arrayType), Signature("[") {}
      Array(Signature &sig) : Array(sig.spawn()) {}
      std::string sigType() { return "Array"; }
      std::string toString() {
         std::stringstream ss;
         ss << this->base << this->arrayType->toString();

         return ss.str();
      }
      SpawnedSig spawn() { return sig::spawn<Array>(this->arrayType); }
   };

   class Function : public Signature
   {
   public:
      SpawnedSig returnType;
      std::vector<SpawnedSig> arguments;

      Function(SpawnedSig returnType, std::vector<SpawnedSig> arguments)
         : returnType(returnType),
           arguments(std::vector<SpawnedSig>(arguments.begin(), arguments.end())) {}

      Function(SpawnedSig returnType) : returnType(returnType), Signature("(") {}
      template <typename... Args>
      Function(SpawnedSig returnType, SpawnedSig arg, Args... args) : Function(returnType, args...) {
         this->arguments.insert(this->arguments.begin(), arg);
      }

      Function(Signature &returnType) : Function(returnType.spawn()) {}
      template <typename... Args>
      Function(Signature &returnType, Signature &arg, Args... args) : Function(returnType, args...) {
         this->arguments.insert(this->arguments.begin(), arg.spawn());
      }
  
      static SpawnedSig FromJavaString(std::string returnType, std::string argumentList)
      {
         SpawnedSig returnSig = Signature::FromJavaString(returnType);

         if (argumentList.size() == 0) { return Function(returnSig).spawn(); }
         
         std::vector<SpawnedSig> parsedArgs;
         std::size_t commaPos = argumentList.find(",");
         std::size_t parsePos = 0;

         while (commaPos != std::string::npos)
         {
            std::string arg = argumentList.substr(parsePos, commaPos-parsePos);
            parsedArgs.push_back(Signature::FromJavaString(arg));
            parsePos = commaPos+1;
            commaPos = argumentList.find(",", parsePos);
         }

         std::string lastArg = argumentList.substr(parsePos, argumentList.size()-parsePos);
         parsedArgs.push_back(Signature::FromJavaString(lastArg));

         return Function(returnSig, parsedArgs).spawn();
      }
      std::string sigType() { return "Function"; }
      std::string toString()
      {
         std::stringstream ss;
         ss << "(";

         for (auto arg : this->arguments)
         {
            ss << arg->toString();
         }

         ss << ")" << this->returnType->toString();

         return ss.str();
      }
      SpawnedSig spawn() {
         return sig::spawn<Function>(this->returnType, this->arguments);
      }
   };
}}}
