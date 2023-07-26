#ifndef QMC_H_RDHEAD
#   define QMC_H_RDHEAD

#   include <istream>

#   include "def.hxx"

namespace qmc
{
    template<class file_stream_tpl>
    qmc::errno_t read_header(const file_stream_tpl& stream , global_meta_info& info)
    {
        byte_t buf[qmc::expected_header_size];
        if( stream.read(buf , qmc::expected_header_size) < qmc::expected_header_size )
        {
            return qmc::err::
        }
    }
} // namespace qmc


#endif // QMC_H_RDHEAD