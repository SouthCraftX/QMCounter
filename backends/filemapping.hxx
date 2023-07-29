#ifndef QMC_HF_MOD_FMAP
#   define QMC_HF_MOD_FMAP

#   include "../base.hxx"
#   include "fstream.hxx"

#   if defined(POSIX)
#       include <unistd.h>
#       include <fcntl.h>
#       include <sys/mman.h>
#   elif defined(__QMC_WINDOWS__)
#       include <memoryapi.h> // CreateFileMapping , MapViewOfFile , ...
#       include <handleapi.h> // CloseHandle
#   else
#   
#   endif
namespace qmc
{
    
    namespace backend
    {
    
        
        class file_mapper
        {
            private:
                input_file_stream _fstream;
#   if defined(__QMC_WINDOWS__)
                HANDLE _map_hde;
#   endif
                void* _mmap_begin_addr;
                
            public:
                file_mapper(/* args */);
                ~file_mapper();
                [[nodiscard]] qmc::errno_t open(qmc::ccstring_t path);
                void close();
                [[nodiscard]] void* map();
        };

        [[nodiscard]]
        void* file_mapper::map()
        {
            return this->_mmap_begin_addr;
        }

        file_mapper::file_mapper(/* args */)
        {
        }
        
        file_mapper::~file_mapper()
        {
        }
        
#   if defined(__QMC_WINDOWS__)

        [[nodiscard]]
        qmc::errno_t file_mapper::open(qmc::ccstring_t path)
        {
            qmc::errno_t open_ret = this->_fstream.open(path);
            if(open_ret)
            {
                return open_ret;
            }

            this->_map_hde = ::CreateFileMapping( this->_fstream._hde , nullptr ,
                                PAGE_READONLY | SEC_LARGE_PAGES , 0 , 0 , nullptr );

            if(!this->_map_hde)
            {
                goto JUMPPOINT_FAILED;
            }

            this->_mmap_begin_addr = ::MapViewOfFile( this->_map_hde , FILE_MAP_READ |
                                                      FILE_MAP_LARGE_PAGES , 0 , 0 , 0);

            if(this->_mmap_begin_addr)
            {
                return qmc::err::ok;
            }

            JUMPPOINT_FAILED:
            return qmc::err::open_file;
        }

        void file_mapper::close()
        {
            ::UnmapViewOfFile(this->_mmap_begin_addr); 
            ::CloseHandle(this->_map_hde); 
            this->_fstream.close();
            this->_mmap_begin_addr = nullptr;
        }

#   elif defined(POSIX)

#   endif


    } // namespace backend
    

} // namespace qmc


#endif // QMC_HF_MOD_FMAP

