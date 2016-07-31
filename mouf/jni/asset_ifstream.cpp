#include "asset_ifstream.hpp"

AAssetManager* AssetIfstream::am = NULL;

void getline (AssetIfstream& ai, std::string& s) {
  ai.getline(s);
}
