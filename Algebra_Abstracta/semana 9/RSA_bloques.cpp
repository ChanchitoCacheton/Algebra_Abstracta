#include "RSA_bloques.h"

using namespace std;

 string Abc=("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

RSA_bloques::RSA_bloques(int numbits){

    operacion x_0;
    GenPrime(p,numbits);
    GenPrime(q,numbits);

    while(p==q || x_0.mcd(p,q)!=1){

        GenPrime(p,numbits);
        GenPrime(q,numbits);
    }
    N=generarClave_N(p,q);
    phi_n=((p-1)*(q-1));

    e=generarClave_e(phi_n,numbits);
    d=generarClave_d(e,phi_n);

}
RSA_bloques::RSA_bloques(ZZ e,ZZ N){

    this->e=e;
    this->N=N;
    operacion x_0;
    phi_n=N-1;

    d=generarClave_d(e,phi_n);

}

//generar claves
ZZ RSA_bloques::generarClave_N(ZZ p,ZZ q){
    ZZ N=p*q;
    return N;
}
ZZ RSA_bloques::generarClave_e(ZZ phi_n,int numbits){

    operacion x;
    GenPrime(e,numbits);
    while (x.mcd(e,phi_n)!= 1){
        GenPrime(e,numbits);
	}
	return e;
}
ZZ RSA_bloques::generarClave_d(ZZ e,ZZ phi_n){
    operacion x_1;
    ZZ d=x_1.inverso(e,phi_n);

    return d;

}
//utilidades
int RSA_bloques::hallar_nINT(int x){

        string tam_size;
        int len;
        tam_size=ZZToStr(ZZ(x));
        len=tam_size.length();
        return len;
}
int RSA_bloques::hallar_nZZ(ZZ x){

        string tam_size;
        int len;
        tam_size=ZZToStr(x);
        len=tam_size.length();
        return len;

}
//conversiones
string RSA_bloques::ZZToStr(ZZ numero){
    stringstream buffer;
    buffer<<numero;
    return buffer.str();
    
}
int RSA_bloques::StrToInt(string mensaje){
    stringstream ss(mensaje);
    int resultado;
    ss>>resultado;
    return resultado;
}
ZZ RSA_bloques::StrToZZ(string mensaje){
    stringstream ss(mensaje);
    ZZ resultado;
    ss>>resultado;
    return resultado;
}
//herramientas

string RSA_bloques::crearBloque_ZZtoStr(ZZ n,int tam){
    string bloque=ZZToStr(n);
    int tam_bloque=bloque.length();
    bloque.insert(0,tam-tam_bloque,'0');
    return bloque;
}
string RSA_bloques::completarBloque(string bloque,int tam_N_1){

        int tam_bloque=bloque.length();
        bloque.insert(tam_bloque,1,'2');
        bloque.insert(tam_bloque,1,'2');
        return bloque;
}
string RSA_bloques::llenarBloques_ceros(string bloque,int tam){
        int tam_bloque=bloque.length();
        bloque.insert(0,tam-tam_bloque,'0');
        return bloque;
}
string RSA_bloques::cadenaBloques(string mensaje,int tam){

        ostringstream msjNum;    
        for(int i=0;i<mensaje.length();i++){
                ZZ digito=ZZ(Abc.find(mensaje[i])); 
                string bloque=crearBloque_ZZtoStr(digito,tam);
                msjNum<<bloque;
        }
        return msjNum.str();
}

// cifrado 
string RSA_bloques::cifrado(string mensaje){
    operacion x_2;
    string msj_cifrado;
    operacion x_3,x_4,x_5;
    //hallar el numero de digitos de <N>
    int len_N=hallar_nZZ(N);
    cout<<"->tam de N :"<<len_N<<endl;
    //hallar el numero de digitos de la ultima letra del Abc
    int tam_ultimodig_abc=hallar_nINT(Abc.length());
    cout<<"->tam de d: "<<tam_ultimodig_abc<<endl;
    //variable donde se guardaran el digito en el Abc de la letra
    ZZ digito_letra;
    //variable donde el digito se convertira en un bloque
    string bloque,bloque_cifrado; 
    string cadena_bloques;


    //bucle 1: de Mensaje a cadena de digitos con bloques
    
    msj_cifrado=cadenaBloques(mensaje,tam_ultimodig_abc);
    cout<<"[PASO 1]el mensaje con digitos completado con ceros :"<<endl;
    cout<<msj_cifrado<<endl;

    //bucle 2: agrupar en bloques de n-1 y aplicar exp_modular
    for(int i = 0; i < msj_cifrado.length(); i+=(len_N-1))
    {
        if(i+(len_N-1)<=msj_cifrado.length()){
            cout<<"[PASO 2]bloque para cifrar ENCONTRADO:"<<endl;
            bloque=msj_cifrado.substr(i,(len_N-1));
            cout<<"el bloque a cifrar es: "<<endl;
            cout<<bloque;

            digito_letra=StrToZZ(bloque);
            cout<<"->el bloque transformado en numeros es: "<<digito_letra<<endl;
            ZZ M=ZZ(digito_letra);
            digito_letra=x_2.exp_modular(M,e,N);
            cout<<"->el numero exponenciado resulta: "<<digito_letra<<endl;
            bloque_cifrado=ZZToStr(digito_letra);
            

            if(bloque_cifrado.length()<N){
                bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                cout<<"el bloque cifrado llenado de ceros"<<endl;
                cout<<bloque_cifrado<<endl;
            }
            cadena_bloques+=bloque_cifrado;
        }
        else{
            cout<<"[PASO EMERGENCIA] : ya no hay bloques exactos, hay que completar con W o '23'"<<endl;
            cout<<"->posicion del i :"<<i<<endl;
            cout<<"-> si "<<i<<" + "<<len_N-1<<" se salen del bucle entonces evaluamos hasta que el bloque sea exacto"<<endl;
            bloque= msj_cifrado.substr(i,msj_cifrado.length());
            while(bloque.length()!=(len_N-1)){

                if(bloque.length()<(len_N-1)){
                    cout<<"EVALUACION"<<endl;
                    cout<<"->el bloque es menor que N-1, hay que llenarlo de W o 23"<<endl;
                    cout<<"->bloque actual :"<<bloque<<endl;
                    bloque=completarBloque(bloque,(len_N-1));
                    cout<<"->bloque completado :"<<bloque<<endl;
                }
                if(bloque.length()>(len_N-1)){
                    cout<<"EVALUACION"<<endl;
                    cout<<"->el bloque es mayor que N-1, hay que dividirlo en un bloque exacto y un bloque menor y a ese menor llenarlo de W o 23"<<endl;
                    cout<<"->bloque actual :"<<bloque<<endl;
                    string temp=bloque.substr(0,len_N-1);
                    cout<<"->primer bloque(el que sera exacto y se cifrara y despues se unira a los bloques ya cifrados :"<<temp<<endl;
                    digito_letra=StrToZZ(temp);
                    cout<<"->el bloque transformado en numeros es: "<<digito_letra<<endl;
                    ZZ M=ZZ(digito_letra);
                    digito_letra=x_2.exp_modular(M,e,N);
                    cout<<"->el numero exponenciado resulta: "<<digito_letra<<endl;
                    bloque_cifrado=ZZToStr(digito_letra);
                    
                    if(bloque_cifrado.length()<N){
                        bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                        cout<<"el bloque cifrado llenado de ceros"<<endl;
                        cout<<bloque_cifrado<<endl;
                    }
                    cadena_bloques+=bloque_cifrado;

                    cout<<"el segundo bloque ahora sera el nuevo bloque a evaluar"<<endl;
                    string temp_2=bloque.substr(temp.length(),bloque.length());
                    
                    bloque=temp_2;
                    cout<<"el segundo y actual bloque es :"<<bloque<<endl;
                    
                }
                if(bloque.length()==len_N-1){
                    digito_letra=StrToZZ(bloque);
                    ZZ M=ZZ(digito_letra);
                    digito_letra=x_2.exp_modular(M,e,N);
                    bloque_cifrado=ZZToStr(digito_letra);
                    if(bloque_cifrado.length()<N){
                        bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                    }
                    cadena_bloques+=bloque_cifrado;   
                }   
            }
        }
    }
    
    msj_cifrado=cadena_bloques;
    
    //bucle 3: llenar con ceros si el bloque no es de tam N
    

    
    
    return msj_cifrado;
}
//decifrado

string RSA_bloques::decifrado(string msj_cifrado){
    string mensaje;
    operacion x_3;
    

    int len_N=hallar_nZZ(N);
    string bloque_cifrado;
    string bloque_decifrado;
    string cadenaBloques_decifrado;
    ZZ digito_cifrado;
    ZZ digito_decifrado;
    int pos_letra;

    for(int i = 0; i < msj_cifrado.length(); i+=(len_N))
    {
        cout<<"[PASO 3]separamos en bloques de N"<<endl;

        
        bloque_cifrado=msj_cifrado.substr(i,len_N);
        cout<<"->bloque cifrado ENCONTRADO: "<<bloque_cifrado<<endl;
        digito_cifrado=StrToZZ(bloque_cifrado);
        cout<<"->bloque cifrado transformado a numero: "<<digito_cifrado<<endl;
        //ZZ C=ZZ(digito_cifrado);
        digito_decifrado=x_3.exp_modular(digito_cifrado,d,N);
        cout<<"->digito decifrado :"<<ZZToStr(digito_decifrado)<<endl;

        bloque_decifrado=ZZToStr(digito_decifrado);
        cout<<"->el bloque decifrado es"<<bloque_decifrado;

        bloque_decifrado=llenarBloques_ceros(bloque_decifrado,len_N-1);
        cout<<"->el bloque decifrado llenado de ceros es: "<<endl;
        cout<<bloque_decifrado;

        cadenaBloques_decifrado+=bloque_decifrado;
        
        
    }
    cout<<"[PASO4] la compilacion bloques decifrados llevados a N-1 :"<<endl;
   
    cout<<cadenaBloques_decifrado<<endl;
    
    int tam_ultimodig_abc=hallar_nINT(Abc.length());

     for(int i = 0; i < cadenaBloques_decifrado.length(); i+=tam_ultimodig_abc)
    {   
        string digito=cadenaBloques_decifrado.substr(i,tam_ultimodig_abc);
        //cout<<"[PASO 5] separar cada dato en tam de d="<<tam_ultimodig_abc<<endl;
        
        int pos=StrToInt(digito);
        /*
        ZZ pos_zz=x_3.modulo(ZZ(pos),ZZ(Abc.length()));
        string pos_str=ZZToStr(pos_zz);
        pos=StrToInt(pos_str);
        */
        //cout<<"->posicion de letra en el abcedario : "<<pos<<endl;
        mensaje+=Abc[pos];
        //cout<<"->Letra : "<<Abc[pos]<<endl;
    }
    
    

    return mensaje;
}

string RSA_bloques::rubrica(string msj_cifrado){

    string rubrica;
    operacion x_2;
    operacion x_3,x_4,x_5;
    //hallar el numero de digitos de <N>
    int len_N=hallar_nZZ(N);
    cout<<"->tam de N :"<<len_N<<endl;
    //hallar el numero de digitos de la ultima letra del Abc
    int tam_ultimodig_abc=hallar_nINT(Abc.length());
    cout<<"->tam de d: "<<tam_ultimodig_abc<<endl;
    //variable donde se guardaran el digito en el Abc de la letra
    ZZ digito_letra;
    //variable donde el digito se convertira en un bloque
    string bloque,bloque_cifrado; 
    string cadena_bloques;


    //bucle 2: agrupar en bloques de n-1 y aplicar exp_modular
    for(int i = 0; i < msj_cifrado.length(); i+=(len_N-1))
    {
        if(i+(len_N-1)<=msj_cifrado.length()){
            cout<<"[PASO 2]bloque para cifrar ENCONTRADO:"<<endl;
            bloque=msj_cifrado.substr(i,(len_N-1));
            cout<<"el bloque a cifrar es: "<<endl;
            cout<<bloque;

            digito_letra=StrToZZ(bloque);
            cout<<"->el bloque transformado en numeros es: "<<digito_letra<<endl;
            ZZ M=ZZ(digito_letra);
            digito_letra=x_2.exp_modular(M,d,N);
            cout<<"->el numero exponenciado resulta: "<<digito_letra<<endl;
            bloque_cifrado=ZZToStr(digito_letra);
            

            if(bloque_cifrado.length()<N){
                bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                cout<<"el bloque cifrado llenado de ceros"<<endl;
                cout<<bloque_cifrado<<endl;
            }
            cadena_bloques+=bloque_cifrado;
        }
        else{
            cout<<"[PASO EMERGENCIA] : ya no hay bloques exactos, hay que completar con W o '23'"<<endl;
            cout<<"->posicion del i :"<<i<<endl;
            cout<<"-> si "<<i<<" + "<<len_N-1<<" se salen del bucle entonces evaluamos hasta que el bloque sea exacto"<<endl;
            bloque= msj_cifrado.substr(i,msj_cifrado.length());
            while(bloque.length()!=(len_N-1)){

                if(bloque.length()<(len_N-1)){
                    cout<<"EVALUACION"<<endl;
                    cout<<"->el bloque es menor que N-1, hay que llenarlo de W o 23"<<endl;
                    cout<<"->bloque actual :"<<bloque<<endl;
                    bloque=completarBloque(bloque,(len_N-1));
                    cout<<"->bloque completado :"<<bloque<<endl;
                }
                if(bloque.length()>(len_N-1)){
                    cout<<"EVALUACION"<<endl;
                    cout<<"->el bloque es mayor que N-1, hay que dividirlo en un bloque exacto y un bloque menor y a ese menor llenarlo de W o 23"<<endl;
                    cout<<"->bloque actual :"<<bloque<<endl;
                    string temp=bloque.substr(0,len_N-1);
                    cout<<"->primer bloque(el que sera exacto y se cifrara y despues se unira a los bloques ya cifrados :"<<temp<<endl;
                    digito_letra=StrToZZ(temp);
                    cout<<"->el bloque transformado en numeros es: "<<digito_letra<<endl;
                    ZZ M=ZZ(digito_letra);
                    digito_letra=x_2.exp_modular(M,d,N);
                    cout<<"->el numero exponenciado resulta: "<<digito_letra<<endl;
                    bloque_cifrado=ZZToStr(digito_letra);
                    
                    if(bloque_cifrado.length()<N){
                        bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                        cout<<"el bloque cifrado llenado de ceros"<<endl;
                        cout<<bloque_cifrado<<endl;
                    }
                    cadena_bloques+=bloque_cifrado;

                    cout<<"el segundo bloque ahora sera el nuevo bloque a evaluar"<<endl;
                    string temp_2=bloque.substr(temp.length(),bloque.length());
                    
                    bloque=temp_2;
                    cout<<"el segundo y actual bloque es :"<<bloque<<endl;
                    
                }
                if(bloque.length()==len_N-1){
                    digito_letra=StrToZZ(bloque);
                    ZZ M=ZZ(digito_letra);
                    digito_letra=x_2.exp_modular(M,d,N);
                    bloque_cifrado=ZZToStr(digito_letra);
                    if(bloque_cifrado.length()<N){
                        bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                    }
                    cadena_bloques+=bloque_cifrado;   
                }   
            }
        }
    }
    
    
    //bucle 3: llenar con ceros si el bloque no es de tam N
    

    
    
    
    
    msj_cifrado=cadena_bloques;
    rubrica=msj_cifrado;
    return rubrica;

}
string RSA_bloques::firma_digital(string rubrica){
    string firma_digital;
    operacion x_2;
    operacion x_3,x_4,x_5;
    //hallar el numero de digitos de <N>
    int len_N=hallar_nZZ(N);
    cout<<"->tam de N :"<<len_N<<endl;
    //hallar el numero de digitos de la ultima letra del Abc
    int tam_ultimodig_abc=hallar_nINT(Abc.length());
    cout<<"->tam de d: "<<tam_ultimodig_abc<<endl;
    //variable donde se guardaran el digito en el Abc de la letra
    ZZ digito_letra;
    //variable donde el digito se convertira en un bloque
    string bloque,bloque_cifrado; 
    string cadena_bloques;


    //bucle 1: de Mensaje a cadena de digitos con bloques
    

    //bucle 2: agrupar en bloques de n-1 y aplicar exp_modular
    for(int i = 0; i < rubrica.length(); i+=(len_N-1))
    {
        if(i+(len_N-1)<=rubrica.length()){
            cout<<"[PASO 2]bloque para cifrar ENCONTRADO:"<<endl;
            bloque=rubrica.substr(i,(len_N-1));
            cout<<"el bloque a cifrar es: "<<endl;
            cout<<bloque;

            digito_letra=StrToZZ(bloque);
            cout<<"->el bloque transformado en numeros es: "<<digito_letra<<endl;
            ZZ M=ZZ(digito_letra);
            digito_letra=x_2.exp_modular(M,e,N);
            cout<<"->el numero exponenciado resulta: "<<digito_letra<<endl;
            bloque_cifrado=ZZToStr(digito_letra);
            

            if(bloque_cifrado.length()<N){
                bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                cout<<"el bloque cifrado llenado de ceros"<<endl;
                cout<<bloque_cifrado<<endl;
            }
            cadena_bloques+=bloque_cifrado;
        }
        else{
            cout<<"[PASO EMERGENCIA] : ya no hay bloques exactos, hay que completar con W o '23'"<<endl;
            cout<<"->posicion del i :"<<i<<endl;
            cout<<"-> si "<<i<<" + "<<len_N-1<<" se salen del bucle entonces evaluamos hasta que el bloque sea exacto"<<endl;
            bloque= rubrica.substr(i,rubrica.length());
            while(bloque.length()!=(len_N-1)){

                if(bloque.length()<(len_N-1)){
                    cout<<"EVALUACION"<<endl;
                    cout<<"->el bloque es menor que N-1, hay que llenarlo de W o 23"<<endl;
                    cout<<"->bloque actual :"<<bloque<<endl;
                    bloque=completarBloque(bloque,(len_N-1));
                    cout<<"->bloque completado :"<<bloque<<endl;
                }
                if(bloque.length()>(len_N-1)){
                    cout<<"EVALUACION"<<endl;
                    cout<<"->el bloque es mayor que N-1, hay que dividirlo en un bloque exacto y un bloque menor y a ese menor llenarlo de W o 23"<<endl;
                    cout<<"->bloque actual :"<<bloque<<endl;
                    string temp=bloque.substr(0,len_N-1);
                    cout<<"->primer bloque(el que sera exacto y se cifrara y despues se unira a los bloques ya cifrados :"<<temp<<endl;
                    digito_letra=StrToZZ(temp);
                    cout<<"->el bloque transformado en numeros es: "<<digito_letra<<endl;
                    ZZ M=ZZ(digito_letra);
                    digito_letra=x_2.exp_modular(M,e,N);
                    cout<<"->el numero exponenciado resulta: "<<digito_letra<<endl;
                    bloque_cifrado=ZZToStr(digito_letra);
                    
                    if(bloque_cifrado.length()<N){
                        bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                        cout<<"el bloque cifrado llenado de ceros"<<endl;
                        cout<<bloque_cifrado<<endl;
                    }
                    cadena_bloques+=bloque_cifrado;

                    cout<<"el segundo bloque ahora sera el nuevo bloque a evaluar"<<endl;
                    string temp_2=bloque.substr(temp.length(),bloque.length());
                    
                    bloque=temp_2;
                    cout<<"el segundo y actual bloque es :"<<bloque<<endl;
                    
                }
                if(bloque.length()==len_N-1){
                    digito_letra=StrToZZ(bloque);
                    ZZ M=ZZ(digito_letra);
                    digito_letra=x_2.exp_modular(M,e,N);
                    bloque_cifrado=ZZToStr(digito_letra);
                    if(bloque_cifrado.length()<N){
                        bloque_cifrado=llenarBloques_ceros(bloque_cifrado,len_N);
                    }
                    cadena_bloques+=bloque_cifrado;   
                }   
            }
        }
    }
    rubrica=cadena_bloques;
    firma_digital=rubrica;


    return firma_digital;
}