# 🧊 Quintoctet Crystal – OpenGL 3D Visualization

Proyek ini menampilkan model 3D **Quintoctet Crystal** menggunakan **C++**, **OpenGL (GLAD + GLFW + GLM)**, dan struktur kode modular berbasis `Shader`, `VAO`, `VBO`, serta `Camera` class.

Model disusun dari **oktahedron berlapis (5 ring × 6 oktahedron)** yang membentuk pola kristal simetris dengan pencahayaan, pewarnaan, transformasi interaktif, dan rotasi (manual&otomatis).

Proyek ini berganggotakan 3 orang

Ketua  : Jonathan Alexander(03082230031)

Anggota: Misellin Mindany  (03082230005)

         Felicia Alexandra (03082230015)
         
---

## 🚀 Fitur Utama
- **Pemodelan Geometri 3D:** Oktahedron dengan *flat shading* (posisi dan normal per-face).
- **Susunan Simetris:** 5 ring × 6 oktahedron membentuk struktur kristal “Quintoctet”.
- **Transformasi Lengkap:** Translasi, rotasi, dan skala real-time menggunakan input keyboard.
- **Rotasi Otomatis:** Objek berputar terus menerus di sumbu Y untuk efek showcase.
- **Pencahayaan:** Directional light dari depan-atas dengan ambient adjustable.
- **Warna Per-Ring:** Setiap ring memiliki palet warna berbeda untuk menonjolkan kedalaman.
- **Kamera 3D:** Navigasi `WASD` dan mouse (melalui `Camera` class).

---

## 🎮 Kontrol
| Tombol | Fungsi |
|:-------|:--------|
| `W / A / S / D` | Navigasi kamera |
| `X / Y / Z` | Rotasi global pada sumbu X/Y/Z |
| `Shift + X / Y / Z` | Rotasi ke arah negatif |
| `-` / `+` | Scale down / Scale up |
| `←` `→` `↑` `↓` | Translasi global (pindahkan seluruh objek) |
| `R` | Reset rotasi |
| `Backspace` | Reset translasi dan skala |
| *(otomatis)* | Rotasi berkelanjutan pada sumbu Y |

---

## 🧩 Struktur Proyek
```
├── header/
│   ├── Camera.h
│   ├── shaderClass.h
│   ├── VAO.h
│   └── VBO.h
├── include/
│   ├── glad/
│   ├── GLFW/
│   └── glm/
├── lib/
│   └── libglfw3dll.a
├── shaders/
│   ├── crystal.frag
│   ├── crystal.vert
│   ├── light.frag
│   └── light.vert
├── src/
│   ├── Camera.cpp
│   ├── crystal.cpp
│   ├── glad.c
│   ├── shaderClass.cpp
│   ├── VAO.cpp
│   └── VBO.cpp
├── crystal.exe
└── glfw3.dll
```
---

## ⚙️ Dependensi
Pastikan berikut sudah tersedia:

- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [GLM](https://github.com/g-truc/glm)
- Compiler C++17 ke atas (misal: `g++`, `clang++`, atau MSVC)

---

## 🛠️ Cara Build
### Windows
1. Compile codenya
```bash
g++ src/crystal.cpp src/glad.c src/shaderClass.cpp src/VAO.cpp src/VBO.cpp  src/Camera.cpp -o crystal.exe -I "header" -I include -L lib -lglfw3dll -lopengl32 -lgdi32
```
2. Run aplikasinya
```bash
./crystal
```
## 🖼️ Preview
Tampak depan
![Crystal](https://github.com/user-attachments/assets/e9b3e27d-8190-4c2f-bbe2-e809134d7391)

Tampak samping
<img width="1282" height="759" alt="{65587176-563D-497C-8D86-A8093F238C27}" src="https://github.com/user-attachments/assets/0a1fe15e-a3e5-4255-8531-ffdbbef38be0" />

Tampak atas
<img width="1282" height="759" alt="{61F591E4-BEE3-4E05-9579-F385134C7FB4}" src="https://github.com/user-attachments/assets/46c517a9-83ae-4215-9aab-1e4898ed3c75" />
