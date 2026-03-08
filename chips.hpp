#ifndef CHIPS_HPP
#define CHIPS_HPP
#include <cstdint>
#include <vector>

class Chip4017 {
    private:
        unsigned LimitReset;
        bool ClockEnable = false;
        uint32_t Out{0};
        std::vector <bool> Out1{10, 0};  // irei implementar isso posteriormente

    public:
        explicit Chip4017(unsigned limitReset, bool clockEnable) : LimitReset(limitReset), ClockEnable(clockEnable){}
        void shift();
        void reset();
        uint32_t getOut() const;
        unsigned getLimitReset() const;
};



class Chip4081 {
    private:
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
    private:
        std::vector<bool> presetInputs = std::vector<bool>(4, false);
        std::vector<bool> outputs{4, false};

        bool presetEnable = false;
        bool carryIn = false;
        bool UpDown = false;
        bool binaryDecade = false;
        bool carryOut = false;

        void add();
        void subtract();
        void updateCarryOut();
        void updateOutput();

    public:
        explicit Chip4029(const std::vector<bool>& presetInputs) {
            if(presetInputs.size() != 4){
                throw std::invalid_argument("presetInputs precisa de 4 elementos válidos");
            } 
            this->presetInputs = presetInputs;
        }
        void setPresetEnable(bool value);
        void setCarryIn(bool value);
        void setUpDown(bool value);
        void setBinaryDecade(bool value);

        bool getCarryOut() const;
        bool getOutput(size_t index) const;
};

#endif