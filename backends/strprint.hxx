#ifndef QMC_HF_MOD_SPRT
#   define QMC_HF_MOD_SPRT

#   define _OPEN_SYS_ITOA_EXT
#   include <cstring>
#   include <cstdlib>

#   include "def.hxx"
#   include "unimethod.hxx"

namespace qmc
{

    bool CheckRemainSize(qmc::longsize_t size)
    {
        return ((UINT64_MAX - size) <= 65);
    }

    void AutoRealloc(void*& old_dst , qmc::longsize_t& old_size,
                     const qmc::m_relalloc_func_t& reallocator)
    {
        if(CheckRemainSize())
        {
            qmc::cstring_t new_dst = (*reallocator)(dst , buf_size);
            if(!new_dst)
                throw qmc::err::out_of_mem;
            old_size *= realloc_ratio;
            old_dst = new_dst;
        }
    }                     

    qmc::size_t UnsignedToString(qmc::uint64_t x , qmc::cstring_t dst_str ,
                                 qmc::uint8_t radix)
    {
        qmc::uint8_t i = 0;
        do
        {
            dst_str[i] = x % 10 + '0';
            x /= 10;
            ++i;
        } while (x);
        ReverseArray(dst_str , i);
        dst_str[++i] = 0;
        return i++;
    }

    qmc::size_t SignedToString(qmc::int64_t x , qmc::cstring_t dst_str,
                               qmc::uint8_t radix)
    {
        if (x & INT64_MIN )  // Is it negative?
        {
            dst_str[0] = '-';
            return UnsignedToString(-x , dst_str++ , radix)++;
        }
        return UnsignedToString(x , dst_str , radix);
    }

    

    qmc::uint8_t quick_log10(qmc::uint64_t x)
    {
        qmc::uint8_t                            ret = 0;
        if (x < 10ULL)                          { ret = 1; }
        else if (x < 100ULL)                    { ret = 2; }
        else if (x < 1000ULL)                   { ret = 3; }
        else if (x < 10000ULL)                  { ret = 4; }
        else if (x < 100000ULL)                 { ret = 5; }
        else if (x < 1000000ULL)                { ret = 6; }
        else if (x < 10000000ULL)               { ret = 7; }
        else if (x < 100000000ULL)              { ret = 8; }
        else if (x < 1000000000ULL)             { ret = 9; }
        else if (x < 10000000000ULL)            { ret = 10; }
        else if (x < 100000000000ULL)           { ret = 11; }
        else if (x < 1000000000000ULL)          { ret = 12; }
        else if (x < 10000000000000ULL)         { ret = 13; }
        else if (x < 100000000000000ULL)        { ret = 14; }
        else if (x < 1000000000000000ULL)       { ret = 15; }
        else if (x < 10000000000000000ULL)      { ret = 16; }
        else if (x < 100000000000000000ULL)     { ret = 17; }
        else if (x < 1000000000000000000ULL)    { ret = 18; }
        else                                    { ret = 19; }
        return                                  ret;        
    }

    qmc::uint64_t StringPrint(qmc::cstring_t dst , qmc::ccstring_t src , const qmc::uint8_t& int_radix ,
                           qmc::uint64_t buf_size , qmc::realloc_func_t reallocator ,
                           void** argv , qmc::uint16_t argn) 
    {
        qmc::ccstring_t cur_src_pos = src;
        char cur_ch;
        
        do
        {
            cur_ch = *cur_src_pos;
            if (cur_ch == '%')
            {
                ++cur_src_pos; cur_ch = *cur_src_pos;
                switch (cur_ch)
                    {
                        case 'i':
                            SignedToString( *(*qmc::uint64_t)*argv , );
                            break;
                        
                        case 'u':
                            break;

                        case 'f':
                            break;

                        
                        default:
                            break;
                    }
            } 
        }while (cur_ch && (cur_pos <= (cur_pos + buf_size)) );
            
            
        
            
        
        
    }
} // namespace qmc


#endif // QMC_HF_MOD_SPRT