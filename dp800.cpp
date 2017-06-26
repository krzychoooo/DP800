/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DP800.cpp
 * Author: fajnysoft@gmail.com
 *
 * Created on 1 kwietnia 2017, 16:50
 */

#include "dp800.h"
//#include "StdAfx.h"
#include "visa.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <cmath>

#define far

using namespace std;

std::string DP800::float_to_string( float liczba )
{
    std::stringstream floatToStr;
    std::string str;

    floatToStr << liczba;
    floatToStr >> str;
    floatToStr.clear();

    return str;
}



DP800::DP800() {
}


DP800::DP800(std::string miernikId) {
    //typedef int32_t ViStatus;
    //HINSTANCE hUSBTMCLIB;

    //hUSBTMCLIB = LoadLibrary("NIvisa64.dll");
    hUSBTMCLIB = LoadLibrary("visa32.dll");


    signed long (__stdcall * PviOpenDefaultRM_usb) (unsigned long *vi);
    signed long (__stdcall * PviFindRsrc_usb) (unsigned long sesn, char *expr, unsigned long *vi,
            unsigned long *retCnt, char far desc[]);
    signed long (__stdcall * PviOpen_usb) (unsigned long sesn, char *name, unsigned long mode,
            unsigned long timeout, unsigned long *vi);
    signed long (__stdcall * PviClose_usb) (unsigned long vi);
    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviSetAttribute_usb) (unsigned long vi, unsigned long viAttr, unsigned long attrstat);


    PviOpenDefaultRM_usb = (signed long (__stdcall*) (unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viOpenDefaultRM");
    PviFindRsrc_usb = (signed long (__stdcall*) (unsigned long, char*, unsigned long*,
            unsigned long*, char[]))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viFindRsrc");
    PviClose_usb = (signed long (__stdcall*) (unsigned long))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viClose");
    PviOpen_usb = (signed long (__stdcall*) (unsigned long, char*, unsigned long,
            unsigned long, unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viOpen");
    PviWrite_usb	      = (signed long (__stdcall*)(unsigned long, unsigned char*, unsigned long,
                                unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR)"viWrite");
    PviRead_usb			  = (signed long (__stdcall*)(unsigned long, unsigned char*, unsigned long,
                                unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR)"viRead");
    PviSetAttribute_usb = (signed long (__stdcall*) (unsigned long, unsigned long, unsigned long))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viSetAttribute");


    if (PviOpenDefaultRM_usb == NULL ||
            PviFindRsrc_usb == NULL ||
            PviClose_usb == NULL ||
            PviOpen_usb == NULL ||
            PviWrite_usb == NULL ||
            PviRead_usb == NULL ||
            PviSetAttribute_usb == NULL
            ) {
        //FreeLibrary(hUSBTMCLIB);
        printf("%x,%x,%x,%x,%x,%x,%x,",PviOpenDefaultRM_usb, PviFindRsrc_usb, PviClose_usb, PviOpen_usb, PviWrite_usb, PviRead_usb, PviSetAttribute_usb);
        //hUSBTMCLIB = NULL;
        //MessageBox(NULL, "NIVISA for USBTMC library not ready.", "Multimeter device test", MB_OK);
        printf("NIVISA for USBTMC library not ready.");
    }


    // Open Device -- Resource Manager
    status = PviOpenDefaultRM_usb(&m_defaultRM_usbtmc);
    if (status < 0L) {
        PviClose_usb(m_defaultRM_usbtmc);
        hUSBTMCLIB = NULL;
        m_defaultRM_usbtmc = 0;
        //		MessageBox(NULL, "USBTMC resource not found.", "Multimeter device test", MB_OK);
        printf("USBTMC resource not found.");
        //Sleep(5000);
    } else {
        //char id1[]="USB[0-9]*::0x164E::0x0DAD::?*INSTR";
        //char id1[]="USB[0-9]*::"+miernikId+"::?*INSTR";

        //printf("string-%s",miernikId.c_str());
        char * idUSB = new char[miernikId.size()+1];
        strcpy(idUSB, miernikId.c_str());
        status = PviFindRsrc_usb(m_defaultRM_usbtmc, idUSB, &m_findList_usbtmc, &m_nCount, instrDescriptor);

        if (status < 0L) {
            char id2[]="USB[0-9]*::5710::3501::?*INSTR";
            status = PviFindRsrc_usb(m_defaultRM_usbtmc, id2, &m_findList_usbtmc, &m_nCount, instrDescriptor);
            if (status < 0L) {
                PviClose_usb(m_defaultRM_usbtmc);
                hUSBTMCLIB = NULL;
                m_defaultRM_usbtmc = 0;
            } else {
                PviOpen_usb(m_defaultRM_usbtmc, instrDescriptor, 0, 0, &m_instr_usbtmc);
                status = PviSetAttribute_usb(m_instr_usbtmc, VI_ATTR_TMO_VALUE, m_Timeout);
            }
        } else {
            PviOpen_usb(m_defaultRM_usbtmc, instrDescriptor, 0, 0, &m_instr_usbtmc);
            status = PviSetAttribute_usb(m_instr_usbtmc, VI_ATTR_TMO_VALUE, m_Timeout);
            //Sleep(5000);
            //printf("miernik OK\n\r");
        }
    }

    if (!hUSBTMCLIB) {
        printf("Multimeter device connect failed.\n");
        Sleep(5000);
    }

}

DP800::~DP800() {
}

float DP800::read(){
    len = 64;
    pStrout = new char[len];

    memset(pStrout,0, len);

    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);

    PviWrite_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viWrite");
    PviRead_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viRead");

    // Send read command
    strcpy(pStrout, "READ?\0");
    status = PviWrite_usb(m_instr_usbtmc, (unsigned char *) pStrout, 6, &nWritten);
    Sleep(30);
    //printf(" output : READ?\n");

    status = PviRead_usb(m_instr_usbtmc, pStrin, 64, &nRead);
    if (nRead > 0) {
        for (len = 0; len < (long) nRead; len++) {
            buffer[len] = pStrin[len];
        }
    }
    buffer[nRead] = '\0';
    float pomiar;
    sscanf(buffer,"%f",&pomiar);
    //printf(" input : %s\n\n", buffer);
    return pomiar;
}

void DP800::sendCommand(char* command,unsigned char* requestBuffer , int commandSize, bool readDevice){

    signed long (__stdcall * PviWrite_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);
    signed long (__stdcall * PviRead_usb) (unsigned long vi, unsigned char *name, unsigned long len,
            unsigned long *retval);

    PviWrite_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viWrite");
    PviRead_usb = (signed long (__stdcall*) (unsigned long, unsigned char*, unsigned long,
            unsigned long*))GetProcAddress(hUSBTMCLIB, (LPCSTR) "viRead");

    // Send read command

    status = PviWrite_usb(m_instr_usbtmc, (unsigned char *) command, commandSize, &nWritten);
    Sleep(80);
    //cout<<"nW="<<nWritten<<" "<<endl;
    //printf(" output : READ?\n");

    if(readDevice){
        status = PviRead_usb(m_instr_usbtmc, requestBuffer, 64, &nRead);
        int i=0;
        while(status != 0){
            Sleep(80);
            status = PviWrite_usb(m_instr_usbtmc, (unsigned char *) command, commandSize, &nWritten);
            Sleep(80);
            //cout<<"PviRead_usb_start ";
            status = PviRead_usb(m_instr_usbtmc, requestBuffer, 64, &nRead);
            //cout<<"PviRead_usb_end ";
            Sleep(50);
            if(i++>100)break;
        }
        cout<<"i="<<i;
        requestBuffer[nRead] = '\0';
    }

//    if (nRead > 0) {
//        for (int len = 0; len < (long) nRead; len++) {
//            requestBuffer[len] = pStrin[len];
//        }
//    }

    //printf(" input : %s\n\n", buffer);
}

void DP800::setText(std::string displayText){
    len = 64;
    pStrout = new char[len];
    displayText = "DISPlay:TEXT \"" + displayText + "\"\n\0";
    //printf("%s",displayText.c_str());
    sprintf(pStrout, "%s", displayText.c_str());
    //strcpy(pStrout, displayText.c_str());
    sendCommand(pStrout, pStrin, displayText.length(), false);
    delete pStrout;
}

void DP800::setV(int chanel, float volt){
    len = 64;
    pStrout = new char[len];
    std::string command;
	ostringstream ss;

/*Select CHANEL*/
	command = ":INST CH";

	ss << chanel;
	std::string ch = ss.str();
	command = command + ch;
	sprintf(pStrout, "%s", command.c_str());

	//time_t tstart = clock();
	command = command + "\n";
    sendCommand(pStrout, pStrin, command.length(), false);
    //tstart = clock() - tstart;
    //cout<<"->"<<(double)((tstart/CLOCKS_PER_SEC)*1000);


/*Set volt*/
	command = ":VOLT ";

	volt = round(volt*1000)/1000;
	ss.str(""); ss.clear();
    ss<<fixed<<setprecision(3)<<volt;
    std::string value = ss.str();
    if(chanel==1 || chanel==2){
            if(value.length() == 5) value = "0" + value;    // for ch1 or ch2 format is 02.000
    }
    command = command + value;
    command = command + "\n";
    sprintf(pStrout, "%s", command.c_str());

    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


void DP800::setI(int chanel, float current){
        len = 64;
    pStrout = new char[len];
    std::string command;
	ostringstream ss;

/*Select CHANEL*/
	command = ":INST CH";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch;
	//cout<<command<<endl;
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);


/*Set curr*/
	command = ":CURR ";

    current = round(current*1000)/1000;
    ss.str(""); ss.clear();
    ss<<fixed<<setprecision(3)<<current;
    std::string value = ss.str();
    command = command + value;
    command = command + "\n";
    sprintf(pStrout, "%s", command.c_str());

    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}

void DP800::setVI(int chanel, float volt, float current){
    len = 64;
    pStrout = new char[len];
    std::string command;
	ostringstream ss;

/* calculate chanel to string*/
	ss << chanel;
	std::string ch = ss.str();

/* calculate volt*/
    volt = round(volt*1000)/1000;
	ss.str(""); ss.clear();
    ss<<fixed<<setprecision(3)<<volt;
    std::string voltString = ss.str();
    if(chanel==1 || chanel==2){
            if(voltString.length() == 5) voltString = "0" + voltString;    // for ch1 or ch2 format is 02.000
    }

/* calculate current*/
    current = round(current*1000)/1000;
    ss.str(""); ss.clear();
    ss<<fixed<<setprecision(3)<<current;
    std::string currentString = ss.str();

    command = "APPLy CH" + ch + "," + voltString + "," + currentString + "\n";
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);
    //system("PAUSE");
    delete pStrout;
}

void DP800::powerEnable(int chanel, bool onOff){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    /*Select CHANEL*/
	command = ":OUTP CH";
	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + ",";

	if(onOff) command = command + "ON"; else command = command + "OFF";
	sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


void DP800::calibrationStart(int chanel, std::string password){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    //default password is 11111
    if(password.size() == 0)password = "11111";

    command = ":CALibration:Start ";
    command = command + password + ",CH";

	ss << chanel;
	std::string ch = ss.str();
	command = command + ch;

    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


void DP800::calibrationClear(int chanel, std::string type){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    command = ":CALibration:Clear CH";

	ss << chanel;
	std::string ch = ss.str();
	command = command + ch + "," + type;

    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


void DP800::calibrationSet(int chanel, std::string mode, int step, float value, std::string dev){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    ss << chanel;
	std::string ch = ss.str();
	ss.str("");
    ss.clear();
	ss << step;
	std::string stepString = ss.str();
	std::string tgt_valString = float_to_string(value);



    command = ":CALibration:Set CH" + ch +"," + mode + "," + stepString + "," + tgt_valString + "," + dev;
    //cout<<command<<endl;
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);
    delete pStrout;
}


void DP800::calibrationMEAS (int chanel, std::string mode, int step, float value, std::string dev){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

/*calculate chanel int to string*/
    ss << chanel;
	std::string ch = ss.str();

/*calculate step int to string*/
	ss.str("");
    ss.clear();
	ss << step;
	std::string stepString = ss.str();

/* calculate value float to string*/
	std::string tgt_valString = float_to_string(value);
    command = ":CALibration:MEAS CH" + ch +"," + mode + "," + stepString + "," + tgt_valString + "," + dev;

    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);
    delete pStrout;
}


void DP800::calibrationEnd(std::string date, int chanel){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    ss << chanel;
	std::string ch = ss.str();
	command = ":CALibration:End " + date + ",CH" + ch;

    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);
    delete pStrout;
}

void DP800::displayMode(std::string mode){
    len = 64;
    pStrout = new char[len];
    std::string command;

    command = ":DISPlay:MODE " + mode;

    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}

void DP800::dislayClear (void){
    len = 64;
    pStrout = new char[len];
    std::string command;

    command = ":DISP:TEXT:CLE";

    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;

}

void DP800::dislayText (std::string tekst, int x, int y){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    ss << x;
	std::string xString = ss.str();
	ss.str("");
    ss.clear();
	ss << y;
	std::string yString = ss.str();

    command = ":DISP:TEXT \"" + tekst + "\"," + xString + "," + yString;
    //cout<<command<<endl;
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), false);

    delete pStrout;
}


float DP800::getPower(int chanel){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    ss << chanel;
	std::string ch = ss.str();

    command = ":MEAS:POWE? CH" + ch;
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), true);
    std::string valueString(pStrin, pStrin + lenCharArray(pStrin));
    //cout<<pStrin;
    float pomiar = strtof((valueString).c_str(), 0);

    delete pStrout;
    return pomiar;
}

float DP800::getVoltage(int chanel){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    ss << chanel;
	std::string ch = ss.str();

    command = ":MEASure:VOLTage? CH" + ch;
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), true);

    std::string valueString(pStrin, pStrin + lenCharArray(pStrin));
    float pomiar = strtof((valueString).c_str(), 0);

    delete pStrout;
    return pomiar;
}



float DP800::getCurrent(int chanel){
    len = 64;
    pStrout = new char[len];
    std::string command;
    ostringstream ss;

    ss << chanel;
	std::string ch = ss.str();

    command = ":MEAS:CURR? CH" + ch;
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), true);
    std::string valueString(pStrin, pStrin + lenCharArray(pStrin));
    float pomiar = strtof((valueString).c_str(), 0);

    delete pStrout;
    return pomiar;
}

unsigned char  DP800::getEventRegister(){
    len = 64;
    pStrout = new char[len];
    std::string command;

    command = "*ESR?";
    sprintf(pStrout, "%s", command.c_str());
    sendCommand(pStrout, pStrin, command.length(), true);
/*    for(int i=0; i!=8; i++){
        if(pStrin[0] & 0x80){ cout<<"1";}
            else { cout<<"0";}
        pStrin[0] <<= 1;
    }*/
    //std::string valueString(pStrin, pStrin + lenCharArray(pStrin));
    //cout<<valueString<<endl;
    delete pStrout;
    return pStrin[0];
}


int DP800::lenCharArray(unsigned char *pStrin){
    int i;
    for(i=0; i!=10; i++){
        if(pStrin[i] == 0) break;
    }
    return i;
}
