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
    
    namespace backend
    {
        

        
        
        class FileMapping
        {
            private:
                InputFileStream _fstream;
                void* _mmap_begin_addr;
                
            public:
                FileMapping(/* args */);
                ~FileMapping();
                qmc::errno_t Open(qmc::ccstring_t path);
                void Close();
                void* Map();
        };

        void* FileMapping::Map()
        {
            return this->_mmap_begin_addr;
        }

        FileMapping::FileMapping(/* args */)
        {
        }
        
        FileMapping::~FileMapping()
        {
        }
        
#   if defined(__QMC_WINDOWS__)

#   elif defined(POSIX)

#   endif


    } // namespace backend
    

} // namespace qmc


#endif // QMC_HF_MOD_FMAP