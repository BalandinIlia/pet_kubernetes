#include "thread"
#include "../networking_library/messages.h"
#include "../networking_library/inner_interaction.h"
#include "../networking_utils/send_receive.h"
#include "../networking_utils/make_socket.h"
#include "../logger/logger.h"

static std::vector<number> doCalc(number num)
{
    LOG("Calcing for ", num);

    std::vector<number> ans;
    for(number d = 2; d < num; d++)
    {
        if(num % d == 0)
            ans.push_back(d);
    }
    LOG("Quantity of found divisors ", static_cast<number>(ans.size()));
    return ans;
}

static void solveReq(SOCKET id)
{
    setThreadName("Solve request thread");
    const number reqNum = getReqInner(id);
    LOG("Received number ", reqNum);
    std::vector<number> ans = doCalc(reqNum);
    ans.push_back(0);
    answerInner(id, ans);
}

int main()
{
    setThreadName("Main thread");
    LOG(std::string("Starting calc container"));

    CInteractKuberentes::start();

    const SOCKET idSocket = listenInfo();
    LOG(std::string("Main socket created"));

    for (;;)
    {
        SOCKET conn = accept(idSocket, nullptr, nullptr);
        LOG(std::string("New request received"));
        std::thread t(solveReq, conn);
        t.detach();
    }

    return 0;
}