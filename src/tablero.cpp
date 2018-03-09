#include <string>
#include <fstream>
#include "tablero.h"

using namespace std;

bool cargarTablero(tTablero tablero, const string fname){
    string linea;
    ifstream file;
    bool error = false;
    file.open(fname);

    for(unsigned int i = 0; i < MAX_FILAS && !error; i++){
        getline(file, linea); // Así quitamos el EOL
        error = file.fail() || linea.length() != MAX_FILAS;
        for(unsigned int j = 0; j < linea.length() && !error; j++)
            tablero[i][j] = linea[j];
    }

    file.close();

    return !error;
}

void mostrarTablero(const tTablero tablero){
    // Asumimos que se ha hecho ya cls

}
