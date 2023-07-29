#ifndef QMC_H_PFSTEM
#   define QMC_H_PFSTEM

#   include "../base.hxx"
#   include "spinlock.hxx"

namespace qmc
{
    namespace backend
    {

        template<class backend_stream_tpl>   
        class prefetched_stream
        {
            private:
                longsize_t _file_size;
                byte_t* _buf_begin;
                byte_t* _cur_pos;
                dealloc_func_t _deallocator;

            public:
                using io_lock = backend::spinlock;
                errno_t open(ccstring_t path , alloc_func_t allocator);
                longsize_t read(byte_t** buf , longsize_t size);
                void close();
                prefetched_stream();
                prefetched_stream(ccstring_t path);
                ~prefetched_stream();

        };
        
        template<class backend_stream_tpl>
        prefetched_stream<backend_stream_tpl>::prefetched_stream()
        {
            this->_buf_begin = nullptr;
        }
        
        template<class backend_stream_tpl>
        errno_t prefetched_stream<backend_stream_tpl>::open(ccstring_t path , alloc_func_t allocator)
        {
            backend_stream_tpl _backend_stream;
            errno_t open_ret = _backend_stream.open(path);
            if(open_ret)
            {
                return open_ret;
            }
            this->_file_size = _backend_stream.size();
            this->_buf_begin =  (*allocator)(this->_file_size);
            if(this->_buf_begin)
            {
                longsize_t read_size = _backend_stream.read(this->_buf_begin , this->_file_size);
                if(read_size < this->_file_size)
                {
                    return err::open_file;
                }
                this->_cur_pos = this->_buf_begin;
                return err::ok;
            }
            return err::out_of_mem;
        }

        template<class backend_stream_tpl>
        longsize_t prefetched_stream<backend_stream_tpl>::read(byte_t** buf , longsize_t size)
        {
            __QMC_ASSERT_NULLPTR__(this->_buf_begin);
            *buf = this->_cur_pos;
            longsize_t remain_size = this->_file_size - (this->_cur_pos - this->_buf_begin);
            return (remain_size > size) ? size : remain_size;
        }

        template<class backend_stream_tpl>
        void prefetched_stream<backend_stream_tpl>::close()
        {
            if(this->_buf_begin)
            {
                (*this->_deallocator)(this->_buf_begin);
            }
        }

        template<class backend_stream_tpl>
        prefetched_stream<backend_stream_tpl>::~prefetched_stream()
        {
            this->close();
        }
        

    } // namespace backend
    
} // namespace qmc


#endif // QMC_H_PFSTEM 