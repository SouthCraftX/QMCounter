#ifndef QMC_H_UNIMETHOD
#   define QMC_H_UNIMETHOD

#   include "base.hxx"

namespace qmc
{

    template<class flag_tpl>
    constexpr bool has_flag(flag_tpl x , flag_tpl flag)
    {
        return x & flag;
    }
    
    template<class data_tpl>
    inline void swap(data_tpl x , data_tpl y , data_tpl& tmp)
    {
        tmp = x;
        x = y;
        y = tmp;
    }

    template<class data_tpl>
    inline void reverse_array(data_tpl* arr , qmc::longsize_t end_index)
    {
        qmc::longsize_t low = 0;
        data_tpl tmp;
        do
        {
            Swap(arr[low] , arr[end_index] , tmp);
            ++low;
            --end_index;
        } while (low != end_index);
    }

    inline
    uint32_t be_to_le(uint32_t x)
    {
        byte_t* p = (byte_t*)&x;
        return(((uint32_t)*p)+((uint32_t)*(p+1)<<8)+
                ((uint32_t)*(p+2)<<16)+(uint32_t)*(p+3)<<24);
    }

    inline
    uint16_t be_to_le(uint16_t x)
    {
        return (x>>8)+(x<<8);
    }

    uint32_t vlq_to_u32_move(byte_t*& x)
    {
        uint32_t result = 0;
        byte_t b;
        do
        {
            b = *(x++);
            result = (result << 7) | (b & 0x7F);
        } while ((b & 0b10000000) != 0);
        return result;
    }


/*
    // Variable-length quantity --> Unsigned 32-bit interger
    qmc::uint32_t32_t vlq_to_u32(qmc::byte_t* p_vlq)
    {
        qmc::uint32_t32_t n = *p_vlq & 0x7FU;
        while ((*p_vlq)++ & 0x80u)
        {
            (n <<= 7) |= *p_vlq & 0x7FU;
        }
        return n;
    }
*/

} // namespace qmc


#   endif // QMC_H_UNIMETHOD
