#ifndef DICE_ROLL
#define DICE_ROLL
#include <random>
#include <cstddef>
#include <map>
#include <vector>


namespace Dc{

    size_t rolling(size_t max);

    class Dice {                    //Literalmente um dado e tudo que um dado pode ter.
    public:
        std::string faces;          //possiveis faces.
        char roll();                //ação de rolar o dado.
    };
    class Bag {                     // Bolsa que armazena os dados.
    public:
        std::vector<Dice> original_dices;   // Como a bolsa é quando é cheia.
        std::vector<Dice> dices;            // Dados que podem sair quando puxados.

        Dice pull_dice(size_t &dices_qnt);  // Pega e tira um dado aleatório que está na bolsa.
    };

    class Player {                  //Informações auto-explicativas de cada jogador.
    public:
    std::string name;       
    size_t brains_qnt = 0;
    size_t life = 3;
    size_t points = 0;
    size_t turns_played = 0;
 
};
    
}
#endif