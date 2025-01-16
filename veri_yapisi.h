class _veriler {
public:
	int deger;
	_veriler* sonraki_ptr;
};

class veri_dizisi {
private:
	_veriler* sonuncu;
	int toplam_veri_sayisi = 0;
public:
	veri_dizisi()
	{
		sonuncu = nullptr;
	}
	~veri_dizisi()
	{
		delete sonuncu;
	}
	int buyukluk()
	{
		return toplam_veri_sayisi;
	}
	void ekle(const int& sayi)
	{
		_veriler* yeni_veri = new _veriler;
		yeni_veri->deger = sayi;

		if (sonuncu == nullptr)
		{
			yeni_veri->sonraki_ptr = nullptr;
			sonuncu = yeni_veri;
			toplam_veri_sayisi++;
			return;
		}
		yeni_veri->sonraki_ptr = sonuncu;
		sonuncu = yeni_veri;
		toplam_veri_sayisi++;
	}
	void sil(const int& sayi)
	{
		// Liste boşsa direkt çık
		if (sonuncu == nullptr)
			return;

		// İlk eleman eşleşiyorsa onu sil
		while (sonuncu != nullptr && sonuncu->deger == sayi)
		{
			_veriler* temp = sonuncu;
			sonuncu = sonuncu->sonraki_ptr;
			delete temp;
			toplam_veri_sayisi--;
		}

		// Diğer elemanları kontrol et
		_veriler* temp = sonuncu;
		while (temp != nullptr && temp->sonraki_ptr != nullptr)
		{
			if (temp->sonraki_ptr->deger == sayi)
			{
				_veriler* silinecek = temp->sonraki_ptr;
				temp->sonraki_ptr = silinecek->sonraki_ptr;
				delete silinecek;
				toplam_veri_sayisi--;
			}
			else
			{
				temp = temp->sonraki_ptr; // Sadece silme işlemi yapılmadığında ilerle
			}
		}
	}
	int operator[] (int ind)
	{
		int count = toplam_veri_sayisi - 1;
		if (sonuncu == nullptr)
			return -999;

		_veriler* temp = sonuncu;
		while (temp != nullptr)
		{
			if (count == ind)
				return temp->deger;

			temp = temp->sonraki_ptr;
			count--;
		}
		return -999;
	}
	void operator=(veri_dizisi& v1)
	{
		for (int i = 0; i < v1.toplam_veri_sayisi; i++)
		{
			ekle(v1[i]);
		}
	}
	int iceriyormu(const int& sayi)
	{
		if (sonuncu == nullptr)
			return 0;

		_veriler* temp = sonuncu;
		while (temp != nullptr)
		{
			if (temp->deger == sayi)
				return 1; //iceriyor.
			temp = temp->sonraki_ptr;
		}
		return 0;

	}
};
