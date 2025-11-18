#include "thread"
#include "mutex"
#include "shared_mutex"
#include "string"
#include "sstream"
#include "map"
#include "../networking_library/messages.h"
#include "../networking_library/inner_interaction.h"
#include "../networking_utils/send_receive.h"
#include "../networking_utils/make_socket.h"
#include "../logger/logger.h"

static std::map<number, std::vector<number>> cache;
static std::shared_mutex mutCache;

static std::vector<number> askCalc(number num)
{
	const SOCKET idSocketService = connectToService();
	LOG("Service socket id", idSocketService);

	std::vector<number> aNum = askInner(idSocketService, num);
    LOG("Received an answer from calc service. First number is ", aNum[0]);

    return aNum;
}

static void solveReq(SOCKET id)
{
    setThreadName("Solve request thread");

    const number reqNum = getReqInner(id);
    LOG("Request ", reqNum);
    
    std::vector<number> ans;

    bool bFound = false;
    {
        std::shared_lock lk(mutCache);
        if(cache.find(reqNum) != cache.end())
        {
            ans = cache[reqNum];
            bFound = true;
        }
    }
    
    if(bFound)
    {
        LOG("Answer found in cache:", ans);
        answerInner(id, ans);
    }
    else
    {
        LOG("Answer not found in cache, adressing calc service");
        ans = askCalc(reqNum);
        LOG("Answer received:", ans);
        {
            std::unique_lock lk(mutCache);
            cache[reqNum] = ans;
        }
        answerInner(id, ans);
    }
}

int main()
{
    setThreadName("Main thread");
    LOG(std::string("Starting cache container"));

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