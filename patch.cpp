#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        cout << "Usage: " << argv[0] << " <file> <prefix> <string> [offset [length]]" << endl;
        return 0;
    }
    ifstream in;
    in.rdbuf()->pubsetbuf(nullptr, 0);
    in.open(argv[1], ios_base::binary);
    if (!in.is_open())
    {
        cerr << "file [" << argv[1] << "] doesn't exists\n";
        return 1;
    }
    in.seekg(0, ios_base::end);
    string buf;
    buf.resize(in.tellg());
    in.seekg(0, ios_base::beg);
    in.read((char*)buf.data(), buf.size());
    auto pos = buf.find(argv[2]);
    if (pos == string::npos)
    {
        cerr << "string [" << argv[2] << "] doesn't occur in file [" << argv[1] << "]\n";
        return 2; 
    }
    long long offset = std::stoll(argv[4]);
    long long length = std::stoll(argv[5]);
    size_t strlen = std::strlen(argv[3]);
    if (length < 0 || length < strlen)
    {
        cerr << "bad length\n";
        return 3;
    }
    std::memcpy((char*)buf.data() + pos + offset, argv[3], strlen);
    std::memset((char*)buf.data() + pos + offset + strlen, 0, length - strlen);
    ofstream out;
    out.rdbuf()->pubsetbuf(nullptr, 0);
    out.open(argv[1], ios_base::binary);
    out.write(buf.data(), buf.size());
    return 0;
}
