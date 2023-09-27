#include "main.h"

void reset() {      //���� ��� �� map���� ��ġ ����
    printf("player : %d bot1 : %d bot2 : %d bot3 : %d\n", playerscore, bot1score, bot2score, bot3score);
    map[player.x][player.y] = 0;
    map[bot1.x][bot1.y] = 0;
    map[bot2.x][bot2.y] = 0;
    map[bot3.x][bot3.y] = 0;
}

int win() {      //���� �̰���� üũ               //��1 ���ʽ��� //��2 ���ʽ��� //��3 �����ʽ���
    if (player.x == 1) {
        printf("player�� �̰���ϴ�.\n");
        playerscore++;
        return 1;
    }
    else if (bot1.x == 9) {
        printf("bot1�� �̰���ϴ�.\n");
        bot1score++;
        return 1;
    }
    else if (bot2.y == 9) {
        printf("bot2�� �̰���ϴ�.\n");
        bot2score++;
        return 1;
    }
    else if (bot3.y == 1) {
        printf("bot3�� �̰���ϴ�.\n");
        bot3score++;
        return 1;
    }
    else return 0;
}

void initSetting() {
    //�ʱ� ��ġ, ���� ������ ����
    bot1 = { 1,5,5 };
    bot2 = { 5,1,5 };
    bot3 = { 5,9,5 };
    player = { 9,5,5 };
    //����� ������ �پ�ѱ� ���� map�� ��ġ ǥ��
    map[bot1.x][bot1.y] = 1;
    map[bot2.x][bot2.y] = 2;
    map[bot3.x][bot3.y] = 3;
    map[player.x][player.y] = 4;
    //�� �ѹ� ����
    wallnum = 1;
    //�� ���ӽ� �� �ʱ�ȭ
    removeWall();
}

void Start() {                                          //���� ��� ���� ����?
    int keep_going;
    printf("������ �����ұ��? (����=0,�׸�=1)\n");
    scanf("%d", &keep_going);
    if (keep_going == 1) {
        printf("������ �����մϴ�.\n");
        exit(0);
    }
    initSetting();
}

void playermove() {                                     //player��
    int playeract;
    printf("���� �������� 1�� ���� �����̷��� 2��\n");
    scanf("%d", &playeract);
    if (playeract == 1 && player.canwall != 0) {        //�÷��̾ ���� ���´�&&�÷��̾ ���� �� �ִ� ���� ������ 1�� �̻��̸�
        player.canwall--;
        walling();
    }
    else if (playeract == 1) {                          //�÷��̾ ���� ���´� �ߴµ� ���� ������
        printf("���� �������� �����Ƿ� ���� �����Դϴ�.\n");
        move();
    }
    else move();
    printf("�÷��̾ �÷��� ��\n");
    print();
}

void Play() {
    bot1score = 0;
    bot2score = 0;
    bot3score = 0;
    playerscore = 0;
    while (firstattck++) {
        Start();
        while (1) {
            if (firstattck % 4 == 2) {   //player�� �����϶�
                playermove();
                if (win() == 1) break;
                bot1move();
                if (win() == 1) break;
                bot2playing();
                if (win() == 1) break;
                bot3move();
                if (win() == 1) break;
            }
            else if(firstattck%4==3) {                  //bot1����
                bot1move();
                if (win() == 1) break;
                bot2playing();
                if (win() == 1) break;
                bot3move();
                if (win() == 1) break;
                playermove();
                if (win() == 1) break;
            }
            else if (firstattck % 4 == 0) {             //bot2����
                bot2playing();
                if (win() == 1) break;
                bot3move();
                if (win() == 1) break;
                playermove();
                if (win() == 1) break;
                bot1move();
                if (win() == 1) break;
            }
            else{                                        //bot3����
                bot3move();
                if (win() == 1) break;
                playermove();
                if (win() == 1) break;
                bot1move();
                if (win() == 1) break;
                bot2playing();
                if (win() == 1) break;
            }
        }
        reset();
    }
}