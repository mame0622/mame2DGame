#pragma once
#include "Object/Object.h"

class Character : public Object
{
public:
    Character(const std::string& name);
    ~Character() override {}

    void DrawDebug();
};

