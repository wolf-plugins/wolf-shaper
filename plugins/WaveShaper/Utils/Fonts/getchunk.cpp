#include "Base64.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main()
{
    using namespace std;

    string input;
    string output;

    char ch;

    while (cin.get(ch))
    {
        input += ch;
    }

    const char *base64String = &input[0];

    vector<uint8_t> chunk = d_getChunkFromBase64String(base64String);

    for (vector<uint8_t>::size_type i = 0; i < chunk.size() - 1; ++i)
    {
        cout << unsigned(chunk[i]) << ",";
    }

    cout << unsigned(chunk[chunk.size() - 1]);

    return 0;
}
