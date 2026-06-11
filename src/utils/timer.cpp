#include "timer.h"
#include <cuda_runtime.h> // Wymagane dla cudaDeviceSynchronize

void Timer::start() {
    // Pobieramy aktualny, bardzo dokładny punkt w czasie
    m_startTime = std::chrono::high_resolution_clock::now();
}

double Timer::stopCPU() {
    auto endTime = std::chrono::high_resolution_clock::now();

    // Obliczamy różnicę i konwertujemy na milisekunde (double)
    std::chrono::duration<double, std::milli> elapsed = endTime - m_startTime;
    return elapsed.count();
}

double Timer::stopGPU() {
    // KLUCZOWE DLA CUDA:
    // Ponieważ wywołania kerneli (np. <<<blocks, threads>>>) w CUDA są asynchroniczne,
    // procesor rzuciłby zadanie na kartę i od razu poleciał dalej, mierząc np. 0.02 ms.
    // Ta funkcja blokuje wątek CPU i czeka, aż GPU fizycznie skończy liczyć cały fraktal.
    cudaDeviceSynchronize();

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = endTime - m_startTime;
    return elapsed.count();
}