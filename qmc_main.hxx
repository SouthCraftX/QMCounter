#ifndef QMC_H_QMCMAIN
#   define QMC_H_QMCMAIN

#   include "midistream.hxx"

namespace qmc
{
    
    class qmcounter
    {
        private:
            midi_stream _midi_stream;
        public:
            qmcounter(/* args */);
            ~qmcounter();
    };
    
    qmcounter::qmcounter(/* args */)
    {
    }
    
    qmcounter::~qmcounter()
    {
    }
    

} // namespace qmc


#endif // QMC_H_QMCMAIN
