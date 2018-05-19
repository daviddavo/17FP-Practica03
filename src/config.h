#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_
// Archivo para constantes y otras configuraciones

constexpr int CARTAS_INICIALES = 38;

/* TABLERO */
constexpr int MAX_FILAS = 8;
constexpr unsigned MAX_JUGADORES = 4;
constexpr unsigned NUM_TIPOS_CASILLAS = 7;  // Numero de tEstados distintos

/* PUNTUACIONES */
constexpr unsigned INIT_PUNTUACIONES = 8;  // Tamaño inicial del array de puntuaciones
const char FILE_PUNTUACIONES[] = "puntuaciones.txt";

/* INTERFAZ */
const bool MENU_GETCH = true;  // Si quieres que los menus vayan con getch o con entrada cin
const unsigned paleta[NUM_TIPOS_CASILLAS + MAX_JUGADORES] = {1, 11, 7, 4, 12, 5, 1, 13, 9, 10, 3};
const unsigned COLOR_TEXTO = 15;         // Blanco
const unsigned COLOR_CABECERA = 2;       // Verde
const unsigned COLOR_MANO = 1;           // Azul
const unsigned SCROLL_PUNTUACIONES = 8;  // Puntuaciones a mostrar en la pantalla de puntuaciones

const unsigned LOG_SIZE = 10;  // Numero de mensajes que almacenar

const unsigned MOVE_DELAY = 200;   // Tiempo en ms que dura cada "fotograma" de los movimientos
const unsigned LASER_DELAY = 100;  // Tiempo que tardara cada fotograma del laser
// de la animacion de movimiento

// Usando http://en.cppreference.com/w/cpp/language/string_literal
// ASCII ART de
// http://patorjk.com/software/taag/#p=testall&f=Graffiti&t=ROBOT%20TURTLES
// Nota: La terminal de Win2 por defecto solo caben 80 caracteres, asi que
// cuidadin
const unsigned CABECERA_LINEAS = 9;
const char CABECERA[] =
    R"HEAD( ____   ___  ____   ___  ______      ______ __ __ ____  ______ _       ___ _____
|    \ /   \|    \ /   \|      |    |      |  |  |    \|      | |     /  _] ___/
|  D  )     |  o  )     |      |    |      |  |  |  D  )      | |    /  [(   \_ 
|    /|  O  |     |  O  |_|  |_|    |_|  |_|  |  |    /|_|  |_| |___|    _]__  |
|    \|     |  O  |     | |  |        |  | |  :  |    \  |  | |     |   [_/  \ |
|  .  \     |     |     | |  |        |  | |     |  .  \ |  | |     |     \    |
|__|\_|\___/|_____|\___/  |__|        |__|  \__,_|__|\_| |__| |_____|_____|\___|
 David Davo & Victor Martinez

)HEAD";

#endif  // SRC_CONFIG_H_
