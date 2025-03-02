#include "sezgiselkapsama.h"
#include <fstream>
#include <string>
#include <sstream>

#include <ctime>
#include <chrono>

int main(int argc, const char* argv[])
{
    if (argc == 1)
        return 0;

	std::ifstream dosya(argv[1]);
	if (!dosya) {
		printf("file not found!");
		return 1;
	}

	int literalSayisi = 0, cumlecikSayisi = 0;
    std::string satir;
   
    while (getline(dosya, satir))
    {
        if (satir.empty()) continue;
        if (satir[0] == 'c' || satir[0] == '%') continue;
        if (satir[0] == 'p')
        {
            // "p cnf <degiskenSayisi> <clauseSayisi>" formatý okunuyor.
            std::istringstream iss(satir);
            std::string p, cnf;
            iss >> p >> cnf >> literalSayisi >> cumlecikSayisi;
            break;
        }
    }

    SAT3_matris problem(literalSayisi, cumlecikSayisi);
    int okunanCumlecik = 0;
    while (getline(dosya, satir))
    {
        if (satir.empty()) continue;
        if (satir[0] == 'c' || satir[0] == '%') continue;

        std::istringstream iss(satir);
        int a, b, c, son;
        if (!(iss >> a >> b >> c >> son))
            continue;
        if (son != 0)
        {
            std::cerr << "Hatali format" << std::endl;
            continue;
        }

        problem.parantez_ekle(a, b, c);
        okunanCumlecik++;
        if (okunanCumlecik >= cumlecikSayisi)
            break;
    }
    dosya.close();

    auto zamanbaslangic = std::chrono::high_resolution_clock::now();

	mutlakparantez mp;
bas:
	if (problem.cozum_var_mi() == 0)
	{
        auto zamanbitis = std::chrono::high_resolution_clock::now();

        auto toplam_zaman = std::chrono::duration_cast<std::chrono::nanoseconds>(zamanbitis - zamanbaslangic);

		printf("\n3SAT problemi cozulemedi!\n");

        std::cout << "Islem suresi: " << toplam_zaman.count() * 0.000000001 << " saniye\n";

        problem.kapsanamayan_parantezler();
        problem.sonuclar();
		return 1;
	}
	mp = problem.mutlak_parantez_kontrol();
	if (!(mp.mutlakpar == -1)) //MUTLAK PARANTEZ BULUNDU
	{
		int mutlakpara = mp.mutlakpar + 1;
		problem.degisken_sil(mp.degisken, mp.state);
		goto bkontrol;
	}
	problem.kont_nodebug();
bkontrol:
	if (problem.bitti_mi())
	{
		if (!problem.cozum_kontrol())
		{
            auto zamanbitis = std::chrono::high_resolution_clock::now();
            auto toplam_zaman = std::chrono::duration_cast<std::chrono::nanoseconds>(zamanbitis - zamanbaslangic);
            std::cout << "Islem suresi: " << toplam_zaman.count() * 0.000000001 << " saniye\n";

			printf("---------------------\nCOZUM BASARIYLA BITTI\n");
			for (int i = 0; i < literalSayisi; i++)
			{
				if (problem.cozum[i] == 2)
                    std::cout << "x" << i + 1 << " = -" << "\n";
                else
				std::cout << "x" << i + 1 << " = " << problem.cozum[i] << "\n";
			}
			printf("---------------------");
			return 0;
		}
	}
	goto bas;
}
    