#include "cli.h"

using namespace std;

void _sleep(unsigned milsec){
    // Usando time.h
    clock_t fin = clock() + milsec * CLOCKS_PER_SEC/1000;
    while(clock() < fin);
}

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

// La función de log es poder mostrar los últimos mensajes sin tener que perderlos,
void addMsg(tLog log, const string msg){
    for(unsigned i = LOG_SIZE -1; i > 0; i--){
        log[i] = log[i-1];
    }

    log[0] = msg;
}

void flushLog(tLog log){
    for(unsigned i = 0; i < LOG_SIZE; i++){
        log[i] = "";
    }
}

void mostrarJugador(const tJugador & jugador, const unsigned width, const bool jugando){
    // Le ponemos color a la tabla:
    colorFondo(paleta[NUM_TIPOS_CASILLAS+jugador.id]);
    // Calculamos el número de espacios necesarios para mostrar al jugador
    unsigned nSpaces = width - jugador.nombre.length() + 1;
    cout << (jugando?'>':' ') << ' ' << jugador.id+1 << '.';
    // Mostramos los espacios
    for(unsigned i = 0; i < nSpaces; i++) cout << ' ';
    // Mostramos el nombre del jugador
    cout << jugador.nombre << ": ";
    colorReset();
    // Ahora mostramos la mano
    for(unsigned i = 0; i < NADA; i++){
        cout << ((jugador.mano[i] > 9)?" ":"  ") << jugador.mano[i];
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

void cabecera(){
    colorTexto(COLOR_CABECERA);
    cout << CABECERA;
    colorReset();
}

void mostrarJuego(const tJuego & juego){
    clear();
    cabecera();
    mostrarBody(juego.tablero, juego.log);
    cout << endl << "JUGADORES:" << endl;
    mostrarJugadores(juego.jugadores, juego.nJugadores, juego.turno);
}

#ifdef __linux__
// La siguiente función ha sido lo único obtenido de
// https://github.com/manuel-freire/LiberarFdI/blob/master/Fundamentos%20de%20la%20Programacion/colores/colores.h
int _getch(){
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
    cout << "Pulse cualquier tecla para continuar..." << endl;
    _getch();
}

tecla::tTecla leerTecla(){
    cin.sync();
    int dir = _getch();
    cout << "Dir " << dir << endl; // TODO: DEBUG
    if (dir == 0xe0 || dir == 27){ // 27 == ESC a.k.a \ (Empieza una secuencia especial de la terminal (en Unix)
        // 91 = [
        dir = _getch();
        if(dir == '[') dir = _getch();
        cout << "Dir " << dir << endl; // TODO: DEBUG
    }

    switch(dir){
    case 10:
    case 13: return tecla::SALIR; // Enter
    case 65: // On linux, conflict w/ 'A'
    case 72: return tecla::AVANZA;
    case 67: // On linux, conflict w/ 'C'
    case 77: return tecla::DERECHA;
    case 68: // On linux, conflict w/ 'D'
    case 75: return tecla::IZQUIERDA;
    case 'j':
    case 'J': return tecla::JUGAR;
    case 'r':
    case 'R': return tecla::ROBAR;
    case ' ': return tecla::DISPARO;
    default: return tecla::NADA;
    }
}

void imprimirCasilla(const tCasilla casilla){
    // Por defecto color fondo es el de la paleta
    if(casilla.estado!=TORTUGA) colorFondo(paleta[casilla.estado]);

    switch(casilla.estado){
    case VACIA: cout << "  "; break;
    case HIELO: cout << "**"; break;
    case MURO: cout << "||"; break;
    case CAJA: cout << "[]"; break;
    // Sé que dijo 00, pero en la terminal quedan raro
    case JOYA: cout << "00"; break;
    case ANIMATION_LASER:
        colorTexto(casilla.tortuga.numero);
        if(casilla.tortuga.direccion == NORTE || casilla.tortuga.direccion == SUR) cout << "| ";
        else cout << "--";
        break;
    case TORTUGA:
        colorFondo(paleta[casilla.tortuga.numero+NUM_TIPOS_CASILLAS]);
        switch(casilla.tortuga.direccion){
        case NORTE: cout << "^^"; break;
        case SUR: cout << "vv"; break;
        case ESTE: cout << ">>"; break;
        case OESTE: cout << "<<"; break;
        }
    }

    colorReset();
}

bool animateLaser(tJuego & juego, const tDir direccion, unsigned & x, unsigned & y, const unsigned color){
    while(calcularDir(x,y,direccion) && juego.tablero[x][y].estado == VACIA){
        juego.tablero[x][y].estado = ANIMATION_LASER;
        juego.tablero[x][y].tortuga.numero = color;
        juego.tablero[x][y].tortuga.direccion = direccion;

        mostrarJuego(juego);
        juego.tablero[x][y].estado = VACIA;
        _sleep(LASER_DELAY);
    }

    return juego.tablero[x][y].estado != VACIA;
}

void mostrarBody(const tTablero tablero, const tLog log){
    string head = "";
    for(unsigned i = 0; i < MAX_FILAS*2; i++){
        head += " ";
    }
    head += " | MENSAJES:";
    // cout << head << endl; // No me gusta demasiado como queda

    for(unsigned i = 0; i < MAX_FILAS; i++){
        // Primero mostramos la linea del tablero
        for(unsigned j = 0; j < MAX_FILAS; j++){
            imprimirCasilla(tablero[i][j]);
        }
        // Solo se imprime el log si se puede
        cout << " | " << ((i<LOG_SIZE)?log[i]:"");

        cout << endl;
    }
}

bool continuar(const tPuntuaciones puntuaciones, const bool jugando){
    clear();
    cabecera();
    cout << endl;
    mostrarPuntuaciones(puntuaciones);
    cout << endl << endl;
    cout << "\t\t\t Pulse espacio para continuar" << endl;
    if(jugando) cout << "\t\t Cualquier otra tecla para salir del juego" << endl;
    return leerTecla() == tecla::DISPARO;
}

void gameOver(const tJuego &){
    clear();
    // TODO: Mostrar puntuaciones y hacerlo más bonito
    cout << "Gracias por jugar" << endl;
    anyKey();
}

void mainMenu(){
    int response;
    tPuntuaciones puntuaciones;
    cargarPuntuaciones(puntuaciones);
    clear();

    do{
        cabecera();
        cout << endl << endl << endl;
        cout << "\t\t 1. Jugar" << endl;
        cout << "\t\t 2. Mostrar puntuaciones" << endl << endl;
        cout << "\t\t 0. Salir" << endl << endl << endl << "\t\t";

        cin >> response;
        clear();
        if(response == 1){
            jugar(puntuaciones);
            clear();
        }else if(response == 2){
            cin.sync();
            while(!continuar(puntuaciones, false));
            clear();
        }else if(response != 0){
            cout << "Respuesta inválida";
        }
    }while(response != 0);
}

