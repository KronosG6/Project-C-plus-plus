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
    auto start = std::chrono::steady_clock::now(); //Iniciar el contador de tiempo
    
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
    auto end = std::chrono::steady_clock::now(); // Finalizar contador de tiempo
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Tiempo de multiplicación de matrices secuencial: " << elapsed_seconds.count() << " segundos" << std::endl;

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
    auto start = std::chrono::steady_clock::now(); // Iniciar contador de tiempo
    
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
    
    auto end = std::chrono::steady_clock::now(); // Finalizar contador de tiempo
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Tiempo de multiplicación paralela: " << elapsed_seconds.count() << " segundos" << std::endl;
    
    return resultado;
}

int main() {
    srand(time(0)); // Inicializa la semilla para generar números aleatorios
    
    int filasA = 1000;
    int columnasA = 1000;
    int filasB = columnasA;
    int columnasB = 1000; 

    // Generar las matrices aleatorias
    auto matrizA = generarMatrizAleatoria(filasA, columnasA);
    auto matrizB = generarMatrizAleatoria(filasB, columnasB);
    
    /*
    std::cout << "Matriz A:" << std::endl;
    imprimirMatriz(matrizA);
    
    std::cout << "Matriz B:" << std::endl;
    imprimirMatriz(matrizB);
    */


    // Multiplicar las matrices de forma secuencial
    std::cout << "\nMultiplicación secuencial:" << std::endl;
    auto resultado_secuencial = multiplicarMatricesSecuencial(matrizA, matrizB);
    /*
    std::cout << "Resultado de la multiplicación:" << std::endl;
    imprimirMatriz(resultado_secuencial);
    */

    // Multiplicación paralela
    std::cout << "\nMultiplicación paralela:" << std::endl;
    auto resultado_paralelo = multiplicarMatricesParalela(matrizA, matrizB);
    /*
    std::cout << "Resultado de la multiplicación:" << std::endl;
    imprimirMatriz(resultado_paralelo);
    */

    return 0;
}
