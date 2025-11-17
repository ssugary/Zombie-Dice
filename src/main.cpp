/**
 * @file main.cpp
 *
 * @description
 * This program implements an interactive Zombie Dice Game.
 *
 * ===========================================================================
 * @license
 *
 * This file is part of ZOMBIE DICE GAME project.
 *
 * ZOMBIE DICE GAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ZOMBIE DICE GAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZOMBIE DICE GAME.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2024-2025 Selan R. dos Santos.
 * ===========================================================================
 *
 * @author	Selan R dos Santos, <selan.santos@ufrn.br>
 * @date	2024
 */
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

#include "run_options.hpp"


using namespace zdice;


 int main(int argc, char* argv[]){
    Game game;
    
    game.infos.game_config(argc, argv);     //lê o arquivo .ini e carrega suas definições.
    
    game.infos.set_dices();                 //prepara os dados para serem jogados.

    if(!game.infos.set_info())return 0;     //prepara as informações de cada jogador, caso não tenha nenhum jogador, acaba a func.



    while(true){
        game.start();                       //começa o turno de um jogador.
        game.change_State(game.end_game);   //troca o jogador do turno.
        if(game.end_game)break;             //caso desejado, o jogo acaba.
    }

    return 0;
 }