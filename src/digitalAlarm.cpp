/*
[PT-BR]
    O ajuste do horário será feito pelos botões de ajuste rápido e ajuste lento.
    Para os dias da semana, no projeto real seriam utilizados 7 interruptores ON/OFF, mas para evitar o uso excessivo do teclado e 
    manter os valores salvos entre sessões, o arquivo .week localizado na pasta input será o responsável por simular o comportamento
    dos interruptores. Basta o próprio usuário atribuir 1 para ativar o alarme naquele dia ou 0 para desativá-lo.
    
    Segue um exemplo para input.week:

    MON = 1     # comentários poderão ser inseridos desta forma
    TUE = 1
    WED = 1
    THU = 1
    FRI = 1
    SAT = 0
    SUN = 0

    As demais configurações serão feitas diretamente via teclado. Veja as teclas para configuração abxio

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

    MON = 1      # comments can be inserted this way
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