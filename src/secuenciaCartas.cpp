#include "secuenciaCartas.h"

void crearVacia(tMazo & mazo) {
	mazo.contador = 0;
}

bool sacar(tMazo & mazo, tCarta& carta) {
    // Sacamos la primera carta del mazo
	carta = mazo.arrayCarta[0];
	// Y movemos todas una a la izquierda (pila)
	for(unsigned i = 0; i < mazo.contador; i++){
	    mazo.arrayCarta[i] = mazo.arrayCarta[i+1];
	}
	// En c++ al usar un int n en una exp bool
	// te dará true si n > 0
	// Además, el operador -- tras la variable retorna el valor ANTES
	// de restarle uno
	// también uso el operador ternario https://msdn.microsoft.com/en-us/library/e4213hs1.aspx
	// Para que no haga contador-- cuando contador = 0 (Lo que petaría al ser unsigned)
	return mazo.contador ? mazo.contador-- +1:false;
}

bool insertar(tMazo & mazo, tCarta carta) {
    bool posible = mazo.contador < MAX_CARTAS;
    if(posible){
        mazo.arrayCarta[mazo.contador] = carta;
        mazo.contador += 1;
    }
    return posible;
}

void crearMazoOrdenado(tMazo & mazo){
    crearVacia(mazo);
    for (int i = 0;i < MAX_AVANZA;i++) {
        mazo.arrayCarta[mazo.contador] = AVANZAR;
        mazo.contador += 1;
    }
    for (int j = 0;j < MAX_GIRODERECHA;j++) {
        mazo.arrayCarta[mazo.contador] = GIRODERECHA;
        mazo.contador += 1;
    }
    for (int k = 0; k < MAX_GIROIZQUIERDA;k++) {
        mazo.arrayCarta[mazo.contador] = GIROIZQUIERDA;
        mazo.contador += 1;
    }
    for (int l = 0; l < MAX_PISTOLALASER;l++) {
        mazo.arrayCarta[mazo.contador] = LASER;
        mazo.contador += 1;

    }
}

void crearMazoAleatorio(tMazo & mazo) {
    crearMazoOrdenado(mazo);
	// random_shuffle(& mazo.arrayCarta[0])(& mazo.arrayCarta[MAX_CARTAS]);
	std::random_shuffle(mazo.arrayCarta, mazo.arrayCarta+MAX_CARTAS);
}
