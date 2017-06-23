#include <iostream>
#include <cstdlib>
#include "../../DP800/dp800.h"
#include "../csv/csv.h"

using namespace std;

int main()
{
    cout << "Test PT4115 i RIGOL DP832" << endl;

    DP800 zasilacz = DP800("USB0::0x1AB1::0x0E11::DP8C164353664::INSTR");
    csv arkKalkulacyjny("dane.csv");

    zasilacz.setVI(1, 0, 0.5);
    zasilacz.powerEnable(1, true);

    float current=0, power=0;
    for(float u=0; u<28; u=u+0.25){

        zasilacz.setVI(1, u, 0.5);  Sleep(50);
        current = zasilacz.getCurrent(1);
        power = zasilacz.getPower(1);

        cout<<"voltage="<<u<<"\tcurrent="<<current<<"\tmoc="<<power<<endl;
        arkKalkulacyjny.addToRecord(u); arkKalkulacyjny.addToRecord(current); arkKalkulacyjny.addToRecord(power);
        arkKalkulacyjny.storeRecord();
    }

    zasilacz.powerEnable(1, false);

    arkKalkulacyjny.~csv();
    zasilacz.~DP800();
    return 0;
}
