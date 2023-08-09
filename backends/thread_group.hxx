#ifndef QMC_H_THRDGRP
#   define QMC_H_THRDGRP

#   include "../base.hxx"
#   include "thread.hxx"

namespace qmc
{
    namespace backend
    {
        
        _QMC_INTERNAL_
        class thread_arg
        {
            public:
                aflag32_t* _working_stat_ref;
                void* _upstream_arg;
                thread_arg() = default;
                thread_arg(aflag32_t* p_working_stat , void* upstream_arg);
                ~thread_arg() = default;
                thread_arg(thread_arg&& rv_src) = default;
        };
        
        thread_arg::thread_arg(aflag32_t* p_working_stat , void* upstream_arg)
        {
            this->_working_stat_ref = p_working_stat;
            this->_upstream_arg = upstream_arg;
        }

        

        template<thrd_count_t thrd_n>
        class thread_group
        {
            private:
                thrd_count_t _start_successed;
                aflag32_t _working_stat;
                thread _thread_arr[thrd_n];


            public:
                _QMC_FORCEINLINE_ void stop_all();
                [[nodiscard]] _QMC_FORCEINLINE_  thrd_count_t start_successed() const;
                [[nodiscard]] thrd_count_t start_all(func_t start_func , void* start_arg);
                thread_group() = default;
                ~thread_group();
        };
        
        template<thrd_count_t thrd_n>
        thrd_count_t thread_group<thrd_n>::start_all(func_t start_func , void* start_arg)
        {
            thrd_count_t i = 0;
            errno_t create_ret = 0;
            thread_arg arg(&this->_working_stat , arg);
            do
            {
                create_ret |= this->_thread_arr[i].start(start_func , (void*)&arg);
            } while (i < thrd_n && (!create_ret));
            this->start_successed = i;
            return i;
        }

        template<thrd_count_t thrd_n>
        thrd_count_t thread_group<thrd_n>::start_successed() const
        {
            return this->_start_successed;
        }

        template<thrd_count_t thrd_n>
        void thread_group<thrd_n>::stop_all()
        {
            if(this->start_successed)
            {
                thrd_count_t i = this->start_successed;
                do
                {
                    i--;
                } while (i!=0);
                
            }
        }

        template<thrd_count_t thrd_n>
        thread_group<thrd_n>::~thread_group()
        {
        }
        

    } // namespace backend
    
} // namespace qmc


#endif // QMC_H_THRDGRP