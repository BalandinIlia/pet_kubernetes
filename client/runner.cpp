#include <sstream>
#include "runner.h"

std::mutex CRunner::m_mutCons;

CRunner::CRunner(SOCK&& s) : m_sock(std::move(s)), m_id(1) {}

void CRunner::run()
{
    std::thread ts(sendAdapter, this);
    std::thread tr(recvAdapter, this);
    ts.join();
    tr.join();
}

void CRunner::send()
{
    for (;;)
    {
        waitForUser();

        const number num = readNumber();

        std::array<char, 11> mes = MS::serializeRequest(num, m_id);
        const bool bOk = sendAll(m_sock, mes.data(), 11);
        if (!bOk)
        {
            log("Connection with server lost");
            return;
        }

        {
            // Modify the table. So here I lock the table mutex to own the table.
            LG lk(m_mutTable);
            m_table[m_id] = num;
            m_tableTime[m_id] = std::chrono::system_clock::now().time_since_epoch();
        }
        
        m_id++;
    }
}

void CRunner::receive()
{
    for (;;)
    {
        char c;
        if (!recvAll(m_sock, &c, 1))
        {
            log("Connection with server lost");;
            return;
        }
        const MS::ETypeMes t = MS::decodeType(c);
        switch (t)
        {
        case MS::ETypeMes::eAnsNo:
        {
            std::array<char, 2> buf;
            if(!recvAll(m_sock, buf.data(), 2))
            {
                log("Connection with server lost");;
                return;
            }
            const short id = MS::deserializeAnsNo(buf);

            std::ostringstream sError;
            number n = 0;
            TP time = std::chrono::system_clock::now().time_since_epoch();
            {
                // Here I access the table. While I don't modify the table here, I still have to lock the mutex to
                // make sure the table is not modified while I am accessing it. If I don't do this, I can get some 
                // intermediate table state (half-modified).
                LG lk(m_mutTable);
                if (m_table.find(id) != m_table.end())
                {
                    n = m_table[id];
                    time -= m_tableTime[id];
                }
                else
                    sError << "Error: inner table does not contain request id " << id << " received from the server.";
            }

            if (!sError.str().empty())
                log(sError.str());
            else
            {
                const int tPrint = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(time).count());
                std::ostringstream mes;
                mes << "Number " << n << " has no non-trivial divisors. "
                    << "The request took " << tPrint << " ms";
                log(mes.str());
            }
            break;
        }
        case MS::ETypeMes::eAnsYes:
        {
            if (!recvAll(m_sock, &c, 1))
            {
                log("Connection with server lost");
                return;
            }
            const int sz = MS::bufSizeAnsYes(c);
            std::vector<char> buf;
            buf.resize(sz);
            if (!recvAll(m_sock, buf.data(), sz))
            {
                log("Connection with server lost");
                return;
            }
            const std::pair<short, std::vector<number>> ans = MS::deserializeAnsYes(buf);
            const short id = ans.first;
            const std::vector<number>& aComp = ans.second;

            std::ostringstream sError;
            number n = 0;
            TP time = std::chrono::system_clock::now().time_since_epoch();
            {
                LG lk(m_mutTable);
                if (m_table.find(id) != m_table.end())
                {
                    n = m_table[id];
                    time -= m_tableTime[id];
                }
                else
                    sError << "Error: inner table does not contain request id " << id << " received from the server.";
            }

            if (!sError.str().empty())
                log(sError.str());
            else
            {
                const int tPrint = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(time).count());
                std::ostringstream mes;
                mes << "Number " << n << " has the following non-trivial divisors: ";
                for (const number& comp : aComp)
                    mes << comp << " ";
                mes << "The request took " << tPrint << " ms";
                log (mes.str());
            }
            break;
        }
        default:
        {
            log("Incorrect code from server");
            break;
        }
        }
    }
}

void CRunner::waitForUser()
{
    // 'a' is a signal to receive a number from the user
    char c = 0;
    while (c != 'a')
        c = std::cin.get();
}

number CRunner::readNumber()
{
    number num = 0;

    // Take a number from the user (from console). 
    // So here I lock the console mutex to own the console.
    LG lk(m_mutCons);
    while (num <= 0)
    {
        std::cout << "Input a number: ";
        if(!(std::cin >> num))
        {
            num = 0;
            std::cin.clear();
            std::cin.get();
            std::cout << "Must be a number" << std::endl;
        }
        else if (num <= 0)
        {
            num = 0;
            std::cout << "Must be a positive number" << std::endl;
        }
    }
    return num;
}

void CRunner::log(const std::string s)
{
    LG lk(m_mutCons);
    std::cout << s << std::endl;
}