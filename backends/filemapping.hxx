#ifndef QMC_HF_MOD_FMAP
#   define QMC_HF_MOD_FMAP

#   if defined(POSIX)
#       include <unistd
#       include <fcntl.h>
#       include <sys/mman.h>
#   elif defined(__WINDOWS__)

#   else

#   endif

#   include "def.hxx"
#   include "fstream.hxx"

namespace qmc
{
    
    namespace mod
    {
        

        };
        
        class FileMapping
        {
            private:
                InputFileStream _fstream;
                void* _mmap_begin_addr;
                
            public:
                FileMapping(/* args */);
                ~FileMapping();
                qmc::errno_t open(qmc::ccstring_t path);
                void close();
                void* map();
        };

        FileMapping::FileMapping(/* args */)
        {
        }
        
        FileMapping::~FileMapping()
        {
        }
        
#       if defined(POSIX)    

        qmc::errno_t FileMapping::open(qmc::ccstring_t path)
        {
            if( (this->_file_mapping_hde._file_hde = ::open(path , O_RDONLY | O_LARGEFILE)) == -1 )
            {
                switch ( errno )
                {
                    case EACCES:
                        return qmc::err::permission_denied;
                
                    case EISDIR:
                    case EINVAL:
                    case EFAULT:
                    case ENOENT:
                    case ENAMETOOLONG:
                        return qmc::err::bad_path;

                    default:
                        return qmc::err::open_failed;
                }
            }
            return qmc::err::ok;
        }

#       elif defined(__WINDOWS__)

#       else

#       endif

    } // namespace mod
    

} // namespace qmc


#endif // QMC_HF_MOD_FMAP