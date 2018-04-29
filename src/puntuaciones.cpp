#include "puntuaciones.h"

// Funcion interna para sort()
bool sorter(const tPuntuacion &p1, const tPuntuacion &p2) { return p1.puntos > p2.puntos; }

template <class T>
void swap(T &p1, T &p2) {
    T aux = p1;
    p1 = p2;
    p2 = aux;
}

bool criterioAlpha(const tPuntuacionPtr &p1, const tPuntuacionPtr &p2) { return p1->nombre < p2->nombre; }

bool criterioNum(const tPuntuacion &p1, const tPuntuacion &p2) {
    return p1.puntos > p2.puntos || (p1.puntos == p2.puntos && p1.nombre < p2.nombre);
}

// Usado por quickSortPuntuaciones()
template <class T>
int qsPartition(T puntuaciones[], const unsigned bot, const unsigned top, bool (&criterio)(const T &, const T &)) {
    // Elegimos un pivote cualquiera
    T pivote = puntuaciones[top];
    unsigned i = bot;
    for (unsigned j = bot; j <= top - 1; j++) {
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
template <class T>
void quickSortPuntuaciones(T puntuaciones[], const int bot, const int top, bool (&criterio)(const T &, const T &)) {
    if (bot < top) {
        // Esta es la parte fácil de quickSort
        int parti = qsPartition(puntuaciones, bot, top, criterio);

        quickSortPuntuaciones(puntuaciones, bot, parti - 1, criterio);
        quickSortPuntuaciones(puntuaciones, parti + 1, top, criterio);
    }
}

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
    puntuaciones.MAX = INIT_PUNTUACIONES;

    tPuntuacion puntuacion;
    while (file >> puntuacion.nombre >> puntuacion.puntos) {
        if (puntuaciones.cnt == puntuaciones.MAX) redimensionar(puntuaciones);
        // puntuaciones.puntuacionesAlfa[puntuaciones.cnt] = puntuaciones.puntuaciones + puntuaciones.cnt;
        puntuaciones.puntuacionesAlfa[puntuaciones.cnt] = &puntuaciones.puntuaciones[puntuaciones.cnt];
        puntuaciones.puntuaciones[puntuaciones.cnt++] = puntuacion;
    }

    // std::sort(puntuaciones.puntuaciones, puntuaciones.puntuaciones + puntuaciones.cnt, sorter);
    quickSortPuntuaciones(puntuaciones.puntuaciones, 0, puntuaciones.cnt - 1, criterioNum);
    quickSortPuntuaciones(puntuaciones.puntuacionesAlfa, 0, puntuaciones.cnt - 1, criterioAlpha);

    file.close();
    return !file.fail();
}

bool actualizarPuntuacion(tPuntuaciones &puntuaciones, const std::string nombre, const unsigned add) {
    bool f = false;  // Buscamos el jugador
    // TODO: Implementar como busqueda binaria
    for (unsigned i = 0; i < puntuaciones.cnt && !f; i++) {
        if (puntuaciones.puntuaciones[i].nombre == nombre) {
            f = true;
            puntuaciones.puntuaciones[i].puntos += add;
        }
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
    quickSortPuntuaciones(puntuaciones.puntuaciones, 0, puntuaciones.cnt - 1, criterioNum);
    quickSortPuntuaciones(puntuaciones.puntuacionesAlfa, 0, puntuaciones.cnt - 1, criterioAlpha);
    return f;
}

void redimensionar(tPuntuaciones &puntuaciones, const unsigned aumentar) {
    int max = puntuaciones.MAX + aumentar;

    tPuntuacion *aux = NULL;
    tPuntuacionPtr *auxPtrs = NULL;
    aux = new tPuntuacion[puntuaciones.MAX + aumentar];
    auxPtrs = new tPuntuacionPtr[puntuaciones.MAX + aumentar];

    for (unsigned i = 0; i < puntuaciones.cnt; i++) {
        aux[i] = puntuaciones.puntuaciones[i];
        auxPtrs[i] = puntuaciones.puntuacionesAlfa[i];
    }
    liberar(puntuaciones);
    puntuaciones.puntuaciones = aux;
    puntuaciones.puntuacionesAlfa = auxPtrs;
    puntuaciones.MAX = max;
}

void liberar(tPuntuaciones &puntuaciones) {
    delete[] puntuaciones.puntuaciones;
    delete[] puntuaciones.puntuacionesAlfa;
    puntuaciones.MAX = 0;  // Básicamente para hacer el debug más sencillo
}
