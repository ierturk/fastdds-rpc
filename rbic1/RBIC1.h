/******************************************************************************************
   File name: RBIC1.h

   This file contains all the DLL interfacing object declarations
   Notice: we use the same header file for compiling the .DLL and the .exe (application).
   This header file defines a macro which export the target DLL objects if we are building
   a DLL, otherwise it import the DLL objects into an application which uses the DLL. 
   If we define DLLDIR_EX (a preprocessor identifier), then the preprocessor define macro
   DLLDIR (a mnemonic for DLL import/export Direction) becomes an export instruction,
   otherwise its an import instruction by default. 
*******************************************************************************************/ 

#include <windows.h> // for BOOL type

#ifdef DLLDIR_EX
   #define DLLDIR  __declspec(dllexport)   // export DLL information
#else
   #define DLLDIR  __declspec(dllimport)   // import DLL information
#endif 

namespace RBIC1 {
    // The extern "C" declaration allows mixed languages compatibility,
    // it prevents the C++ compiler from using decorated (modified)
    // names for the functions 
    extern "C" { 
        DLLDIR BOOL OutLin(float Zp1m, float Zp2m, float A,   float B,   float *C0, float *C1);
        DLLDIR BOOL TLin  (float Ztmed,float Ztupp,float Tmed,float Tupp,float *Ct0,float *Ct1);

        DLLDIR BOOL OutQuad (float Zp1m, float Zp2m ,float Zp3m,             float A, float B, float M,           int adc_reso, float *C0, float *C1, float *C2); 
        DLLDIR BOOL OutThird(float Zp1m, float Zp2m ,float Zp3m, float Zp4m, float A, float B, float M, float M2, int adc_reso, float *C0, float *C1, float *C2, float *C3); 	

        DLLDIR BOOL TQuad  (float Ztlow,float Ztupp,float Ztmed,float Tlow, float Tupp, float Tmed, int adc_reso, float *Ct0,float *Ct1,float *Ct2); 

        DLLDIR BOOL OutLinTLin  (float Zp1m, float Zp2m,                         float Zp1u, float Zp2u,     float A,     float B,                           float Ztmed, float Ztupp, int adc_reso, float *C0, float *C1,            float *C4,            float *C6); 
        DLLDIR BOOL OutQuadTLin (float Zp1m, float Zp2m, float Zp3m,             float Zp1u, float Zp2u,     float A,     float B,     float M,              float Ztmed, float Ztupp, int adc_reso, float *C0, float *C1, float *C2, float *C4,            float *C6);
    //	DLLDIR BOOL OutThirdTLin(float Zp1m, float Zp2m ,float Zp3m, float Zp4m, float Zp1u, float Zp2u, float soll1, float soll2, float soll3, float soll4, float Ztmed, float Ztupp, int adc_reso, float *C0, float *C1, float *C2, float *C3, float *C4, float *C6);

        DLLDIR BOOL OutLinTQuad  (float Zp1m, float Zp2m,                         float Zp1u, float Zp2u, float Zp1l, float Zp2l, float A, float B,                    float Ztmed, float Ztupp, float Ztlow, int adc_reso, float *C0, float *C1,                       float *C4, float *C6, float *C5, float *C7);
        DLLDIR BOOL OutQuadTQuad (float Zp1m, float Zp2m, float Zp3m,             float Zp1u, float Zp2u, float Zp1l, float Zp2l, float A, float B, float M,           float Ztmed, float Ztupp, float Ztlow, int adc_reso, float *C0, float *C1, float *C2,            float *C4, float *C6, float *C5, float *C7);

        DLLDIR long	ZMD31050_cal1(float Zp1m, float Zp2m ,float Zp3m, float Zp4m, float Zp1u, float Zp2u, float Zp1l, float Zp2l, float A, float B, float M, float M2, float Ztmed, float Ztupp, float Ztlow, int adc_reso, float *C0, float *C1, float *C2, float *C3, float *C4, float *C6, float *C5, float *C7);
        DLLDIR long	ZMD31050_sim1(int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7, int adc_reso, float range_shift, int izmin, int izmax, int ZT, int ZTmin, int ZTmax );

        DLLDIR char* getMessage();

        DLLDIR BOOL DLLVersion ( char* version );
    };

    extern "C" DLLDIR char* msgRBIC1;
    extern "C" DLLDIR long rcRBIC1;
}