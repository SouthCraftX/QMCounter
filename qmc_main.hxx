#ifndef QMC_H_QMCMAIN
#   define QMC_H_QMCMAIN

#   include "base.hxx"
#   include "midi_stream.hxx"
#   include "error_descriptor.hxx"
namespace qmc
{
    
    template<class file_stream_tpl>
    class counter
    {
        private:
            midi_stream<file_stream_tpl> _midi_stream;
            backend::allocator _allocator;
            error_descriptor _error_desc;


        public:
            errno_t load(ccstring_t midi_path);
            errno_t run();
            void reset();
            errno_t unload();
            const error_descriptor& get_last_error(;)
            counter(/* args */);
            ~counter();
    };
    
    template<class file_stream_tpl>
    const error_descriptor& counter<file_stream_tpl>::get_last_error()
    {
        return this->_error_desc;
    }

    counter::counter(/* args */)
    {
    }
    
    counter::~counter()
    {
    }
    

} // namespace qmc


#endif // QMC_H_QMCMAIN
