//ZADATAK 1
//Izvršiti definiciju funkcija na naèin koji odgovara opisu (komentarima) datom neposredno uz pozive ili nazive funkcija. Možete dati komentar na bilo koju liniju code-a koju smatrate da bi trebalo unaprijediti ili da æe eventualno uzrokovati grešku prilikom kompajliranja. Takoðer, možete dodati dodatne funkcije koje æe vam olakšati implementaciju programa.
#include<string>
#include<exception>
#include<thread>
#include<regex>
#include<mutex>
#include <iostream>
using namespace std;
mutex cuvar;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, "
"OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE "
"POMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const char* IspisPojaseva[] = { "BIJELI", "ZUTI", "NARANDZASTI", "ZELENI", "PLAVI", "SMEDJI", "CRNI" };
enum Dupliranje { BEZ_DUPLIKATA, SA_DUPLIKATIMA };

bool ValidirajEmail(string emailAdresa) {
	return regex_search(emailAdresa, regex("[\\d]_([A-Za-z]{3,})[\\._]?([A-Za-z]{3,})@(karate|edu.karate)\\.(ba|com|org)"));
}

char* GetNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2, int max = 15>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int* _trenutno;
	Dupliranje _dupliranje;
public:
	Kolekcija(Dupliranje dupliranje = SA_DUPLIKATIMA) {
		_trenutno = new int(0);
		_dupliranje = dupliranje;
	}

	Kolekcija(const Kolekcija& original) {
		_trenutno = new int(*original._trenutno);
		_dupliranje = original._dupliranje;
		for (int i = 0; i < *_trenutno; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = original._elementi2[i];
		}
	}

	~Kolekcija() {
		delete _trenutno; _trenutno = nullptr;
	}

	T1 getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2 getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno()const { return *_trenutno; }

	bool DaLiImaDuplih(T1 e1, T2 e2) {

		for (int i = 0; i < *_trenutno; i++)
			if (_elementi1[i] == e1 && _elementi2[i] == e2)
				return true;
		return false;
	}

	bool AddElement(T1 e1, T2 e2) {

		if (*_trenutno >= max)
			throw exception("Nije dozvoljen unos, prekoracen maksimalni broj elemenata.");

		if (_dupliranje == BEZ_DUPLIKATA && DaLiImaDuplih(e1, e2))
			throw exception("Nije dozvoljen unos duplikata.");

		_elementi1[*_trenutno] = e1;
		_elementi2[*_trenutno] = e2;
		(*_trenutno)++;

		return true;
	}

	Kolekcija& operator=(const Kolekcija& desni) {
		if (this != &desni) {
			*_trenutno = *desni._trenutno;
			_dupliranje = desni._dupliranje;
			for (int i = 0; i < *_trenutno; i++)
			{
				_elementi1[i] = desni._elementi1[i];
				_elementi2[i] = desni._elementi2[i];
			}
		}
		return *this;
	}

	Kolekcija& operator()(int indeksPoc, int indeksKraj) {

		//ako opseg nije validan, funkcija bi trebala baciti izuzetak
		if (indeksPoc < 0 || indeksKraj >= *_trenutno)
		{
			throw exception("Opseg nije validan.");
		}

		Kolekcija* temp = new Kolekcija();
		for (int i = indeksPoc; i <= indeksKraj; i++)
		{
			temp->AddElement(_elementi1[i], _elementi2[i]);
		}
		return *temp;
	}
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:

	// konstruktori
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

	// ostale metode
	int GetDane() {
		return *_dan + *_mjesec * 30 + *_godina * 365;
	}
	bool DaLiJeIzmedjuZadatihDatuma(Datum pocetni, Datum krajnji) {
		if (pocetni.GetDane() <= this->GetDane() && this->GetDane() <= krajnji.GetDane())
			return true;
		return false;
	}

	// operatori
	Datum& operator=(const Datum& desni) {
		if (this != &desni)
		{
			*_dan = *desni._dan;
			*_mjesec = *desni._mjesec;
			*_godina = *desni._godina;
		}
		return *this;
	}
	bool operator==(const Datum& desni) {

		if (*_dan == *desni._dan && *_mjesec == *desni._mjesec && *_godina == *desni._godina)
			return true;
		return false;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};

int RazmakDana(Datum d1, Datum d2) {
	return abs(d1.GetDane() - d2.GetDane());
}

class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
	Kolekcija<Datum, int>* _ocjene;
public:
	Tehnika(const char* naziv = "", Datum datum = Datum(), int ocjena = 0) {
		_naziv = GetNizKaraktera(naziv);
		_ocjene = nullptr;
		if (ocjena > 0)
			AddOcjena(&datum, ocjena);
	}
	Tehnika(const Tehnika& original) {
		_naziv = GetNizKaraktera(original._naziv);
		_ocjene = new Kolekcija<Datum, int>(*original._ocjene);
	}
	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjene; _ocjene = nullptr;
	}
	char* GetNaziv()const { return _naziv; }
	Kolekcija<Datum, int> GetOcjene() const { return *_ocjene; }

	float GetProsjekOcjenaTehnike() {

		float prosjek = 0;
		if (_ocjene->getTrenutno() == 0)
		{
			return prosjek;
		}
		for (int i = 0; i < _ocjene->getTrenutno(); i++)
		{
			prosjek += _ocjene->getElement2(i);
		}
		return prosjek / _ocjene->getTrenutno();
	}

	bool AddOcjena(Datum* datum, int ocjena) {
		if (_ocjene != nullptr)
		{
			_ocjene->AddElement(*datum, ocjena);
			return true;
		}
		_ocjene = new Kolekcija<Datum, int>();
		_ocjene->AddElement(*datum, ocjena);
		return true;
	}
	float GetProsjekDana() {
		float prosjek = 0;
		if (_ocjene->getTrenutno() < 2)
		{
			return prosjek;
		}
		for (int i = 1; i < _ocjene->getTrenutno(); i++)
		{
			prosjek += RazmakDana(_ocjene->getElement1(i - 1), _ocjene->getElement1(i));
		}
		return prosjek / (_ocjene->getTrenutno() - 1);
	}

	// operatori
	bool operator==(const Tehnika& desni) {

		if (strcmp(_naziv, desni.GetNaziv()) != 0)
			return false;
		return true;
	}

	friend ostream& operator<<(ostream& COUT, Tehnika& obj) {

		COUT << crt;
		COUT << ">> ISPIS TEHNIKE <<" << endl;
		COUT << "Naziv tehnike -> " << obj.GetNaziv() << endl;
		for (int i = 0; i < obj.GetOcjene().getTrenutno(); i++)
		{
			COUT << "Datum polaganja -> " << obj.GetOcjene().getElement1(i) << "\t -> ocjena" << obj.GetOcjene().getElement2(i) << endl;
		}
		COUT << "Prosjecna ocjena -> " << obj.GetProsjekOcjenaTehnike() << endl;
		return COUT;
	}
};
class Polaganje {
	Pojas _pojas;
	//string se odnosi na napomenu o polozenoj tehnici
	Kolekcija<Tehnika*, string> _polozeneTehnike;
public:

	// konstruktori
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
	}

	// getteri
	Kolekcija<Tehnika*, string>& GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }

	// specificne metode
	float GetProsjekPojasa() {

		float prosjek = 0;
		if (_polozeneTehnike.getTrenutno() == 0)
		{
			return prosjek;
		}
		for (int i = 0; i < _polozeneTehnike.getTrenutno(); i++)
		{
			prosjek += _polozeneTehnike.getElement1(i)->GetProsjekOcjenaTehnike();
		}
		return prosjek / _polozeneTehnike.getTrenutno();
	}
	void DodajTehnikuUPolozene(Tehnika& tehnika, string napomena) {
		_polozeneTehnike.AddElement(new Tehnika(tehnika), napomena);
	}

	// operatori
	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << "Pojas -> " << IspisPojaseva[obj._pojas] << endl;
		for (int i = 0; i < obj._polozeneTehnike.getTrenutno(); i++)
		{
			cout << *obj._polozeneTehnike.getElement1(i) << " " << obj._polozeneTehnike.getElement2(i) << endl;
		}
		return COUT;
	}
};
class KaratePolaznik {
	char* _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Polaganje> _polozeniPojasevi;
public:
	KaratePolaznik(const char* imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = ValidirajEmail(emailAdresa) ? emailAdresa : GetNizKaraktera("notSet@edu.karate.ba");
		_brojTelefona = brojTelefona;
		_polozeniPojasevi.push_back(Polaganje());
	}
	KaratePolaznik(const KaratePolaznik& original) {
		_imePrezime = GetNizKaraktera(original._imePrezime);
		_emailAdresa = original._emailAdresa;
		_brojTelefona = original._brojTelefona;
		_polozeniPojasevi = original._polozeniPojasevi;
	}
	~KaratePolaznik() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	//vraca kolekciju tehnika koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
	 //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na tehnici
	Kolekcija<Tehnika*, float>& operator()(Datum pocetniDatum, Datum krajnjiDatum) {
		Kolekcija<Tehnika*, float>* temp = new Kolekcija<Tehnika*, float>();

		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{
			for (int j = 0; j < _polozeniPojasevi[i].GetTehnike().getTrenutno(); j++) {
				for (int k = 0; k < _polozeniPojasevi[i].GetTehnike().getElement1(j)->GetOcjene().getTrenutno(); k++)
				{

					if (_polozeniPojasevi[i].GetTehnike().getElement1(j)->GetOcjene().getElement1(k).DaLiJeIzmedjuZadatihDatuma(pocetniDatum, krajnjiDatum)) {
						temp->AddElement(_polozeniPojasevi[i].GetTehnike().getElement1(j), _polozeniPojasevi[i].GetTehnike().getElement1(j)->GetProsjekDana());
					}
				}
			}
		}

		return *temp;
	}
	KaratePolaznik& operator=(const KaratePolaznik& desni) {
		if (this != &desni)
		{
			delete[]_imePrezime;
			_imePrezime = GetNizKaraktera(desni._imePrezime);
			_emailAdresa = desni._emailAdresa;
			_brojTelefona = desni._brojTelefona;
			_polozeniPojasevi = desni._polozeniPojasevi;
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << obj._polozeniPojasevi[i];
		return COUT;
	}
	vector<Polaganje>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char* GetImePrezime() { return _imePrezime; }

	void SlanjeEmailPoruke(Pojas pojas, Tehnika tehnika) {
		cuvar.lock();
		cout << crt;
		cout << "FROM:info@karate.ba" << endl;
		cout << "TO: " << GetEmail() << "\nPostovani " << GetImePrezime() << ", evidentirana vam je tehnika "
			<< tehnika.GetNaziv() << " za " << IspisPojaseva[pojas] << "pojas. "
			<< "Dosadasnji uspjeh (prosjek ocjena) na pojasu " << IspisPojaseva[pojas] << " iznosi ";
		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{
			if (_polozeniPojasevi[i].GetPojas() == pojas)
				cout << _polozeniPojasevi[i].GetProsjekPojasa();
		}
		cout << ", a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi " << GetProsjekSvihPojaseva();
		cout << "\nPozdrav.\nKARATE Team.\n";
		cuvar.unlock();
	}
	void SlanjeSMSPoruke(Polaganje polaganje, Pojas pojas) {
		cuvar.lock();
		cout << crt;
		cout << "Svaka cast za uspjeh " << polaganje.GetProsjekPojasa() << " na " << IspisPojaseva[pojas] << " pojasu" << endl;
		cuvar.unlock();
	}

	bool AddTehniku(Pojas pojas, Tehnika& tehnika, string napomena) {

		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{
			//onemoguceno dodavanje tehnika za vise pojaseve ako ne postoji najmanje jedna tehnika za nizi pojas(ne mozemo dodati tehniku za NARANDZASTI ako ne postoji niti jedna tehnika za ZUTI pojas)
			if (pojas > _polozeniPojasevi[i].GetPojas() && _polozeniPojasevi[i].GetTehnike().getTrenutno() == 0)
			{
				// osim ako se dodaje pojas ZUTI, njegov preduslov nije da nizi pojas (BIJELI) mora imati barem 1 polozenu tehniku
				if (pojas != ZUTI)
					return false;
			}
			if (pojas == _polozeniPojasevi[i].GetPojas())
			{
				// onemoguceno dodavanje istih tehnika na osnovu jednog pojasa
				if (DaLiPostojiIstaTehnika(pojas, tehnika)) {
					/*cout << "postoji ista tehnika u pojasu" << endl;*/
					return false;
				}
				//onemoguceno dodavanje tehnika u pojaseve kod kojih je prosjecna ocjena manja od 3.5
				if (_polozeniPojasevi[i].GetProsjekPojasa() < 3.5) {
					//cout << "onemoguceno dodavanje tehnika u pojaseve kod kojih je prosjecna ocjena manja od 3.5" << endl;
					return false;
				}

				_polozeniPojasevi[i].DodajTehnikuUPolozene(tehnika, napomena);

				// slanje email-a
				thread t1(&KaratePolaznik::SlanjeEmailPoruke, this, pojas, tehnika);
				t1.join();

				// slanje SMS-a
				if (_polozeniPojasevi[i].GetProsjekPojasa() > 4.5)
				{
					thread t2(&KaratePolaznik::SlanjeSMSPoruke, this, _polozeniPojasevi[i], pojas);
					t2.join();
				}

				return true;
			}
		}

		// dodavanje novog pojasa u uspjeh(Polaganje)
		Polaganje p(pojas);
		p.DodajTehnikuUPolozene(tehnika, napomena);
		_polozeniPojasevi.push_back(p);

		// slanje email-a
		thread t1(&KaratePolaznik::SlanjeEmailPoruke, this, pojas, tehnika);
		t1.join();

		return true;
	}

	bool DaLiPostojiIstaTehnika(Pojas pojas, Tehnika& tehnika) {

		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{
			if (_polozeniPojasevi[i].GetPojas() == pojas)
			{
				for (int j = 0; j < _polozeniPojasevi[i].GetTehnike().getTrenutno(); j++)
				{
					if (*_polozeniPojasevi[i].GetTehnike().getElement1(j) == tehnika)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	float GetProsjekSvihPojaseva() {
		float prosjek = 0;
		if (_polozeniPojasevi.size() == 0)
		{
			return prosjek;
		}
		for (int i = 0; i < _polozeniPojasevi.size(); i++)
		{
			prosjek += _polozeniPojasevi[i].GetProsjekPojasa();
		}
		return prosjek / _polozeniPojasevi.size();
	}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Za sta se koriste modovi ios::ate i ios::trunc ?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Pojasniti ulogu i naèin koristenja iteratora?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
void main() {

	cout << PORUKA;
	cin.get();

	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();

	Datum temp,
		datum19062021(19, 6, 2021),
		datum20062021(20, 6, 2021),
		datum30062021(30, 6, 2021),
		datum05072021(5, 7, 2021);

	int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(BEZ_DUPLIKATA);
	for (int i = 0; i <= kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
		//takodjer, izuzetak se baca i u slucaju da se prekoraci maksimalan broj elemenata
		kolekcija1.AddElement(3, 3);
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	cout << kolekcija1 << crt;

	/*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
	ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
	Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
	cout << kolekcija2 << crt;
	try {
		//primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
		Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
		Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
	}
	catch (exception& err) {
		cout << err.what() << crt;
	}
	//svaka tehnika moze imati vise ocjena i polaze se u vise navrata (istog ili drugog dana)
	//parametri: nazivTehnike, prva ocjena, datum polaganja
	Tehnika choku_zuki("choku_zuki", datum19062021, 5),
		gyaku_zuki("gyaku_zuki", datum20062021, 5),
		kizami_zuki("kizami_zuki", datum30062021, 2),
		oi_zuki("oi_zuki", datum05072021, 5);

	choku_zuki.AddOcjena(&datum05072021, 3);
	choku_zuki.AddOcjena(&datum05072021, 5);

	// ispisuje: naziv tehnike, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu za tu tehniku
	// ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0
	cout << choku_zuki << endl;

	/*
	email adresa treba biti u sljedecem formatu 3_ime.prezime@karate.ba tj. zadovoljavati sljedeæa pravila:
	- poceti sa jednim brojem nakon cega slijedi donja crtica
	- u imenu posjedovati najmanje 3 karaktera
	- izmedju imena i prezimena moze biti tacka ili donja crtica ili nista od navedenog
	- u prezimenu posjedovati najmanje 3 karaktera
	- znak @
	- domenu karate.ba ili edu.karate.ba. Pored drzavne(.ba), dozvoljene su oznake .com i .org.

	za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
	validacija email adrese ce se vrsiti unutar konstruktora klase KaratePolaznik, a u slucaju da nije validna
	postaviti je na defaultnu adresu: notSet@edu.karate.ba
	*/

	if (ValidirajEmail("2_ime.prezime@edu.karate.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("3_ime_prezime@karate.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("4_imeprezime@karate.com"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("8_imeprezime@karate.org"))
		cout << "Email validan" << crt;
	if (!ValidirajEmail("2ime.prezime@edu.karate.org"))
		cout << "Email NIJE validan" << crt;
	if (!ValidirajEmail("5_ime prezime@edu.karate.org"))
		cout << "Email NIJE validan" << crt;


	KaratePolaznik* jasmin = new KaratePolaznik("Jasmin Azemovic", "1_jasmin.azemovic@karate.ba", "033 281 172");
	KaratePolaznik* adel = new KaratePolaznik("Adel Handzic", "2_adel_handzic@edu.karate.ba", "033 281 170");
	KaratePolaznik* emailNotValid = new KaratePolaznik("Ime Prezime", "korisnik@karate.ru", "033 281 170");

	/*
	svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju tehnike)
	uspjeh se dodaje za svaku tehniku na nivou pojasa (ZUTI, ZELENI ... ).
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
	- dodavanje tehnika za vise pojaseve ako ne postoji najmanje jedna tehnika za nizi pojas (ne mozemo dodati tehniku za NARANDZASTI ako ne postoji niti jedna tehnika za ZUTI pojas)
	- dodavanje tehnika kod kojih je prosjecna ocjena manja od 3.5
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/

	//ne treba dodati kizami_zuki jer ne postoji niti jedna tehnika za ZUTI pojas
	if (jasmin->AddTehniku(NARANDZASTI, kizami_zuki, "Napomena 0"))
		cout << "Tehnika 1 uspjesno dodan!" << crt;
	if (jasmin->AddTehniku(ZUTI, gyaku_zuki, "Napomena 1"))
		cout << "Tehnika 2 uspjesno dodan!" << crt;
	if (jasmin->AddTehniku(ZUTI, kizami_zuki, "Napomena 2"))
		cout << "Tehnika 3 uspjesno dodan!" << crt;
	if (jasmin->AddTehniku(ZUTI, oi_zuki, "Napomena 3"))
		cout << "Tehnika 4 uspjesno dodan!" << crt;
	if (jasmin->AddTehniku(ZUTI, choku_zuki, "Napomena 4"))
		cout << "Tehnika 5 uspjesno dodan!" << crt;
	//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
	if (!jasmin->AddTehniku(ZUTI, choku_zuki, "Napomena 5"))
		cout << "Tehnika NIJE uspjesno dodana!" << crt;

	/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
	FROM:info@karate.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
	na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
	Pozdrav.
	KARATE Team.
	ukoliko je prosjek na nivou tog pojasa veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.D na X pojasu".
	slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/
	cout << *jasmin << crt;

	//vraca kolekciju tehnika koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
	//float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na tehnici
	Kolekcija<Tehnika*, float> jasminUspjeh = (*jasmin)(Datum(18, 06, 2021), Datum(21, 06, 2021));
	for (size_t i = 0; i < jasminUspjeh.getTrenutno(); i++)
		cout << *jasminUspjeh.getElement1(i) << " " << jasminUspjeh.getElement2(i) << crt;

	delete jasmin;
	delete adel;
	delete emailNotValid;

	cin.get();
	system("pause>0");
}
