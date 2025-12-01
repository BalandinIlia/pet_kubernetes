#pragma once
#include "vector"
#include "optional"
#include "../parameters/controls.h"
#include "../networking_utils/make_socket.h"

// IC = "Inner Communication"
namespace IC
{
    // This function asks service by inner communication.
    // It returns the answer as an array of numbers if everything is ok and nullopt
    // in case of some error.
    std::optional<std::vector<number>> ask(const SOCK& id, number num);

    // This function answers to inner request.
    bool answer(const SOCK& id, const std::vector<number>& ans);

    // This function allows to receive inner request
    std::optional<number> getReq(const SOCK& id);
}