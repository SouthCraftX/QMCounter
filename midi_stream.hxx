#ifndef QMC_H_MIDISTREAM
#   define QMC_H_MIDISTREAM

#   include "def.hxx"

#   include "backends/fstream.hxx"
#   include "backends/condlock.hxx"
namespace qmc
{
    template<class file_stream_tpl>
    class midi_stream
    {
        private:
            file_stream_tpl _file_stream;
            backend::cond_lock _io_lock;

        public:

            // Warning: Always keep in mind that @read_6_bytes_for_header() only read forward 6 bytes
            // and will NOT move FPI before reading. Only use it when "fpos" is 0.
            qmc::longsize_t read_6_bytes_for_header(qmc::byte_t* buf); 
            
            midi_stream(/* args */);
            ~midi_stream();
    };
    
    qmc::longsize_t midi_stream<backend::input_file_stream>::read_6_bytes_for_header(qmc::byte_t* buf)
    {
        return this->_file_stream.read(buf , 6);
    }

    
} // namespace qmc


#endif // QMC_H_MIDISTREAM