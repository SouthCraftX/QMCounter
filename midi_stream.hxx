#ifndef QMC_H_MIDISTREAM
#   define QMC_H_MIDISTREAM

#   include "def.hxx"

#   include "backends/fstream.hxx"
#   include "backends/prefetched_stream.hxx"
#   include "backends/condlock.hxx"
namespace qmc
{
    template<class file_stream_tpl>
    class midi_stream
    {
        private:
            file_stream_tpl _file_stream;
            file_stream_tpl::io_lock _io_lock;

        public:

            [[nodiscard]] errno_t open(ccstring_t path);
            // Warning: Always keep in mind that @read_6_bytes_for_header() only read forward 6 bytes
            // and will NOT move FPI before reading. Only use it when "fpos" is 0.
            [[nodiscard]] longsize_t read_6_bytes_for_header(byte_t** buf); 
            [[nodiscard]] errno_t read_track(byte_t*& buf_ref , size_t& track_size_ref ,
                                             [[maybe_unused]] alloc_func_t allocator ,     // Prefetched stream doesn't need them
                                             [[maybe_unused]] dealloc_func_t deallocator);
            midi_stream() = default;
            ~midi_stream();
    };

    template<class file_stream_tpl>
    errno_t midi_stream<file_stream_tpl>::open(ccstring_t path)
    {
        return this->_file_stream.open(path);
    }

    template<class file_stream_tpl>
    longsize_t midi_stream<file_stream_tpl>::read_6_bytes_for_header(byte_t** buf)
    {
        return this->_file_stream.read(&buf , 6);
    }

    template<class file_stream_tpl>
    errno_t midi_stream<file_stream_tpl>::read_track(byte_t*& buf_ref , size_t& track_size_ref ,
                                                     alloc_func_t allocator , dealloc_func_t deallocator)
    {
        errno_t ret = err::ok;
        uint32_t track_header[2] = { };
        this->_io_lock.acquire(); 

        // Entering thr critical section
        if([[unlikely]]  this->_file_stream.read((byte_t*)&track_header , 8) < 8)
        {
            ret = err::bad_midi;
            goto GOTO_LEAVE_AND_RETURN;
        }

        if([[unlikely]] track_header[0] != 'MTrk')
        {
            ret = err::bad_track_header;
            goto GOTO_LEAVE_AND_RETURN;
        }

        byte_t* track_buf = (*allocator)(track_header[1]);
        if([[likely]] track_buf)
        {
            if(track_size > this->_file_stream.read(track_buf , track_header[1]))
            { // 文件不完整
                (*deallocator)(track_buf);
                ret = err::bad_midi;
            }
            else [[likely]]
            {
                buf_ref = track_buf , track_size_ref = track_header[1];
            }
        }
        else 
        {
            ret = err::out_of_mem;
        }

        // Leaving the critical section
    GOTO_LEAVE_AND_RETURN:
        this->_io_lock.release();
        return err::ok;
    }

    template<>
    errno_t midi_stream<
        backend::prefetched_stream<backend::input_file_stream>
    >::read_track(
        byte_t*& buf_ref , size_t& track_size_ref ,
        [[maybe_unused]] alloc_func_t allocator ,
        [[maybe_unused]] dealloc_func_t deallocator
    ){
        // (void)allocator;
        // (void)deallocator;

        errno_t ret = err::ok;
        this->_io_lock.acquire(); 

        // Entering thr critical section
        uint32_t track_header[2] = { };
        if( this->_file_stream.read((byte_t**)&track_header , 8) < 8) [[unlikely]] 
        {
            ret = err::bad_midi;
            goto GOTO_LEAVE_AND_RETURN;
        }
        if(track_header[0] != 'MTrk') [[unlikely]] 
        {
            ret = err::bad_track_header;
            goto GOTO_LEAVE_AND_RETURN;
        }

        byte_t* track_buf;
        if(track_header[1] > this->_file_stream.read(&track_buf , track_header[1])) [[unlikely]] 
        {
            ret = bad_midi;
            goto GOTO_LEAVE_AND_RETURN;
        }
        buf_ref = track_buf , track_size_ref = track_header[1];

        // Leaving the critical section
    GOTO_LEAVE_AND_RETURN:
        this->_io_lock.release();
        return ret;
    }
    
    template<class file_stream_tpl>
    midi_stream<file_stream_tpl>::~midi_stream()
    {
        this->_file_stream.close();
    }

} // namespace qmc


#endif // QMC_H_MIDISTREAM