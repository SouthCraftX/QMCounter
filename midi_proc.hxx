#ifndef QMC_H_MIDIPROC
#   define QMC_H_MIDIPROC


#   include "base.hxx"

namespace qmc
{

    

    class midi_proc
    {
        protected:
            friend class qmcount;
        
            tempo_t*    _tempo_arr;    // from caller        
            nps_t*      _nps_arr;
            pps_t*      _pps_arr;
            longsize_t  _buf_len;

            notecount_t _note_count;

            alloc_func_t        _allocator;
            dealloc_func_t      _deallocator;

        public:
            midi_proc();
            ~midi_proc();
            errno_t scan_track(byte_t* track_buf , longsize_t track_size);
    };
    
    midi_proc::midi_proc
    (
        tempo_t*            tempo_arr,
        alloc_func_t        allocator,
        dealloc_func_t      deallocator,
        longsize_t          buf_len
    ){

        __QMC_ASSERT_NULLPTR__(tempo_arr);
        __QMC_ASSERT_NULLPTR__(allocator);
        __QMC_ASSERT_NULLPTR__(deallocator);

        this->_allocator    = allocator;
        this->_deallocator  = deallocator;
        this->_buf_len      = buf_len;

        this->_note_count   = 0;
        this->_nps_arr      = nullptr;
        this->_tempo_arr    = nullptr;
        this->_pps_arr      = nullptr;
    }

    midi_proc::~midi_proc()
    {
        if(_nps_arr)
        {
            (*(this->_deallocator))(this->_nps_arr);
            this->_nps_arr = nullptr;
        }
            
        if(_pps_arr)
        {
            (*(this->_deallocator))(this->_pps_arr);
            this->_pps_arr = nullptr;
        }

        if(_tempo_arr)
        {
            (*(this->_deallocator))(this->_tempo_arr);
            this->_tempo_arr = nullptr;
        }
        
    }


} // namespace qmc


#endif // QMC_H_MIDIPROC