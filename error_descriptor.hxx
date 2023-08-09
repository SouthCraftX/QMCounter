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
            _QMC_FORCEINLINE_ [[nodiscard]] errno_t id() const;
            _QMC_FORCEINLINE_ void set_error(errno_t error);
            _QMC_FORCEINLINE_ [[nodiscard]] ccstring_t to_string() const;
            _QMC_FORCEINLINE_ [[nodiscard]] cstring_t further_info() const;
            error_descriptor() = default;
            error_descriptor(const error_descriptor& src) = default;
            error_descriptor(error_descriptor&& src) = default;
            _QMC_FORCEINLINE_ error_descriptor(errno_t id , cstring_t further_info);
            ~error_descriptor() = default;
    };
    
    _QMC_FORCEINLINE_ [[nodiscard]]
    errno_t error_descriptor::id() const
    {
        return this->_id;
    }

    _QMC_FORCEINLINE_
    void error_descriptor::set_error(errno_t error) 
    {
        this->_id = error;
    }

    _QMC_FORCEINLINE_ [[nodiscard]]
    ccstring_t error_descriptor::to_string() const
    {
        /*
            Working in progress
            It shalls return a string in string table.
            Like:
                return errno_to_string(this->_id);
        */
    }

    _QMC_FORCEINLINE_ [[nodiscard]]
    cstring_t error_descriptor::further_info() const
    {
        return this->_further_info;
    }

    _QMC_FORCEINLINE_
    error_descriptor::error_descriptor(errno_t id , cstring_t further_info)
    {
        this->_id = id , this->_further_info = further_info;
    }
    
    
} // namespace qmc


#endif // QMC_H_ERRDESC