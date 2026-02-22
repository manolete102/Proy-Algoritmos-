#include <iostream>
#include <string>

using namespace std;

// Insertion sort para ordenar un arreglo de enteros
// Lo usamos porque el arreglo siempre es de 4 elementos
// y para arreglos chicos insertion sort jala muy bien
void insertionSort(int arr[], int n){
    int i, j, aux;
    for(i = 1; i < n; i++){
        aux = arr[i];
        j = i - 1;
        // vamos moviendo los elementos mayores a la derecha
        while(j >= 0 && arr[j] > aux){
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = aux;
    }
}

// Separa un numero en sus 4 digitos y los guarda en el arreglo
// Ejemplo: 3201 -> {3, 2, 0, 1}
void separarDigitos(int num, int digitos[]){
    // sacamos cada digito de derecha a izquierda
    for(int i = 3; i >= 0; i--){
        digitos[i] = num % 10;
        num = num / 10;
    }
}

// Arma un numero entero a partir de un arreglo de digitos
// Ejemplo: {1, 2, 3, 4} -> 1234
int armarNumero(int digitos[], int n){
    int resultado = 0;
    for(int i = 0; i < n; i++){
        resultado = resultado * 10 + digitos[i];
    }
    return resultado;
}

// Hace UN paso del proceso de Kaprekar:
// 1) separa los digitos
// 2) los ordena para formar el mayor y el menor
// 3) resta mayor - menor
int pasoKaprekar(int num){
    int digitos[4];
    separarDigitos(num, digitos);

    // ordenamos de menor a mayor con insertion sort
    insertionSort(digitos, 4);

    // el numero menor es con los digitos ya ordenados (ascendente)
    int menor = armarNumero(digitos, 4);

    // para el mayor recorremos el arreglo al reves (descendente)
    int mayor = 0;
    for(int i = 3; i >= 0; i--){
        mayor = mayor * 10 + digitos[i];
    }

    return mayor - menor;
}

// Cuenta cuantas iteraciones necesita un numero para llegar a 6174
// Regresa -1 si nunca llega (caso de repdigits como 1111)
int contarIteraciones(int num){
    int iter = 0;
    while(num != 6174){
        num = pasoKaprekar(num);
        iter++;
        // si llega a 0 es que todos los digitos eran iguales
        if(num == 0){
            return -1;
        }
    }
    return iter;
}

// Revisa si todos los digitos de un numero de 4 cifras son iguales
// Ejemplo: 1111, 2222, 3333... esos nunca llegan a 6174
bool todosDigitosIguales(int num){
    int digitos[4];
    separarDigitos(num, digitos);
    // comparamos todos contra el primero
    if(digitos[0] == digitos[1] && digitos[1] == digitos[2] && digitos[2] == digitos[3]){
        return true;
    }
    return false;
}

// Valida la entrada del usuario
// Debe ser un entero de 4 cifras y no tener todos los digitos iguales
int pedirNumero(){
    int num;
    string entrada;

    while(true){
        cout << "Ingresa un numero entero de 4 cifras: ";
        getline(cin, entrada);

        // checamos que no este vacio y que todos sean digitos
        bool esNumero = true;
        if(entrada.empty()){
            esNumero = false;
        }
        for(int i = 0; i < (int)entrada.length() && esNumero; i++){
            if(entrada[i] < '0' || entrada[i] > '9'){
                esNumero = false;
            }
        }

        if(!esNumero){
            cout << "Error: solo se permiten numeros enteros." << endl;
            continue;
        }

        // convertimos a entero manualmente
        num = 0;
        for(int i = 0; i < (int)entrada.length(); i++){
            num = num * 10 + (entrada[i] - '0');
        }

        // validamos que sea de 4 cifras
        if(num < 1000 || num > 9999){
            cout << "Error: el numero debe ser de 4 cifras (1000 a 9999)." << endl;
            continue;
        }

        // validamos que no sean todos los digitos iguales
        if(todosDigitosIguales(num)){
            cout << "Error: los 4 digitos no pueden ser iguales, nunca llegaria a 6174." << endl;
            continue;
        }

        // si paso todas las validaciones, lo regresamos
        return num;
    }
}

int main(){
    cout << "=== Proceso de Kaprekar para 4 cifras ===" << endl;
    cout << endl;

    // --- Punto 1: iteraciones para un numero dado ---
    int numero = pedirNumero();
    int iteraciones = contarIteraciones(numero);

    cout << endl;
    cout << "El numero " << numero << " llega a 6174 en "
         << iteraciones << " iteraciones." << endl;

    // mostramos el proceso paso a paso para que se vea claro
    cout << "Proceso:" << endl;
    int actual = numero;
    int paso = 1;
    while(actual != 6174){
        int siguiente = pasoKaprekar(actual);
        cout << "  Paso " << paso << ": " << actual << " -> " << siguiente << endl;
        actual = siguiente;
        paso++;
    }

    cout << endl;

    // --- Punto 2: maximo de iteraciones entre todos los numeros de 4 cifras ---
    int maxIter = 0;
    int numConMax = 0;

    for(int i = 1000; i <= 9999; i++){
        // nos saltamos los que tienen todos los digitos iguales
        if(todosDigitosIguales(i)){
            continue;
        }
        int it = contarIteraciones(i);
        if(it > maxIter){
            maxIter = it;
            numConMax = i;
        }
    }

    cout << "Maximo de iteraciones entre todos los numeros de 4 cifras: "
         << maxIter << endl;
    cout << "Ejemplo de numero que requiere " << maxIter
         << " iteraciones: " << numConMax << endl;

    return 0;
}

/*
    === Analisis de complejidad ===

    COMPLEJIDAD TEMPORAL:

    - separarDigitos(): O(1), siempre recorre 4 posiciones.
    - insertionSort(): O(1), porque siempre ordena exactamente 4 elementos.
      En general insertion sort es O(n^2) pero aqui n=4, asi que es constante.
    - armarNumero(): O(1), recorre 4 digitos.
    - pasoKaprekar(): O(1), llama a las funciones de arriba que son todas O(1).
    - contarIteraciones(): O(1), porque esta demostrado que el proceso de Kaprekar
      para 4 cifras converge en maximo 7 iteraciones. Cada iteracion es O(1).
    - pedirNumero(): depende del usuario, pero cada validacion es O(1).
    - El ciclo del punto 2 recorre 9000 numeros (1000 a 9999), y para cada uno
      hace contarIteraciones que es O(1), entonces es O(9000) = O(1) ya que
      es una cantidad fija. Pero si lo pensamos en terminos generales para
      numeros de d cifras, seria O(9 * 10^(d-1)) numeros, cada uno con un
      proceso que converge en un numero acotado de pasos.

    En resumen: todo el programa corre en tiempo constante O(1) porque
    el rango de numeros y las iteraciones de Kaprekar estan acotados.

    COMPLEJIDAD ESPACIAL:

    - Solo usamos arreglos de 4 enteros y unas cuantas variables auxiliares.
    - No usamos memoria dinamica ni estructuras que crezcan.
    - Complejidad de espacio: O(1).
*/
