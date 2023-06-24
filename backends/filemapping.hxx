#ifndef QMC_HF_MOD_FMAP
#   define QMC_HF_MOD_FMAP

#   if defined(POSIX)
#       include <unistd
#       include <fcntl.h>
#       include <sys/mman.h>
#   elif defined(__WINDOWS__)
#       include <windows.h>
#   else

#   endif

#   include "def.hxx"
#   include "fstream.hxx"

namespace qmc
{
    
    namespace backend
    {
        

        
        
        class file_mapper_t
        {
            private:
                input_file_stream_t _fstream;
#   if defined(__QMC_WINDOWS__)
                HANDLE _map_hde;
#   endif
                void* _mmap_begin_addr;
                
            public:
                file_mapper_t(/* args */);
                ~file_mapper_t();
                qmc::errid_t open(qmc::ccstring_t path);
                void close();
                void* map();
        };

        void* file_mapper_t::map()
        {
            return this->_mmap_begin_addr;
        }

        file_mapper_t::file_mapper_t(/* args */)
        {
        }
        
        file_mapper_t::~file_mapper_t()
        {
        }
        
#   if defined(__QMC_WINDOWS__)

        qmc::errid_t file_mapper_t::open(qmc::ccstring_t path)
        {
            qmc::errid_t open_ret = this->_fstream.open(path);
            if(open_ret)
            {
                return open_ret;
            }

            this->_map_hde = ::CreateFileMapingA( this->_fstream._hde , nullptr ,
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
            return qmc::err::open_failed;
        }

        void file_mapper_t::close()
        {
            UnmapViewOfFile(this->_mmap_begin_addr); 
            CloseHandle(this->_map_hde); 
            this->_fstream.close();
            this->_mmap_begin_addr;
        }

#   elif defined(POSIX)

#   endif


    } // namespace backend
    

} // namespace qmc


#endif // QMC_HF_MOD_FMAP