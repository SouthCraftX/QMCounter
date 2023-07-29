#ifndef QMC_H_TYPES
#   define QMC_H_TYPES

#   include <cstdint>
#   include <atomic>

#   include <cstdlib>

#   if __cplusplus > 202300L
#       include <stdfloat>
#   else
#       include <cmath>
#   endif

namespace qmc
{


// Basic Types
    using int8_t = std::int8_t;
    using int16_t = std::int16_t;
    using int32_t = std::int32_t;
    using int64_t = std::int64_t;

    using uint8_t = std::uint8_t;
    using uint16_t = std::uint16_t;
    using uint32_t = std::uint32_t;
    using uint64_t = std::uint64_t;

    using cstring_t = char*;
    using ccstring_t = const char*;

    using aint8_t = std::atomic_int8_t;
    using aint16_t = std::atomic_int16_t;
    using aint32_t = std::atomic_int32_t;
    using aint64_t = std::atomic_int64_t;

#   if __cplusplus > 202300L
    using flt32_t = std::float32_t;
    using flt64_t = std::float64_t;
#   else
    using flt32_t = std::float_t;
    using flt64_t = std::double_t;
#   endif

    using auint8_t  = std::atomic_uint8_t;
    using auint16_t = std::atomic_uint16_t;
    using auint32_t = std::atomic_uint32_t;
    using auint64_t = std::atomic_uint64_t;

    using flag8_t  = int8_t;
    using flag16_t = int16_t;
    using flag32_t = int32_t;

    using byte_t  = uint8_t;
    using errno_t = flag32_t;

    using fmode_t = flag8_t;    


// Types of 'size'
    using size_t    = uint32_t;
#       if defined(__QMC_64BIT__)
    using longsize_t= uint64_t;
#       else
    using longsize_t= size_t;
#       endif

// Type of 'tempo' (BPM)
#       if defined(__QMC_64BIT__)
    using event_len_t   = uint64_t; // unit:tick
    using tempo_t       = flt64_t;
    using tick_t        = uint64_t;
    using notecount_t   = uint64_t;
    using pps_t         = uint64_t;
    using global_event_count_t = uint64_t;
#       else
    using event_len_t   = uint32_t;
    using tempo_t       = flt32_t;
    using tick_t        = uint32_t;
    using notecount_t   = uint32_t;
    using pps_t         = uint32_t; // Polyphony per sec
    using global_event_count_t = uint64_t;
#       endif

    using event_count_t = global_event_count_t;
    using track_event_count_t = uint32_t;
    
    using nps_t         = notecount_t;
    using resolution_t  = uint16_t;

    using func_t = void*(*)(void*);

    using alloc_func_t = void*(*)(longsize_t);
    using m_realloc_func_t = byte_t*(*)(byte_t* , longsize_t , longsize_t); // old ptr , old size , new_size
    using dealloc_func_t = byte_t(*)(byte_t* , longsize_t);

    class global_meta_info
    {
        public:
            flag8_t    _format;
            uint16_t   _track_count;
            uint16_t   _resolution;
            ccstring_t _copyright;
    };
    
    class tickframe_count
    {
        public:
            track_event_count_t _noteon;
            track_event_count_t _noteoff;
            tempo_t _tempo_change; // !0 if there is a tempo change event;
    };

    

} // namespace qmc


#endif // QMC_H_TYPES