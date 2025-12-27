
#include "../../include/dice/dice.hpp"

using namespace Dc;

size_t Dc::rolling(size_t max){
                                    //função que pega um número aleatório de um intervalo de 1 até o selecionado.
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distrib(1, max);

return distrib(gen);
}

Dice Bag::pull_dice(size_t &dices_qnt){ //pega um dado aleatório da bolsa, e retira ele da bolsa
    
    size_t number = Dc::rolling(dices_qnt);     
    Dc::Dice temp = dices[number - 1];        //pega um dado de índice aleatório
    dices.erase(dices.begin() + number - 1); //retira da bolsa
    dices_qnt--;                             //-1 dado na bolsa
    return temp;

}

char Dice::roll() {return faces[rolling(6) - 1];}       //rola um dado de 6 faces.




