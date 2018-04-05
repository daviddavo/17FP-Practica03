#include "juego.h"

using std::string;

/* PROTOTIPOS DE LAS FUNCIONES INTERNAS */

// Movimientos auxiliares
bool empujar(tJuego &, const tCoord &, const tDir);  // Empuja una caja
void crearHielo(tJuego &, const tCoord &);           // Se encarga de crear un muro si es posible

void vaciarMano(tMano mano) {
    for (unsigned i = 0; i < carta::NADA; i++) mano[i] = 0;
}

bool manoVacia(tMano mano) {
    bool vacia = true;
    for (unsigned i = 0; i < carta::NADA && vacia; i++) {
        vacia = mano[i] <= 0;
    }
    return vacia;
}

void cambiarTurno(tJuego &juego) {
    int i = 0;  // Evita que se entre en un bucle infinito
    do {
        juego.turno = (juego.turno + 1) % juego.nJugadores;
        if (i == juego.nJugadores) throw;  // Lanzamos una excepcion para no entrar en bucle
        i++;
    } while (!juego.jugadores[juego.turno].jugando);
}

bool cargarJuego(tJuego &juego, const string fname, const int nJugadores, const string nombres[]) {
    juego.nJugadores = nJugadores;
    bool success = cargarTablero(juego.tablero, fname, juego.nJugadores);

    if (success) {
        int id = 0;
        for (int x = 0; x < MAX_FILAS && id < juego.nJugadores; x++) {
            for (int y = 0; y < MAX_FILAS && id < juego.nJugadores; y++) {
                if (juego.tablero[x][y].estado == TORTUGA) {
                	tJugador & jugador = juego.jugadores[id];
                    jugador.id = id;
                    tCoord coord = {x, y};
                    jugador.pos = coord;
                    jugador.spawn = coord;
                    jugador.jugando = true;
                    jugador.nombre = nombres[id];
                    // Ponemos su mano a 0
                    vaciarMano(juego.jugadores[id].mano);
                    // Y le creamos un mazo
                    crearMazoAleatorio(juego.jugadores[id].mazo);

                    // Sacamos 3 cartas
                    for (unsigned i = 0; i < CARTAS_INICIALES; i++) {
						carta::tCarta carta;
						sacar(jugador.mazo, carta);
						jugador.mano[carta]++;
					}
                    id++;
                }
            }
        }
    }

    // TODO: Meter en interface.cpp el !s

    return success;
}

bool accionRobar(tJuego &juego) {
    carta::tCarta carta;
    tJugador &jugador = juego.jugadores[juego.turno];
    bool posible = sacar(jugador.mazo, carta);
    if (posible) {
        jugador.mano[carta]++;  // Incluimos la carta
        jugador.ultimaJugada.jugada = ROBAR;
        jugador.ultimaJugada.cartaRobada = carta;
    }
    return posible;
}

bool empujar(tJuego &juego, const tCoord &coord, const tDir dir) {
    tCoord tmpCoord = coord;
    bool posible = calcularPos(tmpCoord, dir) && juego.tablero[tmpCoord.x][tmpCoord.y].estado == VACIA;
    if (posible) {
        // Nota: No se pueden empujar dos cajas seguidas
        juego.tablero[tmpCoord.x][tmpCoord.y] = juego.tablero[coord.x][coord.y];
        juego.tablero[coord.x][coord.y].estado = VACIA;
    }

    return posible;
}

bool moverTortuga(tJuego &juego, const tCoord &coord, const tTortuga &tortuga) {
    bool vacia = juego.tablero[coord.x][coord.y].estado == VACIA;
    if (vacia) {  // Solo puedes mover la tortuga a una casilla vacía
        tJugador &jugador = juego.jugadores[tortuga.numero];
        juego.tablero[jugador.pos.x][jugador.pos.y].estado = VACIA;
        juego.tablero[coord.x][coord.y].estado = TORTUGA;
        juego.tablero[coord.x][coord.y].tortuga = tortuga;
        jugador.pos = coord;
    }

    return vacia;
}

bool calcularPos(tCoord &coord, const tDir dir) {
    bool c = false;
    switch (dir) {
        case NORTE:
            c = coord.x > 0;
            if (c) coord.x--;
            break;
        case SUR:
            c = coord.x < MAX_FILAS - 1;
            if (c) coord.x++;
            break;
        case ESTE:
            c = coord.y < MAX_FILAS - 1;
            if (c) coord.y++;
            break;
        case OESTE:
            c = coord.y > 0;
            if (c) coord.y--;
            break;
    }

    return c;
}

/* FUNCIONES DEL 'LOG' */
void addMsg(string log[LOG_SIZE], string newMsg) {
    for (unsigned i = LOG_SIZE - 1; i > 0; i--) log[i] = log[i - 1];
    log[0] = newMsg;
}

/* FUNCIONES DE MOVIMIENTO, ETC */

bool avanzar(tJuego &juego) {
    bool joya = false;
    tCoord tmpCoor = juego.jugadores[juego.turno].pos;
    tTortuga &tortuga = juego.tablero[tmpCoor.x][tmpCoor.y].tortuga;

    if (calcularPos(tmpCoor, tortuga.direccion)) {
        switch (juego.tablero[tmpCoor.x][tmpCoor.y].estado) {
            case HIELO:
            case MURO:
                addMsg(juego.log, "No puedes avanzar a esta casilla, hay un muro");
                break;
            case TORTUGA:
                addMsg(juego.log, "No puedes ponerte encima de una tortuga");
                break;
            case JOYA:
                joya = true;
                juego.joya = juego.turno;
                juego.jugadores[juego.turno].jugando = false;
                juego.tablero[tmpCoor.x][tmpCoor.y].estado = VACIA;  // Si no, "empujará" la joya
                /* no break */
            case CAJA:
                if (!empujar(juego, tmpCoor, tortuga.direccion)) addMsg(juego.log, "No puedes empujar la caja");
                /* no break */
            default:
                addMsg(juego.log, juego.jugadores[juego.turno].nombre + " se ha movido a " + std::to_string(tmpCoor.x) +
                                      ", " + std::to_string(tmpCoor.y));
                moverTortuga(juego, tmpCoor, tortuga);
                break;
        }
    }

    return joya;
}

void girar(tJuego &juego, const bool derecha) {
    tDir &direccion =
        juego.tablero[juego.jugadores[juego.turno].pos.x][juego.jugadores[juego.turno].pos.y].tortuga.direccion;
    // El +4 es para que no entre en el rango de los negativos
    // Va a seguir funcionando en los positivos pues n%4 = (n+4)%4
    if (derecha)
        direccion = static_cast<tDir>((direccion + 1 + 4) % 4);
    else
        direccion = static_cast<tDir>((direccion - 1 + 4) % 4);
}

bool disparar(tJuego &juego) {
    tCoord coord = juego.jugadores[juego.turno].pos;  // Posición a calcular
    tDir dir = juego.tablero[coord.x][coord.y].tortuga.direccion;
    tJugador &jugador = juego.jugadores[juego.turno];
    bool joya = false;
    // Calculamos hasta donde va el laser
    while (calcularPos(coord, dir) && juego.tablero[coord.x][coord.y].estado == VACIA) {
    }
    switch (juego.tablero[coord.x][coord.y].estado) {
        case HIELO:
            juego.tablero[coord.x][coord.y].estado = VACIA;
            jugador.ultimaJugada.bloquesDerretidos[jugador.ultimaJugada.nBloquesDerretidos++] = coord;
            addMsg(juego.log,
                   jugador.nombre + " ha destruido " + std::to_string(coord.x) + ", " + std::to_string(coord.y));
            break;
        case JOYA:  // Ahora es cuando "rebota"
            // Movemos la tortuga a sx, sy, si es posible
            if (moverTortuga(juego, jugador.spawn, juego.tablero[jugador.pos.x][jugador.pos.y].tortuga))
                addMsg(juego.log, jugador.nombre + " ha disparado, ha rebotado, y ha vuelto al inicio");
            break;
        case TORTUGA: {
            tTortuga &tortugaDisparada = juego.tablero[coord.x][coord.y].tortuga;
            // Si disparamos a una tortuga que ya no está jugando, no hacemos nada
            if (juego.jugadores[tortugaDisparada.numero].jugando) {
				addMsg(juego.log, jugador.nombre + " ha disparado a " + juego.jugadores[tortugaDisparada.numero].nombre);
				// Las siguientes dos lineas no son realmente necesarias debido a animateLaser(), que modifica
				// directamente el tablero de juego
				while (calcularPos(coord, dir) && juego.tablero[coord.x][coord.y].estado == VACIA) {
				}
				if (juego.tablero[coord.x][coord.y].estado == JOYA) {
					juego.joya = tortugaDisparada.numero;
					joya = true;
					juego.tablero[coord.x][coord.y].estado = VACIA;
				}
				moverTortuga(juego, coord, tortugaDisparada);
            }

            break;
        }
        default:
            addMsg(juego.log, juego.jugadores[juego.turno].nombre + " ha disparado, pero ha fallado");
            break;
    }

    return joya;
}

void crearHielo(tJuego &juego, const tCoord &coord) {}

void ejecutarBicho(tJuego &juego, int jug) {
    tJugador &jugador = juego.jugadores[jug];
    tUltimaJugada &ultima = jugador.ultimaJugada;

    if (ultima.jugada == ROBAR) {
        // Si esa carta sigue en su mano, se la quitamos
        if (jugador.mano[ultima.cartaRobada] > 0) {
            jugador.mano[ultima.cartaRobada]--;                // Se la quitamos de la mano
            insertar(jugador.mazo, ultima.cartaRobada, true);  // Insertamos por arriba la ultima carta jugada
        }
    } else if (ultima.jugada == SECUENCIA) {
        // moverTortuga ya se encarga de comprobar si está vacía
        tTortuga &tortuga = juego.tablero[jugador.pos.x][jugador.pos.y].tortuga;
        // Movemos la tortuga a su estado anterior
        tortuga.direccion = ultima.prevDir;
        moverTortuga(juego, ultima.prevCoord, tortuga);
        // Restauramos los muros de hielo necesarios
        for (unsigned i = 0; i < ultima.nBloquesDerretidos; i++) {
            tCoord &coord = ultima.bloquesDerretidos[i];
            if (juego.tablero[coord.x][coord.y].estado == VACIA) juego.tablero[coord.x][coord.y].estado = HIELO;
        }

        // Y ahora sólo falta meter las cartas usadas de nuevo a la mano
        // y sacarlas del mazo
        carta::tCarta carta;
        for (unsigned i = 0; i < ultima.nCartasUsadas; i++) {
            // Entendemos que sacar() devolverá true, pues nunca vamos a sacar más cartas
            // de las que hay en el mazo
            sacar(jugador.mazo, carta, false);  // Sacamos una carta de abajo
            jugador.mano[carta]++;              // Y la añadimos a la mano
        }
    }

    ultima.jugada = BICHO;  // No puede recibir 2 bichos en una misma 'ronda'
}
