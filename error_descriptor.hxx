#ifndef QMC_H_ERRDESC
#   define QMC_H_ERRDESC

#   include "base.hxx"

namespace qmc
{
    class error_descriptor
    {
        private:
            errno_t _id;
            cstring_t _further_info;

        public:
            errno_t id() const;
            void set_error(errno_t error);
            ccstring_t to_string() const;
            cstring_t further_info() const;
            error_descriptor() = default;
            error_descriptor(errno_t id , cstring_t further_info);
            ~error_descriptor() = default;
    };
    
    errno_t error_descriptor::id() const
    {
        return this->_id;
    }

    void error_descriptor::set_error(errno_t error) 
    {
        this->_id = error;
    }

    ccstring_t error_descriptor::to_string() const
    {
        /*
            Working in progress
            It shall return a string in string table.
            Like:
                return errno_to_string(this->_id);
        */
    }

    cstring_t error_descriptor::further_info() const
    {
        return this->_further_info;
    }

    error_descriptor::error_descriptor(errno_t id , cstring_t further_info)
    {
        this->_id = id , this->_further_info = further_info;
    }
    
    
} // namespace qmc


#endif // QMC_H_ERRDESC