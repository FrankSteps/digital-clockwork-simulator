#ifndef CHIPS_HPP
#define CHIPS_HPP
#include <cstdint>
#include <vector>
#include <array>
#include <stdexcept>

class Chip4017 {
    private:
        unsigned LimitReset;
        bool ClockEnable = false;
        uint32_t Out{0};
        std::vector <bool> Out1{0,0,0,0,0,0,0,0,0,0};  // irei implementar isso posteriormente

    public:
        explicit Chip4017(unsigned limitReset, bool clockEnable);
        void shift();
        void reset();
        uint32_t getOut() const;
        unsigned getLimitReset() const;
};



class Chip4081 {
    protected:
        std::vector<bool> input_A = std::vector<bool>(4, false);
        std::vector<bool> input_B = std::vector<bool>(4, false);
        std::vector<bool> output_C = std::vector<bool>(4, false);
        void updateOutputs();

    public:
        Chip4081() = default;

        void setInputA(size_t index, bool value);
        void setInputB(size_t index, bool value);
        bool getOutput(size_t index) const;
};


class Chip4029 {
    protected:
        std::array<bool,4> presetInputs{0,0,0,0};
        std::array<bool,4> outputs{0,0,0,0};

        const std::array<bool,4> ZERO{0,0,0,0};
        const std::array<bool,4> NINE{1,0,0,1};
        const std::array<bool,4> FIFTEEN{1,1,1,1};

        bool presetEnable = false;
        bool carryIn = false;
        bool upDown = false;
        bool binaryDecade = false;
        bool carryOut = false;


        void increment() {
            int value = 0;
        
            for (size_t i = 0; i < 4; i++) {
                if (outputs[i]) {
                    value |= (1 << i);
                }
            }
        
            if (carryIn) {
                value = value + 2;
            } else {
                value = value + 1;
            }
        
            if (binaryDecade) {
                if (value > 9) {
                    value = value % 10;
                    carryOut = true;
                } else {
                    carryOut = false;
                }
            } else {
                if (value > 15) {
                    value = value % 16;
                    carryOut = true;
                } else {
                    carryOut = false;
                }
            }
        
            for (size_t i = 0; i < 4; i++) {
                outputs[i] = (value >> i) & 1;
            }
        }

        void decrement() {
            int value = 0;
        
            for (size_t i = 0; i < 4; i++) {
                if (outputs[i]) {
                    value |= (1 << i);
                }
            }
        
            if (carryIn) {
                value = value - 2;
            } else {
                value = value - 1;
            }
        
            if (binaryDecade) {
                if (value < 0) {
                    value = 10 + value; 
                    carryOut = true;
                } else {
                    carryOut = false;
                }
            } else {
                if (value < 0) {
                    value = 16 + value; 
                    carryOut = true;
                } else {
                    carryOut = false;
                }
            }
        
            for (size_t i = 0; i < 4; i++) {
                outputs[i] = (value >> i) & 1;
            }
        }

    public:
        explicit Chip4029(const std::array<bool,4>& presetInputs){
            if(presetInputs.size() != 4){ 
                throw std::invalid_argument("presetInputs requires 4 valid boolean values");
            }
            this->presetInputs = presetInputs;
        }

        void setPresetEnable();
        void setCarryIn(bool value);
        void setUpDown(bool value);
        void setBinaryDecade(bool value);
        bool getCarryOut() const;
        bool getOutput(size_t index) const;

        void reset();
        void clock();
};

#endif