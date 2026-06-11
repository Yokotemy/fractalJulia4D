#ifndef FTESTER_TIMER_H
#define FTESTER_TIMER_H

#include <chrono>

class Timer {
private:
    // Zmienna przechowująca punkt startowy pomiaru na CPU
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;

public:
    // Konstruktor domyślny
    Timer() = default;

    // Rozpoczyna odliczanie czasu
    void start();

    // Kończy pomiar dla operacji wykonywanych na procesorze (CPU)
    // Zwraca czas w milisekundach (ms)
    double stopCPU();

    // Kończy pomiar dla operacji na GPU (CUDA)
    // Wymusza synchronizację z kartą graficzną przed zatrzymaniem zegara
    // Zwraca czas w milisekundach (ms)
    double stopGPU();
};

#endif // FTESTER_TIMER_H