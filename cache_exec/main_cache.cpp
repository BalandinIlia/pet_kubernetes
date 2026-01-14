#include <thread>
#include <mutex>
#include <shared_mutex>
#include <map>
#include "../networking_library/inner_interaction.h"
#include "../networking_utils/make_socket.h"
#include "../logger/logger.h"

// cache with calculated answers
static std::map<number, std::vector<number>> cache;

// mutex protecting the cache
static std::shared_mutex mutCache;

// This function asks calc service about number num
static std::optional<std::vector<number>> askCalc(number num)
{
    const std::optional<SOCK> sockServ = connectToService();
    if(sockServ == std::nullopt)
    {
        LOG2("Failed to connect to service", true)
        return std::nullopt;
    }

    const std::optional<std::vector<number>> aNum = IC::ask(sockServ.value(), num);
    if(aNum != std::nullopt)
        LOG2("Received an answer from calc service:", aNum.value())
    else
        LOG2("Failed to get an answer from calc service", true)

    return aNum;
}

// This function serves an individual request. The function takes the socket corresponding to the
// request.
static void solveReq(SOCK&& sockReq)
{
    CThreadName tn("Solve request thread");

    const std::optional<number> reqNum = IC::getReq(sockReq);
    if(reqNum == std::nullopt)
    {
        LOG2("Failed to receive request value", true)
        return;
    }
    LOG2("Received request", reqNum.value())
    
    std::vector<number> ans;

    bool bFound = false;
    {
        std::shared_lock lk(mutCache);
        if(cache.find(reqNum.value()) != cache.end())
        {
            ans = cache[reqNum.value()];
            bFound = true;
        }
    }
    
    if(bFound)
        LOG2("Answer found in cache:", ans)
    else
    {
        LOG1("Answer not found in cache; asking calc service")
        const std::optional<std::vector<number>> res = askCalc(reqNum.value());
        if(res == std::nullopt)
        {
            LOG2("Failed to receive an answer from calc service", true)
            return;
        }
        else
        {
            ans = res.value();
            LOG2("Answer received:", ans)
            std::unique_lock lk(mutCache);
            cache[reqNum.value()] = ans;
        }
    }

    if(!IC::answer(sockReq, ans))
        LOG2("Failed to send the answer", true)
}

int main()
{
    CThreadName tn("Main thread");
    LOG1(std::string("Starting cache container"));

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
        SOCK sockReq = sockMain.value().acceptS();
        LOG1("New request received");
        std::thread t(solveReq, sockReq.mv());
        t.detach();
    }

    return 0;
}
