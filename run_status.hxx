#ifndef QMC_H_RUNSTAT
#   define QMC_H_RUNSTAT

#   include "base.hxx"

namespace qmc
{
    
    enum status
    {
        stopped = 0 ,
        running ,
        error ,
        abort
    };

    class run_status
    {
        private:
            flag32_t _flag;
            
        public:
            run_status();
            run_status(const run_status& src) = default;
            run_status(run_status&& rv_src) = default;
            run_status(flag32_t& x);
            run_status(flag32_t flag);
            bool running();
            bool error();
            bool abort();
            bool stopped();
            bool operator==(const run_status& x);
            void set_running();
            void set_stopped();
            void set_error();
            void set_abort();
            ~run_status() = default;
    };
    
    run_status::run_status()
    {
        this->set_stopped();
    }

    run_status::run_status(flag32_t& x)
    {
        x = this->_flag;
    }

    run_status::run_status(flag32_t flag)
    {
        this->_flag = flag;
    }
    
    bool run_status::operator==(const run_status& x)
    {
        return this->_flag == x._flag;
    }

    bool run_status::abort()
    {
        return this->_flag == status::abort;
    }

    bool run_status::error()
    {
        return this->_flag == status::error;
    }

    bool run_status::stopped()
    {
        return this->_flag == status::stopped;
    }

    bool run_status::running()
    {
        return this->_flag == status::running;
    }

    void run_status::set_stopped()
    {
        this->_flag = status::stopped;
    }

    void run_status::set_abort()
    {
        this->_flag = status::abort;
    }

    void run_status::set_running()
    {
        this->_flag = status::running;
    }

    void run_status::set_error()
    {
        this->_flag = status::error;
    }
    
} // namespace qmc


#endif // QMC_H_RUNSTAT