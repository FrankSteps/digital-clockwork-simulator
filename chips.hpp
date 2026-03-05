#ifndef CHIPS_HPP
#define CHIPS_HPP
#include <cstdint>
#include <vector>

class Chip4017 {
    private:
        unsigned LimitReset;
        uint32_t Out{0};

    public:
        explicit Chip4017(unsigned limitReset) : LimitReset(limitReset){}
        void shift();
        void reset();
        uint32_t getOut() const;
        unsigned getLimitReset() const;
};

class Chip4081 {
    private:
        std::vector<bool> input_A{4, false};  
        std::vector<bool> input_B{4, false};
        std::vector<bool> output_C{4, false};
        void updateOutputs();

    public:
        Chip4081() = default;

        void setInputA(int index, bool value);
        void setInputB(int index, bool value);
        bool getOutput(int index) const;
};

#endif