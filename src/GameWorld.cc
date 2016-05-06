#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode) {
  asset_manager = std::make_shared<GameAssetManager>(mode);
  //asset_manager->AddAsset(std::make_shared<CubeAsset>(1.0f,1.0f, 1.0f));
  int X, Y;
  int Z = 1;
  int Xaxis = 11;
  int Yaxis = 11;

    int space[Xaxis][Yaxis] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  };

  for(X = 0; X < Xaxis; X++){
    for(Y = 0; Y < Yaxis; Y++){
      if(space[Y][X] == 1){
	asset_manager->AddAsset(std::make_shared<CubeAsset>((X), -1.0f, (Y*Z)));
      }
    }
  }
}


void GameWorld::Draw() {
  asset_manager->Draw();
}
