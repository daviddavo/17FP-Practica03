#include "interface.h"

using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::string;

/* PROTOTIPOS FUNCIONES INTERNAS */

carta::tCarta tecla2carta(tecla::tTecla tecla);  // Definimos nuestro cast de tecla a carta

int pedirMenu(const int, const int, const bool);  // Pide un numero entre a y b (incluidos)
string pedirFichero();                            // Pide el nombre del fichero de donde cargar los tableros
unsigned pedirJugadores();                        // Pide el numero de jugadores
string pedirNombre(const int);                    // Pide el nombre del jugador i
bool continuar(const tPuntuaciones &);            // Cuando un jugador a conseguido una joya
tecla::tTecla leerTecla();                        // Lee una tecla de la secuencia y la convierte a tTecla
bool pedirSecuencia(tJuego &, tMazo &);           // Pide al usuario una secuencia, y la retorna en forma de mazo
int pedirJugBicho(tJuego &);  // Pide al jugador el numero de un jugador valido: Está jugando y no es él mismo

void mostrarCabecera();                                                           // Muestra el titulo del juego
void mostrarJuego(const tJuego &);                                                // Muestra el juego en su totalidad
void mostrarCasilla(const tCasilla &);                                            // Imprime la casilla
void mostrarJugador(const tJugador &, const unsigned, const bool);                // Imprime la linea del jugador
void mostrarJugadores(const tJugador[MAX_JUGADORES], const unsigned, const int);  // Muestra todos los jugadores
void mostrarCuerpo(const tTablero, const string[LOG_SIZE]);                       // Muestra el mapa y los mensajes
void animateLaser(tJuego &, const tDir, const tCoord &, const unsigned);          // Animacion del movimiento del laser

bool ejecutarSecuencia(tJuego &, tMazo &);        // Ejecuta una secuencia recibida en forma de mazo
bool ejecutarTurno(tJuego &);                     // Se encarga de procesar un turno
void ejecutarPartida(tJuego &, tPuntuaciones &);  // Se encarga de las funciones de una 'partida'
void jugar(tPuntuaciones &);                      // Jugar al juego
void mostrarPuntuaciones(const tPuntuaciones &);  // Mostrar las puntuaciones

// FUNCIONES DE FUNCIONALIDADES
#ifdef __linux__
// La siguiente funcion ha sido lo unico obtenido de
// https://github.com/manuel-freire/LiberarFdI/blob/master/Fundamentos%20de%20la%20Programacion/colores/colores.h
int _getch() {
    // from http://stackoverflow.com/a/23035044/15472
    struct termios oldattr, newattr;  // Almacena los "atributos" de la terminal
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);           // Obtenemos los atributos antiguos
    newattr = oldattr;                           // Pasan a ser los "nuevos"
    newattr.c_lflag &= ~(ICANON | ECHO);         // Desactivamos ICANON y ECHO
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);  // Ponemos los 'nuevos' atributos
    ch = getchar();                              // Obtenemos el caracter
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);  // Volvemos a activar ECHO

    // Los caracteres especiales en linux son en realidad tres caracteres
    // Siendo la flecha hacia arriba: 'ESC'[A, o derecha \[D, luego, mediante una
    // pequeña recursion convertiremos dichos caracteres de Linux a Win2
    if (ch == '\e') {
        ch = 0xe0;           // Decimos que es un "caracter especial"
    } else if (ch == '[') {  // Los caracteres especiales en Linux tienen esto
        ch = _getch();       // Si se trataba de un caracter especial, leemos
        switch (ch) {        // que caracter especial es y lo convertimos
            case 65:
                return 72;
                break;  // Arriba
            case 67:
                return 77;
                break;  // Derecha
            case 68:
                return 75;
                break;  // Izquierda
        }
    }
    return ch;
}
#endif

void anyKey(std::string texto = "Pulse cualquier tecla para continuar...") {
    // cin.ignore();  // Puesto que cin.sync y cin.clear no hacen nada...
    cout << texto << endl;
    if (_getch() == 0xe0) _getch();  // Por si se cuela una tecla especial
}

void __sleep(int ms) {  // Si la llamo sleep entra en conflicto
    // Usando time.h (libreria de C)
    // Como time_t funciona con segundos, vamos a usar clock_t
    clock_t fin = clock() + ms * CLOCKS_PER_SEC / 1000;
    while (clock() < fin) {
    }  // Esperamos que se agote el tiempo
    // Nota: Esto se podria optimizar mas con clock_t
}

// CONVERSION DE TIPOS
carta::tCarta tecla2carta(tecla::tTecla tecla) {
    switch (tecla) {
        case tecla::AVANZA:
            return carta::AVANZAR;
        case tecla::DERECHA:
            return carta::DERECHA;
        case tecla::DISPARO:
            return carta::LASER;
        case tecla::IZQUIERDA:
            return carta::IZQUIERDA;
        case tecla::BICHO:
            return carta::BICHO;
        default:
            return carta::NADA;
    }
}

// FUNCIONES DE PEDIR
int pedirMenu(const int a, const int b, const bool getch = MENU_GETCH) {
    // Recibe un intervalo y pide un numero entre a y b
    int n;
    if (getch) {
        n = _getch() - '0';  // Obtenemos el ASCII de la tecla y lo convertimos a int
        cout << endl;        // Esto solo funciona para menus 0-9
    } else {
        cin >> n;
    }
    while (n < a || n > b) {
        cout << "\tIntroduzca una opcion valida: ";
        if (getch) {
            n = _getch() - '0';
            cout << endl;
        } else {
            cin >> n;
        }
    }

    return n;
}

string pedirFichero() {
    string fname;
    cout << "Introduzca el fichero (con extension) del que cargar los mapas): ";
    while (!(cin >> fname) || !validfName(fname)) {
        colorTexto(0b0100);
        cout << "No se ha podrido abrir " << fname << ". Introduzca otro fichero: ";
        colorReset();
    }

    return fname;
}

unsigned pedirJugadores() {
    unsigned j;
    cout << "Introduce el numero de jugadores (1-" << MAX_JUGADORES << "): ";
    // con el !cin y el cin.clear() ya no peta al meter un char
    while (!(cin >> j) || (j < 1 || j > MAX_JUGADORES)) {
        cin.clear();
        cin.ignore();
        colorTexto(0b0100);
        cout << "Introduce un numero del 1 al " << MAX_JUGADORES << ": ";
        colorReset();
    }

    return j;
}

bool invalidName(std::string cadena) {
    // Comprueba si tiene caracteres especiales (!alnum) o empieza por un numero
    bool encontrado = !isalpha(cadena[0]);
    for (unsigned i = 1; i < cadena.length() && !encontrado; i++) {
        encontrado = !isalnum(cadena[i]);
    }

    return encontrado;
}

string pedirNombre(const int i) {
    string name;

    // TODO: Comprobar que no hay caracteres especiales ni cosas raras
    cout << "Introduzca el nombre del jugador " << i << ": ";
    while (!(cin >> name) || invalidName(name)) {  // !isalpha(name[0])
        colorTexto(0b0100);
        cout << "El nombre no puede empezar por un numero ni contener caracteres especiales" << endl;
        colorReset();
        cout << "Vuelva a introducir el nombre del jugador " << i << ": ";
    }

    return name;
}

bool continuar(const tPuntuaciones &puntuaciones) {
    clear();
    mostrarCabecera();
    cout << endl;
    mostrarPuntuaciones(puntuaciones);
    cout << endl << endl;
    cout << "\t\t\t Pulse espacio para continuar" << endl;
    cout << "\t\t Cualquier otra tecla para salir del juego" << endl;
    return leerTecla() == tecla::DISPARO;
}

tecla::tTecla leerTecla() {
    int ch = _getch();  //
    if (ch == 0xe0) ch = _getch();

    // Tambien funciona con WASD
    switch (ch) {
        case 10:  // En la tabla ASCII pueden confundirse \n y \r\n
        case 13:
            return tecla::SALIR;  // Enter
        case 72:
        case 'w':
            return tecla::AVANZA;  // Conflicto con 'H'
        case 77:
        case 'd':
            return tecla::DERECHA;
        case 75:
        case 'a':
            return tecla::IZQUIERDA;
        case 32:
            return tecla::DISPARO;
        case 'b':
            return tecla::BICHO;
        default:
            return tecla::NADA;
    }
}

bool pedirSecuencia(tJuego &juego, tMazo &tmpMazo) {
    bool posible = true;
    tMano tmpMano = {0};
    crearVacia(tmpMazo);

    tecla::tTecla tecla;
    carta::tCarta carta;
    addMsg(juego.log, "Introduzca una secuencia de cartas:");
    mostrarJuego(juego);  // Actualizamos el log
    do {
        tecla = leerTecla();
        if (tecla != tecla::NADA && tecla != tecla::SALIR) {
            carta = tecla2carta(tecla);
            cout << carta2str(carta) << " ";
            insertar(tmpMazo, carta);
            tmpMano[carta]++;
        }
    } while (tecla != tecla::SALIR && posible);

    // Miramos si las cartas necesarias estan en la mano del jugador
    for (unsigned i = 0; i < carta::NADA && posible; i++) {
        posible = tmpMano[i] <= juego.jugadores[juego.turno].mano[i];
    }

    return posible;
}

int pedirJugBicho(tJuego &juego) {
    int j;
    char c;
    bool salir = false;

    do {
        c = _getch();
        if (c == 0xe0) {
            c = 0;
            _getch();
        }
        j = c - '0' - 1;  // Convertimos de caracter a entero del 0 al 9, y este a índice (restamos '1')
        if (std::isdigit(c) && j >= 0 && j <= juego.nJugadores) {
            if (j == juego.turno) {
                juego.log[0] = "No puedes deshacer tu turno, elige otro jugador";
            } else if (!juego.jugadores[j].jugando) {
                juego.log[0] = juego.jugadores[j].nombre + " ya no esta jugando!, introduce otro";
            } else {
                juego.log[0] = juego.jugadores[juego.turno].nombre + " ha deshecho la ultima jugada de " +
                               juego.jugadores[j].nombre;
                salir = true;
            }
            mostrarJuego(juego);
        }
    } while (!salir);

    return j;
}

/* FUNCIONES QUE CREAN LA INTERFAZ */
// Cabecera -> Cuerpo -> Pie
void mostrarCabecera() {
    colorTexto(COLOR_CABECERA);
    cout << CABECERA;
    colorReset();
}

void mostrarCasilla(const tCasilla &casilla) {
    // Ponemos como color de fondo el de la paleta
    if (casilla.estado == TORTUGA)
        colorFondo(paleta[casilla.tortuga.numero + NUM_TIPOS_CASILLAS]);
    else
        colorFondo(paleta[casilla.estado]);

    switch (casilla.estado) {
        case VACIA:
            cout << "  ";
            break;
        case HIELO:
            cout << "**";
            break;
        case MURO:
            cout << "||";
            break;
        case CAJA:
            cout << "[]";
            break;
        case JOYA:
            cout << "00";
            break;
        case ANIMATION_LASER:
            colorTexto(casilla.tortuga.numero);
            if (casilla.tortuga.direccion == NORTE || casilla.tortuga.direccion == SUR)
                cout << "||";
            else
                cout << "==";
            break;
        case TORTUGA:
            switch (casilla.tortuga.direccion) {
                case NORTE:
                    cout << "^^";
                    break;
                case SUR:
                    cout << "vv";
                    break;
                case ESTE:
                    cout << ">>";
                    break;
                case OESTE:
                    cout << "<<";
                    break;
            }
    }
    colorReset();
}

void mostrarJugador(const tJugador &jugador, const unsigned maxw, const bool jugando) {
    colorFondo(paleta[NUM_TIPOS_CASILLAS + jugador.id]);
    unsigned nSpaces = maxw - jugador.nombre.length() + 1;          // Calculamos los espacios necesarios
    cout << (jugando ? '>' : ' ') << ' ' << jugador.id + 1 << '.';  // Imprimimos el nombre y el >
    for (unsigned i = 0; i < nSpaces; i++) cout << ' ';             // Mostramos los espacios para hacer mas limpio tood
    cout << jugador.nombre << ": ";                                 // Mostramos el nombre del jugador
    colorReset();                                                   // Ponemos el color normal
    // Y mostramos la mano del jugador
    for (unsigned i = 0; i < carta::NADA; i++) {
        cout << ((jugador.mano[i] > 9) ? " " : " 0") << jugador.mano[i];
        colorFondo(COLOR_MANO);
        cout << ' ' << carta2str(static_cast<carta::tCarta>(i)) << ' ';
        colorReset();
    }

    cout << endl;
}
void mostrarJugadores(const tJugador jugadores[MAX_JUGADORES], const unsigned nJugadores, const int turno) {
    // Primero vamos a calcular el ancho necesario para igualarlo
    unsigned maxw = 0;
    for (unsigned i = 0; i < nJugadores; i++)
        // Esto hace lo mismo que max pero para unsigned
        maxw = (maxw < jugadores[i].nombre.length()) ? jugadores[i].nombre.length() : maxw;
    // Y ahora los mostramos
    for (unsigned i = 0; i < nJugadores; i++) mostrarJugador(jugadores[i], maxw, jugadores[i].id == turno);
}

void mostrarCuerpo(const tTablero tablero, const string log[LOG_SIZE]) {
    for (unsigned i = 0; i < MAX_FILAS; i++) {
        for (unsigned j = 0; j < MAX_FILAS; j++) mostrarCasilla(tablero[i][j]);
        colorReset();
        // Solo imprime el log si es posible
        cout << " | " << ((i < LOG_SIZE) ? log[i] : "") << endl;
    }
}

void mostrarJuego(const tJuego &juego) {
    clear();
    mostrarCabecera();
    mostrarCuerpo(juego.tablero, juego.log);
    cout << endl << "JUGADORES:" << endl;
    mostrarJugadores(juego.jugadores, juego.nJugadores, juego.turno);
    // Movemos el cursor al final de la linea del 'log'
    move(CABECERA_LINEAS + 1, MAX_FILAS * 2 + 5 + juego.log[0].length());
}

void animateLaser(tJuego &juego, const tDir dir, const tCoord &source, const unsigned color) {
    tCoord coord = source;  // "Copiamos" para realizar las operaciones
    while (calcularPos(coord, dir) && juego.tablero[coord.x][coord.y].estado == VACIA) {
        juego.tablero[coord.x][coord.y].estado = ANIMATION_LASER;
        juego.tablero[coord.x][coord.y].tortuga.numero = color;
        juego.tablero[coord.x][coord.y].tortuga.direccion = dir;

        mostrarJuego(juego);
        juego.tablero[coord.x][coord.y].estado = VACIA;
        __sleep(LASER_DELAY);
    }
    // Ahora a hacer que rebote
    if (juego.tablero[coord.x][coord.y].estado == JOYA) {
        tDir inverso = static_cast<tDir>((dir + 2) % 4);  // Norte <-> Sur, Este <-> Oeste
        animateLaser(juego, inverso, coord, color);
    } else if (juego.tablero[coord.x][coord.y].estado == TORTUGA) {
        // Hacemos "dos veces" el calculo del movimiento, sacrificando unos pocos recursos para poder mantener
        // la interfaz independiente de los calculos del juego
        tTortuga &tortuga = juego.tablero[coord.x][coord.y].tortuga;
        while (calcularPos(coord, dir) && moverTortuga(juego, coord, tortuga)) {
            mostrarJuego(juego);
            __sleep(MOVE_DELAY);
        }
    }
}

bool ejecutarSecuencia(tJuego &juego, tMazo &secuencia) {
    bool joya = false;
    carta::tCarta paso;  // Cada uno de los pasos de la secuancia
    tJugador &jugador = juego.jugadores[juego.turno];

    jugador.ultimaJugada.prevCoord = jugador.pos;
    jugador.ultimaJugada.prevDir = juego.tablero[jugador.pos.x][jugador.pos.y].tortuga.direccion;
    jugador.ultimaJugada.jugada = SECUENCIA;
    jugador.ultimaJugada.nBloquesDerretidos = 0;
    jugador.ultimaJugada.nCartasUsadas = 0;

    while (sacar(secuencia, paso) && !joya) {
        insertar(jugador.mazo, paso);
        jugador.mano[paso]--;
        jugador.ultimaJugada.nCartasUsadas++;
        switch (paso) {
            case carta::AVANZAR:
                joya = avanzar(juego);
                break;
            case carta::DERECHA:
                girar(juego, 1);
                break;
            case carta::IZQUIERDA:
                girar(juego, 0);
                break;
            case carta::LASER: {
                tCoord coord = jugador.pos;
                unsigned color = paleta[juego.turno + NUM_TIPOS_CASILLAS];
                animateLaser(juego, juego.tablero[coord.x][coord.y].tortuga.direccion, coord, color);
                joya = disparar(juego);
                break;
            }
            case carta::NADA:
                cout << "Error?" << endl;
        }
        // TODO: Sleep
        __sleep(MOVE_DELAY);
        mostrarJuego(juego);
    }

    return joya;
}

bool ejecutarTurno(tJuego &juego) {
    // Asumimos que joya = false
    bool joya = false;
    tJugador &jugador = juego.jugadores[juego.turno];
    bool puedeUsarBicho = jugador.mano[carta::BICHO] > 0 && juego.nJugadores > 1;
    string robjug;
    char c;

    if (puedeUsarBicho)
        addMsg(juego.log, jugador.nombre + ", juegas, robas, ¿o usas  al bicho? (R/E/B)");
    else
        addMsg(juego.log, jugador.nombre + ", juegas o robas? (R/E)");

    mostrarJuego(juego);
    do {
        if (MENU_GETCH) {  // Implementamos ambas formas de obtener la jugada
            c = _getch();
            if (c == 0xe0) {
                c = 0;
                _getch();
            }
        } else {
            cin >> robjug;
            c = tolower(robjug[0]);
            cin.ignore();  // Si no leertecla no funcionara
        }
    } while (c != 'r' && c != 'e' && !(c == 'b' && puedeUsarBicho));

    juego.log[0] = jugador.nombre;
    if (manoVacia(jugador.mano)) {
        juego.log[0] += " no tienes cartas, tienes que robar";
        accionRobar(juego);
    } else if (c == 'r') {
        if (accionRobar(juego))
            juego.log[0] += " ha robado una carta";
        else
            juego.log[0] += ", no te quedan cartas en el mazo :(";
    } else if (c == 'e') {
        tMazo secuencia;

        juego.log[0] += " juega su turno";
        mostrarJuego(juego);
        if (pedirSecuencia(juego, secuencia)) {
            joya = ejecutarSecuencia(juego, secuencia);
        } else {
            juego.log[0] = jugador.nombre + " ha intentado una secuencia invalida";
            // TODO: ¿Mostrar la secuencia invalida?
        }
    } else if (c == 'b') {
        jugador.mano[carta::BICHO] = 0;
        juego.log[0] += " introduzca el número del jugador del que quieres deshacer su jugada:";
        mostrarJuego(juego);
        ejecutarBicho(juego, pedirJugBicho(juego));
        mostrarJuego(juego);
    }

    return joya;  // TODO: Retornar si un jugador ha conseguido una joya
}

// FUNCION INTERNA: Ejecuta una partida
void ejecutarPartida(tJuego &juego, tPuntuaciones &puntuaciones) {
    bool salir = false;
    int jugadoresJugando = juego.nJugadores;

    while (jugadoresJugando > 0 && !salir) {
        cambiarTurno(juego);
        if (ejecutarTurno(juego)) {
            actualizarPuntuacion(puntuaciones, juego.jugadores[juego.joya].nombre, jugadoresJugando);
            addMsg(juego.log, juego.jugadores[juego.joya].nombre + " ha ganado con " +
                                  std::to_string(jugadoresJugando) + "puntos");
            salir = !continuar(puntuaciones);
            jugadoresJugando--;
        }
    }
}

void jugar(tPuntuaciones &puntuaciones) {
    tJuego juego;
    // Pedimos el nombre del archivo
    string fname = pedirFichero();
    unsigned nJ = pedirJugadores();
    string jugadores[MAX_JUGADORES];  // MSVS no deja crear arrays de tamaño no fijo
    for (unsigned i = 0; i < nJ; i++) {
        jugadores[i] = pedirNombre(i + 1);
    }
    cin.ignore();  // Ignoramos el ultimo \n

    if (cargarJuego(juego, fname, nJ, jugadores)) {
        ejecutarPartida(juego, puntuaciones);
        guardarPuntuaciones(puntuaciones);

        // TODO gameOver(juego);
    } else {
        colorTexto(0b0100);
        cout << "El fichero existe, pero parece estar corrupto" << endl;
        colorReset();
        anyKey();
    }
}

void mostrarPuntuaciones(const tPuntuaciones &puntuaciones) {
    // Primero vamos a ver cual es el nombre que mas ocupa
    unsigned maxw = 0;
    for (unsigned i = 0; i < puntuaciones.cnt; i++) {
        if (puntuaciones.puntuaciones[i].nombre.length() > maxw) maxw = puntuaciones.puntuaciones[i].nombre.length();
    }

    // Y ahora mostramos las puntuaciones
    colorTexto(0b1100);  // Brillante rojo
    cout << "\t\t\tPuntuaciones:" << endl;
    colorReset();
    for (unsigned i = 0; i < puntuaciones.cnt; i++) {
        cout << "\t\t\t" << std::setw(maxw + 2) << puntuaciones.puntuaciones[i].nombre << std::right << std::setw(5)
             << puntuaciones.puntuaciones[i].puntos << endl;
    }
}

void mainMenu() {
    // Nada mas comenzar el juego, se cargan las puntuaciones
    tPuntuaciones puntuaciones;
    cargarPuntuaciones(puntuaciones);

    int n;
    string basura;

    do {
        clear();
        mostrarCabecera();
        cout << "\t\t\t 1. Jugar" << endl;
        cout << "\t\t\t 2. Mostrar puntuaciones" << endl;
        cout << "\t\t\t 3. Como jugar" << endl << endl;
        cout << "\t\t\t 0. Salir" << endl;
        cout << "\t\t\t ";
        n = pedirMenu(0, 3);
        // cin.get(); // Obtenemos el endl, pero lo descartamos
        // getline(cin, basura); // Por alguna razon cin.clear() no va bien :(
        if (n == 1) {
            jugar(puntuaciones);
        } else if (n == 2) {
            clear();
            mostrarCabecera();
            mostrarPuntuaciones(puntuaciones);
            cout << endl << endl << "\t\t";
            anyKey();
        } else if (n == 3) {
            // TODO: help()
            cout << "AUN SIN IMPLEMENTAR" << endl;
        }
    } while (n != 0);
}
