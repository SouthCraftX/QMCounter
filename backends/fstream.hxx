#ifndef QMC_HF_MDE_FSTM
#   define QMC_HF_MDE_FSTM

#   include <climits>

#   include "def.hxx"

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

        template<class FileHandleT>
        class FileStreamBase
        {

            protected:
                FileHandleT _hde;
                qmc::uint64_t _file_size;
                friend class InputFileStream;
#   if defined(QMC_HF_MOD_FMAP)             // File mapping support
                friend class FileMapping;
#   endif

            public:
                virtual qmc::errno_t Open(qmc::ccstring_t path) = 0;
                virtual void Close() = 0;
                qmc::uint64_t Size();
        };

        template<class FileHandleT>
        qmc::uint64_t FileStreamBase<FileHandleT>::Size()
        {
            return this->_file_size;
        }

#   if  defined(_WIN32)

        class InputFileStream : public qmc::backend::FileStreamBase<::HANDLE>
        {
            public:
                qmc::errno_t Open(qmc::ccstring_t path);
                void Close();
                qmc::longsize_t LongRead(qmc::byte_t* buf , qmc::longsize_t count);
                qmc::size_t Read(qmc::byte_t* buf , qmc::size_t count);
        };

        using InputFileStream = qmc::backend::InputFileStream;

        qmc::errno_t InputFileStream::Open(qmc::ccstring_t path)
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

        void InputFileStream::Close()
        {
            ::CloseHandle(this->_hde);
        }

        qmc::uint32_t InputFileStream::Read(qmc::byte_t* buf , qmc::size_t count)
        {
            qmc::uint32_t total_read;
            if( ::ReadFile(this->_hde , buf , count , &total_read , NULL) )
            {
                return total_read;
            }
            return 0;
        }

    
#   elif defined(POSIX)



        class InputFileStream : public qmc::backend::FileStreamBase<int>
        {
            public:
                qmc::errno_t Open(qmc::ccstring_t path );
                void Close();
                qmc::int64_t Read(qmc::byte_t* buf , qmc::int64_t count);
        };

        using InputFileStream = InputFileStream;

        qmc::errno_t InputFileStream::Open(qmc::ccstring_t path)
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

        qmc::size_t InputFileStream::Read(qmc::byte_t* buf , qmc::size_t count)
        {
            qmc::int64_t ret = ::read(this->_hde , buf , count);
            return (ret == -1) ? ret : 0;
        }


        void InputFileStream::Close()
        {
            ::close(this->_hde);
        }

#   else



#   endif // if defined(...)
    
    qmc::longsize_t InputFileStream::LongRead(qmc::byte_t* buf , qmc::longsize_t count)
    {
#   if  defined(__QMC_64BIT__)
        qmc::longsize_t n_read = 0;
        while (count > UINT32_MAX)
        {
            n_read += this->Read(buf , UINT32_MAX);
            buf += UINT32_MAX;
            count -= UINT32_MAX;
        }
        n_read += this->Read(buf , count);
        return n_read; 
    }
#   else
        return this->Read(buf , count);
#   endif
    } // namespace qmc::backend

} // namespace qmc

#endif // QMC_HF_MDE_FSTM