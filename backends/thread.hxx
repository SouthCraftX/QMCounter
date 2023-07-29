#ifndef QMC_H_THRD
#   define QMC_H_THRD

#   if defined(__QMC_WINDOWS__)
#       include <processthreadsapi.h>
#   elif defined(POSIX)
#       include <pthread.h>
#   endif

#   include "../def.hxx"


namespace qmc
{
    namespace backend
    {

        constexpr const qmc::size_t      init_thrd_stack_size = 4194304L;
       
        class thread
        {
            private:
#   if defined(__QMC_WINDOWS__)
                ::HANDLE _hde;
#   else
                ::pthread _hde;
#
            public:
                qmc::errno_t start(qmc::func_t func , void* arg);
                void exit();
                thread(/* args */);
                ~thread();
        };
        
#   if defined(__QMC_WINDOWS__)

    qmc::errno_t thread::start(qmc::func_t func , void* arg)
    [
        this-> _hde = ::CreateThread(nullptr , init_thrd_stack_size ,
                                     func , arg , 0);
        return (this->_hde ? qmc::err::ok : qmc::err::create_thrd);
    ]
    
    // It's better to throw a exception to exit the current thread instead of directly calling thread::exit()
    thread::exit()
    {
        ::ExitThread(0);
        ::CloseHandle(this->_hde);
    }

#   elif defined(POSIX)

    qmc::errno_t thread::start(qmc::func_t func , void* arg)
    {
        return (::pthread_create(&this->_hde , nullptr , func , arg)) ? 
               qmc::err:create_thrd : qmc::err:ok;
    }

    void thread::exit()
    {
        ::pthread_exit(nullptr);
    }

#   else

#   endif

    } // namespace backend
    
} // namespace qmc


#endif // QMC_H_THRD