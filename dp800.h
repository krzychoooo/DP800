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
	void calibrationStart(int, std::string);    //chanel , password default "11111"
	void calibrationClear(int, std::string);    //clears out the current calibration tables  (chanel, "V"|"C"|"ALL")

    /** \brief
    *\:CALibration:Set <ch>,<mode>,<step>,<tgt_val>,<dev>
    *\<ch> = "CH1", "CH2", "CH3"
    *\<mode> = "V" or "C"
    *\<step> = 0..n
    *\<tgt_val> ideal value (e.g. 0.0V or 0.1A)
    *\<dev> : 0 = ADC, 1 = DAC
     * \param
     * \param
     * \return
     */
	void calibrationSet(int, std::string, int, float, std::string);

	void calibrationMEAS(int , std::string, int, float, std::string);
	void calibrationEnd(std::string, int);      // <date> (e.g. 11/22/2014) , CHANEL
	void displayMode(std::string);      //{NORMal|WAVE|DIAL|CLAS}
	void dislayClear (void);
	void dislayText (std::string, int, int);
    void setText(std::string);

    /**\<html><h3>zwraca moc wyjœciow¹ w kanale 'ch'</h3><p></p></html>
     * \param ch int
     * \return float
     */
    float getPower(int);

    /**
    \<html><h3>zwraca napiêcie wyjœciowe w kanale 'ch'</h3><p></p></html>
     * \param ch int
     * \return float
     */
    float getVoltage(int);

    /** \brief
     *
     <html><h3>zwraca pr¹d wyjœciowy w kanale 'ch'</h3><p></p></html>
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

