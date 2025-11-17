#include "game_data.hpp"


std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");    //sim, eu tenho essa função trim, mas não usei ela na set_info pois esqueci quando tava desenvolvendo o código kk
    if (std::string::npos == first) {           //retira todos os espaços (trim) de uma string.
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

using namespace Gm;
void GameFlow::set_dices() {
        Dice green;     //faz a Bolsa de dados como configurado no arquivo .ini 
        Dice yellow;
        Dice red;
        
        

        green.faces = green_dice_faces;
        yellow.faces = yellow_dice_faces;
        red.faces = red_dice_faces;


        for(size_t i{0}; i < green_dice; ++i){      //coloca todos os dados verdes, amarelos e vermelhos (faces escolhidas no arquivo .ini)
            dice_bag.dices.push_back(green);
            dice_bag.original_dices.push_back(green);
        }
        for(size_t i{0}; i < yellow_dice; ++i){
            dice_bag.dices.push_back(yellow);
            dice_bag.original_dices.push_back(yellow);

        }
        for(size_t i{0}; i < red_dice; ++i){
            dice_bag.dices.push_back(red);
            dice_bag.original_dices.push_back(red);

        }
    }

bool GameFlow::set_info() {             //vai pegar o nome de todos os jogadores e organizar tudo.
    std::string all_names_input;
    std::cout << ">>> ";
    getline(std::cin, all_names_input); //pega os nomes na linha de comando.
    
    std::string current_name;
    size_t start_pos = 0;
    size_t comma_pos;
    all_names_input += ',';

    while ((comma_pos = all_names_input.find(',', start_pos)) != std::string::npos) { //loop que vai pegar cada nome sem espaços (trim)
        current_name = all_names_input.substr(start_pos, comma_pos - start_pos);

        size_t first = current_name.find_first_not_of(" \t\n\r\f\v");
        if (std::string::npos == first) {
            current_name = ""; 
        } else {
            size_t last = current_name.find_last_not_of(" \t\n\r\f\v");
            current_name = current_name.substr(first, (last - first + 1));
        }
        if (!current_name.empty()) {
            Player player;
            player.name = current_name;
            players.push_back(player);
        }
        start_pos = comma_pos + 1;
    }
    if(players.size() == 0){        //caso não pegue nenhum player (inseriu uma linha vazia) retorna falso e sai do jogo.
        return false;
    }
    if(players.size() < 2){             //caso sejam inseridos menos jogadores do que o mínimo, daremos uma "segunda chance" para dar os nomes.
        std::cout << "Erro: Apenas 1 nome foi inserido." << std::endl;
        players.clear();
        return set_info();
    }
    else if(players.size() > max_players){              //caso insiram mais nomes do que o máximo de jogadores, irão abrir 2 casos.
        std::cout << "Erro: Foram inseridos mais jogadores do que o limite (" << std::to_string(max_players) << ")!" << std::endl;
        std::cout << "Iremos utilizar apenas os primeiros " << std::to_string(max_players) << " jogadores?" << std::endl;
        std::cout << "Y + <enter> Para continuar." << std::endl;
        std::cout << "<enter> Para inserir novamente os jogadores" << std::endl;
                                                        //ou o usuário tenta inserir novamente, ou ele tenta "cortar" os últimos para chegar no limite.
        std::string answer;
        getline(std::cin, answer);
        if(answer == "y" or answer == "Y"){     //caso seja sim, ele só troca e acabou, se não, ele recebe a "segunda chance".
            players.erase(players.begin() + max_players, players.end());
        }
        else{
        players.clear();
        return set_info();}
        
    }
    
    
    std::cout << std::endl;                         //imprime a lista de jogadores.
    std::cout << ">>> Os jogadores serão:" << std::endl;
    
    
    for (const auto& p : players) {
        std::cout << '"' << p.name << '"' << std::endl;
        if(small_name < p.name.size())small_name = p.name.size();
    }
    std::random_device rd;                  //embaralha os jogadores e pega algum aleatório para começar, depois confirma para o jogo começar de verdade.
    std::mt19937 g(rd());
    std::shuffle(players.begin(), players.end(), g);
    std::cout << std::endl;
    std::cout << "O(A) jogador(a) que irá começar será: " << '"' << players[0].name << '"' << std::endl << std::endl;
    std::cout << "Pressione <Enter> para começar a partida." << std::endl;
    std::string temp;
    std::cout << ">>>";
    getline(std::cin, temp);

    return true;
}


    void GameFlow::reset_dices(size_t &dices_qnt){
        dice_bag.dices = dice_bag.original_dices;   //faz os dados voltarem as suas "posições iniciais"
        dices_qnt = dice_bag.dices.size();
}   

    

    

    void GameFlow::game_turn(bool &end_turn){
        size_t brains_at_this_turn = 0, shotguns_at_this_turn = 0; 
        Dice random_Dice, temp_dice;
        std::vector<Dice> temp;
        char r;
        
        for(size_t i{0}; i < 3; i++){
        if(footprintDices.empty()){     //caso eu não tenha pego nenhuma face de pegada no turno passado, verificarei se a bolsa está vazia ou não.
            if(dice_qnt == 0 or dice_bag.dices.empty()){
                end_turn = true;                                 //caso a bolsa esteja vazia, o turno acaba.
                break;
            }
        random_Dice = dice_bag.pull_dice(dice_qnt);                //caso não esteja vazia, pegarei um dado aleatorio de lá.
        }
        else{                           //caso eu tenha pego alguma face de pegada no turno passado, irei rodar aquele mesmo dado (e tirarei do vetor
                                                                                                                            // que guarda as pegadas)
            random_Dice = footprintDices[0];
            footprintDices.erase(footprintDices.begin());
        }
        

        r = random_Dice.roll();             //rodo o dado escolhido acima.
        Dices_in_Game.push_back(random_Dice);   //guardo o dado que foi escolhido
        Dices_face_in_game.push_back(r);           //guardo a face que caiu

        if(r == 'b'){
            players[turn_player].brains_qnt++;  //caso a face seja "cérebros", aumento os contadores de cérebros.
            brains_at_this_turn++;
        }
        if(r == 'f'){                           //caso a face seja "pegada", guardarei em um vetor.
            temp.push_back(random_Dice);
        }
        if(r == 's'){                           //caso a face seja "arma", perderei uma vida e aumento o contador de armas.
            if(players[turn_player].life != 0){
            players[turn_player].life--;}
        shotguns_at_this_turn++;
        

        }}
        footprintDices = temp;                      //o vetor de pegadas vai ter todas as pegadas desse turno (wow)     
        brains_in_turn = brains_at_this_turn;       //os contadores de armas e cérebros globais serão alterados para os contadores desse turno.
        shotguns_in_turn = shotguns_at_this_turn;
    
    }

bool GameFlow::game_config(int argc, char* argv[]) {
    std::string temp = "./";        
    if(argc >= 2){              //caso o jogador tenha inserido algum arquivo no imput, ele irá ler ele.
    temp += std::string(argv[1]);
    }
    else{                                                   //se não, irá utilizar os valores padrão
        std::cout << "Arquivo de Configuração não encontrado." << std::endl;
        std::cout << "O jogo será executado com os valores padrão." << std::endl;
    }
    std::ifstream in (temp), welcome_txt{"./welcome.txt"};  //"in" é o arquivo .ini e welcome_txt é o arquivo que tem o texto inicial

    if(!in.is_open()){      //mensagem de erro caso o arquivo inserido não seja encontrado.
        std::cout << "Arquivo de Configuração não encontrado, verifique a pasta onde o arquivo executavel (zdice.out) se encontra." << std::endl;
        std::cout << "O jogo será executado com os valores padrão." << std::endl;
        return 0; //retorna false, já que deu errado :(
    }
    std::map<std::string, size_t*> map {
        {"weak_dice", &green_dice},             //mapeia cada argumento de int do arquivo.ini com um int
        {"strong_dice", &yellow_dice},
        {"tough_dice", &red_dice},
        {"max_players", &max_players},
        {"brains_to_win", &brains_to_win},
    };

    std::map<std::string, std::string*> map2{   //mapeia cada argumento de string do arquivo.ini com um string
        {"weak_dice_faces", &green_dice_faces},
        {"strong_dice_faces", &yellow_dice_faces},
        {"tough_dice_faces", &red_dice_faces}
    };
    std::string line;
    std::string valuestr;
    size_t valueint;

    if(welcome_txt.is_open()){      //imprime o texto inicial
        while(std::getline(welcome_txt, line)){
            std::cout << line << '\n';
        }
    }


    while(std::getline(in, line)){      //loop que pega cada linha do arquivo .ini, tira seus espaços e atribui os valores em cada argumento mapeado.
        line = trim(line);

        if (line.empty() or line[0] == '#') { 
            continue;
        }
        if(line[0] == '[' and line.back() == ']'){
            continue;
        }
        size_t eq_pos = line.find('=');
        if (eq_pos != std::string::npos) {
                std::string param = trim(line.substr(0, eq_pos));
                std::string value = trim(line.substr(eq_pos + 1));

                if (value.length() >= 2 && value.front() == '"' && value.back() == '"') {
                    value = value.substr(1, value.length() - 2);
                }
                
                if(map.count(param)){
                    try{
                    *map[param] = std::stoi(value); //copia o valor
                }catch(...){                        //se não for um valor válido (ex: colocar um string onde deve ser um int), da erro.
                    std::cout << "Ocorreu um erro ao analisar os parâmetros dados de " << param << " no zdice.ini, então utilizaremos o "
                              << "valor padrão desse parâmetro." << std::endl; 
                }  
            }
                else if(map2.count(param)){
                    *map2[param] = value;       //strings não tem esses erros, então já coloca direto.
            }

            }
            
 
        }
    in.close(); //fecha o arquivo

    return 1;       //retorna true, já que deu certo! :)
}



void GameFlow::set_table(){
    for(size_t i{0}; i < Dices_in_Game.size(); i++){            //vai colocar as informações na string que mostra quantos cérebros e tiros o
                                                                // jogador do turno recebeu e mostra os dados que foram tirados no turno.
        if(Dices_in_Game[i].faces == green_dice_faces){
            if(Dices_face_in_game[i] == 'b'){
                brain += "🟩";
                Table_dices.push_back("🧠(🟩)");
            }
            else if(Dices_face_in_game[i] == 's'){
                shotgun += "🟩";
                Table_dices.push_back("💥(🟩)");
            }
            else{
                Table_dices.push_back("👣(🟩)");
            }
        }
        else if(Dices_in_Game[i].faces == yellow_dice_faces){
            if(Dices_face_in_game[i] == 'b'){
                brain += "🟨";
                Table_dices.push_back("🧠(🟨)");
            }
            else if(Dices_face_in_game[i] == 's'){
                shotgun += "🟨";
                Table_dices.push_back("💥(🟨)");

            }
            else{
                Table_dices.push_back("👣(🟨)");
            }
        }
        else{
            if(Dices_face_in_game[i] == 'b'){
                brain += "🟥";
                Table_dices.push_back("🧠(🟥)");
            }
            else if(Dices_face_in_game[i] == 's'){
                shotgun += "🟥";
                Table_dices.push_back("💥(🟥)");
            }
            else{
                Table_dices.push_back("👣(🟥)");
            }
        }
    }

    for(std::string str : Table_dices){ //Faces_on_table vai ter os 3 dados que foram retirados no turno.
        Faces_on_table += str + " ";
    }

}