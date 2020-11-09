#include "Entities.hpp"

// ゲームオブジェクト関連
DEFINE_HEAP(GameEntity, "Game", NE::AllocatePolicy);

// マスターデータ関連
DEFINE_HEAP(MasterEntity, "Master", NE::AllocatePolicy);

// プレイデータ関連
DEFINE_HEAP(PlayDataEntity, "PlayData", NE::AllocatePolicy);
