#include<iostream>
#include<bitset>
#include<string>
#include<vector>

using namespace std;
void multiplicationX(bitset<6> *bits){
    bitset<6> res;
    if(!(*bits).test(5)){
        *bits<<=1;
        //cout<<(bits<<1)<<endl;
    }
    else{
        bitset<6> aux(string("000011"));
        (*bits<<=1)^=aux;
        //cout<<res<<endl;
    }
}
int main(){
    string fx, gx;
    vector<bitset<6>> resParciales;
    cout<<"enter the binary coefficients of f(x)"<<endl;
    cin>>fx;
    cout<<"enter the binary coefficients of g(x)"<<endl;
    cin>>gx;
    
    bitset<6> bitsF(fx), bitsG(gx),aux,fxgx;
    for(int i=5;i>0;i--){//recorre todo el arreglo de indices
        aux=bitsF;
        if(bitsG.test(i)){//si hay un elemento b_i encendido multiplica
            for(int j=i;j>0;j--)//hace la multiplicacion de cada elemento disminuyendo el grado en cada iteracion
                multiplicationX(&aux);
            //cout<<"x^"<<i<<"f(x) = "<<aux<<endl;
            resParciales.push_back(aux);
        }
    }
    if(bitsG.test(0))//agregar el +1
        resParciales.push_back(bitsF);

    fxgx=resParciales[0];
    for(int i=1;i<resParciales.size();i++){
        fxgx^=resParciales[i];
    }
    cout<<"f(x)g(x) mod m(x) = "<<fxgx<<endl;
    return 0;
}