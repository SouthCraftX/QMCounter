#ifndef QMC_H_MOD_THRD
#   define QMC_H_MOD_THRD

#   if defined(__QMC_WINDOWS__)
#       include <processthreadsapi.h>
#   elif defined(POSIX)
#       include <pthread.h>
#   endif

#   include "def.hxx"


namespace qmc
{
    namespace backend
    {

        constexpr const qmc::size_t      init_thrd_stack_size = 4194304L;

        template<class ThreadHandleT>        
        class ThreadBase
        {
            private:
                ThreadHandleT _hde;
            public:
                qmc::errid_t Start(qmc::func_t func , void* arg);
                void Exit();
                ThreadBase(/* args */);
                ~ThreadBase();
        };
        
#   if defined(__QMC_WINDOWS__)
        
    using Thread = ThreadBase<::HANDLE>;

    qmc::errid_t Thread::Start(qmc::func_t func , void* arg)
    [
        this-> _hde = ::CreateThread(nullptr , init_thrd_stack_size ,
                                     func , arg , 0);
        if(!_hde)
        {
            return qmc::err::create_thrd;
        }
        return qmc::err::ok;
    ]
    
    // It's better to throw a exception to exit the current thread instead of directly calling Thread::Exit()
    Thread::Exit()
    {
        ExitThread(0);
        CloseHandle(this->_hde);
    }

#   elif defined(POSIX)

    using Thread = ThreadBase<pthread_t>;

    qmc::errid_t Thread::Start(qmc::func_t func , void* arg)
    {
        ::pthread_attr_t thrd_attr;
        ::pthread_create(this->_hde , nullptr , func , arg);
    }

#   else

#   endif

    } // namespace backend
    
} // namespace qmc


#endif // QMC_H_MOD_THRD