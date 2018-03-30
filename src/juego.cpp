#include "juego.h"

using std::string;

/* PROTOTIPOS DE LAS FUNCIONES INTERNAS */

// Movimientos auxiliares
bool empujar(tJuego &, const int, const int, const tDir);  // Empuja una caja

bool manoVacia(tMano mano) {
    bool vacia = true;
    for (unsigned i = 0; i < carta::NADA && vacia; i++) {
        vacia = mano[i] <= 0;
    }
    return vacia;
}

void cambiarTurno(tJuego& juego) {
  int i = 0;  // Evita que se entre en un bucle infinito
  do {
    juego.turno = (juego.turno + 1) % juego.nJugadores;
    if (i == juego.nJugadores)
      throw;  // Lanzamos una excepcion para no entrar en bucle
    i++;
  } while (!juego.jugadores[juego.turno].jugando);
}

bool cargarJuego(tJuego& juego, const string fname, const int nJugadores, const string nombres[]) {
  juego.nJugadores = nJugadores;
  bool success = cargarTablero(juego.tablero, fname, juego.nJugadores);

  if (success) {
    int id = 0;
    for (unsigned x = 0; x < MAX_FILAS && id < juego.nJugadores; x++) {
      for (unsigned y = 0; y < MAX_FILAS && id < juego.nJugadores; y++) {
        if (juego.tablero[x][y].estado == TORTUGA) {
          juego.jugadores[id].id = id;
          juego.jugadores[id].x = x;
          juego.jugadores[id].y = y;
          juego.jugadores[id].sx = x;
          juego.jugadores[id].sy = y;
          juego.jugadores[id].jugando = true;
          juego.jugadores[id].nombre = nombres[id];
          // Ponemos su mano a 0
          std::fill(juego.jugadores[id].mano, juego.jugadores[id].mano + carta::NADA, 0);
          // Y le creamos un mazo
          crearMazoAleatorio(juego.jugadores[id].mazo);
          id++;
        }
      }
    }
  }

  // TODO: Meter en interface.cpp el !s

  return success;
}

bool accionRobar(tJuego & juego) {
    carta::tCarta carta;
    bool posible = sacar(juego.jugadores[juego.turno].mazo, carta);
    if (posible) juego.jugadores[juego.turno].mano[carta]++;  // Incluimos la carta
    return posible;
}

bool empujar(tJuego & juego, const int x, const int y, const tDir dir) {
    int x2 = x, y2 = y;
    bool posible = calcularPos(x2, y2, dir) && juego.tablero[x2][y2].estado == VACIA;
    if (posible) {
        // Nota: No se pueden empujar dos cajas seguidas
        juego.tablero[x2][y2] = juego.tablero[x][y];
        juego.tablero[x][y].estado = VACIA;
    }

    return posible;
}

bool moverTortuga(tJuego & juego, const int x, const int y, const tTortuga & tortuga) {
    bool vacia = juego.tablero[x][y].estado == VACIA;
    if (vacia) {  // Solo puedes mover la tortuga a una casilla vacía
        tJugador & jugador = juego.jugadores[tortuga.numero];
        juego.tablero[jugador.x][jugador.y].estado = VACIA;
        juego.tablero[x][y].estado = TORTUGA;
        juego.tablero[x][y].tortuga = tortuga;
        jugador.x = x;
        jugador.y = y;
    }

    return vacia;
}

bool calcularPos(int & x, int & y, const tDir dir) {
    bool c = false;
    switch (dir) {
    case NORTE:
        c = x > 0;
        if (c) x--;
        break;
    case SUR:
        c = x < MAX_FILAS-1;
        if (c) x++;
        break;
    case ESTE:
        c = y < MAX_FILAS-1;
        if (c) y++;
        break;
    case OESTE:
        c = y > 0;
        if (c) y--;
        break;
    }

    return c;
}

/* FUNCIONES DEL 'LOG' */
void addMsg(string log[LOG_SIZE], string newMsg) {
    for (unsigned i = LOG_SIZE -1; i > 0; i--) log[i] = log[i-1];
    log[0] = newMsg;
}

/* FUNCIONES DE MOVIMIENTO, ETC */

bool avanzar(tJuego & juego) {
    bool joya = false;
    int tmpx = juego.jugadores[juego.turno].x;
    int tmpy = juego.jugadores[juego.turno].y;
    tTortuga & tortuga = juego.tablero[tmpx][tmpy].tortuga;

    if (calcularPos(tmpx, tmpy, tortuga.direccion)) {
        switch (juego.tablero[tmpx][tmpy].estado) {
        case HIELO:
        case MURO: addMsg(juego.log, "No puedes avanzar a esta casilla, hay un muro"); break;
        case TORTUGA: addMsg(juego.log, "No puedes ponerte encima de una tortuga"); break;
        case JOYA:
            joya = true;
            juego.joya = juego.turno;
            juego.jugadores[juego.turno].jugando = false;
            juego.tablero[tmpx][tmpy].estado = VACIA;  // Si no, "empujará" la joya
            /* no break */
        case CAJA:
            if (!empujar(juego, tmpx, tmpy, tortuga.direccion))
                addMsg(juego.log, "No puedes empujar la caja");
            /* no break */
        default:
            addMsg(juego.log, juego.jugadores[juego.turno].nombre + " se ha movido a " + std::to_string(tmpx) + ", " +
                    std::to_string(tmpy));
            moverTortuga(juego, tmpx, tmpy, tortuga);
            break;
        }
    }

    return joya;
}

void girar(tJuego & juego, const bool derecha) {
    tDir & direccion = juego.tablero[juego.jugadores[juego.turno].x][juego.jugadores[juego.turno].y].tortuga.direccion;
    // El +4 es para que no entre en el rango de los negativos
    // Va a seguir funcionando en los positivos pues n%4 = (n+4)%4
    if (derecha)
        direccion = static_cast<tDir>( (direccion +1 +4)%4);
    else
        direccion = static_cast<tDir>( (direccion -1 +4)%4);
}

bool disparar(tJuego & juego) {
    int x = juego.jugadores[juego.turno].x, y = juego.jugadores[juego.turno].y;
    tDir dir = juego.tablero[x][y].tortuga.direccion;
    tJugador & jugador = juego.jugadores[juego.turno];
    // Calculamos hasta donde va el laser
    while (calcularPos(x, y, dir) && juego.tablero[x][y].estado == VACIA) {}
    switch (juego.tablero[x][y].estado) {
    case HIELO:
        juego.tablero[x][y].estado = VACIA;
        addMsg(juego.log, jugador.nombre + " ha destruido " + std::to_string(x) + ", " +
                std::to_string(y));
        break;
    case JOYA:  // Ahora es cuando "rebota"
        // Movemos la tortuga a sx, sy, si es posible
        if (moverTortuga(juego, jugador.sx, jugador.sy, juego.tablero[jugador.x][jugador.y].tortuga))
            addMsg(juego.log, jugador.nombre + " ha disparado, ha rebotado, y ha vuelto al inicio");
        break;
    case TORTUGA:
    {
        tTortuga & tortugaDisparada = juego.tablero[x][y].tortuga;
        addMsg(juego.log, jugador.nombre + " ha disparado a " + juego.jugadores[tortugaDisparada.numero].nombre);
        // Las siguientes dos lineas no son realmente necesarias debido a animateLaser(), que modifica
        // directamente el tablero de juego
        while (calcularPos(x, y, dir) && juego.tablero[x][y].estado == VACIA ) {}
        moverTortuga(juego, x, y, tortugaDisparada);
        break;
    }
    default:
        addMsg(juego.log, juego.jugadores[juego.turno].nombre + " ha disparado, pero ha fallado"); break;
    }

    // TODO: Hacer la parte opcional de la practica
    return false;
}
