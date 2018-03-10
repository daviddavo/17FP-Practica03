#include "cli.h"

using namespace std;

string pedirFichero(){
    string fname;

    cout << "Introduzca el nombre del mapa (con extension): ";
    cin >> fname;

    return fname;
}

unsigned pedirJugadores(){
    unsigned j;
    cout << "Introduce el numero de jugadores (1-" << MAX_JUGADORES<< "): ";
    cin >> j;
    while(j < 1 || j > MAX_FILAS){
        cout << "Introduce un numero del 1 al " << MAX_JUGADORES << ": ";
        cin >> j;
    }

    return j;
}

string carta2str(const tCarta carta){
    switch(carta){
    case AVANZAR: return "^";
    case GIROIZQUIERDA: return "<";
    case GIRODERECHA: return ">";
    case LASER: return "~";
    default: return " ";
    }
}

void mostrarJugador(const tJugador & jugador, const unsigned width, const bool jugando){
    // Le ponemos color a la tabla:
    colorFondo(paleta[NUM_TIPOS_CASILLAS+jugador.id-1]);
    // Calculamos el número de espacios necesarios para mostrar al jugador
    unsigned nSpaces = width - jugador.nombre.length() + 1;
    cout << (jugando?'>':' ') << ' ' << jugador.id << '.';
    // Mostramos los espacios
    for(unsigned i = 0; i < nSpaces; i++) cout << ' ';
    // Mostramos el nombre del jugador
    cout << jugador.nombre << ": ";
    colorReset();
    // Ahora mostramos la mano
    for(unsigned i = 0; i < NADA; i++){
        cout << ((jugador.mano[i] > 10)?" ":"  ") << jugador.mano[i];
        colorFondo(COLOR_MANO);
        cout << ' ' << carta2str(static_cast<tCarta>(i)) << ' ';
        colorReset();
    }

    cout << endl;
}

void mostrarJugadores(const tJugador jugadores[MAX_JUGADORES], const unsigned nJugadores, const unsigned turno){
    // Jugando es el ID del jugador que está jugando
    // Primero vamos a calcular el ancho necesario para mostrar a los jugadores
    unsigned maxw = 0;
    for(unsigned int i = 0; i < nJugadores; i++){
        // Esto hace lo mismo que max() pero para unsigned
        maxw = (maxw<jugadores[i].nombre.length())?jugadores[i].nombre.length():maxw;
    }
    for(unsigned int i = 0; i < nJugadores; i++){
        mostrarJugador(jugadores[i], maxw, jugadores[i].id == turno);
    }
}

void mostrarJuego(const tJuego & juego){
    clear();
    mostrarTablero(juego.tablero);
    cout << endl << "JUGADORES:" << endl;
    mostrarJugadores(juego.jugadores, juego.nJugadores, juego.turno);
}

#ifdef __linux__
// La siguiente función ha sido lo único obtenido de
// https://github.com/manuel-freire/LiberarFdI/blob/master/Fundamentos%20de%20la%20Programacion/colores/colores.h
int getch(){
   // from http://stackoverflow.com/a/23035044/15472
   struct termios oldattr, newattr;
   int ch;
   tcgetattr( STDIN_FILENO, &oldattr );
   newattr = oldattr;
   newattr.c_lflag &= ~( ICANON | ECHO );
   tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
   ch = getchar();
   tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
   return ch;
}
#endif

void anyKey(){
    cout << endl << "Pulse cualquier tecla para continuar..." << endl;
    getch();
}

tecla::tTecla leerTecla(){
    cin.sync();
    int dir = getch();
    if (dir == 0xe0){
        dir = getch();
    }

    switch(dir){
    case 'CR': return tecla::SALIR; // Enter
    case 72: return tecla::AVANZA;
    case 77: return tecla::DERECHA;
    case 75: return tecla::IZQUIERDA;
    case ' ': return tecla::DISPARO;
    default: return tecla::NADA;
    }
}
