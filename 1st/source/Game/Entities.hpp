#pragma once

#include "../Engine/Memory/MemoryManager.hpp"

#define DEFINE_ENTITY_CLASS(className)  \
    class className                     \
    {                                   \
        DECLARE_HEAP(className);        \
                                        \
    protected:                          \
        className() = default;          \
        virtual ~className() = default; \
    };

// ゲームオブジェクト関連
DEFINE_ENTITY_CLASS(GameEntity);

// マスターデータ関連
DEFINE_ENTITY_CLASS(MasterEntity);

// ファイル関連
DEFINE_ENTITY_CLASS(FileEntity);
