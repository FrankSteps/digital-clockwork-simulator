/*
[PT-BR]
    FreqGenerator é responsável por simular a frequência da rede elétrica brasileira (60 Hz), já convertida para sinal digital. 
    No projeto original de Wagner Rambo, um transformador reduz a tensão da rede para 5V, e um circuito de condicionamento de 
    sinal converte a senoide resultante em um sinal digital de onda quadrada. Dessa forma, tanto a frequência quanto a tensão 
    da rede elétrica são aproveitadas diretamente na lógica do relógio digital, eliminando a necessidade de um oscilador dedicado.

[EN-US]
    FreqGenerator is responsible for simulating the Brazilian power grid frequency (60 Hz), already converted to a digital signal.
    In Wagner Rambo's original design, a transformer steps down the mains voltage to 5V, and a signal conditioning circuit converts
    the resulting sine wave into a digital square wave. This allows both the frequency and the voltage of the power grid to be used 
    directly in the clock's logic, eliminating the need for a dedicated oscillator.
*/


#ifndef FREQGENERATOR_HPP
#define FREQGENERATOR_HPP

#include <condition_variable>
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>


// 60Hz frequency generator
class FreqGenerator{
    private:
        double period_ms = 1000.0 / 60;
        std::atomic<bool> running; 
        std::atomic<bool> state;
        std::thread clkThread;

        std::condition_variable condv;

        // Mutex is mutable so it can be used in const methods without affecting the object's logical state.
        mutable std::mutex mtx;

        void run();

    public:
        FreqGenerator() = default;

        void waitEdge(bool prevState);
        void start();
        void stop();

        bool getState() const;
};

#endif