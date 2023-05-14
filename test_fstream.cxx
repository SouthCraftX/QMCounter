
#include <iostream>
#include "modules/fstream.hxx"

int main()
{

    qmc::mod::WinInputFileStream stream;
    qmc::byte_t buf[64] = { 0 };

    std::cout << "Open:" << stream.open("t.txt") << '\n';
    std::cout << "Read:" << stream.read(buf , 64(qmc::uint32_t));
    std::cout << "Context:" << buf;
    

}