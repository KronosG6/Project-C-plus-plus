#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>

// Función para generar una matriz aleatoria de tamaño filas x columnas
std::vector<std::vector<int>> generarMatrizAleatoria(int filas, int columnas) {
    std::vector<std::vector<int>> matriz(filas, std::vector<int>(columnas));
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            matriz[i][j] = rand() % 100 + 1; // Genera números aleatorios entre 1 y 100
        }
    }
    return matriz;
}

// Función para imprimir una matriz
void imprimirMatriz(const std::vector<std::vector<int>>& matriz) {
    for (const auto& fila : matriz) {
        for (int elemento : fila) {
            std::cout << elemento << " ";
        }
        std::cout << std::endl;
    }
}

// Función para multiplicar dos matrices de manera secuencial
std::vector<std::vector<int>> multiplicarMatricesSecuencial(const std::vector<std::vector<int>>& matrizA, const std::vector<std::vector<int>>& matrizB,
                                                            std::chrono::duration<double>& tiempoSecuencial) {
    auto inicioSecuencial = std::chrono::steady_clock::now(); //Iniciar el contador de tiempo para secuencial
    
    int filasA = matrizA.size();
    int columnasA = matrizA[0].size();
    int columnasB = matrizB[0].size();
    std::vector<std::vector<int>> resultado(filasA, std::vector<int>(columnasB, 0));
    
    for (int i = 0; i < filasA; ++i) {
        for (int j = 0; j < columnasB; ++j) {
            for (int k = 0; k < columnasA; ++k) {
                resultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }

    auto finSecuencial = std::chrono::steady_clock::now(); // Finalizar contador de tiempo para secuencial
    tiempoSecuencial = finSecuencial - inicioSecuencial;
    
    return resultado;
}

// Función para multiplicar una porción de las matrices y guardar el resultado en la matriz de salida
void multiplicarPorcionMatrices(const std::vector<std::vector<int>>& matrizA, const std::vector<std::vector<int>>& matrizB,
                                std::vector<std::vector<int>>& resultado, int inicioFila, int finFila) {
    int columnasA = matrizA[0].size();
    int columnasB = matrizB[0].size();
    
    for (int i = inicioFila; i < finFila; ++i) {
        for (int j = 0; j < columnasB; ++j) {
            for (int k = 0; k < columnasA; ++k) {
                resultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }
}

// Función para multiplicar dos matrices utilizando hilos
std::vector<std::vector<int>> multiplicarMatricesParalela(const std::vector<std::vector<int>>& matrizA, const std::vector<std::vector<int>>& matrizB, 
                                                          std::chrono::duration<double>& tiempoParalelo, int numHilos) {
    auto inicioParalelo = std::chrono::steady_clock::now(); // Iniciar contador de tiempo para paralela
    
    int filasA = matrizA.size();
    int columnasB = matrizB[0].size();
    std::vector<std::vector<int>> resultado(filasA, std::vector<int>(columnasB, 0));
    
    std::vector<std::thread> hilos;
    int filasPorHilo = filasA / numHilos;
    int filaInicial = 0;
    std::cout << "\nNúmero de hilos: " << numHilos << std::endl;

    for (int i = 0; i < numHilos; ++i) {
        int filaFinal = (i == numHilos - 1) ? filasA : filaInicial + filasPorHilo;
        hilos.emplace_back(multiplicarPorcionMatrices, std::ref(matrizA), std::ref(matrizB), std::ref(resultado), filaInicial, filaFinal);
        filaInicial = filaFinal;
    }
    
    for (auto& hilo : hilos) {
        hilo.join();
    }
    
    auto finParalelo = std::chrono::steady_clock::now(); // Finalizar contador de tiempo para paralela
    tiempoParalelo = finParalelo - inicioParalelo;

    return resultado;
}

void resultadosComparacion(std::chrono::duration<double> tiempoSecuencial, std::chrono::duration<double> tiempoParalelo, int numHilos){
    std::cout << "\n\t\t\tResultados:\n" << std::endl;
    std::cout << "Tiempo de multiplicación de matrices secuencial: " << std::fixed << std::setprecision(4) << tiempoSecuencial.count() << " segundos" << std::endl;
    std::cout << "  Tiempo de multiplicación de matrices paralela: " << std::fixed << std::setprecision(4) << tiempoParalelo.count() << " segundos" << std::endl;
    std::cout << "\n   Speedup: "<< std::fixed << std::setprecision(4) << tiempoSecuencial/tiempoParalelo << std::endl;
    std::cout << "Eficiencia: "<< std::fixed << std::setprecision(4) << 100*(tiempoSecuencial/tiempoParalelo)/numHilos << "%" << std::endl;
}

//Funcion para mostrar el menu
void menu(){
    std::cout << "\t\t\tProyecto C++" << std::endl;
    std::cout << "\t\tMultiplicación de Matrices" << std::endl;
    std::cout << "\nElija una opción:\n" << std::endl;
    std::cout << "   1. Multiplicación de matrices 10x10" << std::endl;
    std::cout << "   2. Multiplicación de matrices 1000x1000" << std::endl;
    std::cout << "   3. Ingresar manualmente el tamaño de las matrices" << std::endl;
    std::cout << "   4. Cambiar número de hilos" << std::endl;
    std::cout << "   5. Salir" << std::endl;
    std::cout << "\nOpción → ";
}

//Funcion para la multiplicacion de matrices 10x10
void opcion1(int numHilos) {
    int filasA = 10;
    int columnasA = 10;
    int filasB = columnasA;
    int columnasB = 10;
    std::chrono::duration<double> tiempoSecuencial, tiempoParalelo;

    // Generar las matrices aleatorias
    auto matrizA = generarMatrizAleatoria(filasA, columnasA);
    auto matrizB = generarMatrizAleatoria(filasB, columnasB);
    
    //Mostrando matrices generadas
    std::cout << "Matriz A:" << std::endl;
    imprimirMatriz(matrizA);
    std::cout << "\nMatriz B:" << std::endl;
    imprimirMatriz(matrizB);

    // Multiplicar las matrices de forma secuencial
    std::cout << "\nRealizando la multiplicación de forma secuencial..." << std::endl;
    auto resultado_secuencial = multiplicarMatricesSecuencial(matrizA, matrizB, tiempoSecuencial);

    // Multiplicación paralela
    std::cout << "\nRealizando la multiplicación de forma paralela..." << std::endl;
    auto resultado_paralelo = multiplicarMatricesParalela(matrizA, matrizB, tiempoParalelo, numHilos);

    //Mostrando matriz resultado con ambas formas: Secuencial y Paralela
    std::cout << "\nResultado de la multiplicación secuencial:" << std::endl;
    imprimirMatriz(resultado_secuencial);
    std::cout << "\nResultado de la multiplicación paralela:" << std::endl;
    imprimirMatriz(resultado_paralelo);

    //Resultados
    resultadosComparacion(tiempoSecuencial, tiempoParalelo, numHilos);
}

//Funcion para la multiplicacion de matrices 1000x1000
void opcion2(int numHilos) {
    int filasA = 1000;
    int columnasA = 1000;
    int filasB = columnasA;
    int columnasB = 1000;
    std::chrono::duration<double> tiempoSecuencial, tiempoParalelo;

    // Generar las matrices aleatorias
    auto matrizA = generarMatrizAleatoria(filasA, columnasA);
    auto matrizB = generarMatrizAleatoria(filasB, columnasB);

    // Multiplicar las matrices de forma secuencial
    std::cout << "\nRealizando la multiplicación de forma secuencial..." << std::endl;
    auto resultado_secuencial = multiplicarMatricesSecuencial(matrizA, matrizB, tiempoSecuencial);

    // Multiplicación paralela
    std::cout << "\nRealizando la multiplicación de forma paralela..." << std::endl;
    auto resultado_paralelo = multiplicarMatricesParalela(matrizA, matrizB, tiempoParalelo, numHilos);

    //Resultados
    resultadosComparacion(tiempoSecuencial, tiempoParalelo, numHilos);
}

//Funcion para la multiplicacion de matrices con tamaño definido por el usuario
void opcion3(int numHilos) {
    int filasA, columnasA, filasB, columnasB;
    int mostrarMatrices;
    std::chrono::duration<double> tiempoSecuencial, tiempoParalelo;

    //Ingreso del tamaño de las matrices a multiplicar
    std::cout << "Ingrese el número de filas de la matriz A: ";
    std::cin >> filasA;
    std::cout << "Ingrese el número de columnas de la matriz A: ";
    std::cin >> columnasA;
    std::cout << "Ingrese el número de filas de la matriz B: ";
    std::cin >> filasB;
    std::cout << "Ingrese el número de columnas de la matriz B: ";
    std::cin >> columnasB;     

    //Condicion para que dos matrices puedan multiplicarse
    while (columnasA != filasB) {
        std::cout << "\nPara realizar una multiplicación de matrices, el nro de columnas de la matriz A debe ser igual al número de columnas de la matriz B" << std::endl;
        std::cout << "Número de columnas de la matriz A: " << columnasA << std::endl;
        std::cout << "Ingrese el número de filas de la matriz B: ";
        std::cin >> filasB;
    }

    // Generar las matrices aleatorias
    auto matrizA = generarMatrizAleatoria(filasA, columnasA);
    auto matrizB = generarMatrizAleatoria(filasB, columnasB);

    std::cout << "¿Mostrar matrices generadas? [0]Si [1]No → ";
    std::cin >> mostrarMatrices;

    if (mostrarMatrices == 0){
        std::cout << "Matriz A:" << std::endl;
        imprimirMatriz(matrizA);
        
        std::cout << "\nMatriz B:" << std::endl;
        imprimirMatriz(matrizB);
    }

    // Multiplicar las matrices de forma secuencial
    std::cout << "\nRealizando la multiplicación de forma secuencial..." << std::endl;
    auto resultado_secuencial = multiplicarMatricesSecuencial(matrizA, matrizB, tiempoSecuencial);

    // Multiplicación paralela
    std::cout << "\nRealizando la multiplicación de forma paralela..." << std::endl;
    auto resultado_paralelo = multiplicarMatricesParalela(matrizA, matrizB, tiempoParalelo, numHilos);

    std::cout << "\n¿Mostrar matriz del resultado? [0]Si [1]No → ";
    std::cin >> mostrarMatrices;

    if (mostrarMatrices == 0){
        std::cout << "\nResultado de la multiplicación secuencial:" << std::endl;
        imprimirMatriz(resultado_secuencial);
        std::cout << "\nResultado de la multiplicación paralela:" << std::endl;
        imprimirMatriz(resultado_paralelo);
    }

    //Resultados
    resultadosComparacion(tiempoSecuencial, tiempoParalelo, numHilos);
}

//Funcion cambiar número de hilos
int opcion4(int numHilos) {
    std::cout << "Cantidad de núcleos de procesamiento disponibles en la CPU del sistema: " << std::thread::hardware_concurrency() << std::endl;
    std::cout << "Número de hilos actual: " << numHilos << std::endl;
    
    do {
        std::cout << "\nIngrese el número de hilos que desea ( <= " << std::thread::hardware_concurrency() << " ): ";
        std::cin >> numHilos;
    } while (numHilos > static_cast<int>(std::thread::hardware_concurrency()) || numHilos < 2);

    return numHilos;
    
}

//Funcion para que no se cierre el programa
void esperar() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    srand(time(0)); // Inicializa la semilla para generar números aleatorios
    int numHilos = std::thread::hardware_concurrency();
    int opcion = 0;

    do {
        system("clear");

        menu();
        std::cin >> opcion;

        switch (opcion) {
            case 1: //Matrices 10x10
                system("clear");
                std::cout << "\t\tMultiplicación de Matrices 10x10\n" << std::endl;
                opcion2(numHilos);
                esperar();
                break;

            case 2: //Matrices 1000x1000
                system("clear");
                std::cout << "\t\tMultiplicación de Matrices 1000x1000\n" << std::endl;
                opcion3(numHilos);
                esperar();
                break;

            case 3: //Matrices no definidas
                system("clear");
                std::cout << "\t\tMultiplicación de Matrices de tamaño definidas por el usuario\n" << std::endl;
                opcion4(numHilos);
                esperar();
                break;

            case 4: //Cambiar número de hilos
                system("clear");
                std::cout << "\t\tCambiar número de hilos\n" << std::endl;
                numHilos = opcion4(numHilos);
                esperar();
                break;

            case 5:
                std::cout << "\n\t¡Hasta la proxima!" << std::endl;
                break;

            default:
                std::cout << "\n\tOpción Incorrecta" << std::endl;
                esperar();
                break;
        }

    } while (opcion != 4);

    return 0;
}
