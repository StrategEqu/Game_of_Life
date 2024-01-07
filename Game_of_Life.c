#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLS 80
#define EMPTY ' '
#define POINT 'x'
#define GRN "\x1B[32m"
#define RESET "\x1B[0m"

void init(int data[ROWS][COLS], int* speed, char* symb);
void render(int data[ROWS][COLS], int count, int speed, char symb);
void update(int data[ROWS][COLS]);

void set_random_data(int data[ROWS][COLS]);
int get_neighbours(int data[ROWS][COLS], int y, int x);

int main() {
    int data[ROWS][COLS], is_game = 1, count = 0, speed = 5;
    char symb = POINT;

    init(data, &speed, &symb);

    while (is_game) {
        system("clear");
        render(data, count, speed, symb);
        update(data);
        usleep(speed * 100000);
        count++;
    }

    return 0;
}

void init(int data[ROWS][COLS], int* speed, char* symb) {
    int is_rand = 0, is_end_line = 0;
    char c = ' ';

    scanf("%d %c", speed, symb);

    for (int y = 0; y < ROWS; y++) {
        is_end_line = 0;

        for (int x = 0; x < COLS; x++) {
            if (is_end_line != 1) {
                scanf("%c", &c);
            }

            if (c == 'r') {
                is_rand = 1;
                break;
            }

            if (c != '\n') {
                data[y][x] = c == '*' ? 1 : 0;
            } else {
                data[y][x] = 0;
                is_end_line = 1;
            }
        }

        if (is_rand) {
            break;
        }
    }

    if (is_rand) {
        set_random_data(data);
    }
}

void set_random_data(int data[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            data[y][x] = rand() % 2;
        }
    }
}

int get_neighbours(int data[ROWS][COLS], int x, int y) {
    int count = 0;

    int new_y, new_x;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            new_x = x + j;
            new_y = y + i;

            new_x = (new_x == -1) ? COLS - 1 : new_x;
            new_x = (new_x == COLS) ? 0 : new_x;

            new_y = (new_y == -1) ? ROWS - 1 : new_y;
            new_y = (new_y == ROWS) ? 0 : new_y;

            count += data[new_y][new_x];
        }
    }

    return count;
}

void update(int data[ROWS][COLS]) {
    int new_data[ROWS][COLS];

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            int n = get_neighbours(data, x, y);

            if (data[y][x] == 1) {
                new_data[y][x] = (n == 2 || n == 3) ? 1 : 0;
            } else {
                new_data[y][x] = (n == 3) ? 1 : 0;
            }
        }
    }

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            data[y][x] = new_data[y][x];
        }
    }
}

int get_points(int data[ROWS][COLS]) {
    int count = 0;

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            count = (data[y][x] == 1) ? count + 1 : count;
        }
    }

    return count;
}

void render_line(char c) {
    for (int x = 0; x < COLS; x++) {
        printf("%c", c);
    }
    printf("\n");
}

void render(int data[ROWS][COLS], int count, int speed, char symb) {
    int points = get_points(data);

    printf("\nLife circle: %d | Speed: %.1fs | Points: %d\n", count, 0.1 * speed, points);
    render_line('_');

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            (data[y][x] == 1) ? printf(GRN "%c" RESET, symb) : printf("%c", EMPTY);
        }
        printf("\n");
    }

    render_line('-');
}