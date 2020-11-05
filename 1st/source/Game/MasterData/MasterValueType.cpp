#include "MasterValueType.hpp"
#include <array>
#include <functional>

template <class T>
T ToValue(const NE::String& valueStr, T notFound = T())
{
    if (valueStr == "")
    {
        return notFound;
    }

    NE::IStringStream iss;

    iss.str(valueStr);

    T value;

    iss >> value;

    return value;
}

template <typename T>
NE::U8* WriteValue(NE::U8* pBlock, const NE::String& valueStr)
{
    T value = ToValue<T>(valueStr, T(0));

    T* p = reinterpret_cast<T*>(pBlock);

    (*p) = value;

    return pBlock + sizeof(T);
}

template <>
NE::U8* WriteValue<NE::String>(NE::U8* pBlock, const NE::String& valueStr)
{
    NE::SizeT length = strlen(valueStr.c_str());

    void* pMem = NE_MEM_ALLOC(length + 1);

    memcpy_s(pMem, length + 1, valueStr.c_str(), length);

    NE::U64& addr = (*reinterpret_cast<NE::U64*>(pBlock));

    addr = reinterpret_cast<NE::U64>(pMem);

    return pBlock + sizeof(NE::U64);
}

template <>
NE::U8* WriteValue<void>(NE::U8* pBlock, const NE::String&)
{
    return pBlock;
}

template <typename Type>
void GetValueString(NE::U8* addr, NE::StringStream& ss)
{
    Type value = static_cast<Type>(*reinterpret_cast<Type*>(addr));
    ss << value;
}

template <>
void GetValueString<NE::String>(NE::U8* addr, NE::StringStream& ss)
{
    NE::U64 stringAddr = (*reinterpret_cast<NE::U64*>(addr));
    ss << NE::String(reinterpret_cast<const char*>(stringAddr));
}

template <>
void GetValueString<void>(NE::U8*, NE::StringStream& ss)
{
    ss << "void";
}

MasterValueType::MasterValueType() : m_type(Type::Error)
{
}

MasterValueType::MasterValueType(Type type) : m_type(type)
{
}

NE::SizeT MasterValueType::GetSize() const
{
    constexpr std::array<NE::SizeT, static_cast<NE::SizeT>(Type::Num)>
    SizeList = {
      0,                 sizeof(NE::S8),     sizeof(NE::S16),
      sizeof(NE::S32),   sizeof(NE::S64),    sizeof(NE::U8),
      sizeof(NE::U16),   sizeof(NE::U32),    sizeof(NE::U64),
      sizeof(NE::Float), sizeof(NE::Double), sizeof(NE::U64), // String
    };

    return SizeList[static_cast<NE::SizeT>(m_type)];
}

NE::String MasterValueType::ToString() const
{
    constexpr std::array<const char*, static_cast<NE::SizeT>(Type::Num)>
    StringList = {"Error", "S8",  "S16", "S32",   "S64",    "U8",
                  "U16",   "U32", "U64", "Float", "Double", "String", };

    return StringList[static_cast<NE::SizeT>(m_type)];
}

MasterValueType MasterValueType::FromString(const NE::String& str)
{
    static const std::array<MasterValueType, static_cast<NE::SizeT>(Type::Num)>
    Instances = {
      MasterValueType(Type::Error),  MasterValueType(Type::S8),
      MasterValueType(Type::S16),    MasterValueType(Type::S32),
      MasterValueType(Type::S64),    MasterValueType(Type::U8),
      MasterValueType(Type::U16),    MasterValueType(Type::U32),
      MasterValueType(Type::U64),    MasterValueType(Type::Float),
      MasterValueType(Type::Double), MasterValueType(Type::String), };

    auto it = std::find_if(Instances.begin(), Instances.end(),
                           [str](const MasterValueType& value)
    { return value.ToString() == str; });

    if (it != Instances.end())
    {
        return *it;
    }

    return MasterValueType(Type::Error);
}

bool MasterValueType::Isvalid() const
{
    return m_type != Type::Error;
}

bool MasterValueType::IsString() const
{
    return m_type == Type::String;
}

NE::U8* MasterValueType::WriteMemoryBlock(NE::U8* pBlock,
                                          const NE::String& valueStr) const
{
    NE::U8* pWriteAfterAddr = pBlock;

    typedef std::function<NE::U8*(NE::U8 * pBlock, const NE::String & valueStr)>
    WriteValueFunc;

    const std::array<WriteValueFunc, static_cast<NE::SizeT>(Type::Num)>
    WriteFunctions = {[](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<void>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::S8>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::S16>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::S32>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::S64>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::U8>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::U16>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::U32>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::U64>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::Float>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::Double>(pBlock, valueStr); },
                      [](NE::U8* pBlock, const NE::String& valueStr)
    { return WriteValue<NE::String>(pBlock, valueStr); }, };

    pWriteAfterAddr =
      WriteFunctions[static_cast<NE::SizeT>(m_type)](pBlock, valueStr);

    return pWriteAfterAddr;
}

void MasterValueType::FreeMemoryBlock(NE::U8* pBlock) const
{
    if (!IsString())
    {
        return;
    }

    NE::U64& addr = (*reinterpret_cast<NE::U64*>(pBlock));

    void* pMem = reinterpret_cast<void*>(addr);

    NE_MEM_FREE(pMem);
}

void MasterValueType::GetValueString(NE::U8* pBlock, NE::StringStream& ss) const
{
    typedef std::function<void(NE::U8 * pBlock, NE::StringStream & ss)>
    GetValueStringFunc;

    std::array<GetValueStringFunc, static_cast<NE::SizeT>(Type::Num)>
    GetvalueStringFunctions = {[](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<void>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::S8>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::S16>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::S32>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::S64>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::U8>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::U16>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::U32>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::U64>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::Float>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::Double>(pBlock, ss); },
                               [](NE::U8* pBlock, NE::StringStream& ss)
    { ::GetValueString<NE::String>(pBlock, ss); }, };

    GetvalueStringFunctions[static_cast<NE::SizeT>(m_type)](pBlock, ss);
}
