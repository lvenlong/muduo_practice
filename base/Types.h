#ifndef MUDUO_BASE_TYPES_H
#define MUDUO_BASE_TYPES_H

#include<stdint.h>

#ifdef MUDUO_STD_STRING
#include<string>
#else
#include<ext/vstring.h>
#include<ext/vstring_fwd.h>
#endif

///
/// The most common stuffs.
///
namespace muduo
{

#ifdef MUDUO_STD_STRING
//无特殊处理的C++实现方式
using std::string;
#else  // !MUDUO_STD_STRING
//利用string的短字符优化实现方式
typedef __gnu_cxx::__sso_string string;
#endif

//类型强转，implicit_cast只能执行up-cast，派生类->基类
//这里模拟了boost库里面的implicit_cast
template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}
//进行down_cast 基类->派生类转换，并检查转换是否安全
template<typename To, typename From>     // use like this: down_cast<T*>(foo);
inline To down_cast(From* f)                     // so we only accept pointers
{
  // Ensures that To is a sub-type of From *.  This test is here only
  // for compile-time type checking, and has no overhead in an
  // optimized build at run-time, as it will be optimized away
  // completely.
  if (false)
  {
    implicit_cast<From*, To>(0);
  }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
  //dynamic_cast检查该类型转换是否安全
  assert(f == NULL || dynamic_cast<To>(f) != NULL);  // RTTI: debug mode only!
#endif
  return static_cast<To>(f);
}

}

#endif

