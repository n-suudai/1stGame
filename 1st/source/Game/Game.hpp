﻿#pragma once

#include "../Engine/STL.hpp"
#include "Entities.hpp"

namespace NE
{
namespace Log
{
class Logger;
}
}
class MasterTable;
class Character;
class Game : public GameEntity
{
public:
    Game(const char* title);

    ~Game();

    void Run();

private:
    bool Loop();

private:
    NE::String m_title;
    NE::UniquePtr<Character> m_character;
    NE::UniquePtr<MasterTable> m_masterTable;
    NE::UniquePtr<NE::Log::Logger> m_logger;
};
