#ifndef QMC_H_THRD
#   define QMC_H_THRD

#   include "../base.hxx"

#   if defined(__QMC_WINDOWS__)
#       include <sysinfoapi.h>  // GetSystemInfo
#       include <processthreadsapi.h> // CreateThread , ExitThread , ...
#   elif defined(POSIX)
#       include <unistd.h> // sysconf
#       include <pthread.h> //pthread_*
#   endif




namespace qmc
{
    namespace backend
    {
        constexpr const size_t      init_thrd_stack_size = 4194304L;

        class __cpu_thrd_n
        {
            private:
                thrd_count_t _num;

            public:
                _QMC_FORCEINLINE_ [[nodiscard]] thrd_count_t get();
                __cpu_thrd_n();
                ~__cpu_thrd_n() = default;
        } cpu_thrd_n;
        
        _QMC_FORCEINLINE_ [[nodiscard]]
        thrd_count_t __cpu_thrd_n::get()
        {
            return this->_num;
        }

        _QMC_FORCEINLINE_
        __cpu_thrd_n::__cpu_thrd_n(/* args */)
        {
#   if defined(__QMC_WINDOWS__)
            SYSTEM_INFO sys_info;
            ::GetNativeSystemInfo(&sys_info);
            this->_num = sys_info.dwNumberOfProcessors;
#   else
            this->_num =  sysconf(_SC_NPROCESSORS_ONLN);
#   endif // defined(__QMC_COUNTER__)
        }


        class thread
        {
            private:
#   if defined(__QMC_WINDOWS__)
                ::HANDLE _hde;
#   else
                ::pthread_t _hde;
#   endif // defined(__QMC_WINDOWS__)
            public:
                errno_t start(func_t func , void* arg);
                void exit();
                thread(/* args */);
                ~thread();
        };
        
#   if defined(__QMC_WINDOWS__)

    _QMC_FORCEINLINE_
    errno_t thread::start(func_t func , void* arg)
    {
        this-> _hde = ::CreateThread(nullptr , init_thrd_stack_size ,
                                     (LPTHREAD_START_ROUTINE)func , arg , 0 , nullptr);
        return (this->_hde ? err::ok : err::create_thrd);
    }
    
    // It's better to throw a exception to exit the current thread instead of directly calling thread::exit()
    _QMC_FORCEINLINE_
    thread::exit()
    {
        
        ::ExitThread(0);
        ::CloseHandle(this->_hde);
    }

#   elif defined(POSIX)

    _QMC_FORCEINLINE_
    errno_t thread::start(func_t func , void* arg)
    {
        return (::pthread_create(&this->_hde , nullptr , func , arg)) ?  
               err::create_thrd : err::ok;
    }

    _QMC_FORCEINLINE_
    void thread::exit()
    {
        ::pthread_exit(nullptr);
    }

#   else

#   endif

    } // namespace backend
    
} // namespace qmc


#endif // QMC_H_THRD