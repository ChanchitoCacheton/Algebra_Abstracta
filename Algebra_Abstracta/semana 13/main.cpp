//#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Image.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <vector>
#include <bitset>
#include <NTL/ZZ.h>
#include <sstream>

using namespace std;
using namespace sf;
using namespace NTL;

NTL_CLIENT;

int Modulo(int a,int n){
    int res=a-n*(a/n);
    if(res<0)
        res+=n;
    return res;
}
ZZ StrToInt(string numero){
    stringstream ss(numero);
    ZZ resultado;
    ss>>resultado;
    return resultado;
}
ZZ Stringtozz(string x)
{
    stringstream buffer(x);
    ZZ y;
    buffer >> y;
    return y;
}

vector<int> loadImg(int bit){
    Image img;
    img.loadFromFile("shrek.jpg");
    vector<int> k;
    for(int i=0 ; i<bit; i++){
        Color c_rgb = img.getPixel(i,0);
        k.push_back(c_rgb.r);
        k.push_back(c_rgb.g);
        k.push_back(c_rgb.b);
    }
    return k;
}

vector<int> taps(int nb){
    vector<int> tps;
    int times=nb/10;
    for(int i=0;i<times;i++){
        tps.push_back(rand()%nb);
        int j=i;
        while(tps[i]==tps[j]){
            tps[j]=rand()%nb;
            j--;
        }
    }
    return tps;
}

/*string rc4(int n){
    vector<int> image=loadImg();
    string res;
    //Inicializaci�n
    vector<int> S;
    vector<int> K;
    int f=0;
    int t=0;
    for(int i=0;i<256;i++){
        if(i<n)
            K.push_back(image[i]);
        if(i>=n)
            K.push_back(image[i-n]);
        S.push_back(i);
    }
    for(int i=0;i<256;i++){
        f=Modulo((f+S[i]+K[i]),256);
        int tmp=S[i];
        S[i]=S[f];
        S[f]=tmp;
    }
    //Generar Secuencia
    int i=0;
    f=0;
    while(res.length()<256){
    i=Modulo(i+1,256);
    f=Modulo(f+S[i],256);
    int tmp=S[i];
    S[i]=S[f];
    S[f]=tmp;
    t=Modulo(S[i]+S[f],256);
    bitset<8> res1(S[t]);
    res+=res1.to_string();
    }
    return res;
}*/

string rc4(int n,int numbits){
    vector<int> image=loadImg(numbits);
    string res;
    //Inicializaci�n
    vector<int> S;
    vector<int> K;
    int f=0;
    int t=0;
    for(int i=0;i<numbits;i++){
        if(i<n)
            K.push_back(image[i]);
        if(i>=n)
            K.push_back(image[i-n]);
        S.push_back(i);
    }
    for(int i=0;i<numbits;i++){
        f=Modulo((f+S[i]+K[i]),numbits);
        int tmp=S[i];
        S[i]=S[f];
        S[f]=tmp;
    }
    //Generar Secuencia
    int i=0;
    f=0;
    while(res.length()<numbits){
    i=Modulo(i+1,numbits);
    f=Modulo(f+S[i],numbits);
    int tmp=S[i];
    S[i]=S[f];
    S[f]=tmp;
    t=Modulo(S[i]+S[f],numbits);
    bitset<8> res1(S[t]);
    res+=res1.to_string();
    }
    return res;
}


template<int numbits>
string lfsr(int sd){
    vector<int> tap=taps(numbits);
    string res;
    string block1;
    string block2;
    string seed=rc4(sd,numbits);//seed rc4
    string seed2;

    int tsum=0;//suma de los taps
    int ct0=0;
    int ct1=0;
    for(int i=0;i<numbits;i++){
        if(i<numbits/10)//10% de los bits
            seed2+=seed[i];
        if(i>=numbits/10){
            int tmp=seed2[i-(numbits/10)]-'0';
            int tmp2=seed2[i-(numbits/10)+1]-'0';
            seed2+=to_string(Modulo(tmp+tmp2,2));
        }
        if(i<tap.size()){
            tsum+=tap[i];
        }
        if(seed2[i]=='0')
            ct0++;
        else
            ct1++;
    }
    while(ct0!=ct1){//igualando el numero de ceros y de unos
            int pos;
        if(ct0<ct1){
            pos=rand()%numbits;
            if(pos=='1'){
                seed2[pos]='0';
                ct0++;
                ct1--;
            }
        }
        else{
            pos=rand()%numbits;
            if(pos=='0'){
                seed2[pos]='1';
                ct0--;
                ct1++;
            }
        }
    }
    tsum=tsum%2;
    bitset<numbits> lsa(seed2);
    lsa<<=(1);
    lsa[0]=tsum;
    for(int i=lsa.size()/2;i>=0;i--){
            block1+=to_string(lsa[i]);
            block2+=to_string(lsa[i+lsa.size()/2]);
    }
    bitset<(numbits/2)> bl1(block1);

    if(lsa.size()%2==1){
        block2+=to_string(lsa[0]);
        bitset<(numbits/2)+1> bl2(block2);
        int tmp=bl1[bl1.size()-1];
        bl1<<=1;
        bl1[0]=tmp;
        tmp=bl2[0];
        bl2>>=1;
        bl2[bl2.size()-1]=tmp;
        res+=bl1.to_string();
        res+=bl2.to_string();
    }
    else{
        bitset<(numbits/2)> bl2(block2);
        int tmp=bl1[bl1.size()-1];
        bl1<<=1;
        bl1[0]=tmp;
        tmp=bl2[0];
        bl2>>=1;
        bl2[bl2.size()-1]=tmp;
        res+=bl1.to_string();
        res+=bl2.to_string();
    }
    ZZ tmp=ZZ(1);
    ZZ resultado;
    for(int i=0;i<res.size();i++){
        if(res[i]=='1'){
            tmp<<=(res.size()-1-i);
            resultado+=tmp;
            tmp=1;
        }    
    }
    //tmp<<2;
    //cout<tmp;
    cout<<resultado+2<<endl;
    // 0101001 
    return res;
}
int main()
{
    cout<<lfsr<1024>(8);
    cout<<"\n";
    return 0;
}
//g++ -g -O2 -std=c++11 -pthread -march=native -framework sfml-window -framework sfml-graphics -framework sfml-system main.cpp -lntl -lgmp -lm
//
