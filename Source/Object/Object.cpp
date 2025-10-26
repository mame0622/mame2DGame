#include "Object.h"

int Object::objectIdCount_ = 0;

Object::Object(const std::string& name)
    : name_(name), objectId_(objectIdCount_++)
{
}

// ImGui
void Object::DrawDebug()
{
    transform_.DrawDebug();
}
