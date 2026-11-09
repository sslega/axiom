#pragma once

namespace axiom
{
    class Archive;
    struct TypeDescriptor;

    void SerializeReflected(Archive& ar, const void* obj, const TypeDescriptor& desc);
    void DeserializeReflected(Archive& ar, void* obj, const TypeDescriptor& desc);
}