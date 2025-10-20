
//bibleotecas a usar
#include <iostream>
#include <cstdlib>
#include <ctime>

//para evitar poner "std::"" tan frecuente
using namespace std;


// ESTRUCTURA CARTA
struct Carta {
    char palo;        // 'C' Corazones, 'E' Espadas, 'D' Diamantes, 'T' Tréboles
    int categoria;    // 1..13 (1=A, 11=J, 12=Q, 13=K)
    int valor;        // A=11, J/Q/K=10, resto = categoria
    bool entregada;   // true si ya fue entregada o jugada
};


// ABB (Árbol Binario de Búsqueda)
struct NodoABB {
    Carta carta;
    NodoABB* izq;
    NodoABB* der;
    NodoABB(const Carta &c): carta(c), izq(NULL), der(NULL) {}
};

class ABB {
private:
    NodoABB* raiz;
    int nElems;

    NodoABB* insertarRec(NodoABB* nodo, const Carta &c) {
        if (!nodo) { nElems++; return new NodoABB(c); }
        if (c.categoria < nodo->carta.categoria ||
           (c.categoria == nodo->carta.categoria && c.palo < nodo->carta.palo)) {
            nodo->izq = insertarRec(nodo->izq, c);
        } else {
            nodo->der = insertarRec(nodo->der, c);
        }
        return nodo;
    }

    void inordenRec(NodoABB* nodo, bool mostrarEntregadas) {
        if (!nodo) return;
        inordenRec(nodo->izq, mostrarEntregadas);
        cout << cartaStr(nodo->carta);
        if (mostrarEntregadas && nodo->carta.entregada) cout << " x";
        cout << "\n";
        inordenRec(nodo->der, mostrarEntregadas);
    }

    void limpiarRec(NodoABB* nodo) {
        if (!nodo) return;
        limpiarRec(nodo->izq);
        limpiarRec(nodo->der);
        delete nodo;
    }

    string cartaStr(const Carta &c) {
        string s;
        if (c.categoria == 1) s = "A";
        else if (c.categoria == 11) s = "J";
        else if (c.categoria == 12) s = "Q";
        else if (c.categoria == 13) s = "K";
        else s = to_string(c.categoria);
        s += "-";
        if (c.palo == 'C') s += "Cor";
        else if (c.palo == 'E') s += "Esp";
        else if (c.palo == 'D') s += "Diam";
        else s += "Tre";
        return s;
    }

public:
    ABB(): raiz(NULL), nElems(0) {}
    ~ABB() { limpiarRec(raiz); raiz = NULL; nElems = 0; }

    void insertar(const Carta &c) { raiz = insertarRec(raiz, c); }
    void inorden(bool mostrarEntregadas=false) { inordenRec(raiz, mostrarEntregadas); }
    int size() { return nElems; }
};


// LISTA ENLAZADA SIMPLE
struct NodoL {
    Carta carta;
    NodoL* sig;
    NodoL(const Carta &c): carta(c), sig(NULL) {}
};

class Lista {
private:
    NodoL* head;
    int len;
public:
    Lista(): head(NULL), len(0) {}
    ~Lista() { clear(); }

    void clear() {
        NodoL* cur = head;
        while (cur) { NodoL* t = cur->sig; delete cur; cur = t; }
        head = NULL; len = 0;
    }

    int length() { return len; }

    bool insertAt(int pos, const Carta &c) {
        if (pos < 0) return false;
        if (pos == 0) {
            NodoL* n = new NodoL(c);
            n->sig = head; head = n; len++; return true;
        }
        if (pos > len) pos = len;
        NodoL* cur = head;
        for (int i=0;i<pos-1 && cur;i++) cur = cur->sig;
        if (!cur && pos>0) return false;
        NodoL* n = new NodoL(c);
        n->sig = cur->sig;
        cur->sig = n;
        len++; return true;
    }

    bool append(const Carta &c) { return insertAt(len, c); }

    bool removeAt(int pos, Carta &out) {
        if (pos < 0 || pos >= len) return false;
        if (pos == 0) {
            NodoL* t = head; out = t->carta; head = t->sig; delete t; len--; return true;
        }
        NodoL* cur = head;
        for (int i=0;i<pos-1;i++) cur = cur->sig;
        NodoL* t = cur->sig; out = t->carta; cur->sig = t->sig; delete t; len--; return true;
    }

    bool getAt(int pos, Carta &out) {
        if (pos<0 || pos>=len) return false;
        NodoL* cur = head;
        for (int i=0;i<pos;i++) cur = cur->sig;
        out = cur->carta; return true;
    }

    // busca y elimina la primera carta que coincide por palo+categoria
    bool removeFirstMatch(const Carta &c) {
        NodoL* cur = head; NodoL* prev = NULL;
        while (cur) {
            if (cur->carta.categoria == c.categoria && cur->carta.palo == c.palo) {
                if (prev==NULL) { head = cur->sig; delete cur; }
                else { prev->sig = cur->sig; delete cur; }
                len--; return true;
            }
            prev = cur; cur = cur->sig;
        }
        return false;
    }

    void displayInline() {
        NodoL* cur = head;
        int idx = 0;
        while (cur) {
            cout << idx << ":" << cartaStr(cur->carta);
            if (cur->carta.entregada) cout << " x";
            cout << "  ";
            cur = cur->sig; idx++;
        }
        cout << "\n";
    }

    string cartaStr(const Carta &c) {
        string s;
        if (c.categoria == 1) s = "A";
        else if (c.categoria == 11) s = "J";
        else if (c.categoria == 12) s = "Q";
        else if (c.categoria == 13) s = "K";
        else s = to_string(c.categoria);
        s += "-";
        if (c.palo == 'C') s += "Cor";
        else if (c.palo == 'E') s += "Esp";
        else if (c.palo == 'D') s += "Diam";
        else s += "Tre";
        return s;
    }
};


// UTILIDADES (palos, valores, construir mazo)
char paloDeIndice(int i) {
    if (i==0) return 'C';
    if (i==1) return 'E';
    if (i==2) return 'D';
    return 'T';
}

int valorDeCategoria(int cat) {
    if (cat == 1) return 11;
    if (cat >= 11 && cat <= 13) return 10;
    return cat;
}

// construir mazo: generar 52 cartas y insertarlas en posiciones aleatorias dentro de la lista
void construirMazoAleatorio(Lista &mazo) {
    Carta todas[52];
    int k=0;
    for (int p=0;p<4;p++) {
        char palo = paloDeIndice(p);
        for (int c=1;c<=13;c++) {
            Carta cc; cc.palo = palo; cc.categoria = c; cc.valor = valorDeCategoria(c); cc.entregada = false;
            todas[k++] = cc;
        }
    }
    for (int i=0;i<52;i++) {
        int pos = (mazo.length() > 0) ? rand() % (mazo.length() + 1) : 0;
        mazo.insertAt(pos, todas[i]);
    }
}

// construir ABBs balanceados (insertar por medianas) para cada palo (opcional: sólo para mostrar M)
void buildArbolBalanceado(ABB &arbol, Carta arr[], int l, int r) {
    if (l>r) return;
    int m = (l + r) / 2;
    arbol.insertar(arr[m]);
    buildArbolBalanceado(arbol, arr, l, m-1);
    buildArbolBalanceado(arbol, arr, m+1, r);
}

void construirArbolesPorPalo(ABB palos[4]) {
    for (int p=0;p<4;p++) {
        Carta arr[13];
        for (int i=0;i<13;i++) { arr[i].palo = paloDeIndice(p); arr[i].categoria = i+1; arr[i].valor = valorDeCategoria(i+1); arr[i].entregada = false; }
        buildArbolBalanceado(palos[p], arr, 0, 12);
    }
}

// EVALUACIÓN DE MANOS y PUNTAJES
struct Resultado {
    int tipo;   // 1..7 (1=carta mas alta ... 7=poker)
    int puntos;
};

// ordenar selección para mostrar (selection sort), tratar As como alto para visual
void ordenarDescCategoriaMostrable(Carta arr[], int n) {
    for (int i=0;i<n;i++) {
        int sel = i;
        for (int j=i+1;j<n;j++) {
            int valj = (arr[j].categoria == 1) ? 14 : arr[j].categoria;
            int vali = (arr[sel].categoria == 1) ? 14 : arr[sel].categoria;
            if (valj > vali) sel = j;
        }
        if (sel != i) { Carta tmp = arr[i]; arr[i] = arr[sel]; arr[sel] = tmp; }
    }
}

// contar categorias
void contarCats(Carta arr[], int n, int counts[14]) {
    for (int i=0;i<14;i++) counts[i]=0;
    for (int i=0;i<n;i++) counts[arr[i].categoria]++;
}

bool esColor(Carta arr[], int n) {
    if (n==0) return false;
    char p = arr[0].palo;
    for (int i=1;i<n;i++) if (arr[i].palo != p) return false;
    return true;
}

// detectar escalera en 5 cartas (incluye A como 1 o 14)
bool esEscalera(Carta arr[], int n) {
    if (n < 5) return false;
    int cats[5];
    for (int i=0;i<5;i++) cats[i] = arr[i].categoria;
    // ordenar asc
    for (int i=0;i<5;i++) for (int j=i+1;j<5;j++) if (cats[i] > cats[j]) { int t=cats[i]; cats[i]=cats[j]; cats[j]=t; }
    bool consec = true;
    for (int i=1;i<5;i++) if (cats[i] != cats[i-1] + 1) consec = false;
    if (consec) return true;
    // A alta
    int adj[5];
    for (int i=0;i<5;i++) adj[i] = (cats[i]==1) ? 14 : cats[i];
    for (int i=0;i<5;i++) for (int j=i+1;j<5;j++) if (adj[i] > adj[j]) { int t=adj[i]; adj[i]=adj[j]; adj[j]=t; }
    consec = true;
    for (int i=1;i<5;i++) if (adj[i] != adj[i-1] + 1) consec = false;
    return consec;
}

Resultado evaluarMano(Carta seleccion[], int k) {
    Resultado res; res.tipo = 1; res.puntos = 50;
    if (k==0) { res.puntos = 0; return res; }
    int counts[14]; contarCats(seleccion, k, counts);
    int maxCount = 0;
    for (int i=1;i<=13;i++) if (counts[i] > maxCount) maxCount = counts[i];

    // Póker
    if (maxCount == 4) {
        res.tipo = 7; res.puntos = 400;
        for (int i=1;i<=13;i++) if (counts[i]==4) res.puntos += 4 * i;
        return res;
    }
    // Color (si todas las cartas jugadas son del mismo palo)
    if (esColor(seleccion, k)) {
        res.tipo = 6; res.puntos = 200;
        int s=0; for (int i=0;i<k;i++) s += seleccion[i].categoria;
        res.puntos += s; return res;
    }
    // Escalera (solo aplica claramente con 5 cartas; la espec dice "Existe una escalera de cinco cartas en la mano jugada")
    if (k >= 5 && esEscalera(seleccion, k)) {
        res.tipo = 5; res.puntos = 150;
        int s=0; for (int i=0;i<5;i++) s += seleccion[i].categoria;
        res.puntos += s; return res;
    }
    // Tercia
    if (maxCount == 3) {
        res.tipo = 4; res.puntos = 120;
        for (int i=1;i<=13;i++) if (counts[i]==3) res.puntos += 3 * i;
        return res;
    }
    // Doble Par
    int pares = 0;
    for (int i=1;i<=13;i++) if (counts[i]==2) pares++;
    if (pares >= 2) {
        res.tipo = 3; res.puntos = 80;
        int s=0; for (int i=1;i<=13;i++) if (counts[i]==2) s += 2*i;
        res.puntos += s; return res;
    }
    // Par
    if (pares == 1) {
        res.tipo = 2; res.puntos = 40;
        for (int i=1;i<=13;i++) if (counts[i]==2) res.puntos += 2*i;
        return res;
    }
    // Carta más alta
    int maxCat = 0;
    for (int i=0;i<k;i++) if (seleccion[i].categoria > maxCat) maxCat = seleccion[i].categoria;
    res.tipo = 1; res.puntos = 50 + maxCat;
    return res;
}

// MOSTRAR AYUDA
void mostrarAyuda() {
    cout << "Comandos:\n";
    cout << "  J k i1 i2 ... ik   -> Jugar k cartas (k<=5). Ej: J 5 0 1 2 3 4\n";
    cout << "  D k i1 i2 ... ik   -> Descartar k cartas. Ej: D 2 0 3\n";
    cout << "  M                   -> Mostrar todas las cartas (inorden por palo). Cartas entregadas tienen 'x'\n";
    cout << "  H                   -> Mostrar ayuda\n";
    cout << "  Q                   -> Salir\n";
}


// FUNCIÓN PRINCIPAL (Bucle de juego)
int main() {
    srand((unsigned)time(NULL));
    cout << "Balatro - Tarea 2 (versión completa en español)\n";

    // preparar estructuras
    ABB arboles[4];
    construirArbolesPorPalo(arboles); // para M (no afecta mazo real)
    Lista mazo;
    Lista mano;

    // rondas / ciegas
    int ciega = 0; // 0 chica, 1 grande, 2 jefe
    const int pozoVals[3] = {250, 350, 500};

    bool terminarJuego = false;

    while (!terminarJuego) {
        // comenzar ronda
        int pozo = pozoVals[ciega];
        cout << "--------------------------------------\n";
        cout << "Inicia ronda (ciega ";
        if (ciega==0) cout << "Pequeña"; else if (ciega==1) cout << "Grande"; else cout << "Jefe";
        cout << ") - Pozo a vencer: " << pozo << " fichas\n";

        // reconstruir mazo y mano
        mazo.clear(); mano.clear();
        construirMazoAleatorio(mazo);
        // repartir 8 cartas
        while (mano.length() < 8 && mazo.length() > 0) {
            Carta tmp; mazo.removeAt(0, tmp); tmp.entregada = true; mano.append(tmp);
        }

        int manosDisponibles = 4;
        int descartesDisponibles = 3;
        int puntosTotales = 0;

        mostrarAyuda();

        // bucle de la ronda: mientras haya manos por jugar y usuario no se rinda
        while (manosDisponibles > 0) {
            cout << "\nPuntaje total: " << puntosTotales << " / " << pozo << "\n";
            cout << "Manos disponibles: " << manosDisponibles << "   Descartes disponibles: " << descartesDisponibles << "\n";
            cout << "Cartas en mazo: " << mazo.length() << "/52\n";

            // mostrar mano ordenada para el usuario (izq -> der: mayor a menor, A a la izquierda)
            int n = mano.length();
            Carta copia[20];
            for (int i=0;i<n;i++) mano.getAt(i, copia[i]);
            ordenarDescCategoriaMostrable(copia, n);

            cout << "Mano (izq->der) indices 0.." << (n-1) << ":\n";
            for (int i=0;i<n;i++) {
                // mostrar como: idx:Categoria-Palo
                string s;
                if (copia[i].categoria == 1) s = "A";
                else if (copia[i].categoria == 11) s = "J";
                else if (copia[i].categoria == 12) s = "Q";
                else if (copia[i].categoria == 13) s = "K";
                else s = to_string(copia[i].categoria);
                s += "-";
                if (copia[i].palo == 'C') s += "Cor";
                else if (copia[i].palo == 'E') s += "Esp";
                else if (copia[i].palo == 'D') s += "Diam";
                else s += "Tre";
                cout << i << ":" << s << "  ";
            }
            cout << "\n";

            cout << "Ingrese comando: ";
            string cmd; if (!(cin >> cmd)) { terminarJuego = true; break; }

            if (cmd == "H" || cmd == "h") { mostrarAyuda(); continue; }
            if (cmd == "M" || cmd == "m") {
                cout << "=== Mostrar cartas (inorden por palo): ===\n";
                for (int p=0;p<4;p++) {
                    cout << "Palo " << paloDeIndice(p) << ":\n";
                    arboles[p].inorden(true); cout << "\n";
                }
                continue;
            }
            if (cmd == "Q" || cmd == "q") { cout << "Abandonando juego.\n"; terminarJuego = true; break; }

            if (cmd == "D" || cmd == "d" || cmd == "J" || cmd == "j") {
                int k; if (!(cin >> k)) { cout << "Entrada invalida.\n"; continue; }
                if (k < 1 || k > 5) { cout << "Solo puedes seleccionar entre 1 y 5 cartas.\n"; continue; }
                int indices[5];
                for (int i=0;i<k;i++) {
                    if (!(cin >> indices[i])) { cout << "Indices insuficientes.\n"; break; }
                }
                bool ok = true;
                for (int i=0;i<k;i++) if (indices[i] < 0 || indices[i] >= n) ok = false;
                if (!ok) { cout << "Indices invalidos.\n"; continue; }

                // construir selección a partir de copia ordenada (lo que vio el usuario)
                Carta seleccion[5];
                for (int i=0;i<k;i++) seleccion[i] = copia[indices[i]];

                if (cmd == "D" || cmd == "d") {
                    if (descartesDisponibles <= 0) { cout << "No te quedan descartes.\n"; continue; }
                    // eliminar de la mano las cartas seleccionadas buscando primera coincidencia (categoria+palo)
                    for (int i=0;i<k;i++) {
                        bool removed = mano.removeFirstMatch(seleccion[i]);
                        if (!removed) cout << "Aviso: carta para descartar no encontrada (posible duplicado ya eliminado)\n";
                    }
                    descartesDisponibles--;
                    // entregar cartas para volver a 8
                    while (mano.length() < 8 && mazo.length() > 0) {
                        Carta c; mazo.removeAt(0, c); c.entregada = true; mano.append(c);
                    }
                    // si ciega jefe -> efecto serpiente: entregar 3 cartas extra
                    if (ciega == 2) {
                        cout << "Efecto 'La serpiente' (ciega Jefe): se entregan 3 cartas extra.\n";
                        for (int r=0;r<3;r++) {
                            if (mazo.length()>0) { Carta c; mazo.removeAt(0,c); c.entregada = true; mano.append(c); }
                        }
                    }
                } else { // jugar
                    if (manosDisponibles <= 0) { cout << "No dispones de manos para jugar.\n"; continue; }
                    Resultado res = evaluarMano(seleccion, k);
                    cout << "Mano jugada: tipo " << res.tipo << " -> ganaste " << res.puntos << " fichas.\n";
                    puntosTotales += res.puntos;
                    // eliminar cartas jugadas de la mano
                    for (int i=0;i<k;i++) {
                        bool removed = mano.removeFirstMatch(seleccion[i]);
                        if (!removed) cout << "Aviso: carta para jugar no encontrada (posible duplicado ya eliminado)\n";
                    }
                    manosDisponibles--;
                    // reponer hasta 8
                    while (mano.length() < 8 && mazo.length() > 0) {
                        Carta c; mazo.removeAt(0, c); c.entregada = true; mano.append(c);
                    }
                    // serpiente solo en ciega jefe
                    if (ciega == 2) {
                        cout << "Efecto 'La serpiente' (ciega Jefe): se entregan 3 cartas extra.\n";
                        for (int r=0;r<3;r++) {
                            if (mazo.length()>0) { Carta c; mazo.removeAt(0,c); c.entregada = true; mano.append(c); }
                        }
                    }
                    // verificar si ganó la ronda
                    if (puntosTotales >= pozo) {
                        cout << "¡Has vencido el pozo! Puntos totales: " << puntosTotales << " >= " << pozo << "\n";
                        break;
                    }
                }

                cout << "Cartas en mazo: " << mazo.length() << "/52\n";
                continue;
            }

            cout << "Comando no reconocido.\n";
        } // fin bucle manos de la ronda

        if (terminarJuego) break;

        // evaluar fin de ronda
        if (puntosTotales >= pozo) {
            cout << "Ganaste la ronda. Avanzando a la siguiente ciega.\n";
            ciega++;
            if (ciega > 2) { cout << "Has completado todas las ciegas. ¡Juego terminado (victoria)! \n"; terminarJuego = true; break; }
            else { cout << "Comenzando siguiente ronda...\n"; continue; }
        } else {
            cout << "No lograste vencer el pozo en esta ronda. Juego terminado. Has perdido.\n";
            terminarJuego = true; break;
        }
    } // fin bucle rondas

    cout << "Gracias por jugar Balatro (versión Tarea 2). Hasta luego.\n";
    return 0;
}
