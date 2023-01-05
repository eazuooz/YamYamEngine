#include "yaComponent.h"


namespace ya
{
    Component::Component(eComponentType type)
        : mType(type)
        , mOwner(nullptr)
    {
    }

    Component::~Component()
    {
    }
    void Component::Initialize()
    {
    }
    void Component::Update()
    {
    }
    void Component::FixedUpdate()
    {
    }
    void Component::Render()
    {
    }
}
