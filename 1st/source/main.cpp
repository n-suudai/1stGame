#include "Game/GameDebug.hpp"
#include "Game/Game.hpp"


void* operator new (size_t b)
{
    printf_s("default new is called.\n");
    return malloc(b);
}

void operator delete (void* p)
{
    printf_s("default delete is called.\n");
    free(p);
}

#include <fstream>


int main()
{
    {
        NE::UniquePtr<Game> game = NE::MakeUnique<Game>("1stGame");

        game->Run();

        system("pause");
    }

    PrintDebug_Report_MemoryAll();

    system("pause");

    // ファイル入力テスト
    {
        std::ifstream ifs("Test.txt");

        NE::String text;
        NE::S32 line = 1;

        while (std::getline(ifs, text))
        {
            printf_s("Line %d : %s\n", line++, text.c_str());
        }
    }

    return 0;
}
