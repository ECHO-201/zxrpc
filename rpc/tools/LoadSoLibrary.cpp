#include "LoadSoLibrary.h"

using namespace zxrpc::tools;

LoadSoLibrary::LoadSoLibrary(){

};

LoadSoLibrary::~LoadSoLibrary(){
    dlclose(handle);
};