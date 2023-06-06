#ifndef QMC_H_MOD_CONDLOCK
#   define QMC_H_MOD_CONDLOCK

#   if defined(_WIN32) 

#       include <synchapi.h>

#   elif defined(POSIX)

#       include <pthread.h>

#   else

#   endif // 


namespace qmc
{
    
    namespace mod
    {
        
        template<class LockT , class CondT>
        class CondLockBase
        {
            protected:
                LockT _lock;
                CondT _cond;

            public:
                CondLockBase();
                ~CondLockBase();
                lock();
                unlock();
                wake();     //Call unlock() first before calling wake().
                sleep();
        };
        
#   if defined(_WIN32)

        using WinCondLock = qmc::mod::CondLockBase<CRITICAL_SECTION , CONDITION_VARIABLE>;

        WinCondLock::CondLockBase()
        {
            InitializeCriticalSection(&this->_lock);
            InitializeConditionVariable(&this->_cond);
        }

        WinCondLock::lock()
        {
            EnterCriticalSection(&this->_lock);
        }

        WinCondLock::unlock()
        {
            LeaveCriticalSection(&this->lock);
        }

        WinCondLock::sleep()
        {
            SleepConditionVariableCS(&this->_cond , &this->_lock , INFINITY);  
        }

        WinCondLock::wake()
        {
            WakeConditionVariable(&this->_cond);
        }

#   elif defined(POSIX)


        using PosixCondLock = qmc::mod::CondLockBase<pthread_mutex_t , pthread_cond_t>;

        PosixCondLock::PosixCondLock()
        {
            pthread_mutex_init(&this->_lock , nullptr);
            pthread_cond_init(&this->_cond , nullptr);
        }

        ~PosixCondLock::PosixCondLock()
        {
            pthread_mutex_destory(&this->_lock);
            pthread_cond_destory(&this->_cond);
        }

        PosixCondLock::lock()
        {
            pthread_mutex_lock(&this->_lock);
        }

        PosixCondLOck::unlock()
        {
            pthread_mutex_unlock(&this->_lock);
        }

        PosixCondLock::sleep()
        {
            pthread_cond_wait(&this->_cond , &this->_lock);
        }

        PosixCondLock::wake()
        {
            pthread_cond_signal(&this->_cond);
        }

#   else

#   endif

    } // namespace mod
    

} // namespace qmc


#endif // QMC_H_MOD_CONDLOCK