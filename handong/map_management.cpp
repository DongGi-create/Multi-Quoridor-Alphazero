#include "main.h"

int check[10][10];

bool isRange(int x, int y) {
    if (x < 1 || x > 9 || y < 1 || y > 9)return false;
    return true;
}

bool wallIsRange(int x, int y) {
    if (x < 1 || x > 8 || y < 1 || y > 8)return false;
    return true;
}

struct Queue {
    int front;
    int rear;
    void init() { front = rear = 0; }
    bool isEmpty() {
        if (front == rear) {
            return true;
        }
        else return false;
    }
}q;

bool canReach(int way,int endpoint) {
    int x = que[q.front][0];
    int y = que[q.front++][1];
    if (way==1) {
        if (x == endpoint)return true;
    }
    else {
        if (y == endpoint) return true;
    }
    for (int i = 0; i < 4; i++) {
        int nx = x + px[i];
        int ny = y + py[i];
        if (isRange(nx, ny) && check[nx][ny] == 0) {
            switch (i) {
            case Down:
                if (garowall[x][y] == 0) {
                    check[nx][ny] = 1;
                    que[q.rear][0] = nx;
                    que[q.rear++][1] = ny;
                }
                break;
            case Right:
                if (serowall[x][y] == 0) {
                    check[nx][ny] = 1;
                    que[q.rear][0] = nx;
                    que[q.rear++][1] = ny;
                }
                break;
            case Up:
                if (garowall[x - 1][y] == 0) {
                    check[nx][ny] = 1;
                    que[q.rear][0] = nx;
                    que[q.rear++][1] = ny;
                }
                break;
            case Left:
                if (serowall[x][y - 1] == 0) {
                    check[nx][ny] = 1;
                    que[q.rear][0] = nx;
                    que[q.rear++][1] = ny;
                }
                break;
            }
        }
    }
    return false;
}

int dfs() {         //Ż�Ⱑ������ �˾ƺ��� �Լ�
    int flag = 0;         //Ż�Ⱑ���� �ָ��� +1�ؼ� flag==4�̸� ��� Ż�Ⱑ��!
    q.init();
    memset(check, 0, sizeof(check));
    que[q.rear][0] = player.x;
    que[q.rear++][1] = player.y;
    check[player.x][player.y] = 1;
    while (!q.isEmpty()) {
        if (canReach(1, 1)) {
            flag++;
            break;
        }
    }

    q.init();
    memset(check, 0, sizeof(check));
    que[q.rear][0] = bot1.x;
    que[q.rear++][1] = bot1.y;
    check[bot1.x][bot1.y] = 1;
    while (!q.isEmpty()) {
        if (canReach(1,9)) {
            flag++;
            break;
        }
    }

    q.init();
    memset(check, 0, sizeof(check));
    que[q.rear][0] = bot2.x;
    que[q.rear++][1] = bot2.y;
    check[bot2.x][bot2.y] = 1;
    while (!q.isEmpty()) {
        if (canReach(2, 9)) {
            flag++;
            break;
        }
    }

    q.init();
    memset(check, 0, sizeof(check));
    que[q.rear][0] = bot3.x;
    que[q.rear++][1] = bot3.y;
    check[bot3.x][bot3.y] = 1;
    while (!q.isEmpty()) {
        if (canReach(2, 1)) {
            flag++;
            break;
        }
    }
    if (flag == 4) return 0;      //�� �� �ִ�.
    else return 1;
}

bool crossWall(int type, int startx, int starty) {              //���� �������� �˾ƺ��� �Լ�
    switch (type) {
    case garo:
        if (garowall[startx][starty] == 0 && garowall[startx][starty + 1] == 0) {
            int up = serowall[startx][starty];
            int down = serowall[startx + 1][starty];
            if ((up == 0 && down == 0) || up != down) {
                return false;
            }
        }
        return true;
        break;
    case sero:
        if (serowall[startx][starty] == 0 && serowall[startx + 1][starty] == 0) {
            int left = garowall[startx][starty];
            int right = garowall[startx][starty + 1];
            if ((left == 0 && right == 0) || left != right) {
                return false;
            }
        }
        return true;
        break;
    }
}

void walling() {      //�������         //��������-> �Ѵ�0 || ���̴ٸ���
    int w;
    printf("���κ� : 1 ���κ� :2\n");
    scanf("%d", &w);
    int startx, starty;
    printf("���� ��ġx,y��ǥ�� �Է��ϼ���. //�Է� ���� EX)1 1.\n");
    scanf("%d %d", &startx, &starty);
    if (wallIsRange(startx, starty)) {
        if (!crossWall(w, startx, starty)) {
            switch (w) {
            case garo:
                garowall[startx][starty] = wallnum;
                garowall[startx][starty + 1] = wallnum++;
                if (dfs() != 0) {
                    printf("Ż�ⱸ�� �����ϴ�. ���� �� �����ϴ�. �ٽ� �����ϼ���.\n");
                    garowall[startx][starty] = 0;
                    garowall[startx][starty + 1] = 0;
                    wallnum--;
                    walling();
                }
                break;
            case sero:
                serowall[startx][starty] = wallnum;
                serowall[startx + 1][starty] = wallnum++;
                if (dfs() != 0) {
                    printf("Ż�ⱸ�� �����ϴ�. ���� �� �����ϴ�. �ٽ� �����ϼ���.\n");
                    serowall[startx][starty] = 0;
                    serowall[startx + 1][starty] = 0;
                    wallnum--;
                    walling();
                }
                break;
            }
        }
        else {
            printf("���� �� �� �����ϴ� (���� Ȥ�� �̹� ����). �ٽ� �����ϼ���..\n");
            walling();
        }
    }
    else {
        printf("������ ����ϴ�. �ٽ� �����ϼ���.\n");
        walling();
    }
}

void print() {         //�� ����Ʈ �ϴ� �Լ�
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            printf("%d", map[i][j]);
            if (serowall[i][j] == 0) printf(" ");
            else printf("|");
        }
        printf("%d\n", map[i][9]);
        for (int j = 1; j <= 9; j++) {
            if (garowall[i][j] == 0) printf("  ");
            else printf("- ");
        }
        putchar('\n');
    }
    for (int j = 1; j <= 8; j++) {
        printf("%d", map[9][j]);
        if (serowall[9][j] == 0) printf(" ");
        else printf("|");
    }
    printf("%d\n\n", map[9][9]);
}

void removeWall() {                 //�� �ʱ�ȭ �ϴ� �Լ�
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 8; j++) {
            serowall[i][j] = 0;
            garowall[j][i] = 0;
        }
    }
}