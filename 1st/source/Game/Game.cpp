#include "Game.hpp"
#include "Character.hpp"
#include "GameDebug.hpp"
#include "MasterData/MasterData.hpp"
#include "../Engine/Logging/Logging.hpp"

Game::Game(const char* title) : m_title(title)
{
    m_character = NE::MakeUnique<Character>();

    NE::SharedPtr<IMasterParser> parser = NE::MakeShared<MasterParser_CSV>();
    m_masterTable = NE::MakeUnique<MasterTable>("Character.table", parser);

    m_logger = NE::MakeUnique<NE::Log::Logger>();
}

Game::~Game()
{
}

void Game::Run()
{
    m_logger->Print("HELLO!!");

    printf_s("%s\n", m_title.c_str());

    bool exitRequested = false;
    while (!exitRequested)
    {
        exitRequested = Loop();
    }
}

bool Game::Loop()
{
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
    else if (code == 5)
    {
        m_character->ShowStatus([](const HitPoint& hitPoint)
        {
            printf_s("-----[Status]-----\n");
            printf_s("HitPoint : %I64d / %I64d\n", hitPoint.Value(),
                     hitPoint.Max());
        });
    }
    else if (code == 6)
    {
        printf_s("LevelUp!!\n");
        m_character->LevelUp();
    }
    else if (code == 7)
    {
        m_character->HitTest(Attack(5));
        if (m_character->IsDead())
        {
            printf_s("=== YOU ARE DEAD ===\n");
            return true;
        }
    }

    return false;
}
