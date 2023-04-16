#ifndef QMC_H_RDHEAD
#   define QMC_H_RDHEAD

#   include <istream>

#   include "def.hxx"

namespace qmc
{
    template<class FileStreamT>
    qmc::errno_t read_header(const FileStreamT& stream , GlobalInfo& info)
    {
        byte_t buf[qmc::expected_header_size];
        if( stream.read(buf , qmc::expected_header_size) < 
    }
} // namespace qmc


#endif // QMC_H_RDHEAD