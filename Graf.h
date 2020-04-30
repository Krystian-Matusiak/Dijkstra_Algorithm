#include "lista.h"

class Wierzcholek;
class Krawedz;


/***********
Klasa przechowująca id oraz wskaźnik na pozycje na liscie
************/

class Wierzcholek{
  
 public:

  Wierzcholek();
  ~Wierzcholek();

  int klucz;
  int nr;
  Wezel<Wierzcholek> * wsk_pozycja;
  Lista<Krawedz> * wskazniki_na_kraw;
  
};

Wierzcholek::Wierzcholek(){
  klucz = 0;
  wskazniki_na_kraw = new Lista<Krawedz>;
  wsk_pozycja = new Wezel<Wierzcholek>;  
}

Wierzcholek::~Wierzcholek(){
  delete wskazniki_na_kraw;
  delete wsk_pozycja;
}



/***********
Klasa przechowująca wagę, wierzchołek początkowy,
wierzchołek końcowy oraz wskaźnik na pozycje na liscie
************/

class Krawedz{

 public:
  int waga;

  Krawedz();
  ~Krawedz();
  
  Wierzcholek * poczatek;
  Wierzcholek * koniec;

  Wezel<Krawedz> * wsk_wierz1;
  Wezel<Krawedz> * wsk_wierz2;
  
  Wezel<Krawedz> * wsk_pozycja;

};

Krawedz::Krawedz(){
  waga=0;
  poczatek = new Wierzcholek;
  koniec = new Wierzcholek;
  wsk_wierz1 = new Wezel<Krawedz>;
  wsk_wierz2 = new Wezel<Krawedz>;
  wsk_pozycja = new Wezel<Krawedz>;
}

Krawedz::~Krawedz(){
  delete poczatek;
  delete koniec;
  delete wsk_wierz1;
  delete wsk_wierz2;
  delete wsk_pozycja;
}

