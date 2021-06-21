#include "battlefield.h"
#include "libtgraph.h"

#include <chrono>
#include <limits>
#include <thread>

struct Player 
{
    std::string name;
    bool is_human;
};

struct Input 
{
    char optype;
    long int sid;
    long int x;
    long int y;
    char dir;
};

const long int DEFAULT_PLAYER_COUNT = 2;
const long int DEFAULT_X_RESOLUTION = 80;
const long int DEFAULT_Y_RESOLUTION = 23;

const std::string MAIN_HELP = 
"Вход:\n"
"  play - играть;\n"
"  exit - выйти;\n"
"  help - для просмотра справки по игре\n";

const std::string GAME_HELP =
" X - горизонтальная координата.\n"
" Y - вертикальная координата.\n"
" Чтоб разместить корабли используйте:\n"
"    0a - автоматическое размещение\n"
"    0! - закончить размещение кораблей.\n\n"

"Cтрельба происходит следующим образом:\n"
"    первая цифра отвечает за координату Х\n"
"    вторая за координату Y\n\n"

"Информация по клеткам\n" 
 "Доступные варианты:\n" 
 "  Ваш корабль стоит по горизонтали и он цел >\n  " 
 "Ваш корабль стоит по вертикали и он цел ^\n " 
 " В ваш корабль попали H - hit\n " 
 " Вы попали H - hit\n" 
 "  Туман войны -\n" 
 "  Выстрел в воду X\n";

class Game
{
public:
    Screen gs;
    std::vector<Player> players;
    std::vector<BattleField> playerbtfs;
    long int curr_player;
    long int p_count;
    std::string info_message;
    std::string phase;
    float difficulty;
    char ALIVE_H_SHIP = '>';
    char ALIVE_V_SHIP = '^';
    char DEAD_H_SHIP = 'H';
    char DEAD_V_SHIP = 'H';
    char CLEAN_WATER = '-';
    char BOMBED_WATER = 'X';

    Game()
    {
        gs.resize(DEFAULT_X_RESOLUTION, DEFAULT_Y_RESOLUTION);
        players.push_back(Player{ "Ваше поле", true });
        players.push_back(Player{ "Поле соперника", false });
        playerbtfs.resize(DEFAULT_PLAYER_COUNT);
        curr_player = 0;
        p_count = DEFAULT_PLAYER_COUNT;
        difficulty = 33;
        phase = "Меню";
    }

    void fill_player_matrix(std::vector<std::vector<char>>& field_matrix)
    {
        field_matrix.resize(playerbtfs[curr_player].matrix.size());

        for (long int x = 0; x < playerbtfs[curr_player].matrix.size(); ++x) 
        {
            field_matrix[x].resize(playerbtfs[curr_player].matrix[x].size());
            for (long int y = 0; y < playerbtfs[curr_player].matrix[x].size(); ++y) 
            {
                if (playerbtfs[curr_player].matrix[x][y]) 
                {
                    field_matrix[x][y] = CLEAN_WATER;
                }
                else 
                {
                    field_matrix[x][y] = BOMBED_WATER;
                }
            }
        }
        for (Ship& i : playerbtfs[curr_player].ships) 
        {
            for (long int j = 0; j < i.cells.size(); ++j) 
            {
                if (i.cells[j])
                {
                    if (i.dir == 'h')
                        field_matrix[i.x + j][i.y] = ALIVE_H_SHIP;
                    else
                        field_matrix[i.x][i.y + j] = ALIVE_V_SHIP;
                }
                else
                {
                    if (i.dir == 'h')
                        field_matrix[i.x + j][i.y] = DEAD_H_SHIP;
                    else
                        field_matrix[i.x][i.y + j] = DEAD_V_SHIP;
                }
            }
        }
    }

    void fill_enemy_matrix(std::vector<std::vector<char>>& field_matrix)
    {
        field_matrix.resize(playerbtfs[curr_enemy()].matrix.size());

        for (long int x = 0; x < playerbtfs[curr_enemy()].matrix.size(); ++x) 
        {
            field_matrix[x].resize(playerbtfs[curr_enemy()].matrix[x].size());
            for (long int y = 0; y < playerbtfs[curr_enemy()].matrix[x].size(); ++y) 
            {
                if (playerbtfs[curr_enemy()].matrix[x][y]) 
                {
                    field_matrix[x][y] = CLEAN_WATER;
                }
                else 
                {
                    field_matrix[x][y] = BOMBED_WATER;
                }
            }
        }
        for (Ship& i : playerbtfs[curr_enemy()].ships)
        {
            for (long int j = 0; j < i.cells.size(); ++j) 
            {
                if (!i.cells[j])
                {
                    if (i.dir == 'h')
                    {
                        field_matrix[i.x + j][i.y] = DEAD_H_SHIP;
                    }
                    else
                    {
                        field_matrix[i.x][i.y + j] = DEAD_V_SHIP;
                    }
                }
            }
        }
    }

    void setup_n_place_matrixes(std::string& text_object) 
    {
        std::vector<std::vector<char>> field_matrix;

        text_object = players[curr_player].name;
        gs.place_text(2, 2, text_object);
        text_object.clear();
        gs.place_text(4, 3, text_object);
        text_object.clear();
        fill_player_matrix(field_matrix);
        gs.place_matrix(4, 5, field_matrix);
        field_matrix.clear();

        text_object = players[curr_enemy()].name;
        gs.place_text(24, 2, text_object);
        text_object.clear();
        gs.place_text(26, 3, text_object);
        text_object.clear();
        fill_enemy_matrix(field_matrix);
        gs.place_matrix(26, 5, field_matrix);
        for (int8_t i = 48; i < 10 + 48; ++i)
            text_object.push_back(i);
        gs.place_text(26, 4, text_object);
        gs.place_text(26, 15, text_object);
        gs.place_text(4, 4, text_object);
        gs.place_text(4, 15, text_object);
        gs.place_text(24, 5, text_object, false);
        gs.place_text(37, 5, text_object, false);
        gs.place_text(2, 5, text_object, false);
        gs.place_text(15, 5, text_object, false);
        text_object.clear();
    }

    void compose_game_screen()
    {
        std::string text_object = "GLHF";
        gs.place_text(0, 0, text_object);
        text_object.clear();
        setup_n_place_matrixes(text_object);
        for (long int i = 0; i < gs.xsize(); ++i)
            text_object.push_back('-');
        gs.place_text(0, gs.ysize() - 1, text_object);
        text_object.clear();
        for (long int i = 0; i < gs.ysize(); ++i)
            text_object.push_back(' ');
        gs.rel_place_text(0.5, 0., text_object, false);
        text_object.clear();
    }
    void print_frame()
    {
        if (phase == "размещение" || phase == "стрельба") 
        {
            compose_game_screen();
            gs.print();
        }
    }

    bool is_defeated(long int player_n)
    {
        for (Ship& i : playerbtfs[player_n].ships)
        {
            if (!i.is_killed())
                return false;
        }
        return true;
    }

    void regenerate_screen()
    {
        gs.clear();
        info_message.clear();
        if (phase == "размещение" || phase == "стрельба")
            compose_game_screen();
    }

    bool is_game_end()
    {
        for (long int i = 0; i < players.size(); ++i) 
        {
            if (is_defeated(i))
                return true;
        }
        return false;
    }
    
    bool is_correct_positioning(long int player_n, bool silent = false) // для ручного размещения, проверка на положения коробля
    {
        BattleField tmp;
        for (Ship i : playerbtfs[player_n].ships) 
        {
            if (playerbtfs[player_n].is_out_of_matrix(i)) 
            {
                if (!silent)
                    std::cout << "Корабль вне поля!" << std::endl;
                return false;
            }
            long int xend = i.x + 1 + (i.cells.size()) * (i.dir == 'h') + (i.dir == 'v');
            long int yend = i.y + 1 + (i.cells.size()) * (i.dir == 'v') + (i.dir == 'h');
            for (long int x = i.x - (i.x != 0); x < xend; ++x) 
            {
                for (long int y = i.y - (i.y != 0); y < yend; ++y) 
                {
                    if ((x < tmp.matrix.size()) && (y < tmp.matrix.size()))
                    {
                        if (!tmp.matrix[x][y]) 
                        {
                            if (!silent)
                                std::cout << "Корабль перекрывает другой! " << x << " " << y << std::endl;
                            return false;
                        }
                    }
                }
            }
            for (long int j = 0; j < i.cells.size(); ++j)
                tmp.matrix[i.x + j * (i.dir == 'h')][i.y + j * (i.dir == 'v')] = false;
        }
        if (!silent)
            std::cout << "Нет ошибок, можете начать стрельбу, первая цифра это координата Х, вторая Y" << std::endl;
        return true;
    }
    

    Input placer_AI() // основной режим, автоматическое размещение
    {
        srand(time(nullptr));
        std::vector<Ship>* ships = &playerbtfs[curr_player].ships;
        while (!is_correct_positioning(curr_player, true))
            for (long int i = 0; i < ships->size(); ++i) 
            {
                if (rand() % 100 < 90)
                {
                    ships->at(i).x = rand() % playerbtfs[curr_player].matrix.size();
                    ships->at(i).y = rand() % playerbtfs[curr_player].matrix.size();
                }
                if (rand() % 100 < 50)
                {
                    playerbtfs[curr_player].rotate_ship(i);
                }
            }
        return { '!', 0, 0, 0 };
    }

    Input fighter_AI()
    {
        Input buffer;
        print_frame();
        buffer.optype = 'f';
        uint8_t chance = rand() % 100;
        std::vector<Ship>& ships = playerbtfs[curr_enemy()].ships;
        if (chance < difficulty) 
        {
            long int ship_n = 0;
            for (; ship_n < ships.size(); ++ship_n)
                if (!ships[ship_n].is_killed())
                    break;

            for (long int i = 0; i < ships[ship_n].cells.size(); ++i)
            {
                if (playerbtfs[curr_enemy()].matrix[ships[ship_n].x + i * (ships[ship_n].dir == 'h')] [ships[ship_n].y + i * (ships[ship_n].dir == 'v')]) {
                    playerbtfs[curr_enemy()].punch_cell(ships[ship_n].x + i * (ships[ship_n].dir == 'h'), ships[ship_n].y + i * (ships[ship_n].dir == 'v'));
                    if (ships[ship_n].is_killed())
                        eject_killed_ship_domain(curr_enemy(), ships[ship_n]);
                    if (chance >= difficulty / 3)
                        break;
                }
            }
        }
        buffer.x = rand() % playerbtfs[curr_enemy()].matrix.size();
        buffer.y = rand() % playerbtfs[curr_enemy()].matrix.size();

        return buffer;
    }
    Input receive_input(long int player_n)
    {
        Input buffer;
        if (players[player_n].is_human) 
        {
            do 
            {
                if (std::cin.fail()) 
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                if (phase == "размещение") 
                {
                    std::cin >> buffer.sid;
                    std::cin >> buffer.optype;
                    if (buffer.optype == 'p') 
                    {
                        std::cin >> buffer.x;
                        std::cin >> buffer.y;
                    }
                }
                else 
                {
                    buffer.optype = 'f';
                    std::cin >> buffer.x;
                    std::cin >> buffer.y;
                }
            } while (std::cin.fail());
        }
        else 
        {
            if (phase == "размещение") 
            {
                buffer = placer_AI();
            }
            else 
            {
                buffer = fighter_AI();
            }
        }
        return buffer;
    }

    void players_place_ships()
    {
        phase = "размещение";
        bool is_successful = false;
        curr_player = 0;
        print_frame();
        Input action;
        for (; curr_player < players.size(); ++curr_player) 
        {

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            regenerate_screen();
            action = receive_input(curr_player);
            while (action.optype != '!' || !is_correct_positioning(curr_player))
            {
                info_message.clear();
                if (action.optype == 'a') 
                {
                    placer_AI(); //авторазмещение
                }
                // else if (action.optype == 'p') 
                //{
                //    is_successful = playerbtfs[curr_player].place_ship(
                //        action.sid, action.x, action.y);
                //}
                //else if (action.optype == 'r')
                //{
                //    is_successful = playerbtfs[curr_player].rotate_ship(action.sid);
                //}
                else if (action.optype != '!')
                {
                    info_message = "Неизвестная операция! ";
                }
                if (is_successful)
                    info_message += "Успех!";
                else
                    info_message += "ошибка!";
                print_frame();
                action = receive_input(curr_player);
            }
        }
        curr_player = 0;
    }

    void eject_killed_ship_domain(long int player_n, Ship& i)
    {
        std::cout << i.dir;
        long int xend =
            i.x + 1 + (i.cells.size()) * (i.dir == 'h') + (i.dir == 'v');
        long int yend =
            i.y + 1 + (i.cells.size()) * (i.dir == 'v') + (i.dir == 'h');
        for (long int x = i.x - (i.x != 0); x < xend; ++x) 
        {
            for (long int y = i.y - (i.y != 0); y < yend; ++y) 
            {
                if ((x < playerbtfs[player_n].matrix.size()) && (y < playerbtfs[player_n].matrix.size())) 
                {
                    playerbtfs[player_n].matrix[x][y] = false;
                }
            }
        }
    }
    long int curr_enemy()
    {
        return (curr_player + 1) % p_count;
    }

    void battle_loop()
    {
        phase = "стрельба";
        Input action;
        uint8_t result = 0;
        while (!is_game_end()) 
        {
            print_frame();
            action = receive_input(curr_player);
            result = playerbtfs[curr_enemy()].punch_cell(action.x, action.y);
            if (result != 0) 
            {
                if (result == 1) 
                {
                    info_message = "Lost time&shots";
                }
                else if (result == 2)
                {
                    info_message = "Damaged a ship!";
                }
                else 
                {
                    info_message = "Killed #" + std::to_string(result - 3);
                    eject_killed_ship_domain(curr_enemy(), playerbtfs[curr_enemy()].ships[result - 3]);
                }
            }
            else
            {
                info_message = "Error!";
            }
            if (result == 1) 
            {
                curr_player = curr_enemy();
                regenerate_screen();
            }
        }
        if (is_defeated(curr_player)) 
        {
            curr_player = curr_enemy();
        }
        std::cout << std::endl << std::endl << players[curr_player].name << " победа" << std::endl << std::endl;
        if (players[curr_player].is_human) 
        {
            std::cout << "поздравляю" << std::endl;
        }
        else 
        {
            std::cout << "407" << std::endl;
        }
    }
};

int
main()
{
    setlocale(LC_ALL, "ru");
    Game game;
    std::string action = "help";
    while (action != "exit")
    {
        if (action == "help")
        {
            std::cout << GAME_HELP << std::endl;
        }
        else if (action == "play")
        {
            game.players_place_ships();
            game.battle_loop();
        }
        std::cout << MAIN_HELP << std::endl;
        std::cin >> action;
    }
    return 0;
}
