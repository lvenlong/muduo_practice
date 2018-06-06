#ifndef MUDUO_BASE_LOGGING_H
#define MUDUO_BASE_LOGGING_H

#include <muduo/base/LogStream.h>
#include <muduo/base/Timestamp.h>

namespace muduo
{
	class TimeZone;

	class Logger
	{
	public:
		enum LogLevel
		{
			TRACE,
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL,
			NUM_LOG_LEVELS,
		};

		// compile time calculation of basename of source file
		class SourceFile // 嵌套类 get basebame
		{
		public:
			template<int N>
			inline SourceFile(const char(&arr)[N]) : data_(arr), size_(N - 1)
			{
				const char* slash = strrchr(data_, '/'); // builtin function 查找一个字符c在另一个字符串str中末次出现的位置
				if (slash)
				{
					data_ = slash + 1;
					size_ -= static_cast<int>(data_ - arr);
				}
			}

			explicit SourceFile(const char* filename) : data_(filename)
			{
				// strrchr函数 从字符串的右侧开始查找字符c首次出现的位置），
				// 并返回从字符串中的这个位置起，一直到字符串结束的所有字符
				const char* slash = strrchr(filename, '/');
				if (slash)
				{
					data_ = slash + 1; // data_不包括/ 这样就得到了basename 不包括路径的
				}
				size_ = static_cast<int>(strlen(data_));
			}

			const char* data_;
			int size_;
		};

		Logger(SourceFile file, int line);
		Logger(SourceFile file, int line, LogLevel level);
		Logger(SourceFile file, int line, LogLevel level, const char* func);
		Logger(SourceFile file, int line, bool toAbort);
		~Logger();

		LogStream& stream() // stream()函数返回一个stream对象 因此可以调用<<来将数据写入缓冲区
		{
			return impl_.stream_;
		}

		static LogLevel logLevel();
		static void setLogLevel(LogLevel level);

		typedef void(*OutputFunc)(const char* msg, int len);
		typedef void(*FlushFunc)();

		// 在析构函数中会调用这两个函数将缓冲区数据写入到文件或者标准输出
		static void setOutput(OutputFunc); // 输出函数
		static void setFlush(FlushFunc); // 清空缓冲区

	private:
		class Impl //嵌套类  Impl隐藏信息的手法 这个类才是真正的实现
		{
			typedef Logger::LogLevel LogLevel;
			Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
			void formatTome(); // 时间的格式化函数
			void finish(); // 最后将string格式化写入缓冲区

			Timestamp time_; // 时间
			LogStream stream_; // 日志的缓存区
			LogLevel level_; // 等级
			int line_; // 行
			SourceFile basename_; // 名字
		};
		Impl impl_; // Logger包含了一个Impl对象
	};
}

#endif