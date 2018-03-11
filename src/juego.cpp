#include "juego.h"

#include "cli.h"
#include "mazo.h"

using namespace std;

bool calcularDir(unsigned & x, unsigned & y, const tDir dir){
    bool c = false;
    switch(dir){
    case NORTE:
        c = x > 0;
        if(c) x--;
        break;
    case SUR:
        c = x < MAX_FILAS-1;
        if(c) x++;
        break;
    case ESTE:
        c = y < MAX_FILAS-1;
        if(c) y++;
        break;
    case OESTE:
        c = y > 0;
        if(c) y--;
        break;
    }

    return c;
}

bool empujar(tJuego & juego, const unsigned x, const unsigned y, const tDir dir){
    unsigned x2 = x, y2 = y;
    bool vacia = false;
    if(calcularDir(x2, y2, dir)){
        vacia = juego.tablero[x2][y2].estado == VACIA;
        if(vacia){
            juego.tablero[x2][y2] = juego.tablero[x][y];
            juego.tablero[x][y].estado = VACIA;
            avanzar(juego);
        }
    }

    return vacia;
}

bool avanzar(tJuego & juego){
    bool joya;
    unsigned x2 = juego.jugadores[juego.turno].x;
    unsigned y2 = juego.jugadores[juego.turno].y;
    tTortuga tortuga = juego.tablero[x2][y2].tortuga;

    cout << tortuga.direccion << endl;

    if(!calcularDir(x2, y2, tortuga.direccion)){
        addMsg(juego.log, "No puedes moverte a esta casilla, está fuera del tablero");
    }else{
        switch(juego.tablero[x2][y2].estado){
        case HIELO:
        case MURO: addMsg(juego.log, "No puedes avanzar a esa casilla, hay un muro"); break;
        case TORTUGA: addMsg(juego.log, "No puedes ponerte encima de una tortuga"); break;
        case CAJA:
            if(!empujar(juego, x2, y2, tortuga.direccion))
                addMsg(juego.log, "No puedes empujar la caja");
            break;
        case JOYA:
            joya = true;
            juego.joya = juego.turno;
            /* no break */
        default:
            addMsg(juego.log, juego.jugadores[juego.turno].nombre + " se ha movido a " + to_string(x2) + "," + to_string(y2));
            juego.tablero[x2][y2].estado = TORTUGA;
            juego.tablero[x2][y2].tortuga = tortuga;
            juego.tablero[juego.jugadores[juego.turno].x][juego.jugadores[juego.turno].y].estado = VACIA;
            juego.jugadores[juego.turno].x = x2;
            juego.jugadores[juego.turno].y = y2;
        }
    }

    return joya;
}

void gira(tJuego & juego, const bool derecha){
    unsigned x = juego.jugadores[juego.turno].x, y = juego.jugadores[juego.turno].y;
    tDir direccion = juego.tablero[x][y].tortuga.direccion;
    if(derecha){
        // El +4 es para que no entre en el rango de los negativos
        // Va a seguir funcionando en los positivos pues n%4 = (n+4)%4
        direccion = static_cast<tDir>( (direccion+1+4)%4 );
    } else {
        direccion = static_cast<tDir>( (direccion-1+4)%4 );
    }
    juego.tablero[x][y].tortuga.direccion = direccion;
}

void disparo(tJuego & juego){
    unsigned x = juego.jugadores[juego.turno].x, y = juego.jugadores[juego.turno].y;
    tDir dir = juego.tablero[x][y].tortuga.direccion;
    unsigned tortugaId = juego.tablero[x][y].tortuga.numero;
    animateLaser(juego, dir, x, y, paleta[NUM_TIPOS_CASILLAS+tortugaId]);
    if(juego.tablero[x][y].estado == HIELO){
        juego.tablero[x][y].estado = VACIA;
        addMsg(juego.log, juego.jugadores[juego.turno].nombre + " ha destruido " + to_string(x) + "," + to_string(y));
    }else{
        addMsg(juego.log, juego.jugadores[juego.turno].nombre + " ha disparado, pero ha fallado");
    }
}

bool accionSecuencia(tJuego & juego, tMano & cartas){
    bool joya = false;
    tJugador jugador = juego.jugadores[juego.turno];
    tecla::tTecla tecla = tecla::NADA;
    // cin.ignore();

    while(!joya && tecla != tecla::SALIR){
        tCarta carta = NADA;
        tecla = leerTecla();
        cout << tecla << endl; // TODO: DEBUG
        switch(tecla){
        case tecla::AVANZA:
            if(cartas[AVANZAR] > 0){carta = AVANZAR; joya = avanzar(juego);}
            else addMsg(juego.log, "No te quedan cartas de avanzar ( ^ )");
            break;
        case tecla::DERECHA:
            if(cartas[GIRODERECHA] > 0){carta = GIRODERECHA; gira(juego, 1);}
            else addMsg(juego.log, "No te quedan cartas de girar a la derecha ( > )");
            break;
        case tecla::IZQUIERDA:
            if(cartas[GIROIZQUIERDA]){carta = GIROIZQUIERDA; gira(juego, 0);}
            else addMsg(juego.log, "No te quedan cartas de girar a la izquierda ( < )");
            break;
        case tecla::DISPARO:
            if(cartas[LASER]){carta = LASER; disparo(juego);}
            else addMsg(juego.log, "No te quedan cartas de disparo ( ~ )");
            break;
        default: break;
        }
        if(carta != NADA){
            cartas[carta]--;
            insertar(jugador.mazo, carta);
        }
        mostrarJuego(juego);
    }

    // mostrarJuego(juego);


    return joya;
}

bool accionRobar(tJuego & juego){
    tCarta carta;
    bool success = sacar(juego.jugadores[juego.turno].mazo, carta);
    if(success){
        incluirCarta(juego.jugadores[juego.turno].mano, carta);
    }
    return success;
}

bool manoVacia(const tMano mano){
    // Retorna si una mano está vacía
    bool vacia = true;
    for(unsigned i = 0; i < NADA && vacia; i++){
        vacia = mano[i] == 0;
    }

    return vacia;
}

unsigned jugadoresJugando(const tJuego & juego){
    unsigned cnt = 0;
    for(unsigned i = 0; i < juego.nJugadores; i++)
        cnt += juego.jugadores[i].jugando?1:0;
    return cnt;
}

bool ejecutarTurno(tJuego & juego){
    bool joya;
    tecla::tTecla tecla;

    if(manoVacia(juego.jugadores[juego.turno].mano)){
        addMsg(juego.log, juego.jugadores[juego.turno].nombre + " no tienes cartas, tienes que robar");
        accionRobar(juego);
    }else{

        addMsg(juego.log, juego.jugadores[juego.turno].nombre + ", juegas o robas? (R/J)");
        mostrarJuego(juego);
        // cin.ignore();

        do{
            tecla = leerTecla();
        }while(tecla != tecla::ROBAR && tecla != tecla::JUGAR);

        juego.log[0] = juego.jugadores[juego.turno].nombre;
        if(tecla == tecla::ROBAR){
            if(accionRobar(juego)){
                juego.log[0] += " ha robado una carta";
            }else{
                juego.log[0] += " no le quedan cartas en el mazo, toca jugar las que tienes";
                tecla = tecla::ROBAR;
            }
        }
        if(tecla != tecla::ROBAR){
            juego.log[0] += " juega su turno";
            mostrarJuego(juego);
            joya = accionSecuencia(juego, juego.jugadores[juego.turno].mano);
        }
    }
    if(joya){
        addMsg(juego.log, juego.jugadores[juego.joya].nombre + " ha conseguido una joya");
        juego.jugadores[juego.joya].jugando = false;
    }else{
        addMsg(juego.log, "Fin del turno de " + juego.jugadores[juego.turno].nombre);
    }
    mostrarJuego(juego);

    return joya;
}

void cambiarTurno(tJuego & juego){
    do{
    juego.turno = (juego.turno+1)%juego.nJugadores;
    // Cuidado con lo siguiente, no llamar a la función cuando haya terminado el juego
    }while(!juego.jugadores[juego.turno].jugando);
}

void incluirCarta(tMano & mano, tCarta carta){
    mano[carta]++;
}

string pedirJugador(const unsigned i){
    string nombre;
    cout << "Introduzca el nombre del jugador " << i << " (sin espacios): ";
    cin >> nombre;
    cin.sync();
    return nombre;
}

bool cargarJuego(tJuego & juego){
    // s = success
    juego.nJugadores = pedirJugadores();
    // Leemos el tablero
    bool s = cargarTablero(juego.tablero, pedirFichero(), juego.nJugadores);
    // juego.turno = 0;

    if(!s){
        // Asignamos los jugadores buscando las tortugas por el tablero
        unsigned int i = 0;
        for(unsigned x = 0; x < MAX_FILAS && i < juego.nJugadores; x++){
            for(unsigned y = 0; y < MAX_FILAS && i < juego.nJugadores; y++){
                if(juego.tablero[x][y].estado == TORTUGA){
                    juego.jugadores[i].id = i;
                    juego.jugadores[i].x = x;
                    juego.jugadores[i].y = y;
                    juego.jugadores[i].jugando = true;
                    juego.jugadores[i]. nombre = pedirJugador(i+1);
                    // Ponemos la mano del jugador a 0
                    fill(juego.jugadores[i].mano, juego.jugadores[i].mano+NADA, 0);
                    // Creamos un mazo
                    crearMazoAleatorio(juego.jugadores[i].mazo);
                    i++;
                }
            }
        }
    }else{
        cout << "Error al cargar el fichero" << endl;
    }

    return s;
}

void jugar(){
    tJuego juego;
    bool salir = false;

    cargarJuego(juego);
    while(jugadoresJugando(juego) > 1 && !salir){
        cambiarTurno(juego);
        if(ejecutarTurno(juego)){
            salir = !continuar(juego);
        }
    }
}
