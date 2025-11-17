# Introduction

<!-- TODO -->

O projeto basicamente é uma versão virtual do jogo Zombie Dice Deluxe.

# Author(s)


nome: Heitor Fernandes Paes Leme Campos
email: Heitor.campos.711@ufrn.edu.br

# Problems found or limitations

Não chega a ser uma limitação, mas eu achei mais legal fazer um "pódio" de cada jogador do que botar apenas o vencedor.

Talvez esteja mal otimizado, mas garanto a funcionalidade para grupos pequenos/médios de pessoas.

Cada nome pode ter até 23 dígitos, se não resulta em erro.

A interface está completamente desorganizada, mas da para consertar facilmente caso recomece do zero.

# Grading

<!-- TODO -->

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                     | Estimated Value |
| ---------------------------------------- | :-------------: |
| The project contains at least 2 classes  |       7         |
| Show help as requested `-h`              |       4         |
| Read player names                        |       10        |
| Show all 5 regions in user interface     |       12        |
| Keeps overall scores                     |       5         |
| Keeps bag of dice consistent             |       5         |
| Handles automatic turn hand over         |       5         |
| Handles hold requests                    |       5         |
| Handles roll requests                    |       5         |
| Handles ties                             |       12        |
| Move brains dice back to bag when needed |       10        |
| Manages turns correctly                  |       5         |
| Reads configuration file                 |       15        |
| Handles user errors                      |       5         |
| Show winner correctly                    |       5         |
| Program working fine                     |       10        |

# Compiling and Runnig

<!-- TODO -->
primeiro abra o terminal e entre no diretório  ./trabalho-06-projeto-zombie-dice-thriller/src/

execute o comando:

c++ main.cpp dice.cpp game_data.cpp run_options.cpp -o zdice

após isso, use o comando:

./zdice zdice.ini

Note que o "zdice.ini" é o arquivo de configurações, então se tiver algum outro, desde que esteja na pasta "src", ele irá ler.
O arquivo "welcome.txt" apenas deve ser alterado se você quiser mudar o texto inicial/que aparece quando utiliza o comando "-h".
