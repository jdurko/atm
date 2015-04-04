#include <map>
#include <string.h>
#include <utility>
#include <fstream>
#include <iostream>
// 1. Define a C++ struct to act as your record

typedef unsigned int   DWORD;
typedef unsigned short WORD;
const size_t MAX_NAME_LEN = 32;

struct CustomerRecord
{
    DWORD m_dwRecordID;
    char  m_szFirstName[MAX_NAME_LEN];
    char  m_szLastName[MAX_NAME_LEN];
    char  m_szEmailAddress[MAX_NAME_LEN];
    WORD  m_wPIN;
    DWORD dw_SSN;
    DWORD m_dwDDL;


    CustomerRecord()
        : m_dwRecordID(0),
          m_wPIN(0),
          dw_SSN(0),
          m_dwDDL(0)
    {
        memset(m_szFirstName, 0, sizeof(m_szFirstName));
        memset(m_szLastName, 0, sizeof(m_szLastName));
        memset(m_szEmailAddress, 0, sizeof(m_szEmailAddress));
    };
    // An initialization constructor
    CustomerRecord(DWORD dwKey, const char* szFirstName, const char* szLastName, const char* szEmail,
                   WORD wPIN, DWORD dwSSN, DWORD dwDDL)
                   : m_dwRecordID(dwKey), m_wPIN(wPIN), dw_SSN(dwSSN), m_dwDDL(dwDDL)
    {
        memset(m_szFirstName, 0, sizeof(m_szFirstName));
        memset(m_szLastName, 0, sizeof(m_szLastName));
        memset(m_szEmailAddress, 0, sizeof(m_szEmailAddress));

        if (szFirstName)
            strncpy(m_szFirstName, szFirstName, sizeof(m_szFirstName) - 1);

        if (szLastName)
            strncpy(m_szLastName, szLastName, sizeof(m_szLastName) - 1);

        if (szEmail)
            strncpy(m_szEmailAddress, szEmail, sizeof(m_szEmailAddress) - 1);
    };

// 2. Overload the comparison operators so that
// a CustomerRecord is only using the m_dwRecordID
// field on comparisons.

    bool operator< (const CustomerRecord& rhs) const
    { return (m_dwRecordID < rhs.m_dwRecordID); };

    bool operator> (const CustomerRecord& rhs) const
    { return (m_dwRecordID > rhs.m_dwRecordID); };

    bool operator== (const CustomerRecord& rhs) const
    { return (m_dwRecordID == rhs.m_dwRecordID); };
};

// 3. Define a table of CustomerRecords using an STL map

std::map<DWORD, CustomerRecord> g_mapCustomerRecords;

