#include <iostream>
#include <bitset>

#define SYNC_BYTE_MASK 0xff000000
#define TEI_MASK 0x800000
#define PAYLOAD_START_MASK 0x400000
#define PRIORITY_MASK 0x200000
#define PID_MASK 0x1fff00
#define SCRAMBLING_CTL_MASK 0xc0
#define ADAPTATION_FIELD_MASK 0x20
#define HAS_PAYLOAD_MASK 0x10
#define COUNTER_MASK 0xf

#define HEADER_BYTES 4
#define HEADER_BITS 8 * HEADER_BYTES

using namespace std;

class Header
{

public:
    bitset<HEADER_BITS> *full;

    unsigned char syncByte;
    bool tei;
    bool payloadStart;
    bool priority;
    int16_t pid;
    bitset<2> *scramblingCtl;
    bool adaptationField;
    bool hasPayload;
    int counter;

    Header(const char *, size_t);
    void display();
};

Header::Header(const char *header, size_t n)
{
    uint32_t bytes = convertLittleEndian(header, n);
    this->full = new bitset<HEADER_BITS>(bytes);

    uint32_t tmp = bytes & SYNC_BYTE_MASK;
    this->syncByte = ((char *)&tmp)[n - 1];

    this->tei = bytes & TEI_MASK;
    this->payloadStart = bytes & PAYLOAD_START_MASK;
    this->priority = bytes & PRIORITY_MASK;
    this->pid = bytes & PID_MASK;
    this->pid >>= 8;
    this->scramblingCtl = new bitset<2>(bytes & SCRAMBLING_CTL_MASK);
    this->adaptationField = bytes & ADAPTATION_FIELD_MASK;
    this->hasPayload = bytes & HAS_PAYLOAD_MASK;
    this->counter = bytes & COUNTER_MASK;
}

void Header::display()
{
    cout << "pid: " << this->pid << endl;
}