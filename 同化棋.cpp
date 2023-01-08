#include<iostream>
#include<fstream>
#include<Windows.h>
using namespace std;

int board[8][8] = { 0 }, temp_board[8][8] = { 0 },temp_board2[8][8];//记录棋盘，1白子玩家，2黑子电脑
int cnt[3] = { 0 };//记录黑、白、空棋子数目
int near_block[8][2] = { 0,1,0,-1,1,0,-1,0,1,1,-1,1,1,-1,-1,-1 };//邻近八格
int move_x[25], move_y[25];//24种移动方式，内八外十六
int move_list[25] = { 0 };//用于AI遍历
int s_x, s_y, e_x, e_y;//玩家输入的起点终点
int over_flag = 0;//游戏结束标志


void printf_chessboard();//打印棋盘
void menu();//菜单栏
void init();//初始化 棋盘 和 移动方式数组
void sum(int a[8][8]);//棋子数目求和
int check_move(int, int);//检查是否可移动，并返回移动方式种数
void move(int, int, int, int, int, int a[8][8]);//移动棋子，翻转
void save();//存档
void game_begin();//游戏总框架
void artificial_retard();//ai决策
void copy(int a[8][8], int b[8][8]);//复制棋盘，供ai和悔棋使用
void help();//帮助手册
bool judge_player();//判断电脑是否弃权
bool judge_ai();//判断玩家是否弃权
void check();//检查玩家输入的合法性
void takeback();//悔棋
void over();//判断游戏结束

int main()
{
    menu();
    return 0;
}

void printf_chessboard()//打印棋盘
{
    sum(board);
    cout << "Player:   " << cnt[1] <<  "  AI:   " << cnt[2] << endl;

    cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━ " << endl;
    cout << "     1   2   3   4   5   6   7  " << endl;
    cout << "   ┏━━━┳━━━┳━━━┳━━━┳━━━┳━━━┳━━━┓" << endl;
    cout << " 1 ┃";
    for (int i = 1; i <= 7; i++)
    {
        if (board[1][i] == 1)
            cout << " ●┃";
        else if (board[1][i] == 2)
            cout << " ○┃";
        else
            cout << "   ┃";
    }
    cout << endl;

    for (int i = 2; i <= 7; i++)
    {
        cout << "   ┣━━━╋━━━╋━━━╋━━━╋━━━╋━━━╋━━━┫" << endl;
        cout << ' ' << i << " ┃";
        for (int j = 1; j <= 7; j++)
        {
            if (board[i][j] == 1)
                cout << " ●┃";
            else if (board[i][j] == 2)
                cout << " ○┃";
            else
                cout << "   ┃";
        }
        cout << endl;
    }
    cout << "   ┗━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻━━━┛" << endl;
}

void menu()//菜单栏
{
    cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━ " << endl;
    cout << "同化棋" << endl;
    cout << "您执白棋,电脑执黑棋" << endl;
    cout << "请输入数字以选择" << endl;
    cout << " ━━━━━━━━━━━━━━━━━━━━━━━━━━━━ " << endl;
    cout << "1.新的开始" << endl;
    cout << "2.读取存档" << endl;
    cout << "3.帮助" << endl;
    cout << "4.退出游戏" << endl;

    int order;
    cin >> order;

    if (order == 1)
    {
        init();
        game_begin();
    }
    else if (order == 2)
    {
        init();
        ifstream fin;
        fin.open("record.txt");
        for (int i = 1; i <= 7; i++)
        {
            for (int j = 1; j <= 7; j++)
            {
                fin >> board[i][j];
            }
        }
        fin.close();
        int temp = 0;
        for (int i = -2; i <= 2; i++)
        {
            for (int j = -2; j <= 2; j++)
            {
                temp++;
                if (!(i == 0 && j == 0))
                {
                    move_x[temp] = i;
                    move_y[temp] = j;
                }
            }
        }
        game_begin();
    }
    else if(order == 3)
    {
        help();
        cout << endl;
        cout << "请输入数字以选择" << endl;
        cout << "1.返回上一级" << endl;
        cout << "2.退出游戏" << endl;

        int temp;
        cin >> temp;
        if (temp == 1)
        {
            system("cls");
            menu();
        }
        else
            return;
    }
    else if (order == 4)
    {
        return;
    }
    else
    {
        cout << "请输入 1 或 2 或 3 或 4";
        menu();
    }
}

void init()//初始化
{
    board[1][7] = 1;
    board[7][1] = 1;
    board[1][1] = 2;
    board[7][7] = 2;

    int temp = 0;
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            temp++;
            if (!(i == 0 && j == 0))
            {
                move_x[temp] = i;
                move_y[temp] = j;
            }
        }
    }

}

void sum(int a[8][8])//统计各种棋子的数目
{
    cnt[0] = 0;
    cnt[1] = 0;
    cnt[2] = 0;
    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j <= 7; j++)
        {
            cnt[a[i][j]]++;
        }
    }
    return;
}

int check_move(int x, int y)
{
    int cnt = 0;
    int nx, ny;
    for (int i = 1; i <= 24; i++)
    {
        nx = x + move_x[i];
        ny = y + move_y[i];
        if (nx >= 1 && nx <= 7 && ny >= 1 && ny <= 7 && !(board[nx][ny]))
        {
            cnt++;
            move_list[cnt] = i;
        }
    }
    return cnt;
}

void move(int x1, int y1, int x2, int y2, int chess, int a[8][8])//移动棋子（已经检查过安全性后）,chess表示种类是黑子还是白子
{
    if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1)//邻近八个
    {
        a[x2][y2] = chess;
        for (int i = 0; i < 8; i++)//新位置的邻近八个
        {
            int new_x = x2 + near_block[i][0];
            int new_y = y2 + near_block[i][1];
            if (a[new_x][new_y])//有敌棋再变色
            {
                a[new_x][new_y] = chess;
            }
        }
    }
    else//外十六个
    {
        a[x1][y1] = 0;//原本位置置空
        a[x2][y2] = chess;
        for (int i = 0; i < 8; i++)
        {
            int new_x = x2 + near_block[i][0];
            int new_y = y2 + near_block[i][1];
            if (a[new_x][new_y])
            {
                a[new_x][new_y] = chess;
            }
        }
    }
}

void save()//存档
{
    ofstream fout;
    fout.open("record.txt");
    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j <= 7; j++)
        {
            fout << ' ' << board[i][j];
        }
        fout << endl;
    }
    fout.close();
}

void game_begin()
{
    system("cls");
    while (1)//处理每一回合
    {
        printf_chessboard();
        
        over();
        if (over_flag == 1)
            break;

        cout << "继续游戏输入1，退出游戏输入0" << endl;
        int tem;
        cin >> tem;
        if (tem == 0)
            over_flag = 1;
        if (over_flag == 1)
            break;

        cout << "存档请输入1，否则输入0" << endl;//存档否
        int order;
        cin >> order;
        if (order == 1)
        {
            cout << "已存档！" << endl;
            save();
            return;
        }
        copy(board, temp_board2);

        if (judge_player())
        {
            check();
            move(s_x, s_y, e_x, e_y, 1, board);//移动棋子
            system("cls");
            printf_chessboard();
            Sleep(1500);//休眠1.5秒
        }
        
        if (judge_ai())
        {
            artificial_retard();//computer  电脑
            printf_chessboard();
        }
        takeback();
    }
}

void copy(int a[8][8], int b[8][8])
{
    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j <= 7; j++)
        {
            b[i][j] = a[i][j];
        }
    }
}

void help()
{
    cout << "同化棋[1](Ataxx），是Dave Crummack和Craig Galley在1988年发明，" << endl
        << "1990年出品于电视游戏而流行的两人棋类，可说是黑白棋的衍生。" << endl
        << "原本叫Infection，在Amiga、Commodore 64和Atari ST运行。" << endl
        << "初始布置为双方各将两枚棋子放在最外的对角格。" << endl
        << "玩家必须轮流移动一枚己子到一个空棋位，该棋位可以是邻近八格（包括对角相邻" << endl
        << "的格）之一，或相隔一格的次邻十二格之一。移动的新棋位，会使邻近的所有敌棋如黑" << endl
        << "白棋一样变成己方。如果棋子移到的是邻接八格，会有一颗新己棋出现在原先棋位。" << endl
        << "无法行棋需弃权。当两方都无法行棋时，游戏结束。以最多子者胜。" << endl;
}

void artificial_retard()
{
    system("cls");
    cout << "电脑陪您玩儿啦！！！" << endl;
    int best_i, best_j, ni, nj, best = 0;
    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j <= 7; j++)
        {
            if (board[i][j] == 2)
            {
                if (check_move(i, j) != 0)
                {
                    for (int k = 1; k <= check_move(i, j); k++)
                    {
                        int nx = i + move_x[move_list[k]];
                        int ny = i + move_y[move_list[k]];

                        copy(board, temp_board);

                        move(i, j, nx, ny, 2, temp_board);

                        sum(temp_board);
                        if (cnt[2] > best)
                        {
                            best = cnt[2];
                            best_i = i;
                            best_j = j;
                            ni = nx;
                            nj = ny;
                        }
                    }
                }
            }
        }
    }

    move(best_i, best_j, ni, nj, 2, board);
    cout << "电脑将（" << best_i << "," << best_j << ")移动到了（" << ni << "," << nj << "）！" << endl;
}

bool judge_player()
{
    bool temp = false;
    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j <= 7; j++)
        {
            if (board[i][j] == 1 && !temp)
            {
                if (check_move(i, j))
                {
                    temp = true;
                }
            }
        }
    }
    return temp;
}

bool judge_ai()
{
    bool temp = false;
    for (int i = 1; i <= 7; i++)//电脑无棋可下，就判赢
    {
        for (int j = 1; j <= 7; j++)
        {
            if (board[i][j] == 2 && !temp)
            {
                if (check_move(i, j))
                {
                    temp = true;
                }
            }
        }
    }
    return temp;
}

void check()
{
    while (1)//处理输入起点和终点合法性
    {
        cout << "请输入你想移动的棋子所在行和列，以空格隔开" << endl;
        cin >> s_x >> s_y;
        int flag = 0;

        if (board[s_x][s_y] != 1)//玩家输入错误
        {
            cout << "该位置没有你的棋子" << endl;
        }
        else if (check_move(s_x,s_y) == 0)
        {
            cout << "该位置的棋子无法移动" << endl;
        }
        else
        {
            while (1)//处理输入终点合法性
            {
                cout << "请输入你想要移动到的位置，以空格隔开" << endl;
                cin >> e_x >> e_y;

                if (abs(e_x - s_x) > 2 || abs(e_y - s_y) > 2 || board[e_x][e_y] == 2)//移动不合法
                {
                    cout << "你无法移动到该处" << endl;
                    continue;
                }
                else
                {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag == 1)//找到了正确输入起点终点，退出循环
            break;
    }
}

void takeback()
{
    cout << "是否悔棋，悔棋输入1，否则输入0" << endl;
    int back;
    cin >> back;
    system("cls");
    if (back == 1)
    {
        copy(temp_board2, board);
    }
}

void over()
{
    if ( (!judge_player()) || (!judge_ai()) )
    {
        cout << "游戏结束" << endl;
        sum(board);
        if (cnt[1] > cnt[2])
        {
            cout << "你赢了！！";
            over_flag = 1;
        }
        else
        {
            cout << "你输了...";
            over_flag = 1;
        }
        return;
    }
}