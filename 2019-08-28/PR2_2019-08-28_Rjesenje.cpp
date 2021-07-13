#include<iostream>
#include<regex>
#include<exception>
#include<thread>
#include<mutex>
#include<vector>
#include<string>
#include<iterator>
using namespace std;
mutex cuvar;

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* ispisRazred[] = { "NEPOZNATO", "PRVI", "DRUGI", "TRECI", "CETVRTI" };
enum SortirajPo { T1, T2 };

/*
email adresa mora biti u formatu: text@outlook.com ili text@edu.fit.ba
u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@edu.fit.ba
za provjeru koristiti regex
*/

bool ValidirajEmail(string emailAdresa) {
	return regex_search(emailAdresa, regex("[A-Za-z]+@(outlook.com|edu.fit.ba)"));
}

char* Alociraj(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Kolekcija(bool omoguciDupliranje = true) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_omoguciDupliranje = omoguciDupliranje;
	}
	Kolekcija(const Kolekcija& original) {
		_trenutno = original._trenutno;
		_omoguciDupliranje = original._omoguciDupliranje;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = original._elementi2[i];
		}
	}
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	bool DaLiImaDuplih(T1 e1, T2 e2) {
		for (size_t i = 0; i < _trenutno; i++)
			if (_elementi1[i] == e1 && _elementi2[i] == e2)
				return true;
		return false;
	}

	bool AddElement(T1 e1, T2 e2) {
		if (!_omoguciDupliranje && DaLiImaDuplih(e1, e2))
		{
			throw exception("Nije dozvoljen unos duplikata.");
		}

		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutno] = e1;
		temp2[_trenutno] = e2;
		_trenutno++;
		delete[]_elementi1;
		delete[]_elementi2;
		_elementi1 = temp1; temp1 = nullptr;
		_elementi2 = temp2; temp2 = nullptr;

		return true;
	}

	void SortirajRastuci(SortirajPo sort) {
		bool flag = true;
		while (flag)
		{
			flag = false;
			for (size_t i = 1; i < _trenutno; i++)
			{
				if (sort == SortirajPo::T1)
				{
					if (_elementi1[i - 1] > _elementi1[i]) {
						T1 temp1 = _elementi1[i - 1];
						T2 temp2 = _elementi2[i - 1];
						_elementi1[i - 1] = _elementi1[i];
						_elementi2[i - 1] = _elementi2[i];
						_elementi1[i] = temp1;
						_elementi2[i] = temp2;
						flag = true;
					}
				}
				else
				{
					if (_elementi2[i - 1] > _elementi2[i]) {
						T1 temp1 = _elementi1[i - 1];
						T2 temp2 = _elementi2[i - 1];
						_elementi1[i - 1] = _elementi1[i];
						_elementi2[i - 1] = _elementi2[i];
						_elementi1[i] = temp1;
						_elementi2[i] = temp2;
						flag = true;
					}
				}
			}
		}
	}

	Kolekcija& operator=(const Kolekcija& desni) {
		if (this != &desni)
		{
			_trenutno = desni._trenutno;
			_omoguciDupliranje = desni._omoguciDupliranje;
			delete[]_elementi1;
			delete[]_elementi2;
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = desni._elementi1[i];
				_elementi2[i] = desni._elementi2[i];
			}
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minuti = new int(*obj._minuti);
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}

	int GetDane() {
		return *_dan + *_mjesec * 30 + *_godina * 365 + *_sati / 24 + *_minuti / 1440;
	}

	int GetMinute() {
		return GetDane() * 1440;
	}

	DatumVrijeme& operator=(const DatumVrijeme& desni) {
		if (this != &desni)
		{
			*_dan = *desni._dan;
			*_mjesec = *desni._mjesec;
			*_godina = *desni._godina;
			*_sati = *desni._sati;
			*_minuti = *desni._minuti;
		}
		return *this;
	}

	bool operator==(const DatumVrijeme& desni) {
		if (*_dan == *desni._dan &&
			*_mjesec == *desni._mjesec &&
			*_godina == *desni._godina &&
			*_sati == *desni._sati &&
			*_minuti == *desni._minuti)
			return true;
		return false;
	}

	friend ostream& operator<< (ostream& COUT, const DatumVrijeme& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}

};

class Predmet {
	char* _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char* naziv = "", int ocjena = 0, string napomena = "") {
		_naziv = Alociraj(naziv);
		_ocjena = ocjena;
		_napomena = napomena;
	}
	Predmet(const Predmet& original) {
		_naziv = Alociraj(original._naziv);
		_ocjena = original._ocjena;
		_napomena = original._napomena;
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}

	string GetNapomena() { return _napomena; }
	char* GetNaziv() { return _naziv; }
	int GetOcjena() { return _ocjena; }

	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}

	Predmet& operator=(const Predmet& desni) {
		if (this != &desni)
		{
			delete[] _naziv;
			_naziv = Alociraj(desni._naziv);
			_ocjena = desni._ocjena;
			_napomena = desni._napomena;
		}
		return *this;
	}

	bool operator==(const Predmet& desni) {
		return strcmp(_naziv, desni._naziv) == 0;
	}

	friend ostream& operator<< (ostream& COUT, Predmet& obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
};

class Uspjeh {
	eRazred* _razred;
	//formalni argument DatumVrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh(eRazred razred) {
		_razred = new eRazred(razred);
	}
	Uspjeh(const Uspjeh& original) {
		_razred = new eRazred(*original._razred);
		_predmeti = original._predmeti;
	}
	~Uspjeh() { delete _razred; _razred = nullptr; }

	Kolekcija<Predmet, DatumVrijeme>* GetPredmeti() { return &_predmeti; }
	eRazred* GetERazred() { return _razred; }

	float GetProsjekRazreda() {
		float prosjek = 0;
		for (size_t i = 0; i < _predmeti.getTrenutno(); i++)
		{
			prosjek += _predmeti.getElement1(i).GetOcjena();
		}
		return prosjek / _predmeti.getTrenutno();
	}

	bool DaLiImaIstihPredmeta(eRazred razred, Predmet& predmet) {
		for (size_t i = 0; i < _predmeti.getTrenutno(); i++)
		{
			if (strcmp(_predmeti.getElement1(i).GetNaziv(), predmet.GetNaziv()) == 0)
				return true;
		}
		return false;
	}

	Uspjeh& operator=(const Uspjeh& desni) {
		if (this != &desni)
		{
			*_razred = *desni._razred;
			_predmeti = desni._predmeti;
		}
		return *this;
	}

	friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
		COUT << *obj._razred << " " << obj._predmeti << endl;
		return COUT;
	}
};

class Kandidat {
	char* _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;
public:
	Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = Alociraj(imePrezime);
		_emailAdresa = ValidirajEmail(emailAdresa) ? emailAdresa : "notSet@edu.fit.ba";
		_brojTelefona = brojTelefona;
	}
	Kandidat(const Kandidat& original) {
		_imePrezime = Alociraj(original._imePrezime);
		_emailAdresa = original._emailAdresa;
		_brojTelefona = original._brojTelefona;
	}
	~Kandidat() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}

	vector<Uspjeh>* GetUspjeh() { return &_uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char* GetImePrezime() { return _imePrezime; }
	/*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
FROM:info@edu.fit.ba
TO: emailKorisnika
Postovani ime i prezime, evidentirali ste uspjeh za X razred.
Pozdrav.
FIT Team.

ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh X.X u X razredu".
slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
*/
	void SlanjeEmailPoruke(eRazred razred) {
		cuvar.lock();
		cout << "FROM:info@edu.fit.ba" << endl;
		cout << "TO: " << GetEmail() << endl;
		cout << "Postovani " << GetImePrezime() << ", evidentirali ste uspjeh za " << razred << " razred." << endl;
		cout << "Pozdrav." << endl;
		cout << "FIT Team." << endl;
		cuvar.unlock();
	}

	void SlanjeSMSPoruke(eRazred razred, float prosjekRazreda) {
		cuvar.lock();
		cout << "Svaka cast za uspjeh " << prosjekRazreda << " u " << ispisRazred[razred] << " razredu" << endl;
		cout << "" << endl;
		cuvar.unlock();
	}

	/*
uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
tom prilikom onemoguciti:
- dodavanje istoimenih predmeta na nivou jednog razreda,
- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 5 minuta).
razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
*/
	bool AddPredmet(eRazred razred, Predmet& predmet, DatumVrijeme datumVr) {

		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GetERazred() == razred) {

				if (_uspjeh[i].DaLiImaIstihPredmeta(razred, predmet))
				{
					return false;
				}
				for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					if (abs(_uspjeh[i].GetPredmeti()->getElement2(j).GetMinute() - datumVr.GetMinute()) < 5)
						return false;
				}

				_uspjeh[i].GetPredmeti()->AddElement(predmet, datumVr);

				thread emailThread(&Kandidat::SlanjeEmailPoruke, this, razred);
				emailThread.join();

				float prosjekRazreda = _uspjeh[i].GetProsjekRazreda();
				if (prosjekRazreda > 4.5)
				{
					thread smsThread(&Kandidat::SlanjeSMSPoruke, this, razred, prosjekRazreda);
					smsThread.join();
				}

				return true;
			}
		}

		Uspjeh u(razred);
		u.GetPredmeti()->AddElement(predmet, datumVr);
		_uspjeh.push_back(u);

		thread emailThread(&Kandidat::SlanjeEmailPoruke, this, razred);
		emailThread.join();

		return true;
	}

	int BrojPonavljanjaRijeci(string trazenaRijec) {
		int brojPonavljanja = 0;
		string tekst = "";
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++) {
				tekst += _uspjeh[i].GetPredmeti()->getElement1(j).GetNapomena();
			}
		}
		regex reg(trazenaRijec);
		sregex_iterator it_pocetak(tekst.begin(), tekst.end(), reg);
		sregex_iterator it_kraj;
		while (it_pocetak != it_kraj)
		{
			brojPonavljanja++;
			it_pocetak++;
		}
		return brojPonavljanja;
	}

	Kandidat& operator=(const Kandidat& desni) {
		if (this != &desni)
		{
			delete[]_imePrezime;
			_imePrezime = Alociraj(desni._imePrezime);
			_emailAdresa = desni._emailAdresa;
			_brojTelefona = desni._brojTelefona;
			_uspjeh = desni._uspjeh;
		}
		return *this;
	}

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> operator()(DatumVrijeme pocetni, DatumVrijeme krajnji) {
		vector<Predmet> temp;
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{
				if (_uspjeh[i].GetPredmeti()->getElement2(j).GetMinute() > pocetni.GetMinute() &&
					_uspjeh[i].GetPredmeti()->getElement2(j).GetMinute() < krajnji.GetMinute())
					temp.push_back(_uspjeh[i].GetPredmeti()->getElement1(j));
			}
		}
		return temp;
	}

	Uspjeh* operator[](eRazred razred) {
		Uspjeh* temp = new Uspjeh(razred);
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GetERazred() == razred) {
				for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					temp->GetPredmeti()->AddElement(_uspjeh[i].GetPredmeti()->getElement1(j), _uspjeh[i].GetPredmeti()->getElement2(j));
				}
			}
		}
		return temp;
	}

	friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
		COUT << ">> KANDIDAT <<" << endl;
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		COUT << ">> USPJEH KANDIDATA <<" << endl;
		for (vector<Uspjeh>::iterator i = obj.GetUspjeh()->begin(); i != obj.GetUspjeh()->end(); i++)
		{
			COUT << *i._Ptr << endl;
		}
		return COUT;
	}
};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR AE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEEILO GUBLJENJE URA?ENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKO?ER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIENI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUEAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOANE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAEENO.
	7. IMATE POTPUNU SLOBODU DA U MAIN FUNKCIJI, ZA POTREBE TESTIRANJA, DODAJETE VISE POZIVA ILI SALJETE DRUGE VRIJEDNOSTI PARAMETARA
	****************************************************************************/

	DatumVrijeme temp,
		datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);

	const int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(false);
	for (size_t i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i + 1, 20 - i);

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata, metoda AddElement baca izuzetak
		kolekcija1.AddElement(6, 15);
	}
	catch (exception& err) {
		cout << err.what() << endl;
	}
	cout << kolekcija1 << endl;

	//na osnovu vrijednosti parametra sortira clanove kolekcije u rastucem redoslijedu 
	kolekcija1.SortirajRastuci(SortirajPo::T2);
	cout << kolekcija1 << endl;


	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	Kolekcija<int, int> kolekcija3;
	kolekcija3 = kolekcija1;
	cout << kolekcija3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
		Fizika("Fizika", 5),
		Hemija("Hemija", 2),
		Engleski("Engleski", 5);
	Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh");
	cout << Matematika << endl;

	/*
	email adresa mora biti u formatu: text@outlook.com ili text@edu.fit.ba
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@edu.fit.ba
	za provjeru koristiti regex
	*/

	if (ValidirajEmail("jasmin@outlook.com"))
		cout << "Validna email adresa!" << endl;
	if (ValidirajEmail("adel@edu.fit.ba"))
		cout << "Validna email adresa!" << endl;
	if (ValidirajEmail("korisnik@klix.ba"))
		cout << "Validna email adresa!" << endl;

	Kandidat jasmin("Jasmin Azemovic", "jasmin@outlook.com", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
	Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 5 minuta).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
	Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(DRUGI, Fizika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, datum30062019_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, datum19062019_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, datum05072019_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Fiziku jer nije proslo 5 minuta od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(PRVI, Fizika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	/*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
	FROM:info@edu.fit.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirali ste uspjeh za X razred.
	Pozdrav.
	FIT Team.

	ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh X.X u X razredu".
	slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/

	cout << "USPJEH ISPISATI KORISTECI OSTREAM_ITERATOR" << endl;
	cout << jasmin << endl;
	//vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> jasminUspjeh = jasmin(DatumVrijeme(18, 06, 2019, 10, 15), DatumVrijeme(21, 06, 2019, 10, 10));
	for (size_t i = 0; i < jasminUspjeh.size(); i++)
		cout << jasminUspjeh[i] << endl;

	Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
	if (uspjeh_Irazred != nullptr)
		cout << *uspjeh_Irazred << endl;

	cin.get();
	system("pause>0");
}