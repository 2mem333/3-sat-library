#include "veriyapisi.h"
#include <iostream>
#include <iomanip>

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
	int* matris_data;
	int tersal(const int& sayi)
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
		matris_data = new int[degisken_sayisi * parantez_sayisi];
		for (int i = 0; i < degisken_sayisi; i++)
		{
			matris[i] = matris_data + i * parantez_sayisi;
		}
		cozum = new int[degisken_sayisi];
		for (int i = 0; i < degisken_sayisi; i++) {
			cozum[i] = 2;
			for (int a = 0; a < parantez_sayisi; a++)
			{
				matris[i][a] = 2; //bos matris yapiruz
			}
		}
	}
	~SAT3_matris()
	{
		delete[] cozum;
		delete[] matris_data;
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
	int cozum_kontrol()
	{
		for (int a = 0; a < parantez_sayisi; a++)
		{
			if (silinen_parantezler.iceriyormu(a))
				continue;
			bool satisfied = false;
			for (int i = 0; i < degisken_sayisi; i++)
			{
				if (silinen_degiskenler.iceriyormu(i))
					continue;
				if (matris[i][a] != 2)
				{
					if (matris[i][a] == 1 && cozum[i] != 0)
					{
						satisfied = true;
						break;
					}
					if (matris[i][a] == 0 && cozum[i] != 1)
					{
						satisfied = true;
						break;
					}
				}
			}
			if (!satisfied)
				return 1;
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
					std::cout << "x" << i;
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
			if (silinen_parantezler.iceriyormu(i))
				continue;
			int agirlik = 0;
			for (int a = 0; a < degisken_sayisi; a++)
			{
				if (silinen_degiskenler.iceriyormu(a))
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
		return mp;
	}
	void karsilik_gelen_parantez_sil(int degisken, int state)
	{
		for (int i = 0; i < parantez_sayisi; i++)
		{
			if (silinen_parantezler.iceriyormu(i))
				continue;
			if (matris[degisken][i] == state)
			{
				silinen_parantezler.ekle(i);
			}
		}
	}
	int parantez_agirligi(int p)
	{
		int agr = 0;
		for (int h = 0; h < degisken_sayisi; h++)
		{
			if (silinen_degiskenler.iceriyormu(h))
				continue;
			if (matris[h][p] != 2)
				agr++;
		}
		return agr;
	}
	void parantezleri_bul()
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
		for (int i = 0; i < parantez_sayisi; i++)
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
		}
	}
	void kont_nodebug()
	{
		int enbuyuk = -99999, degisken = 0, state = 0;
		float agir_0, agir_1;
		int* clause_weight = new int[parantez_sayisi];
		for (int i = 0; i < parantez_sayisi; i++) {
			clause_weight[i] = (silinen_parantezler.iceriyormu(i)) ? -1 : parantez_agirligi(i);
		}
		float* sigma0 = new float[degisken_sayisi];
		float* sigma1 = new float[degisken_sayisi];
		for (int d = 0; d < degisken_sayisi; d++) {
			int count0 = 1, count1 = 1;
			if (!silinen_degiskenler.iceriyormu(d)) {
				for (int i = 0; i < parantez_sayisi; i++) {
					if (silinen_parantezler.iceriyormu(i))
						continue;
					if (matris[d][i] == 0)
						count0++;
					else if (matris[d][i] == 1)
						count1++;
				}
				sigma0[d] = (count0 > count1) ? 0.154f : 0.0f;
				sigma1[d] = (count1 > count0) ? 0.154f : 0.0f;
			}
			else {
				sigma0[d] = sigma1[d] = 0.0f;
			}
		}
		for (int a = 0; a < degisken_sayisi; a++)
		{
			if (silinen_degiskenler.iceriyormu(a))
				continue;
			agir_0 = 0;
			agir_1 = 0;
			bool eklendi = true;
			for (int i = 0; i < parantez_sayisi; i++)
			{
				if (silinen_parantezler.iceriyormu(i))
					continue;
				if (matris[a][i] == 1)
				{
					if (clause_weight[i] == 2)
					{
						if (eklendi)
							agir_1 += parantez_sayisi * 10;
						eklendi = false;
					}
					agir_1++;
					for (int d = 0; d < degisken_sayisi; d++)
					{
						if (d == a)
							continue;
						if (matris[d][i] == 1)
							agir_1 -= sigma1[d];
					}
				}
				if (matris[a][i] == 0)
				{
					if (clause_weight[i] == 2)
					{
						if (eklendi)
							agir_0 += parantez_sayisi * 10;
						eklendi = false;
					}
					agir_0++;
					for (int d = 0; d < degisken_sayisi; d++)
					{
						if (d == a)
							continue;
						if (matris[d][i] == 0)
							agir_0 -= sigma0[d];
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
		delete[] clause_weight;
		delete[] sigma0;
		delete[] sigma1;
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
			if (silinen_parantezler.iceriyormu(i))
				continue;
			std::cout << std::setw(4) << i + 1;
		}
		std::cout << "\n";
		for (int a = 0; a < degisken_sayisi; a++)
		{
			if (silinen_degiskenler.iceriyormu(a))
				continue;
			std::cout << "x" << "" << a << "    " << std::setw(4);
			for (int i = 0; i < parantez_sayisi; i++)
			{
				if (silinen_parantezler.iceriyormu(i))
					continue;
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

	int parantez_kapsanirmi(int p)
	{
		for (int h = 0; h < degisken_sayisi; h++)
		{
			if (silinen_degiskenler.iceriyormu(h))
				continue;
			if (matris[h][p] != 2)
				return 1;
		}
		return 0;
	}
	int cozum_var_mi()
	{
		if (bitti_mi())
			return 1;
		for (int a = 0; a < parantez_sayisi; a++)
		{
			if (silinen_parantezler.iceriyormu(a))
				continue;
			if (!parantez_kapsanirmi(a))
				return 0;
		}
		return 1;
	}

	void kapsanamayan_parantezler()
	{
		std::cout << "Kapsanmasi imkansiz cumlecikler: [  ";
		for (int a = 0; a < parantez_sayisi; a++)
		{
			if (silinen_parantezler.iceriyormu(a))
				continue;
			if (!parantez_kapsanirmi(a))
				std::cout << a << "  ";
		}
		std::cout << "]\n";

		std::cout << "Kapsanamayan cumlecikler [  ";
		for (int a = 0; a < parantez_sayisi; a++)
		{
			if (silinen_parantezler.iceriyormu(a))
				continue;
			std::cout << a << "  ";
		}
		std::cout << "]\n";
	}
	void sonuclar()
	{
		std::cout << "Parantezlerin " << silinen_parantezler.buyukluk() << "/" << parantez_sayisi << " kadari kapsandi.\n";
	}
};
