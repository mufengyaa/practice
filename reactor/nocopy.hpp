#pragma once

struct no_copy
{
    no_copy() {}
    ~no_copy() {}
    no_copy(const no_copy &t) = delete;
    no_copy operator=(const no_copy &t) = delete;
};