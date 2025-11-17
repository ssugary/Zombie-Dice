#include "run_options.hpp"

using namespace zdice;

size_t count_chars_on_emotes(std::string str){
    size_t size{0};                 //como os emojis não ocupam apenas 1 caractere, fiz essa função (que acabou sendo útil para apenas 1) para consertar erros de interface.
    for(char c : str){
        if((c & 0xC0) != 0x80){
            size++;
        }
    }
    
    return size;
}


void print_separator(int width) {
                                            //imprime a faixa que separa cada bloco de texto
    std::cout << "+" << std::string(width - 2, '-') << "+" << std::endl;
}

void print_centered(const std::string& text, int width) {   //imprime alguma mensagem centralizada.
    int padding = (width - 2 - count_chars_on_emotes(text)) / 2;
    std::cout << "| " << std::string(padding, ' ') << text << std::string(width - text.size() - padding, ' ') << " |" << std::endl;
}


void print_global_score(const std::string& left_text, const std::string& right_text, int width){    //imprime informações globais de algum jogador.
    size_t padding = width - 4 - left_text.size() - right_text.size();
    std::string s = "| " + left_text + std::string(padding, ' ') + right_text;
    std::cout << s << std::string(24, ' ') << " |" << std::endl;

}

void print(const std::string& left_text, const std::string& right_text, int width){
std::string s = "| " + left_text + ' ' + right_text;             //apenas imprime algo dentro do bloco de texto.
std::cout << s << std::string(width - s.size() - 2, ' ') << " |" << std::endl;
}
void message_area_question(){      //imprime as possiveis ações do jogador.
    size_t screen_width = 100;
    print("Pronto para jogar?", "", screen_width);
    print("  <enter> - jogar os dados", "", screen_width);
    print("  H + <enter> - passar a vez", "", screen_width);
    print("  Q + <enter> - sair do jogo", "", screen_width);

}

void Game::change_Player(bool &veri){       //função que troca o jogador do turno
    infos.players[infos.turn_player].turns_played++;//aumenta a quantidade de turnos jogados
    if(infos.turn_player == infos.players.size() - 1){
        infos.turn_player = 0;  //caso o jogador seja o último antes da troca de turnos globais, volta pro primeiro jogador.
        infos.turns_qnt++;
        veri = true;

        }
    else{                       //passa para o próximo jogador.
        infos.turn_player++;
    }
}

void Game::change_State(bool &state){
    if(quit_game){      //caso o usuário solicite a saída do jogo, nem roda o código.
        return;
    }

    if(infos.players[infos.turn_player].life == 0){ //caso ele tenha tomado 3 tiros, não receberá nenhum cérebro.
        infos.players[infos.turn_player].brains_qnt = 0;
    }
    infos.players[infos.turn_player].points += infos.players[infos.turn_player].brains_qnt;
    infos.players[infos.turn_player].brains_qnt = 0;            //soma os cérebros aos pontos e reseta a vida.
    infos.players[infos.turn_player].life = 3;

    infos.reset_dices(infos.dice_qnt);  //reseta a bolsa dos dados.
    
    infos.brain = "🧠: ";   //reseta as strings da área da mesa.
    infos.shotgun = "💥: ";
    
    infos.Dices_in_Game.clear();    //limpa a mesa e retira tudo o que o jogador fez.
    infos.Dices_face_in_game.clear();
    
    bool veri{false};   //verifica se foi o último jogador do turno
    change_Player(veri);
    if(veri and !prorrog){
                                //caso passem o turno e o jogo não esteja em prorrogação:
        for(Player player : infos.players){
            if(player.points >= infos.brains_to_win){
            winners.push_back(player); //todos os jogadores que tenham mais pontos que o necessário viram "candidatos" para a vitória
        }
        else{                           //se não forem candidatos, serão jogados direto no podium e deixados pra lá.
            infos.podium.push_back(player);
        }
        }
        
        if(!winners.empty()){       //caso existam candidatos, essa parte verificará se existe algum com mais pontos que os outros.
        Player param = winners[0];                      //no fim, vai sobrar apenas o jogador com mais pontos
        for(size_t i{1}; i < winners.size(); i++){      //e os jogadores que empatarem com ele.
            if(param.points > winners[i].points){
                infos.podium.push_back(winners[i]);
                winners.erase(winners.begin() + i);
            }
            else if(param.points < winners[i].points){
               
                for(Player player : winners){
                    infos.podium.push_back(player);
                }
                 winners.clear();
                param = winners[i];
                winners.push_back(param);
            }
        }    
        end(state); //passa para o "end" que faz a transição para a "prorrogação" ou para o "fim" do jogo.
        } 
    }
    else if (prorrog and veri) {        //caso esteja na prorrogação e o turno global acabe, ele verificará se existe alguem com mais pontos que os outros
        Player param = infos.players[0];    //no fim apenas vai sobrar o que tem mais pontos e os que empatarem com ele.
        winners.push_back(param);
        for(size_t i{1}; i < infos.players.size();i++){
           if(param.points < infos.players[i].points){
            winners.clear();
            param = infos.players[i];
            winners.push_back(param);
           }
           else if(param.points == infos.players[i].points){
            winners.push_back(infos.players[i]);
           }
        }
        
            end(state);  //passa para o end que faz a transição para a "prorrogação" de novo, ou para o "fim" do jogo.       
        
    }
}

void Game::start(){     //começa o turno de algum jogador
    std::string option;//comando do jogador.
    
    status();   //imprime a interface
    

    while(true){                                //turno do jogador
    message_area_question();//pergunta o que ele vai fazer
    print_separator(100);
    std::cout << ">>>";         
    std::getline(std::cin, option); //pega o comando
    
    if(!player_Options.count(option)){
        std::cout << "insira um comando válido!" << std::endl;
        continue;       //se o comando não for encontrado, repete a pergunta.
    }
    
    if(option == ""){       //"" é o mesmo que apenas <enter> que vai rolar os dados.
        status();
        infos.game_turn(end_turn); //joga os dados
        message_area_result(infos.brains_in_turn, infos.shotguns_in_turn);  //mostra os resultados dos dados

    }
    else if (option == "q" or option == "Q"){               //"q" ou "Q" é o mesmo que sair do jogo
        status();
        print("Tem certeza que deseja sair do jogo? ", "",100 );
        print(" Q + <enter> para fechar o jogo.", "", 100);
        print(" <enter> para continuar o jogo.", "", 100);
        print_separator(100);       //pergunto se o usuário realmente quer fechar o jogo
        std::cout << ">>>";
        getline(std::cin, option);
        if(option == "q" or option == "Q"){
        end_game = true;    //caso queira, muda o estado e acaba o turno.
        quit_game = true;
        break;}
        else {              //se ele muda de ideia, continua o turno normalmente.
            status();
            continue;
        }
    }
    else if (option == "h" or option == "H"){   //"h" ou "H" é o mesmo que "passar o turno", ou seja, terminar o turno alí mesmo.
        end_turn = true;        //muda o estado
        print_swaped_players(); //imprime a decisão de troca do jogador.
        break;
    }
    else if(option == "-h"){            //"-h" vai imprimir o texto inicial para auxiliar jogadores perdidos. 
        std::ifstream is{"./welcome.txt"};
        std::string line;   //lê cada linha do arquivo onde o texto está.
        while(getline(is, line)){
            std::cout << line << std::endl;
        }
        std::cout << "Digite <enter> para continuar o jogo!" << std::endl << ">>>";
        getline(std::cin, line);    //confirmar
        status();
        continue;
    }
    

    if(infos.players[infos.turn_player].life == 0){ //quando o jogador perdeu todas as vidas:
        end_turn = true; //muda o estado
        infos.set_table(); //já atualiza a mesa e imprime ela.
        status();
        std::string temp;
        std::cout << "| Suas vidas zeraram! O jogador " << infos.players[infos.turn_player].name << " Perdeu seus cérebros!" << std::string(44 - infos.players[infos.turn_player].name.size(), ' ') << " |" << std::endl;
        std::cout << "| Seu turno irá terminar imediatamente, digite <enter> para confirmar!" << std::string(28, ' ') << " |" << std::endl;
        print_separator(100);   //imprime o acontecimento e confirma ele.
        std::cout << ">>>";
        getline(std::cin, temp);
        infos.Dices_face_in_game.clear();
        infos.Dices_in_Game.clear();        //limpa a mesa para o próximo jogador.
        infos.Table_dices.clear();
        infos.Faces_on_table.clear();
        break;
    }
    else if(infos.dice_qnt == 0){   //caso o jogador puxe todos os dados
        end_turn = true;    //muda o estado
        status();   //imprime a mesa e tudo o que aconteceu, depois pede para confirmar.
        print("Acabaram os dados! Sua jogada será passada.", "", 101);
        print(" Aperte <enter> para confirmar.", "", 100);
        print_separator(100);
        std::cout << ">>>";
        getline(std::cin, option);
        break;
    }

    

    
    infos.set_table();  //atualiza a mesa.
    status();           //imprime a mesa.

    infos.Dices_face_in_game.clear();   //limpa a mesa.
    infos.Dices_in_Game.clear();
    infos.Table_dices.clear();
    infos.Faces_on_table.clear();
    }
    
}






std::string print_brains(const size_t points, const size_t turns_played, const Gm::GameFlow infos){
    std::string str{""};            //cria a string que mostra quantos cérebros cada jogador pegou
    size_t temp{0};
    while(temp != points){  
        str += "🧠";        //pra cada ponto soma 1 cérebro.
        temp++;
        if(temp == infos.brains_to_win)break;
    }
    while(temp != infos.brains_to_win){
        str += "🔸";        //para cada ponto que falta soma 1 "negocio laranja" que representa o "vazio".
        temp++;
    }
            //termina a string
    return str + "(" + std::to_string(points) + "), # turns played: " + std::to_string(turns_played);
}

std::string print_turn_player(Gm::GameFlow infos, Player turn_player){   //cria a string que diz a situação do jogador do turno. 
    return '"' + turn_player.name + '"' + " | Turn #: " + std::to_string(turn_player.turns_played + 1) + " | Bag has: " + std::to_string(infos.dice_qnt) + " 🎲";
}
void Game::status(){ //imprime a interface do jogo

    size_t screen_width = 100;  //setei o tamanho como 100
    size_t i{0};
    std::string temp;
    
    print_separator(screen_width);
    print_centered("[🧟] Zombie Dice Delux [🧟]", screen_width);
    print_separator(screen_width);          //imprime o nome do jogo.

    print("Pontuação Global:", "", screen_width + 2);  //imprime a pontuação global.  

    for(Player player : infos.players){ //imprime a informação de cada jogador.
    if(i == infos.turn_player){
        temp = "> ";        //jogador do turno tem um sinalizador ">"
    }
    else{
        temp = "  ";
    }
    temp += player.name + ": ";

    print_global_score(temp, print_brains(player.points, player.turns_played, infos), screen_width);
    i++;
   }
   

   print_separator(screen_width);   //separa o bloco de texto

   if(end_switch){      //se o jogo terminou, não imprime o resto da tela (é importante para a interface final)
    end_switch = false;
    return;
   }

   print("Jogador(a): " + print_turn_player(infos, infos.players[infos.turn_player]), "", screen_width + 2); //imprime o jogador do turno e informações gerais.
   
   if(infos.Faces_on_table.empty()){    
   std::cout << "| Dados na mesa: " << std::string(screen_width - 19, ' ') << " |" << std::endl; //imprime a mesa vazia
   }
   else{                                                                                            //imprime a mesa cheia (kk)
       std::cout << "| Dados na mesa: " << infos.Faces_on_table << std::string(screen_width - 40, ' ') << " |" << std::endl; 

   }        //vão imprimir a quantidade total de cérebros e tiros obtidos nesse turno do jogador.
   std::cout << "| " << infos.brain << "(" << std::to_string(infos.players[infos.turn_player].brains_qnt) + ")" << std::string(screen_width - 2*( count_chars_on_emotes(infos.brain + "()") )- std::to_string(infos.players[infos.turn_player].brains_qnt).size()  , ' ') << " |" << std::endl;
   std::cout << "| " << infos.shotgun << "(" << std::to_string(3 - infos.players[infos.turn_player].life) + ")" << std::string(screen_width - 2*( count_chars_on_emotes(infos.shotgun + "()") )- 1  , ' ') << " |" << std::endl;
   print_separator(screen_width);


   


}

void Game::message_area_result(size_t brains, size_t shotguns){
    size_t screen_width = 100;
    std::string s;
    print("Resultados da rolagem: ", "", screen_width); //imprime o resultado da rolagem e confirma.
    print(" Cerebros que você comeu: " + std::to_string(brains), "", screen_width + 1);
    print(" Tiros que você recebeu: " + std::to_string(shotguns), "", screen_width + 1);
    print("Aperte <enter> para continuar ", "", screen_width);
    print_separator(screen_width);
    std::cout << ">>>";
    getline(std::cin, s);
}

void Game::end(bool &state){    //"termina" o jogo (se for empate ele volta)
    end_switch = true; //muda o estado para o fim.
    status();          //imprime a mesa.

    size_t screen_width = 100;
    if(winners.size() == 1){    //caso o vencedor seja decidido, imprime o podio dos jogadores.
    std::cout << "Eis aqui o pódio!" << std::endl;
        state = true;
        
        infos.podium.push_back(winners[0]);     //coloca o vencedor do podio

        for(size_t i{0}; i < infos.podium.size() - 1;i++){ //organiza o podio
            for(size_t j{i}; j < infos.podium.size(); j++){
                if(infos.podium[i].points > infos.podium[j].points){
                    std::swap(infos.podium[i], infos.podium[j]);
                }
            }
        }
        Player temp;
        size_t pos = infos.podium.size();
        for(size_t i{0}; i < infos.podium.size() - 1; i++){
            temp = infos.podium[i];             //imprime cada jogador em sua posição do podio.
            if(pos == 1)break;
            std::cout << pos << "⁰ lugar: ";
            
            for(size_t j{i}; j < infos.podium.size();j++){
                if(infos.podium[j].points == infos.podium[i].points){
                    std::cout << infos.podium[j].name << ' ';
                    pos--;
                }
                else{
                    break;
                }
            }
            std::cout << std::endl;

        }
        //imprime o ganhador.
        std::cout << "1⁰ Lugar: " << infos.podium[infos.podium.size() - 1].name << "!!!" << std::endl;


}
    else{   //caso o jogo não tenha um vencedor ainda:

        infos.players = winners;
        winners.clear();    //os vencedores vão jogar novamente uma prorrogação para definir o ganhador
   
    
    std::string names{""}, s;
    for(size_t i{0}; i < infos.players.size();i++){
        names += infos.players[i].name;     //cria a string com os nomes dos jogadores que estarão na prorrogação
        if(i == infos.players.size() - 2){
            names += " e ";
        }
        else if(i != infos.players.size() - 1){
            names += ", ";
        }
    }

    prorrog = true;     //imprime o texto de prorrogação e os jogadores que estão nela, depois confirma tudo e volta o jogo.
    std::cout << "| Prorrogação!! " <<  std::string(screen_width - 18, ' ') << " |" << std::endl;
    std::cout << "| Os jogadores " << names << std::string(screen_width - names.size() - 17, ' ') << " |" << std::endl;
    std::cout << "| Empataram e agora irão batalhar entre si!" << std::string(screen_width - 45, ' ') << " |" << std::endl;
    print("Aperte <enter> para continuar ", "", screen_width);
    print_separator(100);
    std::cout << ">>>";
    getline(std::cin, s);
}
}

void Game::print_swaped_players(){//função que imprime a ação do jogador de passar o turno
    end_switch = true;//muda o estado e imprime a mesa
    status();
    size_t screen_width = 100;
    std::string s;
                            //imprime que o jogador passou o turno e confirma.printa
    print("O(A) jogador(a) " + infos.players[infos.turn_player].name + " passou o turno","", screen_width);
    print("Aperte <enter> para continuar ", "", screen_width);
    print_separator(100);
    std::cout << ">>>";
    getline(std::cin, s);

}
