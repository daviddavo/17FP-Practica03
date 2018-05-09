#include "puntuaciones.h"

template <class T>
void swap(T &p1, T &p2) {
    T aux = p1;
    p1 = p2;
    p2 = aux;
}

bool criterioAlpha(const tPuntuacionPtr p1, const tPuntuacionPtr p2) { return p1->nombre < p2->nombre; }

bool criterioNum(const tPuntuacionPtr p1, const tPuntuacionPtr p2) {
    return p1->puntos > p2->puntos || (p1->puntos == p2->puntos && p1->nombre < p2->nombre);
}

// Usado por quickSortPuntuaciones()
int qsPartition(tPuntuacionPtr puntuaciones[], const int bot, const int top,
                bool (&criterio)(const tPuntuacionPtr, const tPuntuacionPtr)) {
    // Elegimos un pivote cualquiera
    tPuntuacionPtr pivote = puntuaciones[top];
    int i = bot;
    for (int j = bot; j <= top - 1; j++) {
        // Movemos todos los elementos mayores que el pivote a la derehca
        // Y los menores a la izquierda
        // i será el índice en el que queda el pivote
        if (criterio(puntuaciones[j], pivote)) {
            swap(puntuaciones[j], puntuaciones[i]);
            i++;
        }
    }
    // Intercambiamos con el pivote
    swap(puntuaciones[i], puntuaciones[top]);
    // Retornamos el indice del pivote
    return i;
}

// https://en.wikipedia.org/wiki/Don%27t_repeat_yourself
// Nos evitamos el tener que hacer copia y pega en una función para ordenar alfabeticamente, o por puntuaciones
// Aunque esto sería aún mejor con un template
void quickSortPuntuaciones(tPuntuacionPtr puntuaciones[], const int bot, const int top,
                           bool (&criterio)(const tPuntuacionPtr, const tPuntuacionPtr)) {
    if (bot < top) {
        // Esta es la parte fácil de quickSort
        int parti = qsPartition(puntuaciones, bot, top, criterio);

        // quickSortPuntuaciones(puntuaciones, bot, parti - 1, criterio);
        quickSortPuntuaciones(puntuaciones, bot, parti - 1, criterio);
        quickSortPuntuaciones(puntuaciones, parti + 1, top, criterio);
    }
}

// void quickSortPuntuaciones(tPuntuacionPtr puntuaciones[], int, int, bool (&)(const tPuntuacionPtr &, const
// tPuntuacionPtr &));

bool guardarPuntuaciones(const tPuntuaciones &puntuaciones) {
    std::ofstream file(FILE_PUNTUACIONES);

    for (unsigned i = 0; i < puntuaciones.cnt && !file.fail(); i++)
        file << puntuaciones.puntuaciones[i].nombre << " " << puntuaciones.puntuaciones[i].puntos << std::endl;

    file.close();
    return !file.fail();
}

bool cargarPuntuaciones(tPuntuaciones &puntuaciones) {
    std::ifstream file(FILE_PUNTUACIONES);
    puntuaciones.cnt = 0;
    puntuaciones.puntuaciones = new tPuntuacion[INIT_PUNTUACIONES];
    puntuaciones.puntuacionesAlfa = new tPuntuacionPtr[INIT_PUNTUACIONES];
    puntuaciones.puntuacionesNum = new tPuntuacionPtr[INIT_PUNTUACIONES];
    puntuaciones.MAX = INIT_PUNTUACIONES;

    tPuntuacion puntuacion;
    while (file >> puntuacion.nombre >> puntuacion.puntos) {
        if (puntuaciones.cnt == puntuaciones.MAX) redimensionar(puntuaciones);
        // puntuaciones.puntuacionesAlfa[puntuaciones.cnt] = puntuaciones.puntuaciones + puntuaciones.cnt;
        puntuaciones.puntuacionesAlfa[puntuaciones.cnt] = &puntuaciones.puntuaciones[puntuaciones.cnt];
        puntuaciones.puntuacionesNum[puntuaciones.cnt] = &puntuaciones.puntuaciones[puntuaciones.cnt];
        puntuaciones.puntuaciones[puntuaciones.cnt++] = puntuacion;
    }

    quickSortPuntuaciones(puntuaciones.puntuacionesAlfa, 0, puntuaciones.cnt - 1, criterioAlpha);
    // quickSortPuntuaciones(puntuaciones.puntuacionesNum, 0, puntuaciones.cnt - 1, criterioNum);

    file.close();
    return !file.fail();
}

/*
int busquedaBin(const tPuntuacionPtr puntuaciones[], int bot, int top, std::string nombre) {
    int i = -1;
    if (top >= bot) {
        int midi = bot + (top - bot) / 2;
        if (puntuaciones[midi]->nombre == nombre)
            i = midi;
        else if (puntuaciones[midi]->nombre < nombre)  // Si es menor que el pivote, buscamos en la parte superior
            i = busquedaBin(puntuaciones, midi + bot, top, nombre);
        else
            i = busquedaBin(puntuaciones, bot, midi - bot, nombre);
    }

    return i;
}*/

bool actualizarPuntuacion(tPuntuaciones &puntuaciones, const std::string nombre, const unsigned add) {
    bool f = false;  // Buscamos el jugador
    int j = -1;
    // TODO: Implementar como busqueda binaria
    for (unsigned i = 0; i < puntuaciones.cnt && !f; i++) {
        if (puntuaciones.puntuacionesAlfa[i]->nombre == nombre) {
            f = true;
            j = i;
        }
    }
    // int i = busquedaBin(puntuaciones.puntuacionesAlfa, 0, puntuaciones.cnt - 1, nombre);
    if (j != -1) {
        // puntuaciones.puntuacionesAlfa[i]->puntos += add;
        puntuaciones.puntuacionesAlfa[j]->puntos = 5000;
        f = true;
    }

    // No lo encontramos, lo añadimos
    if (!f) {
        // Al ser una lista ordenada de mayor a menor, en caso de estar llena
        // La posicion MAX -1 sera la de menor puntuacion
        unsigned pos;
        if (puntuaciones.cnt == puntuaciones.MAX) {  // Si la lista está llena
            // if (puntuaciones.puntuaciones[MAX_PUNTUACIONES-1].puntos < add) // Si el menor es menor que el nuevo
            // pos = MAX_PUNTUACIONES - 1;
            redimensionar(puntuaciones);
        }
        pos = puntuaciones.cnt;
        puntuaciones.cnt++;
        puntuaciones.puntuaciones[pos].nombre = nombre;
        puntuaciones.puntuaciones[pos].puntos = add;
        puntuaciones.puntuacionesAlfa[pos] = &puntuaciones.puntuaciones[pos];
    }

    // Al ser una lista tan pequeña es mas facil ordenarla que crear un metodo
    // "actualizar" Y un metodo insertar
    // std::sort(puntuaciones.puntuaciones, puntuaciones.puntuaciones + puntuaciones.cnt, sorter);

    return f;
}

void redimensionar(tPuntuaciones &puntuaciones, const unsigned aumentar) {
    int max = puntuaciones.MAX + aumentar;

    tPuntuacion *aux = NULL;
    tPuntuacionPtr *auxPtrsAlfa = NULL;
    tPuntuacionPtr *auxPtrsNum = NULL;
    aux = new tPuntuacion[puntuaciones.MAX + aumentar];
    auxPtrsAlfa = new tPuntuacionPtr[puntuaciones.MAX + aumentar];
    auxPtrsNum = new tPuntuacionPtr[puntuaciones.MAX + aumentar];

    for (unsigned i = 0; i < puntuaciones.cnt; i++) {
        aux[i] = puntuaciones.puntuaciones[i];
        // auxPtrs[i] = puntuaciones.puntuacionesAlfa[i];
        auxPtrsAlfa[i] = puntuaciones.puntuacionesAlfa[i] - puntuaciones.puntuaciones + aux;
        auxPtrsNum[i] = puntuaciones.puntuacionesNum[i] - puntuaciones.puntuaciones + aux;
    }
    liberar(puntuaciones);
    puntuaciones.puntuaciones = aux;
    puntuaciones.puntuacionesAlfa = auxPtrsAlfa;
    puntuaciones.puntuacionesNum = auxPtrsNum;
    puntuaciones.MAX = max;
}

void liberar(tPuntuaciones &puntuaciones) {
    delete[] puntuaciones.puntuacionesNum;
    delete[] puntuaciones.puntuacionesAlfa;
    delete[] puntuaciones.puntuaciones;
    puntuaciones.MAX = 0;  // Básicamente para hacer el debug más sencillo
}
