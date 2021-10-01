#include <stdafx.h>

#include "StrongReference.h"
#include "RTTILocator.h"

#include "CET.h"

static RTTILocator s_sIScriptableType{RED4ext::FNV1a("IScriptable")};

StrongReference::StrongReference(const TiltedPhoques::Lockable<sol::state, std::recursive_mutex>::Ref& aView,
                                 RED4ext::Handle<RED4ext::IScriptable> aStrongHandle)
    : ClassType(aView, nullptr)
    , m_strongHandle(std::move(aStrongHandle))
{
    if (m_strongHandle)
    {
        m_pType = m_strongHandle->GetType();
    }
}

StrongReference::StrongReference(const TiltedPhoques::Lockable<sol::state, std::recursive_mutex>::Ref& aView,
                                 RED4ext::Handle<RED4ext::IScriptable> aStrongHandle,
                                 RED4ext::CHandle* apStrongHandleType)
    : ClassType(aView, nullptr)
    , m_strongHandle(std::move(aStrongHandle))
{
    if (m_strongHandle)
    {
        auto const cpClass = reinterpret_cast<RED4ext::CClass*>(apStrongHandleType->GetInnerType());

        m_pType = cpClass->IsA(s_sIScriptableType) ? m_strongHandle->GetType() : cpClass;
    }
}

StrongReference::~StrongReference()
{
    // Nasty hack so that the Lua VM doesn't try to release game memory on shutdown
    if (!CET::IsRunning())
    {
        m_strongHandle.instance = nullptr;
        m_strongHandle.refCount = nullptr;
    }
}


RED4ext::ScriptInstance StrongReference::GetHandle() const
{
    return m_strongHandle.instance;
}
