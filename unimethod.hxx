#ifndef QMC_H_UNIMETHOD
#   define QMC_H_UNIMETHOD

#   include "def.hxx"

namespace qmc
{

    template<class FlagT>
    constexpr bool HasFlag(FlagT x , FlagT flag)
    {
        return x & flag;
    }
    
    template<class DataT>
    inline void Swap(DataT x , DataT y , DataT& tmp)
    {
        tmp = x;
        x = y;
        y = tmp;
    }

    template<class DataT>
    inline void ReverseArray(DataT* arr , qmc::longsize_t end_index)
    {
        qmc::longsize_t low = 0;
        DataT tmp;
        do
        {
            Swap(arr[low] , arr[end_index] , tmp);
            ++low;
            --end_index;
        } while (low != end_index);
    }

    // Variable-length quantity --> Unsigned 32-bit interger
    qmc::uint32_t vlq_to_u32(qmc::byte_t* p_vlq)
    {
        qmc::uint32_t n = *p_vlq & 0x7FU;
        while ((*p_vlq)++ & 0x80u)
        {
            (n <<= 7) |= *p_vlq & 0x7FU;
        }
        return n;
    }

} // namespace qmc


#   endif // QMC_H_UNIMETHOD
