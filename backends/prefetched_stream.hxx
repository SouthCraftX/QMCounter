#ifndef QMC_H_PFSTEM
#   define QMC_H_PFSTEM

#   include "../base.hxx"

namespace qmc
{
    namespace backend
    {

        template<class backend_stream_tpl>   
        class prefetched_stream
        {
            private:
                qmc::longsize_t _file_size;
                qmc::byte_t* _buf_begin;
                qmc::byte_t* _cur_pos;
                qmc::dealloc_func_t _deallocator;

            public:
                qmc::errno_t open(qmc::ccstring_t path , qmc::alloc_func_t allocator);
                qmc::longsize_t read(qmc::byte_t* buf , qmc::longsize_t size);
                void close();
                prefetched_stream();
                prefetched_stream(qmc::ccstring_t path);
                ~prefetched_stream();

        };
        
        template<class backend_stream_tpl>
        prefetched_stream<backend_stream_tpl>::prefetched_stream()
        {
            this->_buf_begin = nullptr;
        }
        
        template<class backend_stream_tpl>
        qmc::errno_t prefetched_stream<backend_stream_tpl>::open(qmc::ccstring_t path , qmc::alloc_func_t allocator)
        {
            backend_stream_tpl _backend_stream;
            qmc::errno_t open_ret = _backend_stream.open(path);
            if(open_ret)
            {
                return open_ret;
            }
            this->_file_size = _backend_stream.size();
            this->_buf_begin =  (*allocator)(this->_file_size);
            if(this->_buf_begin)
            {
                qmc::longsize_t read_size = _backend_stream.read(this->_buf_begin , this->_file_size);
                if(read_size < this->_file_size)
                {
                    return qmc::err::open_file;
                }
                this->_cur_pos = this->_buf_begin;
                return qmc::err::ok;
            }
            return qmc::err::out_of_mem;
        }

        template<class backend_stream_tpl>
        qmc::longsize_t prefetched_stream<backend_stream_tpl>::read(qmc::byte_t* buf , qmc::longsize_t size)
        {
            __QMC_ASSERT_NULLPTR__(this->_buf_begin);
            buf = this->_cur_pos;
            qmc::longsize_t remain_size = this->_file_size - (this->_cur_pos - this->_buf_begin);
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