#ifndef QMC_H_QMCMAIN
#   define QMC_H_QMCMAIN

#   include "base.hxx"
#   include "backends.hxx"
#   include "midi_stream.hxx"
#   include "error_descriptor.hxx"
#   include "run_status.hxx"

namespace qmc
{
    
    class run_flags
    {
        private:
            

        public:
            run_flags(/* args */);
            ~run_flags();
    };
    
    run_flags::run_flags(/* args */)
    {
    }
    
    run_flags::~run_flags()
    {
    }
    

    template<class file_stream_tpl>
    class counter
    {
        private:
            run_status                      _run_status;
            tickframe_count*                _tickframe_counter;
            global_meta_info                _meta;
            midi_stream<file_stream_tpl>    _midi_stream;
            run_flags                       _run_flag;
            backend::track_allocator        _track_allocator;
            backend::reallocatable_allocator _small_allocator;
            error_descriptor                _error_desc;
            
            errno_t proc_track();

        public:
            errno_t init_io(ccstring_t midi_path);
            errno_t run();
            _QMC_FORCEINLINE_ void set_run_flags(const run_flags& rflag); // Copy construction
            _QMC_FORCEINLINE_ void set_run_flags(run_flags&& rv_rflag); // Move construction
            _QMC_FORCEINLINE_ void get_meta(global_meta_info&);
            _QMC_FORCEINLINE_ [[nodiscard]] run_status get_run_status();
            _QMC_FORCEINLINE_ void reset();
            [[nodiscard]]errno_t unload();
            _QMC_FORCEINLINE_ error_descriptor& get_last_error();
            counter(counter&& src);
            counter(const counter&) = delete;
            counter(/* args */);
            ~counter();
    };
    
    template<class file_stream_tpl>
    error_descriptor& counter<file_stream_tpl>::get_last_error()
    {
        return this->_error_desc;
    }

    template<class file_stream_tpl>
    void counter<file_stream_tpl>::get_meta(global_meta_info& meta_ref)
    {
        meta_ref = this->_meta;
    }

    template<class file_stream_tpl>
    void counter<file_stream_tpl>::set_run_flags(const run_flags& rflag)
    {
        this->_run_flag = rflag;
    }

    template<class file_stream_tpl>
    void counter<file_stream_tpl>::set_run_flags(run_flags&& rv_rflag)
    {
        this->_run_flag = rflag;
    }

    template<class file_stream_tpl>
    run_status counter<file_stream_tpl>::get_run_status()
    {
        return this->_run_status;
    }

    template<class file_stream_tpl>
    errno_t counter<file_stream_tpl>::init_io(ccstring_t midi_path)
    {
        errno_t ret = this->_midi_stream.open(midi_path);
        if(ret)
        {
            this->_error_desc.set_error(ret);
        }
        return ret;
    }

    template<class file_stream_tpl>
    errno_t counter<file_stream_tpl>::run()
    {

        backend::thread_group<backend::cpu_thrd_n.get()> thrd_group;

    }

    template<class file_stream_tpl>
    counter<file_stream_tpl>::counter(/* args */)
    {
    }
    
    template<class file_stream_tpl>
    counter<file_stream_tpl>::~counter()
    {
    }
    

} // namespace qmc


#endif // QMC_H_QMCMAIN
