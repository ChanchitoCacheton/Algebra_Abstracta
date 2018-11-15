#pragma once
#define _OPERACION_H_

#include <iostream>
#include <NTL/ZZ.h>
#include <vector>
#include <string>
using namespace std;
using namespace NTL;


NTL_CLIENT;


class operacion{


    public:
        ZZ a,b;

        operacion(){
            a=0;
            b=0;
        }
        operacion(ZZ a, ZZ b){
            this->a=a;
            this->b=b;
        }

        ZZ mcd(ZZ u, ZZ v){

            ZZ shift;
                if( u==0 ) return v;
                if( v==0 ) return u;
                for(shift=0;((u|v)& 1)==0;++shift)
                {
                u=u>>1;
                v=v>>1;
                }
                while((u&1)==0)
                    u=u>>1;
                do{
                    while(((v&1)==0))
                        v=v>>1;
                    if(u>v){
                        ZZ t(v);
                        v=u;
                        u=t;
                    }
                    v=v-u;      
                }while( v!=0);
                return u<<conv<int>(shift);
        }
        
        vector<ZZ> extendido(ZZ a,ZZ b){
            vector <ZZ> euclides_extendido;

            ZZ R,S,T;
            ZZ r_1=a;
            ZZ r_2=b;
            ZZ s_1=ZZ(1);
            ZZ s_2=ZZ(0);
            ZZ t_1=ZZ(0);
            ZZ t_2=ZZ(1);
            ZZ q;

            while(r_2>0){
        
                q=r_1/r_2;
                R=r_1-q*r_2;
                r_1=r_2;
                r_2=R;

                S=s_1-q*s_2;
                s_1=s_2;
                s_2=S;

                T=t_1-q*t_2;
                t_1=t_2;
                t_2=T;

            }

            S=s_1;
            T=t_1;

            euclides_extendido.push_back(R);
            euclides_extendido.push_back(S);
            euclides_extendido.push_back(T);

            return euclides_extendido;
        }
        ZZ inverso(ZZ a,ZZ b){
            vector <ZZ> euclides_extendido;
            ZZ d;
            euclides_extendido=extendido(a,b);
            d=euclides_extendido[1];
            if(d<0){
                d=b+d;
            }
            return d;
        }
        ZZ modulo(ZZ a,ZZ b){
            ZZ r=a-(a/b)*b;
            if(r<0){
                r=r+b;
                }
            return r;
        }

        ZZ exp_modular(ZZ a,ZZ b,ZZ n){
            ZZ res=ZZ(1);
            ZZ x=modulo(a,n);
            while(b>0){
                if(b%2!=0){
                    res=modulo((res*x),n); 
                }
                x=modulo((x*x),n);
                b=b/2;
            }
            return res;
        }
        


};