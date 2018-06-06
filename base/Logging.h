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
		class SourceFile // Ƕ���� get basebame
		{
		public:
			template<int N>
			inline SourceFile(const char(&arr)[N]) : data_(arr), size_(N - 1)
			{
				const char* slash = strrchr(data_, '/'); // builtin function ����һ���ַ�c����һ���ַ���str��ĩ�γ��ֵ�λ��
				if (slash)
				{
					data_ = slash + 1;
					size_ -= static_cast<int>(data_ - arr);
				}
			}

			explicit SourceFile(const char* filename) : data_(filename)
			{
				// strrchr���� ���ַ������Ҳ࿪ʼ�����ַ�c�״γ��ֵ�λ�ã���
				// �����ش��ַ����е����λ����һֱ���ַ��������������ַ�
				const char* slash = strrchr(filename, '/');
				if (slash)
				{
					data_ = slash + 1; // data_������/ �����͵õ���basename ������·����
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

		LogStream& stream() // stream()��������һ��stream���� ��˿��Ե���<<��������д�뻺����
		{
			return impl_.stream_;
		}

		static LogLevel logLevel();
		static void setLogLevel(LogLevel level);

		typedef void(*OutputFunc)(const char* msg, int len);
		typedef void(*FlushFunc)();

		// �����������л��������������������������д�뵽�ļ����߱�׼���
		static void setOutput(OutputFunc); // �������
		static void setFlush(FlushFunc); // ��ջ�����

	private:
		class Impl //Ƕ����  Impl������Ϣ���ַ� ��������������ʵ��
		{
			typedef Logger::LogLevel LogLevel;
			Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
			void formatTome(); // ʱ��ĸ�ʽ������
			void finish(); // ���string��ʽ��д�뻺����

			Timestamp time_; // ʱ��
			LogStream stream_; // ��־�Ļ�����
			LogLevel level_; // �ȼ�
			int line_; // ��
			SourceFile basename_; // ����
		};
		Impl impl_; // Logger������һ��Impl����
	};
}

#endif