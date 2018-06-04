#ifndf MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>

#include <boost/operators.hpp>

namespace muduo{
class Timestamp : public muduo::copyable,
				  public boost::equality_comparable<Timestamp>,
				  public boost::less_than_comparable<Timestamp>
{
public:

	///
	/// Constucts an invalid Timestamp.
	///
	Timestamp() : microSecondsSinceEpoch_(0)
	{
	}
	///
  /// Constucts a Timestamp at specific time
  ///
  /// @param microSecondsSinceEpoch
	explicit Timestamp(int64_t microSecondsSinceEpochArg) :  microSecondsSinceEpoch_(microSecondsSinceEpochArg)
	{
	}
	
	void swap(Timestamp& that)
	{
		std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
	}
	
	// default copy/assignment/dtor are Okay
	string toString()const;
	
	string toFormattedString(bool showMicroseconds = true) const;
	
	bool valid()const
	{
		return microSecondsSinceEpoch_ > 0;
	}
	
	// for internal usage.
	int64_t microSecondsSinceEpoch()const
	{
		return microSecondsSinceEpoch_;
	}
	
	time_t SecondsSinceEpoch()const
	{
		return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	}
	
	///
	/// Get time of now.
	///
	static Timestamp now();
	static Timestamp invalid()
	{
		Timestamp();
	}
	
	static Timestamp fromUnixTime(time_t t)
	{
		return fromUnixTime(t,0);
	}
	
	static Timestamp fromUnixTime(time_t t, int microSeconds)
	{
		return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microSeconds);
	}
	
	static const int kMicroSecondsPerSecond = 1000 * 1000;
	
private:
	int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}


///
/// Gets time difference of two timestamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microsecond
/// resolution for next 100 years.
inline double TimeDifference(Timestamp hign, Timestamp low)
{
	int64_t diff = hign.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
	return static_cast<double>diff;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
	int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
	return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}
}

#endif
	