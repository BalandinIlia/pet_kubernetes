#include "thread"
#include "../networking_library/inner_interaction.h"
#include "../networking_utils/make_socket.h"
#include "../logger/logger.h"

// This function does the actual work: finds non-trivial divisors of number num
static std::vector<number> doCalc(number num)
{
    LOG2("Calcing for", num);

    // magical crash number
    if(num == 42)
        throw std::exception();

    std::vector<number> ans;
    for(number d = 2; d < num; d++)
    {
        if(num % d == 0)
            ans.push_back(d);
    }
    LOG2("Found divisors", ans)
    return ans;
}

// This function serves an individual request. The function takes the socket corresponding to the
// request.
static void solveReq(SOCK&& sock)
{
    CThreadName tn("Solve request thread");

    const std::optional<number> reqNum = IC::getReq(sock);
    if(reqNum == std::nullopt)
    {
        LOG2("Failed to receive request number", true)
        return;
    }
    LOG2("Received number ", reqNum.value())
    const std::vector<number> ans = doCalc(reqNum.value());
    IC::answer(sock, ans);
}

int main()
{
    CThreadName tn("Main thread");
    LOG1(std::string("Starting calc container"))

    CInteractKuberentes::start();

    const std::optional<SOCK> sockMain = listenInfo();
    if(sockMain.has_value())
        LOG1("Main socket created")
    else
    {
        LOG2("Failed to create main socket", true)
        CInteractKuberentes::terminateLive();
    }

    for (;;)
    {
        SOCK conn = sockMain.value().acceptS();
        LOG1(std::string("New request received"))
        std::thread t(solveReq, conn.mv());
        t.detach();
    }

    return 0;
}