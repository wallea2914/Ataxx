#include<iostream>
#include<fstream>
#include<Windows.h>
using namespace std;

int board[8][8] = { 0 }, temp_board[8][8] = { 0 },temp_board2[8][8];//��¼���̣�1������ң�2���ӵ���
int cnt[3] = { 0 };//��¼�ڡ��ס���������Ŀ
int near_block[8][2] = { 0,1,0,-1,1,0,-1,0,1,1,-1,1,1,-1,-1,-1 };//�ڽ��˸�
int move_x[25], move_y[25];//24���ƶ���ʽ���ڰ���ʮ��
int move_list[25] = { 0 };//����AI����
int s_x, s_y, e_x, e_y;//������������յ�
int over_flag = 0;//��Ϸ������־


void printf_chessboard();//��ӡ����
void menu();//�˵���
void init();//��ʼ�� ���� �� �ƶ���ʽ����
void sum(int a[8][8]);//������Ŀ���
int check_move(int, int);//����Ƿ���ƶ����������ƶ���ʽ����
void move(int, int, int, int, int, int a[8][8]);//�ƶ����ӣ���ת
void save();//�浵
void game_begin();//��Ϸ�ܿ��
void artificial_retard();//ai����
void copy(int a[8][8], int b[8][8]);//�������̣���ai�ͻ���ʹ��
void help();//�����ֲ�
bool judge_player();//�жϵ����Ƿ���Ȩ
bool judge_ai();//�ж�����Ƿ���Ȩ
void check();//����������ĺϷ���
void takeback();//����
void over();//�ж���Ϸ����

int main()
{
    menu();
    return 0;
}

void printf_chessboard()//��ӡ����
{
    sum(board);
    cout << "Player:   " << cnt[1] <<  "  AI:   " << cnt[2] << endl;

    cout << " �������������������������������������������������������� " << endl;
    cout << "     1   2   3   4   5   6   7  " << endl;
    cout << "   ���������ש������ש������ש������ש������ש������ש�������" << endl;
    cout << " 1 ��";
    for (int i = 1; i <= 7; i++)
    {
        if (board[1][i] == 1)
            cout << " ��";
        else if (board[1][i] == 2)
            cout << " ��";
        else
            cout << "   ��";
    }
    cout << endl;

    for (int i = 2; i <= 7; i++)
    {
        cout << "   �ǩ������贈�����贈�����贈�����贈�����贈�����贈������" << endl;
        cout << ' ' << i << " ��";
        for (int j = 1; j <= 7; j++)
        {
            if (board[i][j] == 1)
                cout << " ��";
            else if (board[i][j] == 2)
                cout << " ��";
            else
                cout << "   ��";
        }
        cout << endl;
    }
    cout << "   ���������ߩ������ߩ������ߩ������ߩ������ߩ������ߩ�������" << endl;
}

void menu()//�˵���
{
    cout << " �������������������������������������������������������� " << endl;
    cout << "ͬ����" << endl;
    cout << "��ִ����,����ִ����" << endl;
    cout << "������������ѡ��" << endl;
    cout << " �������������������������������������������������������� " << endl;
    cout << "1.�µĿ�ʼ" << endl;
    cout << "2.��ȡ�浵" << endl;
    cout << "3.����" << endl;
    cout << "4.�˳���Ϸ" << endl;

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
        cout << "������������ѡ��" << endl;
        cout << "1.������һ��" << endl;
        cout << "2.�˳���Ϸ" << endl;

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
        cout << "������ 1 �� 2 �� 3 �� 4";
        menu();
    }
}

void init()//��ʼ��
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

void sum(int a[8][8])//ͳ�Ƹ������ӵ���Ŀ
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

void move(int x1, int y1, int x2, int y2, int chess, int a[8][8])//�ƶ����ӣ��Ѿ�������ȫ�Ժ�,chess��ʾ�����Ǻ��ӻ��ǰ���
{
    if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1)//�ڽ��˸�
    {
        a[x2][y2] = chess;
        for (int i = 0; i < 8; i++)//��λ�õ��ڽ��˸�
        {
            int new_x = x2 + near_block[i][0];
            int new_y = y2 + near_block[i][1];
            if (a[new_x][new_y])//�е����ٱ�ɫ
            {
                a[new_x][new_y] = chess;
            }
        }
    }
    else//��ʮ����
    {
        a[x1][y1] = 0;//ԭ��λ���ÿ�
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

void save()//�浵
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
    while (1)//����ÿһ�غ�
    {
        printf_chessboard();
        
        over();
        if (over_flag == 1)
            break;

        cout << "������Ϸ����1���˳���Ϸ����0" << endl;
        int tem;
        cin >> tem;
        if (tem == 0)
            over_flag = 1;
        if (over_flag == 1)
            break;

        cout << "�浵������1����������0" << endl;//�浵��
        int order;
        cin >> order;
        if (order == 1)
        {
            cout << "�Ѵ浵��" << endl;
            save();
            return;
        }
        copy(board, temp_board2);

        if (judge_player())
        {
            check();
            move(s_x, s_y, e_x, e_y, 1, board);//�ƶ�����
            system("cls");
            printf_chessboard();
            Sleep(1500);//����1.5��
        }
        
        if (judge_ai())
        {
            artificial_retard();//computer  ����
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
    cout << "ͬ����[1](Ataxx������Dave Crummack��Craig Galley��1988�귢����" << endl
        << "1990���Ʒ�ڵ�����Ϸ�����е��������࣬��˵�Ǻڰ����������" << endl
        << "ԭ����Infection����Amiga��Commodore 64��Atari ST���С�" << endl
        << "��ʼ����Ϊ˫��������ö���ӷ�������ĶԽǸ�" << endl
        << "��ұ��������ƶ�һö���ӵ�һ������λ������λ�������ڽ��˸񣨰����Խ�����" << endl
        << "�ĸ�֮һ�������һ��Ĵ���ʮ����֮һ���ƶ�������λ����ʹ�ڽ������е������" << endl
        << "����һ����ɼ�������������Ƶ������ڽӰ˸񣬻���һ���¼��������ԭ����λ��" << endl
        << "�޷���������Ȩ�����������޷�����ʱ����Ϸ���������������ʤ��" << endl;
}

void artificial_retard()
{
    system("cls");
    cout << "�������������������" << endl;
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
    cout << "���Խ���" << best_i << "," << best_j << ")�ƶ����ˣ�" << ni << "," << nj << "����" << endl;
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
    for (int i = 1; i <= 7; i++)//����������£�����Ӯ
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
    while (1)//�������������յ�Ϸ���
    {
        cout << "�����������ƶ������������к��У��Կո����" << endl;
        cin >> s_x >> s_y;
        int flag = 0;

        if (board[s_x][s_y] != 1)//����������
        {
            cout << "��λ��û���������" << endl;
        }
        else if (check_move(s_x,s_y) == 0)
        {
            cout << "��λ�õ������޷��ƶ�" << endl;
        }
        else
        {
            while (1)//���������յ�Ϸ���
            {
                cout << "����������Ҫ�ƶ�����λ�ã��Կո����" << endl;
                cin >> e_x >> e_y;

                if (abs(e_x - s_x) > 2 || abs(e_y - s_y) > 2 || board[e_x][e_y] == 2)//�ƶ����Ϸ�
                {
                    cout << "���޷��ƶ����ô�" << endl;
                    continue;
                }
                else
                {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag == 1)//�ҵ�����ȷ��������յ㣬�˳�ѭ��
            break;
    }
}

void takeback()
{
    cout << "�Ƿ���壬��������1����������0" << endl;
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
        cout << "��Ϸ����" << endl;
        sum(board);
        if (cnt[1] > cnt[2])
        {
            cout << "��Ӯ�ˣ���";
            over_flag = 1;
        }
        else
        {
            cout << "������...";
            over_flag = 1;
        }
        return;
    }
}