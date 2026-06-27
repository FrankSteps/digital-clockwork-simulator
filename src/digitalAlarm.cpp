/*
[PT-BR]
    O ajuste do horário será feito pelos botões de ajuste rápido e ajuste lento.
    Para os dias da semana, no projeto real seriam utilizados 7 interruptores ON/OFF, mas para evitar o uso excessivo do teclado e 
    manter os valores salvos entre sessões, o arquivo .week localizado na pasta input será o responsável por simular o comportamento
    dos interruptores. Basta o próprio usuário atribuir 1 para ativar o alarme naquele dia ou 0 para desativá-lo.
    
    Segue um exemplo para input.week:

    MON = 1             # comentários poderão ser inseridos desta forma
    TUE = 1
    WED = 1
    THU = 1
    FRI = 1
    SAT = 0
    SUN = 0

    As demais configurações serão feitas diretamente via teclado. Veja as teclas para configuração abaixo

    Key_P (Program):    Programa o alarme para o horário atual exibido no display (aplica um pulso de clock nos flip-flops)
    Key_D (Disarm):     Desarma o alarme enquanto ele está tocando
    Key_A (Advance):    Avança o dia no CD4017
    Key_R (Reset):      Aplica reset em todos os flip-flops

[EN-US]
    The time adjustment will be made using the fast and slow adjustment buttons.
    For the days of the week, the real project would use 7 ON/OFF switches, but to avoid excessive keyboard use and
    keep values saved between sessions, the .week file located in the input folder will be responsible for simulating
    the behavior of the switches. The user simply assigns 1 to enable the alarm on that day or 0 to disable it.
    
    See an example for input.week:

    MON = 1             # comments can be inserted this way
    TUE = 1
    WED = 1
    THU = 1
    FRI = 1
    SAT = 0
    SUN = 0

    The remaining settings will be made directly via keyboard. See the configuration keys below 

    Key_P (Program):    Programs the alarm to the current time shown on the display (applies a clock pulse to the flip-flops)
    Key_D (Disarm):     Disarms the alarm while it is ringing
    Key_A (Advance):    Advances the day on the CD4017
    Key_R (Reset):      Applies reset to all flip-flops
*/

#include "digitalAlarm.hpp"
#include <fstream>


// Reads the .week file and populates weekDays. Skips comment lines (#) and blank lines.
// Expects exactly 7 days in order SUN..SAT. Throws std::runtime_error on any violation.
void DigitalAlarm::loadWeekFile(const std::string& path){
    if(path.size() < 5 || path.substr(path.size() - 5) != ".week"){
        throw std::runtime_error("loadWeekFile: file must have .week extension");
    }

    std::ifstream file(path);
    if(!file.is_open()){
        throw std::runtime_error("loadWeekFile: could not open file: " + path);
    }

    const std::array<std::string, 7> expected = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

    std::string line;
    int lineNum = 0;
    int dayIndex = 0;

    while(dayIndex < 7 && std::getline(file, line)){
        lineNum++;

        // strip comment
        size_t hashPos = line.find('#');
        if(hashPos != std::string::npos){
            line = line.substr(0, hashPos);
        }

        // skip empty lines
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
        if(trimmed.empty()) continue;

        // parse key and value around '='
        size_t eqPos = trimmed.find('=');
        if(eqPos == std::string::npos){
            throw std::runtime_error("loadWeekFile: line " + std::to_string(lineNum) + ": missing '='");
        }

        // extract and trim key
        std::string key = trimmed.substr(0, eqPos);
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);

        // extract and trim value
        std::string val = trimmed.substr(eqPos + 1);
        val.erase(0, val.find_first_not_of(" \t"));
        val.erase(val.find_last_not_of(" \t") + 1);

        // check key sequence
        if(key != expected[dayIndex]){
            throw std::runtime_error("loadWeekFile: line " + std::to_string(lineNum) + ": expected '" + expected[dayIndex] + "', got '" + key + "'");
        }

        // check value
        if(val != "0" && val != "1"){
            throw std::runtime_error("loadWeekFile: line " + std::to_string(lineNum) + ": invalid value '" + val + "', expected 0 or 1");
        }

        weekDays[dayIndex] = (val == "1");
        dayIndex++;
    }

    if(dayIndex < 7){
        throw std::runtime_error("loadWeekFile: expected 7 days, got " + std::to_string(dayIndex));
    }
}


// AND gates combine cd4017 active output (current day) with weekDays switches.
// OR cascade reduces the 7 AND outputs to a single day-match signal.
// Called on advanceDay() and as part of updateComparators().
void DigitalAlarm::updateDayComparators(){
    // AND gates: Qn AND SWn
    cd4081[0].setInputA(0, cd4017.getOutput(0)); cd4081[0].setInputB(0, weekDays[0]);
    cd4081[0].setInputA(1, cd4017.getOutput(1)); cd4081[0].setInputB(1, weekDays[1]);
    cd4081[0].setInputA(2, cd4017.getOutput(2)); cd4081[0].setInputB(2, weekDays[2]);
    cd4081[0].setInputA(3, cd4017.getOutput(3)); cd4081[0].setInputB(3, weekDays[3]);

    cd4081[1].setInputA(0, cd4017.getOutput(4)); cd4081[1].setInputB(0, weekDays[4]);
    cd4081[1].setInputA(1, cd4017.getOutput(5)); cd4081[1].setInputB(1, weekDays[5]);
    cd4081[1].setInputA(2, cd4017.getOutput(6)); cd4081[1].setInputB(2, weekDays[6]);

    // OR cascade layer 1
    cd4071[0].setInputA(0, cd4081[0].getOutput(0)); cd4071[0].setInputB(0, cd4081[0].getOutput(1));
    cd4071[0].setInputA(1, cd4081[0].getOutput(2)); cd4071[0].setInputB(1, cd4081[0].getOutput(3));
    cd4071[0].setInputA(2, cd4081[1].getOutput(0)); cd4071[0].setInputB(2, cd4081[1].getOutput(1));
    cd4071[0].setInputA(3, cd4081[1].getOutput(2)); cd4071[0].setInputB(3, false);

    // OR cascade layer 2
    cd4071[1].setInputA(0, cd4071[0].getOutput(0)); cd4071[1].setInputB(0, cd4071[0].getOutput(1));
    cd4071[1].setInputA(1, cd4071[0].getOutput(2)); cd4071[1].setInputB(1, cd4071[0].getOutput(3));

    // OR final
    cd4071[1].setInputA(2, cd4071[1].getOutput(0)); cd4071[1].setInputB(2, cd4071[1].getOutput(1));
}


// cd4063 cascade compares programmed time (cd4013 outputs) against current time (busData).
// Propagates equal/greater/smaller from unit of minutes up to meridiem.
// Last AND gate (cd4081[1] gate 3) combines checkDay() and checkTime() into alarm output.
// Called as part of updateComparators() on every clock cycle via setDataMemory().
void DigitalAlarm::updateTimeComparators(std::array<std::array<bool, 4>, 4> busData, bool currentMeridiem){
    // cd4063[0]: unit of minutes (bits 0-3)
    cd4063[0].setInputEqual(true);
    cd4063[0].setInputGreater(false);
    cd4063[0].setInputSmaller(false);
    cd4063[0].setInputA(0, cd4013[0].getOutput(0)); cd4063[0].setInputB(0, busData[0][0]);
    cd4063[0].setInputA(1, cd4013[0].getOutput(1)); cd4063[0].setInputB(1, busData[0][1]);
    cd4063[0].setInputA(2, cd4013[1].getOutput(0)); cd4063[0].setInputB(2, busData[0][2]);
    cd4063[0].setInputA(3, cd4013[1].getOutput(1)); cd4063[0].setInputB(3, busData[0][3]);

    // cd4063[1]: tens of minutes (bits 4-7)
    cd4063[1].setInputEqual(cd4063[0].getOutputEqual());
    cd4063[1].setInputGreater(cd4063[0].getOutputGreater());
    cd4063[1].setInputSmaller(cd4063[0].getOutputSmaller());
    cd4063[1].setInputA(0, cd4013[2].getOutput(0)); cd4063[1].setInputB(0, busData[1][0]);
    cd4063[1].setInputA(1, cd4013[2].getOutput(1)); cd4063[1].setInputB(1, busData[1][1]);
    cd4063[1].setInputA(2, cd4013[3].getOutput(0)); cd4063[1].setInputB(2, busData[1][2]);
    cd4063[1].setInputA(3, cd4013[3].getOutput(1)); cd4063[1].setInputB(3, busData[1][3]);

    // cd4063[2]: unit of hours (bits 8-11)
    cd4063[2].setInputEqual(cd4063[1].getOutputEqual());
    cd4063[2].setInputGreater(cd4063[1].getOutputGreater());
    cd4063[2].setInputSmaller(cd4063[1].getOutputSmaller());
    cd4063[2].setInputA(0, cd4013[4].getOutput(0)); cd4063[2].setInputB(0, busData[2][0]);
    cd4063[2].setInputA(1, cd4013[4].getOutput(1)); cd4063[2].setInputB(1, busData[2][1]);
    cd4063[2].setInputA(2, cd4013[5].getOutput(0)); cd4063[2].setInputB(2, busData[2][2]);
    cd4063[2].setInputA(3, cd4013[5].getOutput(1)); cd4063[2].setInputB(3, busData[2][3]);

    // cd4063[3]: tens of hours (bits 12-15)
    cd4063[3].setInputEqual(cd4063[2].getOutputEqual());
    cd4063[3].setInputGreater(cd4063[2].getOutputGreater());
    cd4063[3].setInputSmaller(cd4063[2].getOutputSmaller());
    cd4063[3].setInputA(0, cd4013[6].getOutput(0)); cd4063[3].setInputB(0, busData[3][0]);
    cd4063[3].setInputA(1, cd4013[6].getOutput(1)); cd4063[3].setInputB(1, busData[3][1]);
    cd4063[3].setInputA(2, cd4013[7].getOutput(0)); cd4063[3].setInputB(2, busData[3][2]);
    cd4063[3].setInputA(3, cd4013[7].getOutput(1)); cd4063[3].setInputB(3, busData[3][3]);

    // cd4063[4]: meridiem (bit 16) + 3 false bits
    cd4063[4].setInputEqual(cd4063[3].getOutputEqual());
    cd4063[4].setInputGreater(cd4063[3].getOutputGreater());
    cd4063[4].setInputSmaller(cd4063[3].getOutputSmaller());
    cd4063[4].setInputA(0, cd4013[8].getOutput(0)); cd4063[4].setInputB(0, currentMeridiem);
    cd4063[4].setInputA(1, false);                  cd4063[4].setInputB(1, false);
    cd4063[4].setInputA(2, false);                  cd4063[4].setInputB(2, false);
    cd4063[4].setInputA(3, false);                  cd4063[4].setInputB(3, false);

    // alarm output
    cd4081[1].setInputA(3, checkDay());
    cd4081[1].setInputB(3, checkTime());
}


// Orchestrates the full combinational update: day path first, then time path.
void DigitalAlarm::updateComparators(std::array<std::array<bool, 4>, 4> busData, bool currentMeridiem){
    updateDayComparators();
    updateTimeComparators(busData, currentMeridiem);
}


// Returns true if the current day matches any enabled day in the .week file.
bool DigitalAlarm::checkDay(){
    return cd4071[1].getOutput(2);
}


// Returns true if the programmed time matches the current time.
bool DigitalAlarm::checkTime(){
    return cd4063[4].getOutputEqual();
}


// Clocks all 17 time flip-flops, capturing the current busData into cd4013 outputs.
// The disarm flip-flop (cd4013[8] FF1) is clocked separately in programAlarm().
void DigitalAlarm::loadMemory(){
    cd4013[0].clock(0); cd4013[0].clock(1);
    cd4013[1].clock(0); cd4013[1].clock(1);
    cd4013[2].clock(0); cd4013[2].clock(1);
    cd4013[3].clock(0); cd4013[3].clock(1);
    cd4013[4].clock(0); cd4013[4].clock(1);
    cd4013[5].clock(0); cd4013[5].clock(1);
    cd4013[6].clock(0); cd4013[6].clock(1);
    cd4013[7].clock(0); cd4013[7].clock(1);
    cd4013[8].clock(0);
}


// Resets all 18 flip-flops, clearing programmed time, meridiem and disarm state.
void DigitalAlarm::resetMemory(){
    cd4013[0].reset(0); cd4013[0].reset(1);
    cd4013[1].reset(0); cd4013[1].reset(1);
    cd4013[2].reset(0); cd4013[2].reset(1);
    cd4013[3].reset(0); cd4013[3].reset(1);
    cd4013[4].reset(0); cd4013[4].reset(1);
    cd4013[5].reset(0); cd4013[5].reset(1);
    cd4013[6].reset(0); cd4013[6].reset(1);
    cd4013[7].reset(0); cd4013[7].reset(1);
    cd4013[8].reset(0); cd4013[8].reset(1);
}


// Configures cd4017 for 7-day counting, resets all flip-flops and loads the .week file.
DigitalAlarm::DigitalAlarm(const std::string& weekFilePath){
    cd4017.setReset(7);
    cd4017.setClockEnable(true);
    resetMemory();
    loadWeekFile(weekFilePath);
}


// Key_P: clocks all time flip-flops via loadMemory(), then arms the alarm (cd4013[8] FF1).
void DigitalAlarm::programAlarm(){
    loadMemory();
    cd4013[8].clock(1);
}


// Key_A: advances the cd4017 by one day and updates the day comparators immediately.
// Also called by DigitalClockwork on the AM meridiem edge (midnight).
void DigitalAlarm::advanceDay(){
    cd4017.shift();
    updateDayComparators();
}


// Key_R: resets all flip-flops to initial state.
void DigitalAlarm::reset(){
    resetMemory();
}


// Stores the current meridiem from DigitalClockwork for use in the next setDataMemory() call.
void DigitalAlarm::setMeridiem(bool value){
    meridiem = value;
}


// Receives HH:MM from DigitalClockwork and updates cd4013 data inputs without clocking.
// Time is only captured into outputs when programAlarm() is called (Key_P).
// Triggers the full combinational update via updateComparators().
void DigitalAlarm::setDataMemory(std::array<std::array<bool, 4>, 4> busData){
    cd4013[0].setData(0, busData[0][0]); cd4013[0].setData(1, busData[0][1]);
    cd4013[1].setData(0, busData[0][2]); cd4013[1].setData(1, busData[0][3]);
    cd4013[2].setData(0, busData[1][0]); cd4013[2].setData(1, busData[1][1]);
    cd4013[3].setData(0, busData[1][2]); cd4013[3].setData(1, busData[1][3]);
    cd4013[4].setData(0, busData[2][0]); cd4013[4].setData(1, busData[2][1]);
    cd4013[5].setData(0, busData[2][2]); cd4013[5].setData(1, busData[2][3]);
    cd4013[6].setData(0, busData[3][0]); cd4013[6].setData(1, busData[3][1]);
    cd4013[7].setData(0, busData[3][2]); cd4013[7].setData(1, busData[3][3]);
    cd4013[8].setData(0, meridiem);

    updateComparators(busData, meridiem);
}


// Returns the enabled state of a given weekday switch. Index 0 = SUN, index 6 = SAT.
bool DigitalAlarm::getWeekDaysStatus(size_t index) const{
    if(index >= 7){
        throw std::invalid_argument("DigitalAlarm error: index out of range. Valid indices are 0 to 6.");
    }
    return weekDays[index];
}


// Returns the active output of the cd4017 for a given day. Index 0 = SUN, index 6 = SAT.
bool DigitalAlarm::getCurrentDay(size_t index) const{
    if(index >= 7){
        throw std::invalid_argument("DigitalAlarm error: index out of range. Valid indices are 0 to 6.");
    }
    return cd4017.getOutput(index);
}


// Returns true when current day and time match the programmed alarm (cd4081[1] gate 3).
bool DigitalAlarm::getOutputSystem() const{
    return cd4081[1].getOutput(3);
}