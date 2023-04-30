#include "modules\fstream.hxx"
#include <iostream>
int main()
{

    qmc::mod::InputFileStream stream;
    qmc::byte_t buf[64] = { 0 };

    std::cout << "Open:" << stream.open("t.txt") << '\n';
    std::cout << "Read:" << stream.read(buf , 64);
    std::cout << "Context:" << buf;
    

}