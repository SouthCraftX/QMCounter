#ifndef QMC_H_TEMPO
#   define QMC_H_TEMPO

#   include "base.hxx"

namespace qmc
{

    inline
    tempo_t tick_len_microsec(uint32_t tempo , uint16_t resolution)
    {
        return (tempo_t)tempo / resolution;
    }

    class tempo_info
    {
        public:
            tempo_t _tempo;
            event_len_t _len;
    };
    
   

    

/*
    Return true if the current event is tempo-change event , in that case ,
    `p_tempo` would be written.
*/
    inline
    bool scan_meta_track(byte_t* event_begin , tempo_info* p_tempo ,
                         resolution_t resolution)
    {

        tick_t base_tick = 0;

        if((*event_begin == 0xFF) && (*(uint16_t*)(++event_begin) == 0x5103))
        {
            //p_tempo->_tempo = 
        }
        return false;
    }

    /*
        @tempo_count : 
    */
    void analyse_tempo_track(tempo_info* tempo_buf , longsize_t tempo_count , 
                             longsize_t& out_tempo_count , byte_t* track_buf , 
                             size_t track_size , m_realloc_func_t reallocator )
    {
        __QMC_ASSERT_NULLPTR__(tempo_buf);
        __QMC_ASSERT__(tempo_count);
        __QMC_ASSERT_NULLPTR__(track_buf);
        __QMC_ASSERT_NULLPTR__(reallocator);

        byte_t* end_of_track = track_buf + track_size;
        while (track_buf <= end_of_track)
        {
            /* code */
        }
        

    }
} // namespace qmc




#endif // QMC_H_TEMPO