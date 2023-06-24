#ifndef QMC_H_MIDISTREAM
#   define QMC_H_MIDISTREAM

#   include "def.hxx"

namespace qmc
{
    template<class file_stream_tpl>
    class midi_stream
    {
        private:
            file_stream_tpl _fstream;
        public:
            qmc::errid_t read_header();
            midi_stream(/* args */);
            ~midi_stream();
    };
    
    midi_stream::midi_stream(/* args */)
    {
    }
    
    midi_stream::~midi_stream()
    {
    }
    
} // namespace qmc


#endif // QMC_H_MIDISTREAM