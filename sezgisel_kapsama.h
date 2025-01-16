#include <iostream>
#include "efe_veri_yapisi.h"

struct mutlakparantez {
	int degisken;
	int state;
	int mutlakpar;
};

class SAT3_matris {
public:
	veri_dizisi silinen_parantezler;
	veri_dizisi silinen_degiskenler;

	veri_dizisi bulunacak_parantezler;

	veri_dizisi harf_state;

	int** matris;
	int eklenen = 0;
	int parantez_sayisi;
	int degisken_sayisi;
	int* cozum;

private:
	int tersal(const int& sayi) //cozum kontrol yaparken kullaniyoruz
	{
		if (sayi == 1)
			return 0;
		return 1;
	}
public:
	SAT3_matris(int degisken_sayi, int parantez_sayi)
	{
		parantez_sayisi = parantez_sayi;
		degisken_sayisi = degisken_sayi;

		matris = new int* [degisken_sayisi];
		for (int i = 0; i < degisken_sayisi; i++)
		{
			matris[i] = new int[parantez_sayisi];
		}

		cozum = new int[degisken_sayi];

		for (int i = 0; i < degisken_sayi; i++)
			cozum[i] = 2;

		for (int i = 0; i < degisken_sayisi; i++) {
			for (int a = 0; a < parantez_sayisi; a++)
			{
				matris[i][a] = 2; //bos matris yapiruz
			}
		}
	}
	~SAT3_matris()
	{
		delete[] cozum;
		for (int i = 0; i < degisken_sayisi; ++i)
		{
			delete[] matris[i];
		}
		delete[] matris;
	}

	void parantez_ekle(int x, int y, int z, int d1, int d2, int d3)
	{
		matris[x][eklenen] = d1;
		matris[y][eklenen] = d2;
		matris[z][eklenen] = d3;
		eklenen++;
	}
	void parantez_ekle(int x, int y, int z)
	{
		int d1 = 1; int d2 = 1; int d3 = 1;

		if (x < 0)
		{
			x *= -1;
			d1 = 0;
		}
		if (y < 0)
		{
			y *= -1;
			d2 = 0;
		}
		if (z < 0)
		{
			z *= -1;
			d3 = 0;
		}
		matris[x - 1][eklenen] = d1;
		matris[y - 1][eklenen] = d2;
		matris[z - 1][eklenen] = d3;
		eklenen++;
	}
	int cozum_kontrol() //hata varsa 1 yoksa 0,  deneme_cozum'u kontrol eder
	{
		int toplam; //toplam en az 1 olmalı.

		for (int a = 0; a < parantez_sayisi; a++)
		{
			toplam = 0;
			for (int i = 0; i < degisken_sayisi; i++)
			{
				if (matris[i][a] == 1) //1 ise çözümün kendisini degilse tam tersini al
				{
					toplam += cozum[i];
				}
				if (matris[i][a] == 0)
				{
					toplam += tersal(cozum[i]);
				}
			}
			if (toplam == 0) //minterm 0 cikti...
			{
				return 1;
			}
		}
		return 0;
	}
	void problemi_yazdir()
	{
		std::cout << "Problem: ";
		int cnt;
		for (int a = 0; a < parantez_sayisi; a++)
		{
			cnt = 0;
			std::cout << "(";
			for (int i = 0; i < degisken_sayisi; i++)
			{
				if (matris[i][a] != 2)
				{
					std::cout << karsilik_gelen_harf(i);
					//std::cout << "x"<<i;
					if (matris[i][a] == 0)
						std::cout << "'";
					cnt++;
					if (cnt != 3)
						std::cout << "+";
				}

			}
			std::cout << ").";
		}
	}
	void cozumyazdir()
	{
		std::cout << "Cozum: ";
		for (int i = 0; i < degisken_sayisi; i++)
			std::cout << cozum[i];
	}

	mutlakparantez mutlak_parantez_kontrol()
	{
		mutlakparantez mp;
		mp.mutlakpar = -1;
		for (int i = 0; i < parantez_sayisi; i++)
		{
			if (silinen_parantezler.iceriyormu(i)) //eger degisken silinmis ise bosver
				continue;

			int agirlik = 0;
			for (int a = 0; a < degisken_sayisi; a++)
			{
				if (silinen_degiskenler.iceriyormu(a)) //eger parantez silinmis ise bosver
					continue;

				if (matris[a][i] != 2)
				{
					agirlik++;
					mp.degisken = a;
					mp.state = matris[a][i];
				}

			}
			if (agirlik == 1)
			{
				mp.mutlakpar = i;
				return mp;
			}
		}
		return mp; //mutlak parantez yok
	}
	void karsilik_gelen_parantez_sil(int degisken, int state)
	{
		for (int i = 0; i < parantez_sayisi; i++)
		{
			if (silinen_parantezler.iceriyormu(i)) //eger parantez silinmis ise bosver
				continue;

			if (matris[degisken][i] == state)
			{
				silinen_parantezler.ekle(i);
				//std::cout << i+1 << ". parantez silindi\n";
			}
		}
	}

	int parantez_agirligi(int p)
	{
		int agr = 0;
		for (int h = 0; h < degisken_sayisi;h++)
		{
			if (silinen_degiskenler.iceriyormu(h))
				continue;

			if (matris[h][p] != 2)
				agr++;
		}
		return agr;
	}
	void parantezleri_bul() //en az agir parantezlerden buluncak harfleri secer,,, hatalı çalışıyor
	{
		while (bulunacak_parantezler.buyukluk() != 0)
		{
			bulunacak_parantezler.sil(bulunacak_parantezler[0]);
		}

		int en_kucuk_agirlik = 100000;
		for (int p = 0; p < parantez_sayisi; p++)
		{
			if (silinen_parantezler.iceriyormu(p))
				continue;

			int agirlik = parantez_agirligi(p);
			if (agirlik == en_kucuk_agirlik)
			{
				bulunacak_parantezler.ekle(p);
			}
			if (agirlik < en_kucuk_agirlik)
			{
				en_kucuk_agirlik = agirlik;
				while (bulunacak_parantezler.buyukluk() != 0)
				{
					bulunacak_parantezler.sil(bulunacak_parantezler[0]);
				}
				bulunacak_parantezler.ekle(p);
			}
		}
	}


	float sigma_hesapla(int harf, int state)
	{
		float sayi_0 = 1;
		float sayi_1 = 1;

		for (int i = 0; i < parantez_sayisi;i++)
		{
			if (silinen_parantezler.iceriyormu(i))
				continue;

			switch (matris[harf][i])
			{
			case 0:
				sayi_0++;
				break;
			case 1:
				sayi_1++;
				break;
			}
		}

		int etki = parantez_sayisi - silinen_parantezler.buyukluk();
		if (state == 1)
		{
			//return (sayi_1 / sayi_0 ) *0.025; //??????
			if (sayi_1 > sayi_0)
			{
				return 0.154;
			}
			return 0;
		}
		else
		{
			if (sayi_0 > sayi_1)
			{
				return 0.154;
			}
			return 0;
			//return (sayi_0 / sayi_1) * 0.025; //0.4, 0.2
		}
	}


	void kont()
	{
		parantezleri_bul();
		int enbuyuk = -99999;
		int degisken, state;
		std::cout << "\n";
		for (int a = 0; a < degisken_sayisi; a++)
		{
			if (silinen_degiskenler.iceriyormu(a)) //eger degisken silinmis ise bosver
				continue;

			float agir_0 = 0;
			float agir_1 = 0;
			bool eklendi = 1;
			for (int i = 0; i < parantez_sayisi; i++)
			{
				if (silinen_parantezler.iceriyormu(i)) //eger parantez silinmis ise bosver
					continue;
				if (matris[a][i] == 1)
				{
					if (parantez_agirligi(i) == 2) //eger agirligi 2 ise'de yapilabilir
					{
						if (eklendi)
							agir_1 += parantez_sayisi * 10;
						eklendi = false;
					}

					agir_1++;
					for (int d = 0; d < degisken_sayisi; d++)
					{
						if (d == a) //mevcut degisken icin sigma hesaplanmaz
							continue;
						if (matris[d][i] == 1)
						{
							agir_1 -= sigma_hesapla(d, 1);
						}
					}
				}
				if (matris[a][i] == 0)
				{
					if (parantez_agirligi(i) == 2)
					{
						if (eklendi)
							parantez_sayisi * 10;
						eklendi = false;
					}
					agir_0++;
					for (int d = 0; d < degisken_sayisi; d++)
					{
						if (d == a) //mevcut degisken icin sigma hesaplanmaz
							continue;
						if (matris[d][i] == 0)
						{
							agir_0 -= sigma_hesapla(d, 0);
						}
					}
				}
			}
			//std::cout << karsilik_gelen_harf(a) << " icin agirliklar: " << agir_0 << "  " << agir_1 << "\n";
			std::cout << "x" << a << " icin agirliklar: " << agir_0 << "  " << agir_1 << "\n";
			if (agir_0 > agir_1)
			{
				if (agir_0 > enbuyuk)
				{
					enbuyuk = agir_0;
					degisken = a;
					state = 0;
				}
			}
			else
			{
				if (agir_1 > enbuyuk)
				{
					enbuyuk = agir_1;
					degisken = a;
					state = 1;
				}
			}
		}

		std::cout << "x" << degisken << "|" << state << " seciliyor ve siliniyor\n";
		cozum[degisken] = state;
		silinen_degiskenler.ekle(degisken);
		karsilik_gelen_parantez_sil(degisken, state);
	}

	void kont_nodebug()
	{
		//parantezleri_bul();
		int enbuyuk = -99999;
		int degisken, state;
		//std::cout << "\n";
		for (int a = 0; a < degisken_sayisi; a++)
		{
			if (silinen_degiskenler.iceriyormu(a)) //eger degisken silinmis ise bosver
				continue;

			float agir_0 = 0;
			float agir_1 = 0;
			bool eklendi = true;
			for (int i = 0; i < parantez_sayisi; i++)
			{
				if (silinen_parantezler.iceriyormu(i)) //eger parantez silinmis ise bosver
					continue;
				if (matris[a][i] == 1)
				{
					if (parantez_agirligi(i) == 2) //eger agirligi 2 ise'de yapilabilir
					{
						if (eklendi)
							agir_1 += parantez_sayisi * 10;
						eklendi = false;
					}

					agir_1++;
					for (int d = 0; d < degisken_sayisi; d++)
					{
						if (d == a) //mevcut degisken icin sigma hesaplanmaz
							continue;
						if (matris[d][i] == 1)
						{
							agir_1 -= sigma_hesapla(d, 1);
						}
					}
				}
				if (matris[a][i] == 0)
				{
					if (parantez_agirligi(i) == 2)
					{
						if (eklendi)
							agir_0 += parantez_sayisi * 10;
						eklendi = false;
					}

					agir_0++;
					for (int d = 0; d < degisken_sayisi; d++)
					{
						if (d == a) //mevcut degisken icin sigma hesaplanmaz
							continue;
						if (matris[d][i] == 0)
						{
							agir_0 -= sigma_hesapla(d, 0);
						}
					}
				}
			}

			if (agir_0 > agir_1)
			{
				if (agir_0 > enbuyuk)
				{
					enbuyuk = agir_0;
					degisken = a;
					state = 0;
				}
			}
			else
			{
				if (agir_1 > enbuyuk)
				{
					enbuyuk = agir_1;
					degisken = a;
					state = 1;
				}
			}
		}

		cozum[degisken] = state;
		silinen_degiskenler.ekle(degisken);
		karsilik_gelen_parantez_sil(degisken, state);
	}


	void degisken_sil(int degisken, int state)
	{
		silinen_degiskenler.ekle(degisken);
		cozum[degisken] = state;
		karsilik_gelen_parantez_sil(degisken, state);
	}

	void sezgiselmat_yazdir()
	{
		std::cout << "\n      ";
		for (int i = 0; i < parantez_sayisi; i++)
		{
			if (silinen_parantezler.iceriyormu(i)) // Eğer parantez silinmişse, atla
				continue;
			std::cout << std::setw(4) << i + 1; // Sütun genişliğini 4 yap
		}
		std::cout << "\n";

		for (int a = 0; a < degisken_sayisi; a++)
		{
			if (silinen_degiskenler.iceriyormu(a)) // Eğer değişken silinmişse, atla
				continue;

			//std::cout << karsilik_gelen_harf(a)<<"" << a << "    " << std::setw(4); // Değişkeni yazdır ve hizala
			std::cout << "x" << "" << a << "    " << std::setw(4); // Değişkeni yazdır ve hizala
			for (int i = 0; i < parantez_sayisi; i++)
			{
				if (silinen_parantezler.iceriyormu(i)) // Eğer parantez silinmişse, atla
					continue;

				// Matris değerine göre çıktıyı yazdır
				if (matris[a][i] == 1)
					std::cout << std::setw(4) << "1";
				else if (matris[a][i] == 0)
					std::cout << std::setw(4) << "0";
				else if (matris[a][i] == 2)
					std::cout << std::setw(4) << " ";
			}
			std::cout << "\n";
		}

		std::cout << "\n---------------------\n";
	}

	int bitti_mi()
	{
		return silinen_parantezler.buyukluk() == parantez_sayisi ? 1 : 0;
	}

	int cozum_var_mi()
	{
		if (bitti_mi())
			return 1;

		for (int a = 0; a < parantez_sayisi; a++)
		{
			if (silinen_parantezler.iceriyormu(a))
				continue;

			for (int i = 0; i < degisken_sayisi; i++)
			{
				if (silinen_degiskenler.iceriyormu(i))
					continue;

				if (matris[i][a] != 2)
					return 1;
			}
		}
		return 0;
	}
};