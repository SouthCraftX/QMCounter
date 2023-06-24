#ifndef QMC_H_MOD_THRD
#   define QMC_H_MOD_THRD

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

        template<class thread_handle_tpl>        
        class thread_base_t
        {
            private:
                thread_handle_tpl _hde;
            public:
                qmc::errid_t start(qmc::func_t func , void* arg);
                void exit();
                thread_base_t(/* args */);
                ~thread_base_t();
        };
        
#   if defined(__QMC_WINDOWS__)
        
    using thread_t = thread_base_t<::HANDLE>;

    qmc::errid_t thread_t::start(qmc::func_t func , void* arg)
    [
        this-> _hde = ::CreateThread(nullptr , init_thrd_stack_size ,
                                     func , arg , 0);
        return (this->_hde ? qmc::err::ok : qmc::err::create_thrd);
    ]
    
    // It's better to throw a exception to exit the current thread_t instead of directly calling thread_t::exit()
    thread_t::exit()
    {
        ExitThread(0);
        CloseHandle(this->_hde);
    }

#   elif defined(POSIX)

    using thread_t = thread_base_t<pthread_t>;

    qmc::errid_t thread_t::start(qmc::func_t func , void* arg)
    {
        return (::pthread_create(&this->_hde , nullptr , func , arg)) ? 
               qmc::err:create_thrd : qmc::err:ok;
    }

    void thread_t::exit()
    {
        ::pthread_exit(nullptr);
    }

#   else

#   endif

    } // namespace backend
    
} // namespace qmc


#endif // QMC_H_MOD_THRD