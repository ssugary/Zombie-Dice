#ifndef ZDICE_GAME
#define ZDICE_GAME
#include "../game_data/game_data.hpp"
#include "iomanip"

namespace zdice{

class Game {
private:

std::map<std::string, bool> player_Options{             
{"H", end_turn}, {"h", end_turn},           //Mapeia cada comando com uma variável booleana (não usei pra isso rs)
{"-h", true},                                   //(usei para conferir se é um comando válido).
{"", continue_turn},
{"Q", end_game}, {"q", end_game}
};

public:

bool end_switch{false};         //switches que dizem o estado do jogo.
bool end_game{false};
bool end_turn{false};
bool continue_turn{false};
bool prorrog{false};
bool quit_game{false};

Gm::GameFlow infos;           //vai guardar todas as informações importantes.
std::vector<Player> winners;  //guarda os jogadores que ganharam ou empataram.

void change_Player(bool &veri); //troca o player atual para o próximo.

void change_State(bool &state); //troca o turno para o do próximo jogador.

void status();                  //imprime parte da interface (tudo menos a caixa de comentários)

void start();                   //começa o turno de um jogado.

void end(bool &state);          //acaba o jogo e decide se vai ser empate ou não.

void message_area_result(size_t brains, size_t shotguns); //função que imprime os resultados de uma rolagem de dados.

void print_swaped_players();                               //função que imprime quando o jogador passa o turno para outro.
};}
#endif