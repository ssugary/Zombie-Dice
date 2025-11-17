#ifndef GAME_FLOW
#define GAME_FLOW


#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>


#include "dice.hpp"



using namespace Dc;

namespace Gm {

class GameFlow {
  private:
  Bag dice_bag;               
  
  size_t green_dice     = 6;                //quantidades de cada dado (pode ser alterado no zdice.ini)
  size_t yellow_dice    = 4;
  size_t red_dice       = 3;
  size_t max_players    = 6;                //número máximo permitido de jogadores (pode ser alterado no zdice.ini)
  

  std::string green_dice_faces = "bbbffs";    //faces (podem ser alteradas no zdice.ini)
  std::string yellow_dice_faces = "bbffss";
  std::string red_dice_faces = "bffsss";

  std::vector<Dice> footprintDices;   //vetor que armazena todos os dados de pegada que puxarmos.
public:
  

  std::vector<Player> players;    //players que estão jogando
  std::vector<Player> podium;     //todos os players que entraram no jogo (vai ser útil para marcar a posição de cada jogador)

  std::string brain = "🧠: ";   //começo das strings (da mesa) da interface
  std::string shotgun = "💥: ";

  std::vector<char> Dices_face_in_game; //vetor que armazena quais faces já foram puxadas no turno de algum jogador
  std::vector<std::string> Table_dices; //vetor que armazena todos os tipos de dados (verde, vermelho, amarelo) que já foram puxadar no turno de algum jogador
  std::string Faces_on_table;           //string que armazena as faces que caíram após alguma jogada.
  std::vector<Dice> Dices_in_Game;      //vetor que armazena todos os dados que foram puxados no turno de algum jogador.

  size_t small_name     = 1;   //menor tamanho de nome possivel.
  size_t dice_qnt       = 13;  //informações que são alteraveis no zdice.ini
  size_t turn_player    = 0;
  size_t turns_qnt      = 1;
  size_t brains_to_win  = 13;
  size_t brains_in_turn;      //quantos cérebros foram pegos no turno.
  size_t shotguns_in_turn;    //quantas armas foram pegas no turno.


    void set_dices();   //prepara os dados do jogo.

    bool set_info();    //prepara as informações dos jogadores.
   
    void set_table();   //faz as strings que irão gerar a interface da "mesa" do jogo.

    bool game_config(int argc,char* argv[]);  //lê o arquivo .ini.
    
    void game_turn(bool &end_turn);         //joga os dados do jogador do turno.

    void reset_dices(size_t &dices_qnt);    //volta a bolsa para seu estado original.
  };
}
#endif