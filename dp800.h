/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dp800.h
 * Author: fajnysoft@gmail.com
 *
 * Created on 1 kwietnia 2017, 16:50
 */

#ifndef DP800_H
#define DP800_H

//#include "./../StdAfx.h"
#include "visa.h"
#include <windows.h>
#include <string>
//using namespace std;

#define DAC "1"
#define ADC "0"

//extern HINSTANCE hUSBTMCLIB;
class DP800 {

public:
    HINSTANCE hUSBTMCLIB;
    unsigned long m_defaultRM_usbtmc, m_instr_usbtmc;
    unsigned long m_findList_usbtmc;
    unsigned long m_nCount;
    ViStatus status;
    const static int m_Timeout = 700;
    char *pStrout; // Write out data buffer
    unsigned char pStrin[64]; // Read in data buffer
    int len;
    unsigned long nWritten;
    unsigned long  nRead;
    char buffer[256];
    char instrDescriptor[256];
    DP800();
    DP800(std::string deviceId);
    virtual ~DP800();
    void sendCommand(char*, unsigned char*, int, bool);
    std::string float_to_string( float liczba );
    float read();
    void setV(int, float);
    void setI(int, float);
    void setVI(int, float, float);
	void powerEnable(int, bool);
	void calibrationStart(int, std::string);
	void calibrationClear(int, std::string);
	void calibrationSet(int, std::string, int, float, std::string);
	void calibrationMEAS(int , std::string, int, float, std::string);
	void calibrationEnd(std::string, int);
	void displayMode(std::string);
	void dislayClear (void);
	void dislayText (std::string, int, int);
    void setText(std::string);

    /**\<html><h3>zwraca moc wyj�ciow� w kanale 'ch'</h3><p></p></html>
     * \param ch int
     * \return float
     */
    float getPower(int);

    /**
    \<html><h3>zwraca napi�cie wyj�ciowe w kanale 'ch'</h3><p></p></html>
     * \param ch int
     * \return float
     */
    float getVoltage(int);

    /** \brief
     *
     <html><h3>zwraca pr�d wyj�ciowy w kanale 'ch'</h3><p></p></html>
     * \param ch int
     * \return float
     *
     */
    float getCurrent(int);

    unsigned char getEventRegister(void);

private:
    int lenCharArray(unsigned char*);
};

#endif /* DP800_H */

