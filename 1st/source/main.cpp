
#include "Game/Game.hpp"
#include "Game/GameDebug.hpp"
#include <windows.h>

_NODISCARD _Ret_notnull_
_Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new(size_t _Size)
{
    printf_s("[operator new(size_t)] is called.\n");
    void* p = malloc(_Size);
    if (p == nullptr)
    {
        throw std::bad_alloc();
    }
    return p;
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL)
  _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new(size_t _Size, std::nothrow_t const&) noexcept
{
    printf_s(
      "[operator new(size_t, std::nothrow_t const&) noexcept] is called.\n");
    return malloc(_Size);
}

_NODISCARD _Ret_notnull_
_Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new [](size_t _Size)
{
    printf_s("[operator new[](size_t)] is called.\n");
    void* p = malloc(_Size);
    if (p == nullptr)
    {
        throw std::bad_alloc();
    }
    return p;
} _NODISCARD _Ret_maybenull_ _Success_(return != NULL)
  _Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new[](size_t _Size, std::nothrow_t const&) noexcept
{
    printf_s(
      "[operator new[](size_t, std::nothrow_t const&) noexcept] is called.\n");
    return malloc(_Size);
}

void __CRTDECL operator delete(void* _Block) noexcept
{
    printf_s("[operator delete(void*) noexcept] is called.\n");
    free(_Block);
}

void __CRTDECL operator delete(void* _Block, std::nothrow_t const&) noexcept
{
    printf_s(
      "[operator delete(void*, std::nothrow_t const&) noexcept] is called.\n");
    free(_Block);
}

void __CRTDECL operator delete[](void* _Block) noexcept
{
    printf_s("[operator delete[](void*) noexcept] is called.\n");
    free(_Block);
}

void __CRTDECL operator delete[](void* _Block, std::nothrow_t const&) noexcept
{
    printf_s(
      "[operator delete[](void*, std::nothrow_t const&) noexcept] is "
      "called.\n");
    free(_Block);
}

void __CRTDECL operator delete(void* _Block, size_t) noexcept
{
    printf_s("[operator delete(void*, size_t) noexcept] is called.\n");
    free(_Block);
}

void __CRTDECL operator delete[](void* _Block, size_t) noexcept
{
    printf_s("[operator delete[](void*, size_t) noexcept] is called.\n");
    free(_Block);
}

int main()
{
    setlocale(LC_CTYPE, "");

    {
        NE::UniquePtr<Game> game = NE::MakeUnique<Game>("1stGame");

        game->Run();

        system("pause");
    }

    PrintDebug_Report_MemoryAll();

    system("pause");

    return 0;
}
