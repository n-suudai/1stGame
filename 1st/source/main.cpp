#include "Game/GameDebug.hpp"
#include "Game/Game.hpp"

int main()
{
    {
        NE::UniquePtr<Game> game = NE::MakeUnique<Game>("1stGame");

        game->Run();

        system("pause");
    }

    PrintDebug_Report_MemoryAll();

    system("pause");

    return 0;
}
