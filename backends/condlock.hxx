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
    
    namespace backend
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
                Lock();
                Unlock();
                Wake();     //Call Unlock() first before calling Wake().
                Sleep();
        };
        
#   if defined(_WIN32)

        using WinCondLock = qmc::backend::CondLockBase<CRITICAL_SECTION , CONDITION_VARIABLE>;

        WinCondLock::CondLockBase()
        {
            InitializeCriticalSection(&this->_lock);
            InitializeConditionVariable(&this->_cond);
        }

        WinCondLock::Lock()
        {
            EnterCriticalSection(&this->_lock);
        }

        WinCondLock::Unlock()
        {
            LeaveCriticalSection(&this->Lock);
        }

        WinCondLock::Sleep()
        {
            SleepConditionVariableCS(&this->_cond , &this->_lock , INFINITY);  
        }

        WinCondLock::Wake()
        {
            WakeConditionVariable(&this->_cond);
        }

#   elif defined(POSIX)


        using PosixCondLock = qmc::backend::CondLockBase<pthread_mutex_t , pthread_cond_t>;

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

        PosixCondLock::Lock()
        {
            pthread_mutex_lock(&this->_lock);
        }

        PosixCondLOck::Unlock()
        {
            pthread_mutex_unlock(&this->_lock);
        }

        PosixCondLock::Sleep()
        {
            pthread_cond_wait(&this->_cond , &this->_lock);
        }

        PosixCondLock::Wake()
        {
            pthread_cond_signal(&this->_cond);
        }

#   else

#   endif

    } // namespace backend
    

} // namespace qmc


#endif // QMC_H_MOD_CONDLOCK