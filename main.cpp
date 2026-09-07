#include "raylib.h"
#include <vector>
#include <string>

enum Yon { DUR, YUKARI, ASAGI, SOL, SAG };

int main()
{
    const int KareBoyutu = 40;
    
    // Yılanın vücut parçalarının pozisyonları
    std::vector<Vector2> yilan = { { 400, 200 } };
    Yon mevcutYon = DUR;

    int score = 0;
    Vector2 merkez = { 120, 120 };

    InitWindow(1080, 720, "Snake Game");
    SetTargetFPS(60);

    // Yılanın adımlarla hareket etmesi için zamanlayıcı
    float hareketZamani = 0.0f;
    const float hareketAraligi = 0.15f; // Hız (saniye cinsinden)

    while (!WindowShouldClose())
    {
        // 1. YÖN KONTROLÜ
        if (IsKeyPressed(KEY_W) && mevcutYon != ASAGI) mevcutYon = YUKARI;
        if (IsKeyPressed(KEY_S) && mevcutYon != YUKARI) mevcutYon = ASAGI;
        if (IsKeyPressed(KEY_A) && mevcutYon != SAG)   mevcutYon = SOL;
        if (IsKeyPressed(KEY_D) && mevcutYon != SOL)   mevcutYon = SAG;

        // 2. ADIM BAZLI HAREKET MANTIĞI
        hareketZamani += GetFrameTime();
        if (hareketZamani >= hareketAraligi && mevcutYon != DUR)
        {
            hareketZamani = 0.0f;

            // Başın yeni pozisyonunu hesapla
            Vector2 yeniBas = yilan[0];
            if (mevcutYon == YUKARI) yeniBas.y -= KareBoyutu;
            if (mevcutYon == ASAGI)  yeniBas.y += KareBoyutu;
            if (mevcutYon == SOL)    yeniBas.x -= KareBoyutu;
            if (mevcutYon == SAG)    yeniBas.x += KareBoyutu;

            // Gövdeyi takip ettir (Sondan başa doğru kaydır)
            for (size_t i = yilan.size() - 1; i > 0; i--) {
                yilan[i] = yilan[i - 1];
            }
            yilan[0] = yeniBas; // Başı güncelle
        }

        // 3. TEMAS KONTROLÜ (Yem yeme)
        Rectangle basKutu = { yilan[0].x, yilan[0].y, (float)KareBoyutu, (float)KareBoyutu };
        if (CheckCollisionCircleRec(merkez, 20, basKutu)) {
            // Yemin yerini ızgaraya uygun rastgele bir konuma taşı
            merkez.x = GetRandomValue(1, (1080 / KareBoyutu) - 2) * KareBoyutu + 20;
            merkez.y = GetRandomValue(1, (720 / KareBoyutu) - 2) * KareBoyutu + 20;
            
            score += 1;
            // Yılanın arkasına yeni bir segment/kare ekle
            yilan.push_back(yilan.back());
        }

        // 4. ÇİZİM
        BeginDrawing();
        ClearBackground(BLACK);

        // Yılanı çiz (Tüm parçaları kare olarak)
        for (size_t i = 0; i < yilan.size(); i++) {
            Color renk = (i == 0) ? GREEN : DARKGREEN; // Baş açık yeşil, gövde koyu yeşil
            DrawRectangle(yilan[i].x, yilan[i].y, KareBoyutu - 2, KareBoyutu - 2, renk);
        }

        // Daireyi (Yem) çiz
        DrawCircle(merkez.x, merkez.y, 15, RED);

        // UI
        DrawText("WASD ile Yön Ver", 10, 10, 20, WHITE);
        std::string metin = "Score: " + std::to_string(score);
        DrawText(metin.c_str(), 10, 40, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
