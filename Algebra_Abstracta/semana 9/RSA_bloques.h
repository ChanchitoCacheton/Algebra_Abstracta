#pragma once

#define _RSA_BLOQUES_H_

#include <iostream>
#include <sstream>
#include <string>
#include <NTL/ZZ.h>

#include "operacion.h"

using namespace std;
using namespace NTL;

NTL_CLIENT;

class RSA_bloques{

    private:
        ZZ p,q;
        ZZ phi_n,d;
    public:
        ZZ  N,e;
        ZZ miN, mie;
        int numbits;

        //constructor
        RSA_bloques(int numbits);
        RSA_bloques(ZZ e,ZZ N);
        //generar
        ZZ generarClave_e(ZZ phi_n,int numbits);
        ZZ generarClave_N(ZZ p,ZZ q);
        ZZ generarClave_d(ZZ e,ZZ phi_n);
        //utilidades
        int hallar_nINT(int x);
        int hallar_nZZ(ZZ x);
        //conversiones
        string ZZToStr(ZZ numero);
        int StrToInt(string mensaje);
        ZZ StrToZZ(string mensaje);
        //herramientas
        string crearBloque_ZZtoStr(ZZ n,int tam);
        string completarBloque(string bloque,int tam_N_1);
        string cadenaBloques(string mensaje,int tam);
        string llenarBloques_ceros(string bloques,int tam); 


        string cifrado(string mensaje);
        string decifrado(string msj_cifrado);


        //firma digitial y rubrica
        string rubrica(string msj_cifrado);
        string firma_digital(string rubrica);
};
//g++ -g -O2 -std=c++11 -pthread -march=native test_1.cpp RSA_bloques.cpp  -lntl -lgmp -lm