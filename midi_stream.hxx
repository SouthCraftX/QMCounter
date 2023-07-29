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

            // Warning: Always keep in mind that @read_6_bytes_for_header() only read forward 6 bytes
            // and will NOT move FPI before reading. Only use it when "fpos" is 0.
            longsize_t read_6_bytes_for_header(byte_t* buf); 
            errno_t read_track(byte_t*& buf_ref , size_t& track_size_ref ,
                               alloc_func_t allocator , dealloc_func_t deallocator);
            midi_stream(/* args */);
            ~midi_stream();
    };
    
    template<class file_stream_tpl>
    longsize_t midi_stream<file_stream_tpl>::read_6_bytes_for_header(byte_t** buf)
    {
        return this->_file_stream.read(&buf , 6);
    }

    template<class file_stream_tpl>
    errno_t midi_stream<file_stream_tpl>::read_track(byte_t*& buf_ref , size_t& track_size_ref ,
                                                     alloc_func_t allocator , dealloc_func_t deallocator)
    {
        this->_io_lock.acquire(); 

        // Entering thr critical section
        uint32_t track_header[2] = { };
        this->_file_stream.read((byte_t*)&track_header , 8);
        if(track_header[0] != 'MTrk')
        {
            // Set error
        }

        byte_t* track_buf = (*allocator)(track_header[1]);
        if(track_buf)
        {
            if(track_size > this->_file_stream.read(track_buf , track_header[1]))
            {
                // MIDI 不完整
            }
            buf_ref = track_buf , track_size_ref = track_header[1];
        }
        return err::out_of_mem;
        // Leaving the critical section

        this->_io_lock.release();
        return err::ok;
    }

    template<>
    errno_t midi_stream<
        backend::prefetched_stream<backend::input_file_stream>
    >::read_track(
        byte_t*& buf_ref , size_t& track_size_ref ,
        alloc_func_t allocator , dealloc_func_t deallocator
    ){
        (void)allocator;
        (void)deallocator;

        this->_io_lock.acquire(); 

        // Entering thr critical section
        uint32_t track_header[2] = { };
        this->_file_stream.read((byte_t**)&track_header , 8);
        if(track_header[0] != 'MTrk')
        {
            // Set error
        }

        byte_t* track_buf;
        if(track_header[1] > this->_file_stream.read(&track_buf , track_header[1]))
        {
            // Set error
        }
        buf_ref = track_buf , track_size_ref = track_header[1];

        // Leaving the critical section
        this->_io_lock.release();
        return err::ok;
    }

    
} // namespace qmc


#endif // QMC_H_MIDISTREAM