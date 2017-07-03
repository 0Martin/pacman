#include "Map.h"

Map::Map(){
}

Map::~Map(){
}

bool Map::loadMap(const std::string path){
    int i,j;
    FILE* file = fopen(path.c_str(), "r");
    if( file == NULL ) {
        printf("Impossible to open the file %s!\n", path);
        return false;
    }

    fscanf(file, "%i %i\n", &arrayWidth, &arrayHeight);
    array = (int*)malloc(arrayWidth*arrayHeight* sizeof(int));

    int cislo;
    for(i=0;i<arrayHeight;i++){
        for(j=0;j<arrayWidth;j++){
            fscanf(file, "%i", &cislo);
            array[i*arrayWidth+j] =  cislo;
        }
    }


    fclose(file);
    return true;
}

int Map::getElement(int width, int height){
    return array[height*arrayWidth+width];
}