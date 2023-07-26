#ifndef QMC_HF_MDE_FSTM
#   define QMC_HF_MDE_FSTM

#   include <climits>

#   include "../base.hxx"

#   if  defined(__QMC_WINDOWS__)
#       include <windows.h>
#   elif defined(POSIX)
#       include <sys/stat.h>
#       include <fcntl.h>
#       include <unistd.h>
#   endif

#   include "def.hxx"
#   include "unimethod.hxx"
#   include "filemapping.hxx"

namespace qmc
{

    namespace backend
    {

        template<class file_handle_tpl>
        class file_stream_base_t
        {

            protected:
                file_handle_tpl _hde;
                qmc::uint64_t _file_size;
                friend class input_file_stream_t;
#   if defined(QMC_HF_MOD_FMAP)             // File mapping support
                friend class file_mapper_t;
#   endif

            public:
                virtual qmc::errno_t open(qmc::ccstring_t path) = 0;
                virtual void close() = 0;
                qmc::uint64_t Size();
        };

        template<class file_handle_tpl>
        qmc::uint64_t file_stream_base_t<file_handle_tpl>::Size()
        {
            return this->_file_size;
        }

#   if  defined(_WIN32)

        class input_file_stream_t : public qmc::backend::file_stream_base_t<::HANDLE>
        {
            public:
                qmc::errno_t open(qmc::ccstring_t path);
                void close();
                qmc::longsize_t long_read(qmc::byte_t* buf , qmc::longsize_t count);
                qmc::size_t read(qmc::byte_t* buf , qmc::size_t count);
        };

        using input_file_stream_t = qmc::backend::input_file_stream_t;

        qmc::errno_t input_file_stream_t::open(qmc::ccstring_t path)
        {
            // ccstring --> wchar*
            wchar_t wc_path[MAX_PATH];
            std::mbstate_t state = std::mbstate_t();
            std::mbsrtowcs(wc_path , &path , MAX_PATH , &state );

            this->_hde = ::CreateFile(wc_path , GENERIC_READ , FILE_SHARE_READ , NULL ,
                                       OPEN_EXISTING , FILE_FLAG_SEQUENTIAL_SCAN , NULL);

            if( !this->_hde )
            {
                switch (::GetLastError())
                {
                    case ERROR_FILE_NOT_FOUND:
                    case ERROR_INVALID_DRIVE:
                    case ERROR_PATH_NOT_FOUND:  return qmc::err::bad_path;
                    case ERROR_ACCESS_DENIED:   return qmc::err::permission_denied;
                default:
                    return qmc::err::open_failed;
                }
            }

            return 0;
        }

        void input_file_stream_t::close()
        {
            ::CloseHandle(this->_hde);
        }

        qmc::uint32_t input_file_stream_t::read(qmc::byte_t* buf , qmc::size_t count)
        {
            qmc::uint32_t total_read;
            if( ::ReadFile(this->_hde , buf , count , &total_read , NULL) )
            {
                return total_read;
            }
            return 0;
        }

    
#   elif defined(POSIX)



        class input_file_stream : public qmc::backend::file_stream_base_t<int>
        {
            public:
                qmc::errno_t open(qmc::ccstring_t path );
                void close();
                qmc::int64_t read(qmc::byte_t* buf , qmc::int64_t count);
        };

        using input_file_stream_t = input_file_stream_t;

        qmc::errno_t input_file_stream_t::open(qmc::ccstring_t path)
        {

            // Opening the file.
            if( (this->_hde = ::open(path , O_RDONLY | O_LARGEFILE)) == -1 )
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

            //Getting the szie of the file.
            struct ::stat file_stat;
            if (::fstat(this->_hde , &file_stat))
            {
                return qmc::err:get_file_size;
            }
            this->_file_size = file_stat.st_size;

            return qmc::err::ok;
        }

        qmc::size_t input_file_stream_t::read(qmc::byte_t* buf , qmc::size_t count)
        {
            qmc::int64_t ret = ::read(this->_hde , buf , count);
            return (ret == -1) ? ret : 0;
        }


        void input_file_stream_t::close()
        {
            ::close(this->_hde);
        }

#   else



#   endif // if defined(...)
    
    qmc::longsize_t input_file_stream_t::long_read(qmc::byte_t* buf , qmc::longsize_t count)
    {
#   if  defined(__QMC_64BIT__)
        qmc::longsize_t n_read = 0;
        while (count > UINT32_MAX)
        {
            n_read += this->read(buf , UINT32_MAX);
            buf += UINT32_MAX;
            count -= UINT32_MAX;
        }
        n_read += this->read(buf , count);
        return n_read; 
    }
#   else
        return this->read(buf , count);
#   endif
    } // namespace qmc::backend

} // namespace qmc

#endif // QMC_HF_MDE_FSTM