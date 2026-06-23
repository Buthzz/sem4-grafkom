# Panduan Studi Kelompok & Dokumentasi Kode Sumber `main.cpp`
## Proyek UAS Grafika Komputer (Labirin 3D & Animasi NRP)

Dokumentasi ini dirancang secara khusus untuk **studi kelompok** dalam mempersiapkan **Asistensi Demo**. Panduan ini membedah arsitektur kode secara menyeluruh, mencakup pemetaan kriteria penilaian langsung ke baris kode, penjelasan pustaka, fungsi OpenGL/GLUT, analisis algoritma, panduan modifikasi parameter, serta **daftar pertanyaan jebakan (Q&A) yang sering ditanyakan asisten**.

---

## 1. Panduan Demo: Pemetaan Kriteria Penilaian ke Baris Kode

Berikut adalah contekan cepat (*cheat sheet*) untuk menjawab pertanyaan asisten praktikum saat demo berdasarkan 12 Kriteria Penilaian yang diujikan:

### Kriteria 1: Semua vertex diberi vektor normalnya (Bobot: 25)
*   **Lokasi Kode**: Fungsi `drawDinding` (baris 57-109), `drawPlayer` (baris 123-175), `drawLantai` (baris 190-242), dan `drawNRP3D` (baris 274-741).
*   **Cara Pembuktian**: Tunjukkan perintah `glNormal3f(nx, ny, nz)` sebelum perintah `glVertex3f(...)` di setiap poligon.
*   **Penjelasan**: Vektor normal menentukan arah hadap permukaan objek secara tegak lurus. Tanpa normal ini, OpenGL tidak bisa menghitung pantulan cahaya (diffuse dan specular) dari lampu, sehingga objek hanya akan terlihat rata/datar (*flat* tanpa shading).

### Kriteria 2: Semua objek diberi material/warna sendiri (Bobot: 5)
*   **Lokasi Kode**:
    *   **Dinding (Merah)**: `drawDinding` (baris 48-55)
    *   **Pemain (Biru)**: `drawPlayer` (baris 114-121)
    *   **Lantai (Abu-abu)**: `drawLantai` (baris 181-188)
    *   **NRP (Hijau)**: `drawNRP3D` (baris 265-272)
*   **Cara Pembuktian**: Tunjukkan deklarasi array material (`mat_ambient`, `mat_diffuse`, `mat_specular`, `mat_shininess`) lalu panggil `glMaterialfv(GL_FRONT, ..., ...)`.
*   **Penjelasan**: Kami menggunakan model material bawaan OpenGL (`glMaterialfv` dengan target `GL_FRONT`) dan menghindari pewarnaan datar `glColor` agar objek merespons intensitas cahaya secara realistis (memantulkan kilau dan bayangan).

### Kriteria 3: Sumber cahaya Ambient, Diffuse, Specular (Bobot: 5)
*   **Lokasi Kode**: Fungsi `myinit` (baris 1069-1091) and `updateLighting` (baris 807-855).
*   **Cara Pembuktian**:
    *   **Ambient Global**: `GL_LIGHT_MODEL_AMBIENT` (baris 1070-1071)
    *   **Diffuse Light (`LIGHT0`)**: Berkas cahaya dari atas `(8.5, 8.5, 10.0)` menggunakan parameter `GL_DIFFUSE` (baris 1074-1079).
    *   **Specular Light (`LIGHT1`)**: Berkas cahaya samping `(0.0, 0.0, 5.0)` menggunakan parameter `GL_SPECULAR` (baris 1082-1087).

### Kriteria 4: Mode view ketiga (FPS Camera) ketika tombol V ditekan (Bobot: 5)
*   **Lokasi Kode**: Fungsi `keyboard` pada kasus `'v' || 'V'` (baris 956-975) dan fungsi `display` (baris 879-886).
*   **Cara Pembuktian**: Tunjukkan bahwa `viewMode` diubah antara 0 (2D Ortho), 1 (3D Isometrik), dan 2 (3D FPS). Tekan tombol `V` pada program untuk mendemonstrasikan perubahan sudut pandang.

### Kriteria 5: Navigasi berjalan di dalam maze menggunakan tombol panah (Bobot: 25)
*   **Lokasi Kode**: Fungsi `specialKey` (baris 1007-1048).
*   **Cara Pembuktian**:
    *   `GLUT_KEY_UP` & `GLUT_KEY_DOWN` mengubah koordinat kamera `camPosX` dan `camPosY` berdasarkan arah hadap horizontal `camYaw` (baris 1016-1025).
    *   `GLUT_KEY_LEFT` & `GLUT_KEY_RIGHT` mengubah nilai sudut hadap `camYaw` kamera (baris 1026-1033).
    *   Sistem tabrakan mencegah kamera menembus dinding (`mazeGrid[tileY][tileX] == false` pada baris 1039-1044).

### Kriteria 6: Posisi awal kamera FPS di pintu masuk dan menghadap ke bawah (Bobot: 5)
*   **Lokasi Kode**: Fungsi `keyboard` saat tombol `V` pertama kali ditekan ke `viewMode == 2` (baris 958-964).
*   **Cara Pembuktian**:
    *   `camPosX = 7.5f` & `camPosY = 1.5f` (Pintu masuk labirin berada di grid X=7, Y=0. Menggunakan offset Y=1.5 agar kamera berada di dalam sel pertama).
    *   `camYaw = 90.0f` (Radian 90 derajat searah sumbu Y positif, yaitu menghadap ke arah dalam/bawah grid labirin).

### Kriteria 7: Tombol 1 untuk men-On/Off Ambient Light (Bobot: 5)
*   **Lokasi Kode**: Fungsi `keyboard` (baris 984-987) dan `updateLighting` (baris 808-819).
*   **Cara Pembuktian**: Tekan tombol `1` di keyboard $\rightarrow$ bayangan bayang-bayang dasar di area tertutup labirin akan langsung menjadi hitam pekat (off) atau terang abu-abu (on).

### Kriteria 8: Tombol 2 untuk men-On/Off Diffuse Light (Bobot: 5)
*   **Lokasi Kode**: Fungsi `keyboard` (baris 988-991) dan `updateLighting` (baris 821-833).
*   **Cara Pembuktian**: Tekan tombol `2` di keyboard $\rightarrow$ pencahayaan utama dari arah atas akan padam (`glDisable(GL_LIGHT0)`).

### Kriteria 9: Tombol 3 untuk men-On/Off Specular Light (Bobot: 5)
*   **Lokasi Kode**: Fungsi `keyboard` (baris 992-995) dan `updateLighting` (baris 835-847).
*   **Cara Pembuktian**: Tekan tombol `3` di keyboard $\rightarrow$ titik kilap berkilau pada kubus pemain (biru) dan angka NRP (hijau) akan hilang (`glDisable(GL_LIGHT1)`).

### Kriteria 10: Tombol 4 untuk mengubah mode siang/malam (Bobot: 5)
*   **Lokasi Kode**: Fungsi `keyboard` (baris 996-999) dan `updateLighting` (baris 850-854).
*   **Cara Pembuktian**: Tekan tombol `4` di keyboard $\rightarrow$ warna latar belakang (`glClearColor`) akan berubah dari putih benderang (siang) menjadi biru malam gelap gulita (malam), serta warna intensitas lampu meredup kebiruan.

### Kriteria 11: Tombol 5 untuk men-On/Off transparansi dinding (Bobot: 5)
*   **Lokasi Kode**: Fungsi `keyboard` (baris 1000-1002) dan fungsi `display` (baris 896-911).
*   **Cara Pembuktian**: Tekan tombol `5` di keyboard $\rightarrow$ seluruh dinding merah labirin akan langsung berubah menjadi tembus pandang (semi-transparan dengan alpha 0.3).

### Kriteria 12: Deteksi tabrakan (sekali saja) antara player dan NRP (Bobot: 5)
*   **Lokasi Kode**: Fungsi `cekTabrakan` (baris 857-868).
*   **Cara Pembuktian**:
    *   Fungsi ini mengecek apakah indeks grid pemain `playerX` dan `playerY` sama dengan posisi grid integer NRP `nrpTileX` dan `nrpTileY`.
    *   **Sekali saja**: Menggunakan flag `if (collisionDetected) return;` di baris 858. Begitu tabrakan terjadi sekali, status diset `true` dan program tidak akan mencetak tulisan berulang kali sampai labirin di-reset menggunakan tombol `C`.

---

## 2. Tanya Jawab (Q&A) / Pertanyaan Jebakan Asisten

Berikut adalah kumpulan pertanyaan teoretis dan praktis yang sering dilontarkan oleh asisten saat proses demo berlangsung, beserta panduan jawaban terbaiknya:

### Q1: "Kenapa aplikasi ini memakai `GLUT_DOUBLE` (Double Buffering) saat inisialisasi? Apa bedanya dengan `GLUT_SINGLE`?"
*   **Jawaban**: `GLUT_DOUBLE` menggunakan dua buffer (buffer depan yang ditampilkan ke layar dan buffer belakang tempat menggambar). Saat animasi bergerak, rendering dilakukan di buffer belakang, lalu ditukar secara instan menggunakan `glutSwapBuffers()`. Hal ini mencegah efek berkedip (*flickering*). Jika memakai `GLUT_SINGLE`, penggambaran langsung dilakukan di layar sehingga pengguna dapat melihat proses penggambaran baris demi baris yang memicu kedipan layar.

### Q2: "Kenapa koordinat lantai dasar digambar di posisi `Z = -0.01` hingga `-0.21`, bukan pas di `0.0`?"
*   **Jawaban**: Untuk mencegah masalah **Z-fighting** (perebutan kedalaman pixel). Jika lantai digambar tepat di koordinat `Z = 0.0`, posisinya akan berimpit tepat dengan sisi bawah kubus dinding dan player yang juga berada di `Z = 0.0`. OpenGL akan kebingungan menentukan pixel mana yang harus digambar di atas, menyebabkan layar berkedip dengan pola aneh saat kamera bergerak. Memberikan offset tipis (`-0.01`) memastikan lantai berada mutlak di bawah objek lainnya.

### Q3: "Mengapa objek transparan (dinding) harus digambar belakangan setelah objek solid (lantai, player, NRP)?"
*   **Jawaban**: Dalam grafika komputer 3D, efek transparansi (*alpha blending*) menggabungkan warna objek transparan (sumber) dengan warna objek yang sudah ada di belakangnya pada frame buffer (tujuan). Jika dinding transparan digambar pertama kali, frame buffer masih kosong (hanya warna latar), sehingga objek solid yang digambar belakangan akan menimpa dinding dan efek tembus pandangnya akan rusak/hilang.

### Q4: "Kenapa kamu menggunakan `glDepthMask(GL_FALSE)` saat menggambar dinding transparan?"
*   **Jawaban**: Agar penulisan ke Depth Buffer (*Z-buffer*) dimatikan sementara selama penggambaran dinding transparan. Jika tidak dimatikan, OpenGL akan mengira area tersebut sudah terisi oleh objek pekat terdepan, sehingga objek solid yang posisinya berada di belakang dinding tidak akan digambar sama sekali akibat tersaring oleh *Depth Test*.

### Q5: "Apa bedanya `GL_SMOOTH` dan `GL_FLAT` pada fungsi `glShadeModel`?"
*   **Jawaban**: `GL_SMOOTH` mengaktifkan Gouraud Shading, yaitu melakukan interpolasi warna dan pencahayaan secara halus di sepanjang permukaan poligon berdasarkan normal di setiap verteks. Sedangkan `GL_FLAT` memberikan satu warna seragam datar pada seluruh permukaan poligon berdasarkan normal dari verteks terakhir, sehingga objek terlihat kaku dan kotak-kotak (*low-poly*).

### Q6: "Bagaimana logika matematika di balik pergerakan maju-mundur kamera FPS?"
*   **Jawaban**: Sudut hadap horizontal kamera disimpan di variabel `camYaw` dalam derajat. Kita ubah ke radian: `radYaw = camYaw * PI / 180.0f`. 
    *   Untuk melangkah maju: `nextX = camPosX + cos(radYaw) * moveSpeed` dan `nextY = camPosY + sin(radYaw) * moveSpeed`.
    *   Penggunaan $\cos$ dan $\sin$ memproyeksikan arah hadap kamera menjadi vektor pergeseran koordinat X dan Y di bidang labirin.

### Q7: "Apa fungsi dari `glPushMatrix()` dan `glPopMatrix()` di fungsi `drawNRP3D`?"
*   **Jawaban**: `glPushMatrix()` menduplikasi dan menyimpan matriks transformasi aktif saat ini ke dalam tumpukan (*stack*). Kita kemudian bebas melakukan rotasi lokal (`glRotated`) pada objek NRP tanpa memengaruhi objek lainnya. Setelah selesai menggambar NRP, `glPopMatrix()` mengambil kembali matriks yang disimpan sebelumnya, mengembalikan sistem koordinat ke keadaan semula sebelum NRP diputar.

### Q8: "Mengapa kita harus mendefinisikan normal vektor (`glNormal3f`) untuk setiap sisi kubus?"
*   **Jawaban**: Vektor normal memberi tahu OpenGL arah hadap permukaan secara spasial. OpenGL membutuhkan informasi ini untuk menghitung seberapa besar intensitas cahaya lampu (diffuse & specular) yang memantul dari permukaan tersebut ke mata kamera berdasarkan hukum pemantulan cahaya Phong/Lambert. Tanpa normal vektor, pencahayaan tidak akan berfungsi dan objek tampak gelap gulita atau datar tanpa dimensi.

---

## 3. Pustaka (Libraries) & API yang Digunakan

Aplikasi ini menggunakan beberapa pustaka C++ standar dan pustaka grafis OpenGL/GLUT:

### A. Pustaka Standar C++
*   **`<windows.h>`**: Diperlukan di sistem operasi Windows sebelum menyertakan pustaka OpenGL (`GL/glut.h`). Pustaka ini menyediakan interaksi tingkat rendah dengan OS Windows (seperti alokasi memori dan windowing context).
*   **`<cmath>`**: Menyediakan fungsi-fungsi matematika seperti `cos()`, `sin()`, dan perhitungan konstanta $\pi$ (Pi). Pustaka ini digunakan untuk menghitung arah pandang kamera FPS berdasarkan sudut horizontal (yaw) dalam radian: $\text{lookX} = \text{camPosX} + \cos(\text{radYaw})$.
*   **`<cstdio>`**: Digunakan untuk operasi input/output standar di konsol, seperti fungsi `printf()` untuk mencetak pesan `"Dapat NRP!"` saat terjadi tabrakan antara pemain dan objek tujuan.

### B. OpenGL & GLUT Utility Toolkit (`<GL/glut.h>`)
*   **OpenGL Utility Toolkit (GLUT)**: Pustaka pembantu (*windowing library*) yang mengabstraksi pembuatan window, penanganan input keyboard/mouse, serta pengaturan timer dan event loop utama (`glutMainLoop`). Ini membuat kode portabel antar sistem operasi (Windows/macOS/Linux).

---

## 4. Kamus Istilah API OpenGL & GLUT (Fungsi Bawaan)

Asisten dosen sering menanyakan arti dari fungsi-fungsi OpenGL/GLUT yang diawali dengan awalan `gl`, `glu`, atau `glut`. Berikut adalah fungsi-fungsi yang digunakan dalam kode ini beserta kegunaannya:

### A. GLUT Callbacks & Windowing (`glut*`)
*   **`glutInit`**: Menginisialisasi pustaka GLUT dan memproses argumen baris perintah.
*   **`glutInitDisplayMode`**: Mengatur mode tampilan untuk window. Kode kita menggunakan:
    *   `GLUT_DOUBLE`: Menggunakan dua buffer (*double buffering*) untuk mencegah efek berkedip (*flickering*) pada animasi. Gambar baru di-render di buffer belakang, lalu ditukar dengan buffer depan menggunakan `glutSwapBuffers()`.
    *   `GLUT_RGB`: Menggunakan format pewarnaan Red, Green, Blue.
    *   `GLUT_DEPTH`: Mengaktifkan *Z-buffer* untuk pengujian kedalaman objek 3D.
    *   `GLUT_ALPHA`: Menyediakan saluran alpha untuk transparansi warna (blending).
*   **`glutInitWindowSize`**: Mengatur ukuran awal jendela aplikasi (700 x 700 piksel).
*   **`glutCreateWindow`**: Membuat jendela visual dengan judul tertentu.
*   **`glutMainLoop`**: Memulai loop utama GLUT yang menangani penggambaran ulang, interaksi keyboard, dan mouse secara terus-menerus.
*   **`glutPostRedisplay`**: Memberitahu GLUT bahwa scene perlu digambar ulang pada frame berikutnya (memicu pemanggilan fungsi `display`).
*   **`glutDisplayFunc`**: Mendaftarkan fungsi callback untuk menggambar scene grafis (mendaftarkan fungsi `display`).
*   **`glutKeyboardFunc`**: Mendaftarkan callback untuk tombol keyboard standar (seperti `W, A, S, D, V, C, 1-5`).
*   **`glutSpecialFunc`**: Mendaftarkan callback untuk tombol keyboard khusus (seperti tombol panah atas/bawah/kiri/kanan).
*   **`glutMouseFunc`**: Mendaftarkan callback untuk menangani klik mouse (kiri, kanan, tengah).
*   **`glutReshapeFunc`**: Mendaftarkan callback saat ukuran jendela diubah oleh pengguna.
*   **`glutIdleFunc`**: Mendaftarkan callback yang dijalankan terus-menerus saat program dalam kondisi santai/tidak menerima input (digunakan untuk memperbarui animasi rotasi NRP).

### B. OpenGL Utility (`glu*`)
*   **`gluPerspective(fov, aspect, zNear, zFar)`**: Mengatur proyeksi perspektif 3D.
    *   `fov` (Field of View): Sudut vertikal bidang pandang kamera (65.0 derajat).
    *   `aspect`: Rasio lebar banding tinggi jendela agar objek tidak memanjang/memipih saat window di-resize.
    *   `zNear` dan `zFar`: Jarak bidang potong terdekat (0.1 unit) dan terjauh (100.0 unit) dari kamera.
*   **`gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz)`**: Menentukan posisi dan arah hadap kamera FPS.
    *   `eye`: Posisi mata kamera berada.
    *   `center`: Titik koordinat yang sedang dipandang oleh kamera.
    *   `up`: Vektor arah atas kamera (searah sumbu Z, yaitu `0.0, 0.0, 1.0`).

### C. OpenGL Core (`gl*`)
*   **`glClear`**: Membersihkan layar dan buffer kedalaman (`GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT`) agar frame sebelumnya tidak menumpuk.
*   **`glLoadIdentity`**: Mereset matriks transformasi aktif saat ini menjadi matriks identitas (kembali ke posisi awal/default).
*   **`glMatrixMode`**: Memilih matriks mana yang akan dimodifikasi:
    *   `GL_PROJECTION`: Digunakan untuk mengatur kamera/proyeksi (seperti lensa kamera).
    *   `GL_MODELVIEW`: Digunakan untuk mengatur perpindahan, rotasi, dan skala objek (seperti tata letak benda di panggung).
*   **`glOrtho`**: Mengatur proyeksi ortogonal (proyeksi sejajar 2D/3D tanpa perspektif).
*   **`glViewport`**: Mengatur area layar di dalam jendela yang akan digunakan untuk menggambar.
*   **`glBegin` / `glEnd`**: Menandai awal dan akhir penggambaran primitif grafis (seperti `GL_POLYGON` untuk membuat bidang datar).
*   **`glVertex3f`**: Menentukan koordinat verteks 3D `(x, y, z)`.
*   **`glNormal3f`**: Menentukan arah normal permukaan 3D `(x, y, z)`. Arah normal harus keluar dari permukaan agar efek pencahayaan (lighting) dapat dihitung dengan benar oleh OpenGL.
*   **`glPushMatrix` / `glPopMatrix`**: Menyimpan dan mengambil kembali konfigurasi matriks transformasi. Sangat penting untuk mengisolasi transformasi lokal suatu objek (misal: memutar NRP saja tanpa memutar labirin).
*   **`glRotated` / `glTranslated`**: Melakukan operasi perkalian matriks rotasi (putar) dan translasi (geser) pada objek grafis.
*   **`glEnable` / `glDisable`**: Mengaktifkan atau menonaktifkan fitur bawaan OpenGL (seperti pencahayaan `GL_LIGHTING`, pengujian kedalaman `GL_DEPTH_TEST`, atau pencampuran warna `GL_BLEND`).
*   **`glMaterialfv`**: Mengatur parameter material permukaan objek (seperti kilap, warna redup, warna diffuse).
*   **`glLightfv`**: Mengatur parameter sumber cahaya (posisi, warna diffuse, warna specular).
*   **`glLightModelfv`**: Menentukan model pencahayaan global (seperti ambient global).
*   **`glShadeModel`**: Menentukan teknik bayangan warna (shading). Kita menggunakan `GL_SMOOTH` (Gouraud shading) yang menghitung interpolasi warna antar verteks agar tampak mulus, berbeda dari `GL_FLAT` yang memberi warna datar seragam per poligon.
*   **`glBlendFunc`**: Mengatur fungsi pencampuran warna untuk efek transparansi.
*   **`glDepthMask`**: Mengaktifkan/menonaktifkan penulisan ke Z-buffer. Dinonaktifkan saat merender objek transparan agar objek di belakangnya tetap digambar.

---

## 5. Alur Eksekusi Program (Flow of Execution)

Berikut adalah urutan jalannya program dari awal dijalankan hingga interaksi pengguna terjadi:

```mermaid
graph TD
    A[Program Mulai: main] --> B[Inisialisasi GLUT & Jendela]
    B --> C[Memanggil myinit]
    C --> D[Generate Labirin & Tempatkan NRP Pertama Kali]
    C --> E[Konfigurasi Awal Lampu & Nyalakan Pencahayaan]
    C --> F[Set Warna Background & Smooth Shading]
    F --> G[Daftarkan Semua Callback: Display, Keyboard, Mouse, Reshape, Idle]
    G --> H[Masuk Loop Utama: glutMainLoop]
    H --> I{Ada Event dari Sistem?}
    I -- Tidak ada input --> J[Fungsi Idle: spinDisplay memperbarui sudut NRP & trigger redraw]
    I -- Window Resize --> K[Fungsi reshape menyesuaikan aspek rasio kamera]
    I -- Tombol Keyboard biasa/spesial --> L[Fungsi keyboard/specialKey memproses gerakan & status lampu]
    I -- Klik Mouse --> M[Fungsi mouse mengganti sumbu rotasi objek NRP]
    J --> N[Memicu display untuk menggambar frame baru]
    K --> N
    L --> N
    M --> N
    N --> O[display: Bersihkan buffer -> Transformasi Kamera -> Gambar Lantai, Player, NRP -> Gambar Dinding -> Swap Buffers]
    O --> I
```

---

## 6. Analisis Struktur Fungsi & Sample Code

Berikut adalah daftar fungsi utama di dalam kode `main.cpp` beserta cuplikan kodenya untuk dipelajari:

### A. Pembentukan Labirin (`generateMaze`)
Algoritma ini menggunakan pembentukan labirin acak berbasis grid biner terhubung.

```cpp
void generateMaze() {
    // 1. Set seluruh grid berukuran 17x17 sebagai dinding solid (true)
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            mazeGrid[y][x] = true;
        }
    }
    // 2. Iterasi sel jalan ganjil (y dan x melompati 2 sel)
    for (int y = 1; y < 16; y += 2) {
        for (int x = 1; x < 16; x += 2) {
            mazeGrid[y][x] = false; // Buka sel jalan aktif
            bool canMoveRight = (x + 2 < 16);
            bool canMoveDown = (y + 2 < 16);
            
            // 3. Rubuhkan partisi pembatas ke kanan atau ke bawah secara acak (rand % 2)
            if (canMoveRight && canMoveDown) {
                if (rand() % 2 == 0) {
                    mazeGrid[y][x + 1] = false; // Buka dinding kanan
                } else  {
                    mazeGrid[y + 1][x] = false; // Buka dinding bawah
                }
            } else if (canMoveRight) {
                mazeGrid[y][x + 1] = false;
            } else if (canMoveDown) {
                mazeGrid[y + 1][x] = false;
            }
        }
    }
    // 4. Buka gerbang masuk dan keluar
    mazeGrid[0][7] = false; 
    mazeGrid[16][7] = false;
}
```

*   **Cara Kerja**: Dinding pembatas antar sel dikosongkan secara acak. Karena setiap sel jalan ganjil dijamin terhubung ke tetangganya (baik kanan atau bawah), algoritma ini menghasilkan labirin terhubung sempurna (*perfect maze*) tanpa ada bagian jalan yang terisolasi.

### B. Penempatan NRP Acak (`letakkanNRP`)
Mencegah objek hijau "103" muncul di atas dinding labirin.

```cpp
void letakkanNRP() {
    int rx, ry;
    do {
        rx = 1 + rand() % 15; // acak index X dari 1 s.d 15
        ry = 1 + rand() % 15; // acak index Y dari 1 s.d 15
    } while (mazeGrid[ry][rx] == true); // ulangi jika koordinat tersebut adalah dinding

    nrpPosX = (float)rx + 0.5; // atur posisi di tengah sel grid
    nrpPosY = (float)ry + 0.4;
}
```

*   **Cara Kerja**: Loop `do-while` akan terus mengacak koordinat baris (`ry`) dan kolom (`rx`) sampai menemukan sel di mana `mazeGrid[ry][rx] == false` (jalan kosong).

### C. Menggambar Objek Dinding Transparan & Blending (`display`)
Bagian ini mengatur bagaimana dinding digambar tembus pandang jika diaktifkan.

```cpp
    // Di dalam fungsi display()...
    
    // 1. Gambar objek solid terlebih dahulu dengan Depth Mask aktif dan Blending mati
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    drawPlayer((float)playerX, (float)playerY);
    drawNRP3D();
    drawLantai();

    // 2. Gambar objek transparan (Dinding) dengan blending diaktifkan
    if (wallTransparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE); // Matikan penulisan ke Depth Buffer
    }
    
    // Gambar seluruh dinding di grid
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            if (mazeGrid[y][x] == true) {
                drawDinding((float)x, (float)y);
            }
        }
    }
    
    // Kembalikan status state ke normal
    if (wallTransparent) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
```

*   **Penting**: Objek transparan harus digambar **setelah** objek pekat (solid). Penonaktifkan penulisan ke Depth Buffer dengan `glDepthMask(GL_FALSE)` mencegah OpenGL memblokir rendering pixel di belakang dinding transparan tersebut.

---

## 7. Studi Kasus: "Jika Diubah Jadi Apa?" (Panduan Modifikasi)

Asisten praktikum sering menanyakan pemahaman kode dengan skenario perubahan. Berikut adalah jawaban cepat yang dapat Anda berikan:

### A. "Bagaimana jika ukuran labirin diubah menjadi 25x25?"
*   **Yang harus diubah**:
    1.  Ubah deklarasi grid: `bool mazeGrid[25][25]`.
    2.  Ubah batas loop di `generateMaze` (misalnya batas `y < 24` dan `x < 24`), `display`, dan penanganan batas keyboard.
    3.  Ubah titik spawn pintu masuk/keluar.
    4.  Ubah parameter `glOrtho` di fungsi `reshape` dan `myinit` (misalnya batas kanan/bawah diubah dari `21.0` ke `29.0`) agar seluruh labirin berukuran lebih besar muat di dalam layar.

### B. "Bagaimana jika ingin mempercepat gerakan kamera FPS?"
*   **Yang harus diubah**: Ubah nilai variabel `moveSpeed` di dalam fungsi `specialKey` (baris 1010) dari `0.3f` menjadi lebih besar (misal: `0.8f`).
    ```cpp
    float moveSpeed = 0.8f; // Kamera bergerak lebih cepat
    ```

### C. "Bagaimana jika ingin memperhalus gerakan rotasi objek NRP?"
*   **Yang harus diubah**: Ubah increment sudut rotasi di dalam fungsi `spinDisplay` (baris 786) dari `0.8` menjadi lebih kecil (misal: `0.2`) namun program dijalankan lebih sering, atau diubah menjadi lebih besar jika ingin putaran lebih cepat.
    ```cpp
    nrpSpinAngle = nrpSpinAngle + 2.0; // Putaran menjadi lebih cepat
    ```

### D. "Bagaimana jika warna dinding ingin diubah menjadi kuning?"
*   **Yang harus diubah**: Ubah komponen warna `diffuse` dan `ambient` material di dalam fungsi `drawDinding` (baris 48-49) menjadi format RGB kuning (merah + hijau):
    ```cpp
    GLfloat mat_ambient[]  = { 0.3f, 0.3f, 0.0f, alpha }; // Ambient kuning redup
    GLfloat mat_diffuse[]  = { 0.8f, 0.8f, 0.0f, alpha }; // Diffuse kuning terang
    ```

### E. "Bagaimana jika ingin mengubah arah cahaya lampu diffuse?"
*   **Yang harus diubah**: Ubah koordinat posisi lampu `light0_pos` di dalam fungsi `myinit` (baris 1074).
    ```cpp
    GLfloat light0_pos[] = { -5.0f, 8.5f, 10.0f, 1.0f }; // Lampu digeser lebih ke kiri (nilai X negatif)
    ```
    *   Jika parameter keempat bernilai `1.0f`, lampu bersifat posisional (seperti lampu bohlam). Jika diubah menjadi `0.0f`, lampu akan menjadi directional (seperti sinar matahari dari arah vektor tersebut).

### F. "Bagaimana jika ingin membuat dinding benar-benar tidak terlihat saat transparan?"
*   **Yang harus diubah**: Ubah nilai alpha pada variabel `alpha` di dalam fungsi `drawDinding` (baris 46) dari `0.3f` menjadi `0.0f`.
    ```cpp
    float alpha = wallTransparent ? 0.0f : 1.0f; // Dinding transparan menjadi sepenuhnya tak terlihat
    ```

### G. "Apa yang terjadi jika `glShadeModel(GL_SMOOTH)` diganti menjadi `GL_FLAT`?"
*   **Jawaban**: Permukaan objek 3D tidak akan memiliki gradasi pencahayaan yang halus lagi. Setiap poligon pembentuk kubus/NRP akan memiliki satu warna solid datar yang seragam berdasar kalkulasi normal verteks terakhirnya, sehingga efek visualnya akan tampak lebih kotak-kotak (*low-poly look*).

### H. "Apa kegunaan dari `glEnable(GL_DEPTH_TEST)` di `myinit`?"
*   **Jawaban**: Untuk menyalakan pengujian kedalaman (*Z-buffer*). Jika dinonaktifkan, OpenGL tidak akan memedulikan jarak objek dari kamera, sehingga objek yang digambar terakhir kali akan selalu muncul di depan layar meskipun secara spasial posisinya ada di belakang objek lain.

---

## 8. Cheat Sheet Kontrol Cepat Asistensi

| Kategori | Tombol | Aksi / Efek Visual |
| :--- | :--- | :--- |
| **Pemain** | `W, A, S, D` | Gerak Maju, Kiri, Mundur, Kanan (kubus biru) |
| **Labirin** | `C` | Mengacak ulang labirin secara prosedural & reset posisi NRP |
| **Kamera** | `V` | Berganti mode visualisasi (2D $\rightarrow$ 3D Isometrik $\rightarrow$ 3D FPS) |
| **Mode Isometrik** | `I, K` / `J, L` | Memutar labirin ke atas-bawah / kiri-kanan |
| **Mouse (Mode 1)** | Klik Kiri / Kanan / Tengah | Mengubah sumbu putar objek hijau NRP (Sumbu X / Y / Z) |
| **Mode FPS** | Panah Atas / Bawah | Melangkah maju / mundur searah pandangan kamera |
| **Mode FPS** | Panah Kiri / Kanan | Menengokkan kamera ke kiri / kanan |
| **Pencahayaan** | `1` / `2` / `3` | Toggle lampu Ambient / Diffuse / Specular |
| **Siklus Siang** | `4` | Toggle Siang / Malam (mengubah rona cahaya & background) |
| **Dinding** | `5` | Toggle Transparansi Dinding (aktifkan efek tembus pandang) |
