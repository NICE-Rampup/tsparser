#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;
class header
{
    bitset<32> full_header;
    bitset<8> sync_byte;
    bitset<1> transport_error_indicator;
    bitset<1> payload_unit_start_indicator;
    bitset<1> transport_priority;
    bitset<13> PID;
    bitset<2> transport_scrambling_control;
    bitset<2> adaptation_field_control;
    bitset<4> continuity_counter;

public:
    void parseHeader(int h)
    {
        full_header = bitset<32>(h);
        string str = full_header.to_string();
        bitset<8> b1(str.substr(0, 8));
        bitset<1> b2(str.substr(8, 1));
        bitset<1> b3(str.substr(9, 1));
        bitset<1> b4(str.substr(10, 1));
        bitset<13> b5(str.substr(11, 13));
        bitset<2> b6(str.substr(24, 2));
        bitset<2> b7(str.substr(26, 2));
        bitset<4> b8(str.substr(28, 4));
        sync_byte = b1;
        transport_error_indicator = b2;
        payload_unit_start_indicator = b3;
        transport_priority = b4;
        PID = b5;
        transport_scrambling_control = b6;
        adaptation_field_control = b7;
        continuity_counter = b8;
    }
    void display()
    {
        cout << "sync_byte: " << sync_byte.to_ulong() << endl;
        cout << "transport_error_indicator: " << transport_error_indicator.to_ulong() << endl;
        cout << "payload_unit_start_indicator: " << payload_unit_start_indicator.to_ulong() << endl;
        cout << "transport_priority: " << transport_priority.to_ulong() << endl;
        cout << "PID: " << PID.to_ulong() << endl;
        cout << "transport_scrambling_control: " << transport_scrambling_control.to_ulong() << endl;
        cout << "adaptation_field_control: " << adaptation_field_control.to_ulong() << endl;
        cout << "continuity_counter: " << continuity_counter.to_ulong() << endl
             << endl;
    }
};
int main()
{
    int count = 0;
    fstream file("sample1.ts", ios::in | ios::binary);
    while (!file.eof())
    {
        int h;
        ++count;
        file.read(reinterpret_cast<char *>(&h), sizeof(int));
        header head;
        head.parseHeader(h);
        head.display();
        file.ignore(184); // ignores payload
    }
    cout << "no of packets: " << count << endl;
    return 0;
}