#include <iostream>
#include <string>
#include <sstream>


using namespace std;

int StrToInt(string mensaje){

    stringstream ss(mensaje);
    int resultado;
    ss>>resultado;
    return resultado;

}
int main(){

    string a("ggwp");

    string b=a.substr(a.length()-1,1);

    cout<<a.length()<<endl;

    string c("123");
    int d=StrToInt(c);
    cout<<a[0]<<endl;
    cout<<d<<endl;
    return 0;
}