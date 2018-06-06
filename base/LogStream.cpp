#include <muduo/base/LogStream.h>

#include <algorithm>
#include <limits>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

using namespace muduo;
using namespace muduo::detail;

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wtautological-compare"
#else
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

namespace muduo
{
	namespace detail
	{
		const char digits[] = "9876543210123456789";
		const char* zero = digits + 9;
		BOOST_STARTIC_ASSERT(sizeof(digits) == 20);
		
		const char digitsHex[] = "0123456789ABCDEF";
		BOOST_STARTIC_ASSERT(sizeof digitsHex == 17);
		
		// Efficient Integer to String Conversions, by Matthew Wilson.
		template<typename T>
		size_t convert(char buf[], T value)
		{
			T i = value;
			char* p = buf;
			do{
				int lsd = static_cast<int>(i % 10);
				i /= 10;
				*p++ = zero[lsd];
			}while(i != 0);
			
			if(value < 0)
			{
				*p++ = '-';
			}
			*p = '\0';
			std::reverse(buf,p);
			
			return p - buf;
		}
		
		size_t convertHex(char buf[],uintptr_t value)
		{
			uintptr_t i = value;
			char* p = buf;
			do{
				int lsd = static_cast<int>(i % 16);
				i /= 16;
				*p++ = digitsHex[lsd];
			} while (i != 0);

			*p = '\0';
			std::reverse(buf, p);

			return p - buf;
		}
		
		template class FixedBuffer<kSmallBuffer>;
		template class FixedBuffer<kLargeBuffer>;
	}
}

template<int SIZE>
const char* FixedBuffer<SIZE>::debugString()
{
  *cur_ = '\0';
  return data_;
}

template<int SIZE>
void FixedBuffer<SIZE>::cookieStart()
{
}

template<int SIZE>
void FixedBuffer<SIZE>::cookieEnd()
{
}