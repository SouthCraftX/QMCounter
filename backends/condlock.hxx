#ifndef QMC_H_MOD_CONDLOCK
#   define QMC_H_MOD_CONDLOCK

#   include "../base.hxx"

#   if defined(__QMC_WINDOWS__) 
#       include <synchapi.h>
#   else
#       include <pthread.h>
#   endif // 


namespace qmc
{
    
    namespace backend
    {
        
        class cond_lock
        {
            protected:
#   if defined(__QMC_WINDOWS__)
                ::CRITICAL_SECTION _lock;
                ::CONDITION_VARIABLE _cond;
#   else
                ::pthread_mutex_t _lock;
                ::pthread_cond_t  _cond;
#   endif // defined(__QMC_WINDOWS__)
            public:
                cond_lock();
                ~cond_lock();
                void acquire();
                void release();
                void wake();     //Call release() first before calling wake() to reduce the number of context switches.
                void sleep();
        };
        
#   if defined(__QMC_WINDOWS__)

        cond_lock::cond_lock()
        {
            ::InitializeCriticalSection(&this->_lock);
            ::InitializeConditionVariable(&this->_cond);
        }

        void cond_lock::acquire()
        {
            ::EnterCriticalSection(&this->_lock);
        }

        void cond_lock::release()
        {
            ::LeaveCriticalSection(&this->_lock);
        }

        void cond_lock::sleep()
        {
            ::SleepConditionVariableCS(&this->_cond , &this->_lock , INFINITY);  
        }

        void cond_lock::wake()
        {
            ::WakeConditionVariable(&this->_cond);
        }

        cond_lock::~cond_lock()
        {
            // No functions is needed to destory locks
            // There's nothing to do
        }

#   elif defined(POSIX)

        cond_lock::cond_lock()
        {
            pthread_mutex_init(&this->_lock , nullptr);
            pthread_cond_init(&this->_cond , nullptr);
        }

        cond_lock::~cond_lock()
        {
            pthread_mutex_destory(&this->_lock);
            pthread_cond_destory(&this->_cond);
        }

        void cond_lock::acquire()
        {
            pthread_mutex_lock(&this->_lock);
        }

        void cond_lock::release()
        {
            pthread_mutex_unlock(&this->_lock);
        }

        void cond_lock::sleep()
        {
            pthread_cond_wait(&this->_cond , &this->_lock);
        }

        void cond_lock::wake()
        {
            pthread_cond_signal(&this->_cond);
        }

#   else

#   endif

    } // namespace backend
    

} // namespace qmc


#endif // QMC_H_MOD_CONDLOCK