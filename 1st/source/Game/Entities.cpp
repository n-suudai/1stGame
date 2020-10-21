#include "Entities.hpp"


// ゲームオブジェクト関連
DEFINE_HEAP(GameEntity, "Game", NE::AllocatePolicy);

// マスターデータ関連
DEFINE_HEAP(MasterEntity, "Master", NE::AllocatePolicy);

// ファイル関連
DEFINE_HEAP(FileEntity, "File", NE::AllocatePolicy);

