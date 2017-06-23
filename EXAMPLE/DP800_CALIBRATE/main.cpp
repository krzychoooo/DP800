#include <iostream>
#include <iomanip>
#include "../DP800/DP800/dp800.h"
#include "../Miernik_3500A/m3500a.h"

using namespace std;


float ch1ch2dacV[] = {0.2, 0.5, 1.2, 2, 3.2, 4.1, 5.2, 6.9, 7.5, 8.7, 10.1, 11.8, 12.6, 13.5, 15, 15.8, 16.5, 17.3, 18.5, 19.1, 19.9, 20.2, 20.8, 21.8, 22.4, 22.7, 23.9, 24.3, 25.7, 26.9, 27.9, 28.5, 28.9, 29.8, 30.2, 32};
float ch1ch2adcV[] = {0, 0.05, 0.1, 0.5, 1, 5, 10, 12.8, 20, 30, 32};
float ch1ch2dacI[] = {0.1, 0.25, 0.5, 0.8, 1, 1.25, 1.5, 1.75, 1.9, 2.15, 2.35, 2.5, 2.75, 3, 3.2};
float ch1ch2adcI[] = {0, 0.01, 0.1, 1, 2, 3, 3.2};

float ch3dacV[] = {0.1, 0.2, 0.4, 0.85, 1.2, 1.8, 2.55, 3.1, 3.4, 4.1, 4.5, 5, 5.3};
float ch3adcV[] = {0, 0.005, 0.01, 0.02, 0.05, 0.1, 0.5, 1, 3, 5, 5.3};
float ch3dacI[] = {0.1, 0.5, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.2};
float ch3adcI[] = {0, 0.1, 1, 2, 3, 3.2};


int main()
{
    cout << "Kalibracja RIGOL DP832!" << endl;

    DP800 zasilacz = DP800("USB0::0x1AB1::0x0E11::DP8C164353664::INSTR");
    M3500A miernik = M3500A("USB0::0x164E::0x0DAD::TW00023170::INSTR");

    float odczyt;
    std::string chanel;

    cout<<"wprowadz CH1 | H2 | CH3 | ALL"<<endl;
    cin>>chanel;

    /*                 C H A N E L     1            */
    if(chanel == "CH1" || chanel == "ALL")
    {
        cout << "CH1" << endl;
        zasilacz.calibrationStart(1,"11111");
        zasilacz.calibrationClear(1, "ALL");

        // KALIBRACJA DAC mode=V CH1
        miernik.setMultimetr(0,0.000001,M3500A::VOLTage_DC);  //range=0 -> AUTORANGE
        odczyt = miernik.read();
        zasilacz.dislayText("KANAL 1 DO WOLTOMIERZA  ", 40, 30);
        Beep(523,500);
        system("pause");
        zasilacz.dislayText("CH1 napiecie DAC  ", 50, 20);
        Sleep(500);
        zasilacz.powerEnable(1, true);
        int sizeArray = sizeof(ch1ch2dacV)/sizeof(ch1ch2dacV[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(1, "V", i, ch1ch2dacV[i], DAC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2dacV[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(1, "V", i, odczyt, DAC);
        }

        // KALIBRACJA ADC mode=V CH1
        zasilacz.dislayText("CH1 napiecie ADC  ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch1ch2adcV)/sizeof(ch1ch2adcV[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(1, "V", i, ch1ch2adcV[i], ADC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2adcV[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(1, "V", i, odczyt, ADC);
        }
        zasilacz.powerEnable(1, false);

        // KALIBRACJA DAC mode=I CH1
        //meter to amper
        miernik.setMultimetr(0, 0.000001, M3500A::CURRent_DC);  //range=0 -> AUTORANGE
        zasilacz.dislayText("KANAL 1 DO AMPEROMIERZA  ", 40, 30);
        Beep(523,500);
        system("pause");
        zasilacz.powerEnable(1, true);
        zasilacz.dislayText("CH1 prad DAC     ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch1ch2dacI)/sizeof(ch1ch2dacI[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(1, "C", i, ch1ch2dacI[i], DAC);
            Sleep(500);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2dacI[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            Sleep(5000);
            zasilacz.calibrationMEAS(1, "C", i, odczyt, DAC);
        }

        // KALIBRACJA ADC mode=I CH1
        zasilacz.dislayText("CH1 prad ADC     ", 50, 20);
        Sleep(1000);
        sizeArray = sizeof(ch1ch2adcI)/sizeof(ch1ch2adcI[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(1, "C", i, ch1ch2adcI[i], ADC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2adcI[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(1, "C", i, odczyt, ADC);
        }
        zasilacz.calibrationEnd("11/22/2014", 1);
        zasilacz.powerEnable(1, false);
    }




    /*                 C H A N E L     2            */
    if(chanel == "CH2" || chanel == "ALL")
    {
        zasilacz.calibrationStart(2, "11111");
        zasilacz.calibrationClear(2, "ALL");

        // KALIBRACJA DAC mode=V CH2
        miernik.setMultimetr(0,0.000001,M3500A::VOLTage_DC);  //range=0 -> AUTORANGE
        zasilacz.dislayText("KANAL 2 DO WOLTOMIERZA  ", 40, 30);
        Beep(523,500);
        system("pause");
        zasilacz.powerEnable(2, true);
        zasilacz.dislayText("CH2 napiecie DAC  ", 50, 20);
        Sleep(500);
        int sizeArray = sizeof(ch1ch2dacV)/sizeof(ch1ch2dacV[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(2, "V", i, ch1ch2dacV[i], DAC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2dacV[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(2, "V", i, odczyt, DAC);
        }

        // KALIBRACJA ADC mode=V CH2
        zasilacz.dislayText("CH2 napiecie ADC  ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch1ch2adcV)/sizeof(ch1ch2adcV[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(2, "V", i, ch1ch2adcV[i], ADC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2adcV[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(2, "V", i, odczyt, ADC);
        }
        zasilacz.powerEnable(2, false);

        // KALIBRACJA DAC mode=I CH2
        //meter to amper
        miernik.setMultimetr(0, 0.000001, M3500A::CURRent_DC);  //range=0 -> AUTORANGE
        zasilacz.dislayText("KANAL 2 DO AMPEROMIERZA  ", 40, 30);
        Beep(523,500);
        system("pause");
        zasilacz.powerEnable(2, true);
        zasilacz.dislayText("CH2 prad DAC     ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch1ch2dacI)/sizeof(ch1ch2dacI[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(2, "C", i, ch1ch2dacI[i], DAC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2dacI[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(2, "C", i, odczyt, DAC);
        }

        // KALIBRACJA ADC mode=I CH2
        zasilacz.dislayText("CH2 prad ADC     ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch1ch2adcI)/sizeof(ch1ch2adcI[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(2, "C", i, ch1ch2adcI[i], ADC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch1ch2adcI[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(2, "C", i, odczyt, ADC);
        }
        zasilacz.calibrationEnd("11/22/2014", 2);
        zasilacz.powerEnable(2, false);
    }


    /*                 C H A N E L     3            */
    if(chanel == "CH3" || chanel == "ALL")
    {
        zasilacz.calibrationStart(3, "11111");
        zasilacz.calibrationClear(3, "ALL");

        // KALIBRACJA DAC mode=V CH3
        miernik.setMultimetr(0,0.000001,M3500A::VOLTage_DC);  //range=0 -> AUTORANGE
        zasilacz.dislayText("KANAL 3 DO WOLTOMIERZA  ", 40, 30);
        Beep(523,500);
        system("pause");
        zasilacz.powerEnable(3, true);
        zasilacz.dislayText("CH3 napiecie DAC  ", 50, 20);
        Sleep(500);
        int sizeArray = sizeof(ch3dacV)/sizeof(ch3dacV[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(3, "V", i, ch3dacV[i], DAC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch3dacV[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(3, "V", i, odczyt, DAC);
        }

        // KALIBRACJA ADC mode=V CH3
        zasilacz.dislayText("CH3 napiecie ADC  ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch3adcV)/sizeof(ch3adcV[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(3, "V", i, ch3adcV[i], ADC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch3adcV[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(3, "V", i, odczyt, ADC);
        }
        zasilacz.powerEnable(3, false);

        // KALIBRACJA DAC mode=I CH3
        //meter to amper
        miernik.setMultimetr(0, 0.000001, M3500A::CURRent_DC);  //range=0 -> AUTORANGE
        zasilacz.dislayText("KANAL 3 DO AMPEROMIERZA  ", 40, 30);
        Beep(523,500);
        system("pause");
        zasilacz.powerEnable(3, true);
        zasilacz.dislayText("CH3 prad DAC     ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch3dacI)/sizeof(ch3dacI[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(3, "C", i, ch3dacI[i], DAC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch3dacI[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(3, "C", i, odczyt, DAC);
        }

        // KALIBRACJA ADC mode=I CH3
        zasilacz.dislayText("CH3 prad ADC     ", 50, 20);
        Sleep(500);
        sizeArray = sizeof(ch3adcI)/sizeof(ch3adcI[0]);
        for(int i=0; i!= sizeArray; i++)
        {
            zasilacz.calibrationSet(3, "C", i, ch3adcI[i], ADC);
            Sleep(100);
            odczyt = miernik.read();
            cout<<"krok "<<i<<" ustawianie - "<<ch3adcI[i]<<setw(30)<<"odczyt - "<<odczyt<<endl;
            zasilacz.calibrationMEAS(3, "C", i, odczyt, ADC);
        }
        zasilacz.calibrationEnd("11/22/2014", 3);
        zasilacz.powerEnable(3, false);
    }

    return 0;
}



