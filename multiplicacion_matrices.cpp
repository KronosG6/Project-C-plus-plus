#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

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
std::vector<std::vector<int>> multiplicarMatricesSecuencial(const std::vector<std::vector<int>>& matrizA, const std::vector<std::vector<int>>& matrizB) {
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
std::vector<std::vector<int>> multiplicarMatricesParalela(const std::vector<std::vector<int>>& matrizA, const std::vector<std::vector<int>>& matrizB) {
    int filasA = matrizA.size();
    int columnasB = matrizB[0].size();
    std::vector<std::vector<int>> resultado(filasA, std::vector<int>(columnasB, 0));
    
    int numHilos = std::thread::hardware_concurrency();
    std::vector<std::thread> hilos;
    int filasPorHilo = filasA / numHilos;
    int filaInicial = 0;
    
    for (int i = 0; i < numHilos; ++i) {
        int filaFinal = (i == numHilos - 1) ? filasA : filaInicial + filasPorHilo;
        hilos.emplace_back(multiplicarPorcionMatrices, std::ref(matrizA), std::ref(matrizB), std::ref(resultado), filaInicial, filaFinal);
        filaInicial = filaFinal;
    }
    
    for (auto& hilo : hilos) {
        hilo.join();
    }
    
    return resultado;
}

int main() {
    srand(time(0)); // Inicializa la semilla para generar números aleatorios
    
    int filasA = 1000;
    int columnasA = 1000;
    int filasB = columnasA;
    int columnasB = 1000; 

    if (columnasA != filasB) {
        std::cout << "Para realizar una multiplicación de matrices, el nro de columnas de la matriz A debe ser igual al nro de columnas de la matriz B" << std::endl;
        return 0;
    }

    // Generar las matrices aleatorias
    auto matrizA = generarMatrizAleatoria(filasA, columnasA);
    auto matrizB = generarMatrizAleatoria(filasB, columnasB);
    
    /*
    std::cout << "Matriz A:" << std::endl;
    imprimirMatriz(matrizA);
    
    std::cout << "\nMatriz B:" << std::endl;
    imprimirMatriz(matrizB);
    */

    // Multiplicar las matrices de forma secuencial
    std::cout << "\nRealizando la multiplicación de forma secuencial..." << std::endl;
    auto start = std::chrono::steady_clock::now(); //Iniciar el contador de tiempo para secuencial
    auto resultado_secuencial = multiplicarMatricesSecuencial(matrizA, matrizB);
    auto end = std::chrono::steady_clock::now(); // Finalizar contador de tiempo para secuencial
    std::chrono::duration<double> elapsed_seconds = end - start;

    //std::cout << "\nResultado de la multiplicación secuencial:" << std::endl;
    //imprimirMatriz(resultado_secuencial);

    // Multiplicación paralela
    std::cout << "\nRealizando la multiplicación de forma paralela..." << std::endl;
    auto start2 = std::chrono::steady_clock::now(); // Iniciar contador de tiempo para paralela
    auto resultado_paralelo = multiplicarMatricesParalela(matrizA, matrizB);
    auto end2 = std::chrono::steady_clock::now(); // Finalizar contador de tiempo para paralela
    std::chrono::duration<double> elapsed_seconds2 = end2 - start2;

    //std::cout << "\nResultado de la multiplicación paralela:" << std::endl;
    //imprimirMatriz(resultado_paralelo);

    std::cout << "\nResultados:" << std::endl;
    std::cout << "Tiempo de multiplicación de matrices secuencial: " << elapsed_seconds.count() << " segundos" << std::endl;
    std::cout << "  Tiempo de multiplicación de matrices paralela: " << elapsed_seconds2.count() << " segundos" << std::endl;
    std::cout << "\nRendimiento: "<< elapsed_seconds/elapsed_seconds2 << std::endl;
    std::cout << " Eficiencia: "<< 100*(elapsed_seconds/elapsed_seconds2)/std::thread::hardware_concurrency()  << "%" << std::endl;

    return 0;
}
