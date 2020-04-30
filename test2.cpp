#include <iostream>
#include <fstream>
#include <ctime>

#define STALA 30
#include "Dij.h"

using namespace std;

/*****************************************************************************************
 * Podczas testowania każda funkcja działa zarówno dla listy jak i macierzy sąsiedztwa.  *
 * Elementy listy indeksuje się od 1 lecz w przypadku macierzy od 0. Pomiary zapisywane  *
 * w oddzielnych plikach ze względu na wycieki pamięci. Z tego samego powodu zmniejszono *
 * ilość powtórzeń pomiarów ze 100 do 30.                                                *
 *****************************************************************************************/

void menu(){
  cout<<"\n\n";
  cout<<"1. Dodaj element"<<endl;
  cout<<"2. Usun n-ty element"<<endl;
  cout<<"3. Pokaz krawedzie"<<endl;
  cout<<"4. Pokaz wierzcholki"<<endl;
  cout<<"5. Sprawdz przeciwlegly wierzcholek"<<endl;
  cout<<"6. Podaj krawedzie przylegajace do wierzchołka"<<endl;
  cout<<"7. Czy wierzcholki sa sasiadujace"<<endl;
  cout<<"8. Podmien dane w wierzcholku"<<endl;
  cout<<"9. Podmien dane w krawedzi"<<endl;
  cout<<"10. Tablica dwoch koncow krawedzi"<<endl;
  cout<<"11. Dzialanie algorytmu"<<endl;
  cout<<"12. Koniec programu"<<endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***********************************
 *Funkcje potrzebne do generowania *
 *danych do pliku tekstowego       *
 ***********************************/

void wypisz_dane(int wierzcholki, int il_krawedz, fstream& strumien , int indeks) {
  
  srand( rand() );
  int pozycja_wartosci =0;
  int numer_wierzcholka = wierzcholki;
  int wartosc1=0;
  int wartosc2=0;
  int tmp = 0;
  int licznik = 0;
  int liczba_poz_wyk = il_krawedz - wierzcholki + 1;

  if( indeks == 0){
    strumien.open("dane1.txt", ios_base::app);      
  }
  else if( indeks == 1){
    strumien.open("dane2.txt", ios_base::app);      
  } 
  else if( indeks == 2){
    strumien.open("dane3.txt", ios_base::app);      
  }
  else if( indeks == 3){
    strumien.open("dane4.txt", ios_base::app);      
  }
  else if( indeks == 4){
    strumien.open("dane5.txt", ios_base::app);      
  }
  
  strumien << il_krawedz << " " << wierzcholki << " " << rand()%wierzcholki  << endl;

  
  for( int i=0 ; i<(wierzcholki-1) && i<il_krawedz ; i++){
    strumien << i << " " << i+1 << " " << rand()%10000+1<<endl;
  }
  
  while(licznik < liczba_poz_wyk){ ///wypisujemy pozostale krawdzie w zaleznosci od wypelnienia
    tmp = 0;
    while(licznik < liczba_poz_wyk && tmp < numer_wierzcholka - 2){

      strumien << numer_wierzcholka - 1<<" "<< tmp <<" "<<rand()%10000+1<<endl;
      
      licznik++;
      tmp++;
      }
    numer_wierzcholka--;
  }
  
  strumien.close();           
}


void generuj_dane(fstream& strumien)
{
  
    int wierzcholki[5] = {10, 20, 30, 40 , 50};  
    int max_krawedzi[5];                            
    int krawedzie[5][4];                            
    int wypelnienie[4] = {25, 50, 75, 100};         
    
    for (int i=0 ; i<5 ; i++){

      if( i == 0){
	strumien.open("dane1.txt", ios_base::out);      
	strumien.close();
      }
      else if( i == 1){
	strumien.open("dane2.txt", ios_base::out);      
	strumien.close();
      } 
      else if( i == 2){
	strumien.open("dane3.txt", ios_base::out);      
	strumien.close();
      }
      else if( i == 3){
	strumien.open("dane4.txt", ios_base::out);      
	strumien.close();
      }
      else if( i == 4){
	strumien.open("dane5.txt", ios_base::out);      
	strumien.close();
      }
      
      cout<< "ilosc wierzcholkow : " << wierzcholki[i] <<endl;
      
      max_krawedzi [i] = wierzcholki[i] * (wierzcholki[i] - 1) /2;
      for (int z=0 ; z<4 ; z++){
	
	cout<< "wypelnienie w % : " << wypelnienie[z] <<endl;
	krawedzie[i][z] = wypelnienie[z] * max_krawedzi[i] /100;
	for(int c=0 ; c<STALA ; c++){
	  wypisz_dane (wierzcholki[i], krawedzie[i][z], strumien, i);
	}
      }
      cout<<"poszedl zestaw"<<endl;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////












void** wczytaj_tablice(int** graf,  int& kraw, fstream& wejscie){
  for( int i=0 ; i<kraw ; i++){    ///wczytujemy graf z pliku do tablicy dynamicznej 2-wymiarowej
    wejscie >> graf[i][0];
    wejscie >> graf[i][1];
    wejscie >> graf[i][2];
  }
}

void zapis_na_graf(fstream& wejscie ,   Lista_Sas * lista_sas , Macierz_Sas * macierz_sas){  //////////// Utworzenie listy sąsiedztwa z danych z pliku
  //wejscie.open("dane2.txt", ios_base::in);


  
  int n_wierzch, n_kraw, w_start;  // kolejno ilość wierzchołków, ilość krawędzi oraz wierzchołek startowy
  int ** graf;
  int w1,w2,waga;
  int pozycja_w1, pozycja_w2; // pozycja wierzchołka o wartościach kolejno w1 i w2
  
  wejscie >> n_kraw;            
  wejscie >> n_wierzch;
  wejscie >> w_start;
  
  //cout << "trojka danych : " << n_kraw << "  " << n_wierzch << "  " << w_start << endl;
  

  
  graf = new int* [n_kraw];    // alokowanie pamieci dla grafu
  for(int c = 0; c < n_kraw; c++)
    graf[c] = new int [3];
  
  
    wczytaj_tablice(graf, n_kraw, wejscie);          // inicjujemy wczytywanie grafu z pliku
    
  
  for( int i=0 ; i<n_kraw ; i++){  // Główny fragment kodu, odpowiadający za wypełnianie listy sąsiedztwa
    w1 = graf[i][0];
    w2 = graf[i][1];
    waga = graf[i][2];
    //cout << "trojka danych : " << w1 << "  " << w2 << "  " << waga << endl;
    //cout << "nkraw : "<<i << endl;

    if( ! lista_sas->spr_poz_wierz_x(w1) ) // sprawdzamy czy wierzchołek istnieje. Jeśli nie, funkcja zwróci zero
      lista_sas->dodaj_wierzcholek(w1);
    
    if( ! lista_sas->spr_poz_wierz_x(w2) )
      lista_sas->dodaj_wierzcholek(w2);

    if( ! macierz_sas->spr_poz_wierz_x(w1) )
      macierz_sas->dodaj_wierzcholek(w1);

    if( ! macierz_sas->spr_poz_wierz_x(w2) )
      macierz_sas->dodaj_wierzcholek(w2);

	     
    pozycja_w1 = lista_sas->spr_poz_wierz_x(w1); // Zwraca pozycje danego wierzchołka. Z racji tego, iż ich wartości są różne, szukania dokonuję na podstawię wartości
    pozycja_w2 = lista_sas->spr_poz_wierz_x(w2);

    
    if( w1 == w_start ){
      lista_sas->inicjuj_w_startowy( lista_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w1) );
      macierz_sas->inicjuj_w_startowy( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w1) );
    }
    if( w2 == w_start ){
      lista_sas->inicjuj_w_startowy( lista_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w2) );
      macierz_sas->inicjuj_w_startowy( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w2) );
    }
    
    lista_sas->dodaj_krawedz( lista_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w1) , lista_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w2) , waga);
    macierz_sas->dodaj_krawedz( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w1) , macierz_sas->zwroc_liste_w()->zwracanie_obiektu(pozycja_w2) , waga); 
  }

  
  for(int i=0 ; i<n_kraw; ++i) ///|-> usuwamy tablice dynamiczna przechowujaca dane grafu z pliku
    delete [] graf[i];
  delete [] graf;
 
  //wejscie.close();
}







//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////  MAIN  //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////






int main(){
  
  srand(time(NULL));
  
  fstream dane;
  fstream pomiar_czasu;
  fstream pomiar_drogi;

  int t_n; // Tak (1) lub Nie (0)
  cout<<"Czy generowac dane ?\n1.Tak\n0.Nie\n\n";
  cin >> t_n;
  if( t_n )
    generuj_dane(dane);

  cout<< "\n\n\nRobic pomiar (1) czy testowac recznie (0)?\n\n";
  cin >> t_n;
  if( t_n ){
  
  int tab_stoL[STALA]; // tablice czasów dla 80 pomiarów dla L-listy , M-macierzy
  int tab_stoM[STALA];
  
  int sredniaL; // średnia czasu ze stu pomiarów dla L-listy , M-macierzy
  int sredniaM;
  
  int *Dij_dystans; // tablica zapamietująca dystans z wierzchołka startowego do reszty 
  int *Dij_sciezka; // tablica ścieżek 
  
  int dana1,dana2,dana3 ; // potrzebe do wyciąganu danych z pliku do pliku


  int ww;
  cout<<"Wybierz dla ktorego pomiaru chcesz robic:\n"<<endl;
  cin>>ww;

      Lista_Sas             *** lista_sas;
    Macierz_Sas           *** macierz_sas;
         
    lista_sas =new Lista_Sas ** [4];
    macierz_sas =new Macierz_Sas ** [4];
    
      for(int j=0 ; j<4 ; j++){
	lista_sas[j] = new Lista_Sas*[STALA];
	macierz_sas[j] = new Macierz_Sas*[STALA];
      }
    
      for(int j=0 ; j<4 ; j++)
	for(int k=0 ; k<STALA ; k++){
	  lista_sas[j][k] = new Lista_Sas;
	  macierz_sas[j][k] = new Macierz_Sas;
	}
  
  for( int i=ww ; i<ww+1 ; i++){

    if(i==0){
      pomiar_czasu.open("pomiar_czasu.txt", ios_base::app);
      pomiar_drogi.open("pomiar_drogi.txt", ios_base::app);
      dane.open("dane1.txt", ios_base::in);
    }
    if(i==1){
      pomiar_czasu.open("pomiar_czasu2.txt", ios_base::app);
      pomiar_drogi.open("pomiar_drogi2.txt", ios_base::app);
      dane.open("dane2.txt", ios_base::in);
    }
    if(i==2){
      pomiar_czasu.open("pomiar_czasu3.txt", ios_base::app);
      pomiar_drogi.open("pomiar_drogi3.txt", ios_base::app);
      dane.open("dane3.txt", ios_base::in);
    }
    if(i==3){
      pomiar_czasu.open("pomiar_czasu4.txt", ios_base::app);
      pomiar_drogi.open("pomiar_drogi4.txt", ios_base::app);
      dane.open("dane4.txt", ios_base::in);
    }
    if(i==4){
      pomiar_czasu.open("pomiar_czasu5.txt", ios_base::app);
      pomiar_drogi.open("pomiar_drogi5.txt", ios_base::app);
      dane.open("dane5.txt", ios_base::in);
    }
      
    

    
    //////////////////////////////////////////////////////////////
    //  POMIAR
    
    
    
      for( int j=0 ; j<4 ; j++){
	for( int k=0 ; k<STALA ; k++){
	  
	  zapis_na_graf(dane, lista_sas[j][k], macierz_sas[j][k]);
	  
	  
	  dana1 = lista_sas[j][k]->zwroc_liste_k()->dlugosc_listy();
	  dana2 = lista_sas[j][k]->zwroc_liste_w()->dlugosc_listy();
	  dana3 = lista_sas[j][k]->w_startowy->nr;

	  pomiar_drogi << "Ilosc krawedzi " << dana1 << " Ilosc wierzcholkow " << dana2 << " Wierzcholek startowy " << dana3 << endl;

	  int *Dij_dystans; 
	  int *Dij_sciezka;
	  
	  Dij_dystans = new int [dana2];
	  Dij_sciezka = new int [dana2];

	  
	  auto czasListy = Dij_lista( lista_sas[j][k] , lista_sas[j][k]->w_startowy , Dij_dystans, Dij_sciezka );

	  auto czasMacierzy = Dij_macierz( macierz_sas[j][k] , macierz_sas[j][k]->w_startowy , Dij_dystans, Dij_sciezka );
	  
	  
	  pomiar_drogi << "Cel\t" << "Dystans\t" << "Sciezka\t" << endl;
	  
	  for(int c=0 ; c<lista_sas[j][k]->zwroc_liste_w()->dlugosc_listy() ; c++){
	    
	    pomiar_drogi << c << "\t" << Dij_dystans[c] << "\t\t";
	    int numer = c;
	    
	    if( Dij_sciezka[numer] != -1 )
	      while( Dij_sciezka[numer] != -1 ){
		pomiar_drogi << numer << "<-";
		numer = Dij_sciezka[ numer ];
		if( Dij_sciezka[numer] == -1 )
		  pomiar_drogi  << dana3 << endl; // wyswietlenie ostatniego
	      }
	    else if( Dij_sciezka[numer] == -1 )
	      pomiar_drogi  << dana3 << endl;
	  }
	  	  

	  delete [] Dij_dystans;
	  delete [] Dij_sciezka;
	  
	  tab_stoL[k] = czasListy;
	  tab_stoM[k] = czasMacierzy;
	}
	
	for( int sto=1 ; sto<STALA ; sto++){  // Liczenie średniej czasów i wpisywanie do ostatniego elementu tablicy
	  tab_stoL[sto] = tab_stoL[sto] + tab_stoL[sto-1];
	  tab_stoM[sto] = tab_stoM[sto] + tab_stoM[sto-1];
	}
	
	sredniaL = tab_stoL[STALA-1]/100;
	sredniaM = tab_stoM[STALA-1]/100;
	
	if( i == 0)
	  pomiar_czasu << "Dla " << 10 << endl;
	else if( i == 1)
	  pomiar_czasu << "Dla " << 20 << endl;
	else if( i == 2)
	  pomiar_czasu << "Dla " << 30 << endl;
	else if( i == 3)
	  pomiar_czasu << "Dla " << 40 << endl;
	else if( i == 4)
	  pomiar_czasu << "Dla " << 50 << endl;
	
	pomiar_czasu << "Dla " << (j+1)*25 << "% : ";
	pomiar_czasu << "Pomiar listy : " << (double)sredniaL/1000 << "ms  ";
	pomiar_czasu << "Pomiar macierzy : " << (double)sredniaM/1000 << "ms" << endl << endl << endl;
	
	
      }

    
    
  pomiar_czasu.close();
  pomiar_drogi.close();
  dane.close();

  }


  for(int j=0 ; j<4 ; j++)
    for(int k=0 ; k<STALA ; k++){
      delete lista_sas[j][k];
      delete macierz_sas[j][k];
    }
    
  for(int j=0 ; j<4 ; j++){
    delete [] lista_sas[j];
    delete [] macierz_sas[j];
  }
  
  delete [] lista_sas;
  delete [] macierz_sas;
  
  }









  
  else{
    
  //////////////////////////////////////////////////////////////
  //  TESTY METOD

    dane.open("dane1.txt", ios_base::in);
  
    Lista_Sas * lista_sas;
    Macierz_Sas *macierz_sas;

  lista_sas   = new Lista_Sas;
  macierz_sas = new Macierz_Sas;

  zapis_na_graf(dane, lista_sas, macierz_sas);
 
  
  int info;
  int nr;
  int wyb;
  
  while(1){
    menu();

    cout << "\nPodaj wybor 1-10:\n";
    cin>>wyb;

    macierz_sas->pokaz_macierz(); // dla małych ilości wierzchołków pokazuje macierz
    
    switch(wyb){
      
    case 1:
      cout<<"Podaj jaki element chcesz dodac (1 - Wierzcholek ; 2 - Krawedz):\n";
      cin>>nr;
      
      if(nr == 1){
	cout<<"Podaj wartosc wierzcholka:\n";
	cin>>info;
	lista_sas->dodaj_wierzcholek( info );
	macierz_sas->dodaj_wierzcholek( info );
      }
      else if(nr == 2){
	cout<<"Podaj wage krawedzi:\n";
	cin>>info;

	lista_sas->dodaj_krawedz(lista_sas->zwroc_liste_w()->zwracanie_obiektu(8) , lista_sas->zwroc_liste_w()->zwracanie_obiektu(0) , info );
	macierz_sas->dodaj_krawedz( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(8) , macierz_sas->zwroc_liste_w()->zwracanie_obiektu(0) , info );

      }
      else
	cout<<"Bledne dane\n";
      
      break;

      
    case 2:
      cout<<"Podaj jaki element chcesz usunac (1 - Wierzcholek ; 2 - Krawedz):\n";
      cin>>nr;
      
      if(nr == 1){
	macierz_sas->usun_wierzcholek( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(3)  );
	lista_sas->usun_wierzcholek( lista_sas->zwroc_liste_w()->zwracanie_obiektu(3) );
      }
      else if(nr == 2){
	macierz_sas->usun_krawedz( macierz_sas->zwroc_liste_k()->zwracanie_obiektu(1)  );
	lista_sas->usun_krawedz( lista_sas->zwroc_liste_k()->zwracanie_obiektu(1) );
      }
      else
	cout<<"Bledne dane\n";
      
      break;

      
    case 3:
      lista_sas->pokaz_krawedzie();
      macierz_sas->pokaz_krawedzie();
      break;
      
    case 4:
      lista_sas->pokaz_wierzcholki();
      macierz_sas->pokaz_wierzcholki();
      break;

    case 5:
      cout << "wierz1\twaga\twierz2\n";
      cout << lista_sas->zwroc_liste_w()->zwracanie_obiektu(1)->nr << "\t";
      cout << lista_sas->zwroc_liste_k()->zwracanie_obiektu(1)->waga << "\t";
      cout << lista_sas -> przeciwlegly(
					lista_sas->zwroc_liste_w()->zwracanie_obiektu(1) ,
					lista_sas->zwroc_liste_k()->zwracanie_obiektu(1)
					)->nr << endl;

      cout << macierz_sas->zwroc_liste_w()->zwracanie_obiektu(1)->nr << "\t";
      cout << macierz_sas->zwroc_liste_k()->zwracanie_obiektu(1)->waga << "\t";
      cout << macierz_sas -> przeciwlegly(
					  macierz_sas->zwroc_liste_w()->zwracanie_obiektu(1) ,
					  macierz_sas->zwroc_liste_k()->zwracanie_obiektu(1)
					  )->nr << endl;
      				
      break;


    case 6:
      lista_sas->kraw_przyl_do_w( lista_sas->zwroc_liste_w()->zwracanie_obiektu(1) ) ;
      macierz_sas->kraw_przyl_do_w( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(1) ) ;
      break;
      
    case 7:
      if( lista_sas->czy_sasiednie( lista_sas->zwroc_liste_w()->zwracanie_obiektu(1) , lista_sas->zwroc_liste_w()->zwracanie_obiektu(3) ))
	cout<<"Sasiednie"<<endl;
      else
	cout<<"Nie sasiednie\n";
      
      if( macierz_sas->czy_sasiednie( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(1) , macierz_sas->zwroc_liste_w()->zwracanie_obiektu(2) ))
	cout<<"Sasiednie"<<endl;
      else
	cout<<"Nie sasiednie\n";
      
      break;


      
    case 8:
      lista_sas->podmien_w( lista_sas->zwroc_liste_w()->zwracanie_obiektu(1) , 1111);
      macierz_sas->podmien_w( macierz_sas->zwroc_liste_w()->zwracanie_obiektu(1) , 1111);
      break;


      
    case 9:
      lista_sas->podmien_k( lista_sas->zwroc_liste_k()->zwracanie_obiektu(1) , 1111);
      macierz_sas->podmien_k( macierz_sas->zwroc_liste_k()->zwracanie_obiektu(1) , 1111);

      break;


      
    case 10:
      cout << "Krance krawedzi z listy sasiedztwa:\n";
      cout << "[ " << lista_sas->tab_krancow_kraw( lista_sas->zwroc_liste_k()->zwracanie_obiektu(1) )[0]->nr << " ][ ";
      cout << lista_sas->tab_krancow_kraw( lista_sas->zwroc_liste_k()->zwracanie_obiektu(1) )[1]->nr << " ]\n";

      cout << "Krance krawedzi z macierzy sasiedztwa:\n";
      cout << "[ " << macierz_sas->tab_krancow_kraw( macierz_sas->zwroc_liste_k()->zwracanie_obiektu(1) )[0]->nr << " ][ ";
      cout << macierz_sas->tab_krancow_kraw( macierz_sas->zwroc_liste_k()->zwracanie_obiektu(1) )[1]->nr << " ]\n";

      break;

    case 11:
      int *Dij_dystans; 
      int *Dij_sciezka;
      
      Dij_dystans = new int [ lista_sas->zwroc_liste_w()->dlugosc_listy() ];
      Dij_sciezka = new int [ lista_sas->zwroc_liste_w()->dlugosc_listy() ];
      
      Dij_lista( lista_sas , lista_sas->w_startowy , Dij_dystans, Dij_sciezka );
      
      delete [] Dij_dystans;
      delete [] Dij_sciezka;
      
      break;
      
    case 12:
      goto koniec;
      break;
      
    default:
      break;
      
    }
    cout<<"\n\n///////////////////\n";
  }

  koniec:
  delete lista_sas;
  delete macierz_sas;
  }


  return 0;
}
