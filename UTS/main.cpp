#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

// Konstanta ukuran maze
const int MAZE_CELLS = 8; // Sesuai perhitungan NIM (1+0+6+8+3 = 18 -> ujung 8)
const int GRID_SIZE = MAZE_CELLS * 2 + 1; // 17x17 matrix untuk mencakup dinding

// Matriks grid: 0 = Dinding, 1 = Jalur
int grid[GRID_SIZE][GRID_SIZE];

// Posisi Player
int playerX, playerY;

// Posisi NRP (Acak)
int nrpX, nrpY;

// Struktur untuk algoritma Prim
struct Point { int x, y; };
vector<Point> frontier;

// Fungsi pembantu untuk Algoritma Prim
void addFrontier(int x, int y) {
    if (x >= 1 && x < GRID_SIZE - 1 && y >= 1 && y < GRID_SIZE - 1 && grid[y][x] == 0) {
        bool exists = false;
        for (Point p : frontier) {
            if (p.x == x && p.y == y) { exists = true; break; }
        }
        if (!exists) frontier.push_back({ x, y });
    }
}

// Fungsi menghasilkan Maze Dinamis dengan Randomized Prim's Algorithm
void generateMaze() {
    // 1. Reset semua menjadi dinding (0)
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = 0;
        }
    }
    frontier.clear();

    // 2. Tentukan titik awal jalur (harus angka ganjil)
    grid[1][1] = 1;
    addFrontier(1, 3);
    addFrontier(3, 1);

    // 3. Loop utama Algoritma Prim
    while (!frontier.empty()) {
        // Pilih cell frontier secara acak
        int r = rand() % frontier.size();
        Point curr = frontier[r];
        frontier.erase(frontier.begin() + r);

        grid[curr.y][curr.x] = 1; // Jadikan jalur

        // Cari tetangga yang sudah menjadi jalur (jarak 2)
        vector<Point> visitedNeighbors;
        if (curr.x >= 3 && grid[curr.y][curr.x - 2] == 1) visitedNeighbors.push_back({ curr.x - 2, curr.y });
        if (curr.x <= GRID_SIZE - 4 && grid[curr.y][curr.x + 2] == 1) visitedNeighbors.push_back({ curr.x + 2, curr.y });
        if (curr.y >= 3 && grid[curr.y - 2][curr.x] == 1) visitedNeighbors.push_back({ curr.x, curr.y - 2 });
        if (curr.y <= GRID_SIZE - 4 && grid[curr.y + 2][curr.x] == 1) visitedNeighbors.push_back({ curr.x, curr.y + 2 });

        if (!visitedNeighbors.empty()) {
            int rn = rand() % visitedNeighbors.size();
            Point n = visitedNeighbors[rn];
            // Hancurkan dinding di antara cell saat ini dan tetangga
            grid[(curr.y + n.y) / 2][(curr.x + n.x) / 2] = 1;
        }

        // Tambahkan tetangga baru ke frontier
        addFrontier(curr.x - 2, curr.y);
        addFrontier(curr.x + 2, curr.y);
        addFrontier(curr.x, curr.y - 2);
        addFrontier(curr.x, curr.y + 2);
    }

    // 4. Buat Pintu Masuk (Atas) dan Pintu Keluar (Bawah) di tengah
    // Kolom tengah untuk jalur pada grid 17x17 adalah index 9
    grid[GRID_SIZE - 1][7] = 1; // Pintu Atas
    grid[0][7] = 1;             // Pintu Bawah

    // 5. Letakkan Player di Pintu Atas
    playerX = 7;
    playerY = GRID_SIZE - 1;

    // 6. Tentukan posisi NRP secara acak pada jalur yang valid
    do {
        nrpX = 1 + (rand() % MAZE_CELLS) * 2;
        nrpY = 1 + (rand() % MAZE_CELLS) * 2;
    } while (nrpX == playerX && nrpY == playerY); // Jangan timpa posisi player
}

// Fungsi menggambar angka NRP "103"
// Disusun menggunakan balok (glRectf) tanpa melubangi poligon
void drawNRP(float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f); // Warna Merah

    // Membagi 1 cell menjadi skala kecil (15x15) agar angka muat di dalam 1 kotak jalur
    float u = 1.0f / 15.0f;

    // Lambda function untuk mempermudah gambar kotak kecil
    auto drawBlock = [&](float dx, float dy, float w, float h) {
        glRectf(x + dx * u, y + dy * u, x + (dx + w) * u, y + (dy + h) * u);
    };

    // --- Angka 1 ---
    drawBlock(2, 5, 1, 5); // Garis vertikal

    // --- Angka 0 ---
    drawBlock(5, 5, 3, 1); // Garis bawah
    drawBlock(5, 9, 3, 1); // Garis atas
    drawBlock(5, 5, 1, 5); // Garis kiri
    drawBlock(7, 5, 1, 5); // Garis kanan

    // --- Angka 3 ---
    drawBlock(10, 5, 3, 1); // Garis bawah
    drawBlock(10, 7, 3, 1); // Garis tengah
    drawBlock(10, 9, 3, 1); // Garis atas
    drawBlock(12, 5, 1, 5); // Garis kanan
}

// Fungsi Render OpenGL
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Gambar background warna Biru sebagai Dinding
    glColor3f(0.0f, 0.0f, 1.0f);
    glRectf(0, 0, GRID_SIZE, GRID_SIZE);

    // Gambar jalur warna Putih
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x] == 1) {
                // Menggunakan glRectf (sama dengan GL_POLYGON)
                glRectf(x, y, x + 1, y + 1);
            }
        }
    }

    // Gambar NRP di posisi random
    drawNRP(nrpX, nrpY);

    // Gambar Player (Hijau)
    glColor3f(0.0f, 1.0f, 0.0f);
    glRectf(playerX, playerY, playerX + 1, playerY + 1);

    glutSwapBuffers();
}

// Fungsi Input Keyboard
void keyboard(unsigned char key, int x, int y) {
    int nextX = playerX;
    int nextY = playerY;

    switch (key) {
        case 'w': case 'W': nextY++; break; // Atas
        case 's': case 'S': nextY--; break; // Bawah
        case 'a': case 'A': nextX--; break; // Kiri
        case 'd': case 'D': nextX++; break; // Kanan
        case 'c': case 'C': generateMaze(); break; // Reset Maze
        case 'q': case 'Q': exit(0); break; // Keluar
    }

    // Deteksi Tabrakan (Collision Detection) agar tidak menembus dinding
    if (nextX >= 0 && nextX < GRID_SIZE && nextY >= 0 && nextY < GRID_SIZE) {
        if (grid[nextY][nextX] == 1) {
            playerX = nextX;
            playerY = nextY;
        }
    }

    glutPostRedisplay(); // Render ulang setelah bergerak
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Border luar warna hitam
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Setting Ortho -1 sampai 18 agar ada border hitam di sekeliling grid 17x17
    gluOrtho2D(0, GRID_SIZE, 0, GRID_SIZE);
}

int main(int argc, char** argv) {
    // Menampilkan instruksi di Console
    cout << "Aplikasi maze algoritma Prim (Dinamis)" << endl;
    cout << "Ukuran 8x8 (Logika Matrix 17x17), NRP Terkecil: 103" << endl;
    cout << "Press 'c' to 'c'hange the maze layout" << endl;
    cout << "Press 'a', 's', 'd', and 'w' to move the player object" << endl;
    cout << "Press 'q', to 'q'uit the program" << endl;

    srand(time(0)); // Seed random
    generateMaze(); // Buat maze pertama kali

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600); // Ukuran Window
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Maze Dinamis Algoritma Prim - Kelompok 4 (NRP 103)");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}