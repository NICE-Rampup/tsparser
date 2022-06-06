#include <fstream>
#include <iostream>

using namespace std;

int top = -1;
uint16_t programMapPIDs[500];

struct PacketHeader
{
    uint8_t syncByte;
    bool transportErrorIndicator;
    bool payloadUnitStartIndicator;
    bool transportPriority;
    uint16_t PID;
    uint8_t transportScramblingControl;
    uint8_t adaptationFieldControl;
    uint8_t continuityCounter;
};

bool isProgramMapPID(uint16_t &PID)
{
    for (int i = 0; i <= top; i++)
    {
        if (programMapPIDs[i] == PID)
            return true;
    }
    return false;
}

void parsePacket(unsigned char *p)
{
    PacketHeader ph;

    ph.syncByte = *p; // p[0]

    ++p; // p[1]
    ph.transportErrorIndicator = (*p & 0x80) >> 7;
    ph.payloadUnitStartIndicator = (*p & 0x40) >> 6;
    ph.transportPriority = (*p & 0x20) >> 5;
    ph.PID = (*p & 0x1F) << 8;
    ph.PID = ph.PID | *++p; // p[2]

    ++p; // p[3]
    ph.transportScramblingControl = (*p & 0xC0) >> 6;
    ph.adaptationFieldControl = (*p & 0x30) >> 4; // does not store numeric value
    cout << ((*p & 0x30) >> 4) << endl;           // but prints numeric value
    ph.continuityCounter = *p & 0xF;

    bool hasAdaptationfield;
    bool hasPayload;
    uint8_t adaptationfieldLength = 0; // 0 if no Adaptationfield present

    adaptationfieldLength = 0;
    if (ph.adaptationFieldControl == 10 || ph.adaptationFieldControl == 11) // check adaptation field
    {
        hasAdaptationfield = true;
        if (ph.adaptationFieldControl == 11)
        {
            hasPayload = true;
        }
        else
        {
            hasPayload = false;
        }
        adaptationfieldLength = *++p;
    }
    else
    {
        hasAdaptationfield = false;
        hasPayload = false;
        if (ph.adaptationFieldControl == 01)
            hasPayload = true;
    }
    if (hasPayload) // start from here
    {
        p = p + adaptationfieldLength; // place pointer at start of payload
        if (ph.payloadUnitStartIndicator)
        {
            p = p + *++p; // place pointer at start of new payload unit
        }
        if (ph.PID = 0x0000) // get pat
        {
            uint8_t tableId = *p++;                            // 8
            uint8_t sectionSyntaxIndicator = (*p & 0x40) >> 7; // 1
            uint16_t sectionLength = ((*p & 0xF) << 8) | *++p; // 12
            uint16_t transportStreamId = (*++p << 8) | (*++p); // 16
            uint8_t versionNumber = (*++p & 0x3E) >> 1;        // 5
            uint8_t currentNextIndicatior = *p & 0x1;          // 1
            uint8_t sectionNumber = *++p;                      // 8
            uint8_t lastSectionNumber = *++p;                  // 8

            for (int i = 0; i < (sectionLength - 5 - 4); i++) // condition referred from other source
            {
                uint16_t programNumber = (*p << 8) | (*++p);
                cout << "program number: " << programNumber << endl;
                if (programNumber != 0) // if 0, contains nit
                {
                    uint16_t programMapPID = ((*p & 00011111) << 8) | *++p;
                    if (!isProgramMapPID(programMapPID))
                        programMapPIDs[++top] = programMapPID;
                }
            }
        }
        else if (isProgramMapPID(ph.PID))
        {
            uint8_t tableId = *p++;                                    // 8
            bool sectionSyntaxIndicator = (*p & 0x40) >> 7;            // 1
            uint16_t sectionLength = ((*p & 0xF) << 8) | *++p;         // 12
            uint16_t transportStreamId = (*++p << 8) | (*++p);         // 16
            uint8_t versionNumber = (*++p & 0x3E) >> 1;                // 5
            bool currentNextIndicatior = *p & 0x1;                     // 1
            uint8_t sectionNumber = *++p;                              // 8
            uint8_t lastSectionNumber = *++p;                          // 8
            uint16_t pcrPID = ((*++p & 0x1F) << 8) | (*++p);           // 13
            uint16_t programInfoLength = ((*++p & 0xF) << 8) | (*++p); // 12

            size_t infoBytesRemaining = sectionLength - 9 - programInfoLength - 4;
            while (infoBytesRemaining > 0) // condition referred from other source
            {
                uint8_t streamType = *p;
                uint16_t elementaryPID = ((*++p & 0x1F) << 8) | (*++p);
                cout << "elementary PID: " << elementaryPID << endl;
                uint16_t esInfoLength = ((*++p & 0xF) << 8) | (*++p);
                if (streamType == 0x01 || streamType == 0x02)
                {
                    cout << "stream type: audio" << endl;
                }
                else if (streamType == 0x01 || streamType == 0x02)
                {
                    cout << "stream type: video" << endl;
                }
                infoBytesRemaining -= 5 + esInfoLength;
            }
        }
    }
}

int main()
{
    unsigned char packet[188];
    ifstream file("sample.ts", ios::in | ios::binary);
    while (!file.eof())
    {
        file.read((char *)(&packet[0]), 188);
        parsePacket(packet);
    }
    return 0;
}
