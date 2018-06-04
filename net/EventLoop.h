#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TimerId.h>

namespace muduo
{
	namespace net
	{
		class Channel;
		class Poller;
		class TimerQueue;
		
		///
		/// Reactor, at most one per thread.
		///
		/// This is an interface class, so don't expose too much details.
		class EventLoop : boost::noncopyable
		{
		public:
			typedef boost::function<void()> Functor;
			EventLoop();
			~EventLoop();  // force out-line dtor, for scoped_ptr members.
			
			///
			/// Loops forever.
			///
			/// Must be called in the same thread as creation of the object.
			///
			void loop();
			
			/// Quits loop.
			///
			/// This is not 100% thread safe, if you call through a raw pointer,
			/// better to call through shared_ptr<EventLoop> for 100% safety.
			void quit();
			
			///
			/// Time when poll returns, usually means data arrival.
			///
			Timestamp pollReturnTime() const
			{
				return pollReturnTime_;
			}
			
			int64_t interation() const
			{
				return interation_;
			}
			
			/// Runs callback immediately in the loop thread.
			/// It wakes up the loop, and run the cb.
			/// If in the same loop thread, cb is run within the function.
			/// Safe to call from other threads.
			void runInLoop(const Functor& cb);
			
			/// Queues callback in the loop thread.
			/// Runs after finish pooling.
			/// Safe to call from other threads.
			void queueInLoop(const Functor& cb);
			
			size_t queueSize() const;
			
		#ifdef __GXX_EXPERIMENTAL_CXX0X__
			void runInLoop(Functor&& cb);
			void queueInLoop(Functor&& cb);
		#endif
			
			// timers

			///
			/// Runs callback at 'time'.
			/// Safe to call from other threads.
			///
			TimerId runAt(const Timestamp& time, const TimerCallback& cb);
			
			///
			/// Runs callback after @c delay seconds.
			/// Safe to call from other threads.
			///
			TimerId runAfter(double delay, const TimerCallback& cb);
			
			///
			/// Runs callback every @c interval seconds.
			/// Safe to call from other threads.
			///
			TimerId runEvery(double interval, const TimerCallback& cb);
			
			///
			/// Cancels the timer.
			/// Safe to call from other threads.
			///
			void cancel(TimerId timerId);
			
		#ifdef __GXX_EXPERIMENTAL_CXX0X__
			TimerId runAt(const Timestamp& time, TimerCallback&& cb);
			TimerId runAfter(double delay, TimerCallback&& cb);
			TimerId runEvery(double interval, TimerCallback&& cb);
		#endif
		
			// internal usage
			void wakeup();
			void updateChannel(Channel* channel);
			void removeChannel(Channel* channel);
			bool hasChannel(Channel* channel);
			
			
#endif