#include "lista_sas.h"


class Macierz_Sas{

  Lista<Krawedz> * lista_k;
  Lista<Wierzcholek> * lista_w;
  
 public:
  
  Wierzcholek * w_startowy;

  Krawedz *** macierz;

  Macierz_Sas();
  ~Macierz_Sas();

  ////////////////////////////////////////////////////////////////////////////////////////
  /**********************
   * Funkcje pomocnicze *
   *********************/

  void inicjuj_w_startowy( Wierzcholek * start );
  Wierzcholek * w_start() {return w_startowy;};
  
  void przyporzadkuj_liste_w( Lista<Wierzcholek> * lista_wierz);
  void przyporzadkuj_liste_k( Lista<Krawedz> * lista_kraw);
  void oczysc_listy();
  
  int spr_poz_wierz_x(int wartosc);

  void alokuj_pamiec_macierzy(int nxn);
  void pokaz_macierz();
  
  Lista<Krawedz> * zwroc_liste_k() {return lista_k;};
  Lista<Wierzcholek> * zwroc_liste_w() {return lista_w;};
  /////////////////////////////////////////////////////////////////////////////////////////
  /*******************
   * Metody dostępu  *
   ******************/
  Wierzcholek ** tab_krancow_kraw( Krawedz * K);
  Wierzcholek * przeciwlegly( Wierzcholek * W , Krawedz * K);
  bool czy_sasiednie( Wierzcholek * W1 , Wierzcholek * W2 );
  void podmien_w( Wierzcholek * W , int info );
  void podmien_k( Krawedz * K , int info );

  ////////////////////////////////////////////////////////////////////////////////////////
  /*************************
   * Metody uaktualniające *
   ************************/
  void dodaj_wierzcholek( int info );
  void usun_wierzcholek( Wierzcholek * W );
  
  void dodaj_krawedz( Wierzcholek * wierz1 , Wierzcholek * wierz2 , int info );
  void usun_krawedz(Krawedz * K);

  ///////////////////////////////////////////////////////////////////////////////////////
  /*************************
   * Metody iterujące *
   ************************/

  // Wypisuje wszystkie krawędzie przylegające do wierzchołka W
  void kraw_przyl_do_w( Wierzcholek * W);
  void pokaz_krawedzie();
  void pokaz_wierzcholki();

};


void Macierz_Sas::oczysc_listy(){
  lista_w=nullptr;
  lista_k=nullptr;
  }







////////////////////////////////////////////////////////////////////////////////////////
/**********************
 * Funkcje pomocnicze *
 *********************/

Macierz_Sas::Macierz_Sas(){
  lista_k = new Lista<Krawedz>;
  lista_w = new Lista<Wierzcholek>;
  w_startowy = new Wierzcholek;
}

Macierz_Sas::~Macierz_Sas(){
  //if( lista_k != nullptr )
    delete lista_k;
    //if( lista_w != nullptr )
    delete lista_w;
    //if( w_startowy != nullptr )
    delete w_startowy;
}

void Macierz_Sas::alokuj_pamiec_macierzy(int nxn){

  this->macierz = new Krawedz ** [nxn];
  for( int i=0 ; i<nxn ; i++)
    this->macierz[i] = new Krawedz * [nxn];
  
  for( int j=0 ; j<nxn ; j++)
    for( int i=0 ; i<nxn ; i++)
      this->macierz[j][i] = new Krawedz ;
}

void Macierz_Sas::inicjuj_w_startowy( Wierzcholek * start ){
  this->w_startowy = start;
}

void Macierz_Sas::przyporzadkuj_liste_w( Lista<Wierzcholek> * lista_wierz){
  this->lista_w = lista_wierz;
}

void Macierz_Sas::przyporzadkuj_liste_k( Lista<Krawedz> * lista_kraw){
  this->lista_k = lista_kraw;
}

void Macierz_Sas::pokaz_macierz(){
  int dl;
  dl = lista_w->dlugosc_listy();
  
  for( int i=0 ; i<=dl ; i++){
    for( int j=0 ; j<=dl ; j++){
      if( i==0 && j==0)
	cout<<"X\t";
      else if( i==0 )
	cout << lista_w->zwracanie_obiektu(j)->nr <<"\t";
      else if( j==0 )
	cout << lista_w->zwracanie_obiektu(i)->nr <<"\t";
      else if( macierz[ i-1 ][ j-1 ] == nullptr )
	cout << "0" << "\t";
      else 
	cout << macierz[ i-1 ][ j-1 ]->waga << "\t";

    }
    cout<<"\n\n";
  }
  cout<<"\n\n";
}


/*
  Sprawdza pozycje wierzchołka o wartości x. Pomaga wykryć czy dany wierzchołek istnieje
*/
int Macierz_Sas::spr_poz_wierz_x(int x){ 
  
  this->lista_w->iterator = this->lista_w->head;
  int numer=1;

  // Gdy lista jest pusta, funkcja zwraca zero
  if( this->lista_w->iterator == nullptr ) 
    return 0;

  while( this->lista_w->iterator->obiekt->nr != x ){
    
    // Gdy wierzchołeknie nie istnieje, funkcja zwraca zero
    if( this->lista_w->iterator->nast == nullptr ) 
      return 0;
    
    else{
      this->lista_w->iterator = this->lista_w->iterator->nast;
      numer++;
    }
  }
  return numer;
}









/////////////////////////////////////////////////////////////////////////////////////////
/*******************
 * Metody dostępu  *
 ******************/

Wierzcholek **  Macierz_Sas::tab_krancow_kraw( Krawedz * K){
  Wierzcholek ** krance;
  krance = new Wierzcholek * [2];
  krance[0] = new Wierzcholek;
  krance[1] = new Wierzcholek;
    
  krance[0] = K->poczatek;
  krance[1] = K->koniec;

  return krance;
}


Wierzcholek * Macierz_Sas::przeciwlegly( Wierzcholek * W , Krawedz * K){
  
  int dl_listy;
  dl_listy = lista_w->dlugosc_listy();

  lista_w->iterator = lista_w->head;

  for( int i=0 ; i<dl_listy ; i++) // szukanie krawedzi w macierzy sąsiedztwa
    if( macierz[ W->klucz ][ i ] == K ){//Jeśli znajdziemy naszą krawędz - zrób:
      if( K->poczatek == W ) // zwróc koniec gdy W to początek
	return K->koniec;
      else if(K->koniec == W) // zwróc początek gdy W to koniec
	return K->poczatek;
    }
}

bool Macierz_Sas::czy_sasiednie( Wierzcholek * W1 , Wierzcholek * W2 ){

  if( macierz[ W1->klucz ][ W2->klucz ]->waga == 0 )
    return 0;
  else
    return 1;
}

void Macierz_Sas::podmien_w( Wierzcholek * W , int info ){
  W->nr = info;
}

void Macierz_Sas::podmien_k( Krawedz * K , int info ){
  K->waga = info;
}




////////////////////////////////////////////////////////////////////////////////////////
/*************************
 * Metody uaktualniające *
 ************************/

void Macierz_Sas::dodaj_krawedz( Wierzcholek * wierz1 , Wierzcholek * wierz2 , int info ){ //////////////////////  Dodawanie krawędzi do listy
  
  int indeks;
  Krawedz * kraw;
  kraw = new Krawedz;
  
  indeks = lista_k->dodaj_wezel();
  lista_k->iterator = lista_k->head;

  for( int i=1 ; i<indeks ; i++)
    lista_k->iterator = lista_k->iterator->nast;

  this->lista_k->iterator->obiekt = kraw;
  
  lista_k->zwracanie_obiektu(indeks)->waga = info;
  lista_k->zwracanie_obiektu(indeks)->poczatek = wierz1;
  lista_k->zwracanie_obiektu(indeks)->koniec = wierz2;
  lista_k->zwracanie_obiektu(indeks)->wsk_pozycja = lista_k->zwracanie_wezla(indeks);

  macierz[ wierz1->klucz ][ wierz2->klucz ] = kraw;
  macierz[ wierz2->klucz ][ wierz1->klucz ] = kraw;
}

void Macierz_Sas::usun_krawedz(Krawedz * K){

  int wiersz;
  int kolumna;
  int indeks = 1;
  
  for( int i=0 ; i<this->zwroc_liste_w()->dlugosc_listy() ; i++ )
    for( int j=0 ; j<this->zwroc_liste_w()->dlugosc_listy() ; j++ )
      if( macierz[ i ][ j ] == K ){
	wiersz = i;
	kolumna = j;
	i=this->zwroc_liste_w()->dlugosc_listy();
	j=this->zwroc_liste_w()->dlugosc_listy();
      }

  macierz[ wiersz ][ kolumna ]->poczatek = nullptr;
  macierz[ wiersz ][ kolumna ]->koniec = nullptr;
  macierz[ wiersz ][ kolumna ]->wsk_pozycja = nullptr;
  
  macierz[ wiersz ][ kolumna ] = nullptr;
  macierz[ kolumna ][ wiersz ] = nullptr;

  lista_k->iterator = lista_k->head;

  while( lista_k->iterator->obiekt != K && lista_k->iterator->nast != nullptr){
    lista_k->iterator = lista_k->iterator->nast;
    indeks++;
  }

  lista_k->usun_wezel(indeks);
}


void Macierz_Sas::dodaj_wierzcholek( int info ){///////Dodawanie wierzchołka do listy

  Wierzcholek * wierzch;
  wierzch = new Wierzcholek;

  int indeks;
  int dlugosc;
  indeks = lista_w->dodaj_wezel();

  this->lista_w->iterator = this->lista_w->head;
  for( int i=1 ; i<indeks ; i++)
    this->lista_w->iterator = this->lista_w->iterator->nast;
   
  lista_w->iterator->obiekt = wierzch;
  lista_w->zwracanie_obiektu(indeks)->nr = info;

  dlugosc = lista_w->dlugosc_listy();
  lista_w->zwracanie_obiektu(dlugosc)->klucz = dlugosc-1;

  Krawedz *** macierz2;   // deklaracja nowej macierzy i alokacja jej pamieci;
  macierz2 = new Krawedz ** [ dlugosc];
  for( int i=0 ; i<dlugosc ; i++)
    macierz2[i] = new Krawedz * [ dlugosc ];
  for( int j=0 ; j<dlugosc ; j++)
    for( int i=0 ; i<dlugosc ; i++)
      macierz2[i][j] = new Krawedz;

  
  for( int i=0 ; i<dlugosc ; i++) //  Iteracja po wszystkich wierszach macierzy
    for( int j=0 ; j<dlugosc ; j++){// Iteracja po wszystkich kolumnach macierzy

      // Jeśli kolumna/wiersz nie jest ostatni to przepisuje wartości ze starej macierzy. Dla nowego wierzchołka jednak przyporządkowujemy nullptr
      if( i == dlugosc-1 || j == dlugosc-1 ) 
	macierz2[i][j] = nullptr;
      else
	macierz2[i][j] = macierz[i][j];
    }
  
  for(int i=0 ; i<dlugosc-1 ; i++)
    delete [] macierz[i];
  delete [] macierz ;
  
  macierz = macierz2;
}


void Macierz_Sas::usun_wierzcholek( Wierzcholek * W ){//////  Usuwanie wierzchołka z listy

  int ilosc_usunietych_k=0;
  int ilosc_wierzcholkow;
  int ilosc_krawedzi;
  
  ilosc_wierzcholkow = lista_w->dlugosc_listy();  // zwraca ilosc wszystkich wierzchołków
  ilosc_krawedzi = lista_k->dlugosc_listy();   // zwraca ilość wszystkich krawędzi


  for( int j=0 ; j<ilosc_wierzcholkow ; j++)
    for( int i=0 ; i< (ilosc_krawedzi - ilosc_usunietych_k) ; i++)
      if( macierz[ W->klucz ][ j ] == lista_k->zwracanie_obiektu(i) ){
	this->usun_krawedz( macierz[ W->klucz ][ j ] );
	ilosc_usunietych_k++;
      }
  
  Krawedz *** macierz2;   // deklaracja nowej macierzy i alokacja jej pamieci;
  macierz2 = new Krawedz ** [ ilosc_wierzcholkow-1 ];
  for( int i=0 ; i<ilosc_wierzcholkow ; i++)
    macierz2[i] = new Krawedz * [ ilosc_wierzcholkow-1 ];
  for( int j=0 ; j<(ilosc_wierzcholkow-1) ; j++)
    for( int i=0 ; i<(ilosc_wierzcholkow-1) ; i++)
      macierz2[i][j] = new Krawedz;

  
  
  for( int i=0 ; i<(ilosc_wierzcholkow-1) ; i++){ //  Iteracja po wszystkich wierszach macierzy
    if(  i < W->klucz ){ // Jeśli numer wiersza jest mniejszy od numeru wierzchołka do usunięcia

      for( int j=0 ; j<(ilosc_wierzcholkow-1) ; j++){// Iteracja po wszystkich kolumnach macierzy
	if( j < W->klucz )               // Jeśli numer kolumny jest mniejszy od numeru wierzchołka do usunięcia
	  macierz2[i][j] = macierz[i][j];
	
	else     // Jesli numer kolumny jest większy bądź równy numerowi wierzchołka do usunięcia
	  macierz2[i][j] = macierz[i][j+1];
      }
    }

    else{  // Jesli numer wiersza jest większy bądź równy numerowi wierzchołka do usunięcia
      
      for( int j=0 ; j<(ilosc_wierzcholkow-1) ; j++){
	if( j < W->klucz )               // Jeśli numer kolumny jest mniejszy od numeru wierzchołka do usunięcia
	  macierz2[i][j] = macierz[i+1][j];
	
	else //Jesli numer kolumny jest >/= numerowi usuwanego wierzchołka oraz jest to nieostani wiersz
	  macierz2[i][j] = macierz[i+1][j+1];
      }
    }
  }


  macierz = macierz2;
  
  
  lista_w->zwracanie_obiektu(W->klucz+1)->wsk_pozycja = nullptr;
  lista_w->usun_wezel(W->klucz+1);
}





///////////////////////////////////////////////////////////////////////////////////////
/*************************
 * Metody iterujące *
 ************************/

void Macierz_Sas::kraw_przyl_do_w( Wierzcholek * W){
  int dlugosc;
  dlugosc = lista_w->dlugosc_listy();

  cout<< "Krawedzie przylegajace do wierzcholka nr "<<W->klucz<<" o wartosci "<<W->nr<<endl;
  for( int i=0 ; i<dlugosc ; i++){
    if ( macierz[ W->klucz ][ i ]->waga != 0 )
      cout << macierz[ W->klucz ][ i ]->waga << "   ";
  }
  cout << endl;
}


void Macierz_Sas::pokaz_krawedzie(){/////////////// Pokazuje wszystkie krawędzie

  lista_k->iterator = lista_k->head;
 
  cout<<"\nKrawedzie:\n";
  while( lista_k->iterator->nast != NULL){
    cout << "(" << lista_k->iterator->obiekt->poczatek->nr <<" , ";
    cout << lista_k->iterator->obiekt->koniec->nr <<") ";
    cout << lista_k->iterator->obiekt->waga << endl;
    lista_k->iterator = lista_k->iterator->nast;
  }
  cout << "(" << lista_k->iterator->obiekt->poczatek->nr <<" , ";
  cout << lista_k->iterator->obiekt->koniec->nr <<") ";
  cout << lista_k->iterator->obiekt->waga << endl;

  lista_k->iterator = lista_k->head;
}


void Macierz_Sas::pokaz_wierzcholki(){  //////////////// Pokazuje wszystkie wierzcholki
  lista_w->iterator = lista_w->head;

  cout<<"\nWierzcholki:\n";
  while( lista_w->iterator->nast != NULL){
    cout << lista_w->iterator->obiekt->nr <<", ";
    lista_w->iterator = lista_w->iterator->nast;
  }
  cout << lista_w->iterator->obiekt->nr<<"\n";
  
  lista_w->iterator = lista_w->head;
}

