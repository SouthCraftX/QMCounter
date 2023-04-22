#ifndef QMC_HF_MDE_FSTM
#   define QMC_HF_MDE_FSTM

#   include "def.hxx"
#   include "unimethod.hxx"

namespace qmc
{

    enum fmode
    {
        new_file,
        no_creating,
        exist_only

    } ;

    namespace module
    {

        template<class FileHandleT>
        class FileStreamBase
        {
            protected:
                FileHandleT hde;
                friend class InputFileStream;
            public:
                virtual qmc::errno_t open(qmc::ccstring_t path , fmode_t mode) = 0;
                virtual void close() = 0;
        };

#   if  defined(_WIN32)

#       include <windows.h>

        class WinInputFileStream : public qmc::module::FileStreamBase<::HANDLE>
        {
            public:
                qmc::errno_t open(qmc::ccstring_t path , fmode_t mode);
                void close();
                qmc::uint64_t read(qmc::byte_t* buf , qmc::uint64_t count);
        };

        using InputFileStream = qmc::module::WinInputFileStream;

        qmc::errno_t WinInputFileStream::open(qmc::ccstring_t path , fmode_t mode)
        {
            qmc::uint32_t share_mode = FILE_SHARE_READ;
            qmc::uint32_t create_mode;
            wchar_t wc_path[MAX_PATH];

            std::swprintf(wc_path , MAX_PATH , L"%s" , path);
    
            if( qmc::has_flag<qmc::uint32_t>(mode , qmc::fmode::new_file) )
                create_mode = CREATE_ALWAYS;
            else if( qmc::has_flag<qmc::uint32_t>(mode , qmc::fmode::exist_only) )
                create_mode = OPEN_EXISTING;
            else if( qmc::has_flag<qmc::uint32_t>(mode , qmc::fmode::no_creating) )
                create_mode = OPEN_ALWAYS;

            this->hde = ::CreateFileA(wc_path , mode , share_mode , NULL ,
                                       create_mode , FILE_FLAG_SEQUENTIAL_SCAN , NULL);

            if( !this->hde )
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

        void WinInputFileStream::close()
        {
            ::CloseHandle(this->hde);
        }

        qmc::uint64_t WinInputFileStream::read(qmc::byte_t* buf , qmc::uint64_t count)
        {
            size_t total_read = 0;
            ::ReadFile(this->hde , buf , count , &total_read , NULL);
            return total_read;
        }
    
#   elif defined(POSIX)

#       include <unistd.h>

        class PosixInputFileStream : public qmc::module::FileStreamBase<int>
        {
            public:
                qmc::errno_t open(qmc::ccstring_t path , fmode_t mode);
                void close();
                qmc::uint64_t read(qmc::byte_t* buf , qmc::uint64_t count);
        }

        using InputFileStream = PosixInputFileStream;

        qmc::errno_t PosixInputFileStream::open(qmc::ccstring_t path , fmode_t mode)
        {
            flag32_t flag;
            if( qmc::has_flag<qmc::uint32_t>(mode , qmc::fmode::new_file) )
                create_mode = O_CREAT;
            else if( qmc::has_flag<qmc::uint32_t>(mode , qmc::fmode::exist_only) )
                create_mode = OPEN_EXISTING;
            else if( qmc::has_flag<qmc::uint32_t>(mode , qmc::fmode::no_creating) )
                create_mode = OPEN_ALWAYS;
        }

#   else

#   endif //

    } // namespace module
    




#   endif
    
} // namespace qmc::module


#endif // QMC_HF_MDE_FSTM