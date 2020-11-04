#include "Game.hpp"
#include "Character.hpp"
#include "GameDebug.hpp"
#include "MasterData/MasterData.hpp"

Game::Game(const char* title) : m_title(title)
{
    m_character = NE::MakeUnique<Character>();

    NE::SharedPtr<IMasterParser> parser = NE::MakeShared<MasterParser_CSV>();
    m_masterTable = NE::MakeUnique<MasterTable>("Character.table", parser);
}

Game::~Game()
{
}

void Game::Run()
{
    bool exitRequested = false;
    while (!exitRequested)
    {
        exitRequested = Loop();
    }
}

bool Game::Loop()
{
    printf_s("%s\n", m_title.c_str());

    int code = 0;
    scanf_s("%d", &code);

    if (code == 1)
    {
        return true;
    }
    else if (code == 2)
    {
        PrintDebug_Report_HeapTreeStats();
        // PrintDebug_Report_MemoryAssertions();
    }
    else if (code == 3)
    {
        m_masterTable->DebugPrint();
    }
    else if (code == 4)
    {
        m_masterTable->ForEach([](const MasterRecord* p)
        {
            if (p->Get_U64("Age") >= 25)
            {
                printf_s("%sは年寄りだ...\n", p->Get_String("Name").c_str());
            }
        });
    }

    return false;
}
