#include "main.h"

position player;
position bot1;
position bot2;
position bot3;

int map[10][10];      //1==��1   //2==��2  //3==��3  //4==player
int serowall[10][9];        //���� ��
int garowall[9][10];        //���� ��
int px[12] = { 1,0,-1,0,1,1,-1,-1,2,0,-2,0 };   //�����¿� 4�밢(3,1,9,7) 2ĭ�� �����¿�
int py[12] = { 0,1,0,-1,1,-1,1,-1,0,2,0,-2 };   
int que[90][2];                                 //dfs���� ť
int firstattck;                                 //���� ���ϴ� ����
int playerscore, bot1score, bot2score, bot3score;       //���� ����
int wallnum;                                            //���� �ѹ� �����ؼ� �����ȵǰ� ����

int main() {
    firstattck = 1;
    Play();
}