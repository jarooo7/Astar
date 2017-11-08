/* Algorytm A*
Jaros�aw Piotrowski
student UWM w Olsztynie
Informatyka stpoien 1 ISI rok III*/
#include<iostream>
#include<fstream>
#include<cmath>
#include<list>
#include <windows.h>
using namespace std;
const int x=20;										//sta�a przechowujaca szeroko�c mapy
const int y=20;										//sta�a przechowywujaca wysoko�� mapy
////////////////////////
typedef struct{										//Struktura stworzona z tablicy dwuwymiarowej typu integer
 	int tab[y][x];
}map;
////////////////////////							
class Punkt{										//****1****}		klasa punkt przechowywuj�ca dane punktu (po�o�enie [x,y], kolejka czyli ilo�c p�l
    public:												//od punktu startowego,po�o�enie rodzica[Rx,Ry] warto�� czyli warto�c pola pbliczona ze wzoru
    int x,y,kolejka,Rx,Ry;								//wartosc=kolejka+PIERWIASTEK((x-CEL)^2+(y-CEL)^2;
    double wartosc;	
	Punkt(int x,int y,int cx,int cy,Punkt rodz);    //definicje konstruktor�w
	Punkt();
	Punkt(int x,int y,int cx,int cy);								
    bool operator < (const Punkt &q)const			//warunek sortowania 
    {
        return wartosc>=q.wartosc;
    }
};													//**end*1**}
Punkt::Punkt(int x,int y,int cx,int cy,Punkt rodz){		//Konstruktor klasy Punkt 
	this->x=x;
	this-> y=y;
	this-> kolejka=rodz.kolejka+1;
	this-> wartosc=this-> kolejka+sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy));
	this-> Rx=rodz.x;
	this-> Ry=rodz.y;	
}
Punkt::Punkt(){	}										//Pusty konstruktor klasy Punkt
Punkt::Punkt(int x,int y,int cx,int cy){				//Konstruktor klasy Punkt inicjujacy punkt startowy
	this-> x=x;
	this-> y=y;
	this-> kolejka=0;
	this-> wartosc=this-> kolejka+sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy));
}
////////////////////////
 map WczytajMap(){									//metoda zczytujaca z pliku "map�" i zrwacajaca ja za pomoca struktury map
	ifstream odczyt;
	map mapa;
	odczyt.open("grid.txt",ios::in|ios::out);
	for(int i=y-1;i>=0;i--)
	{
		for(int j=0;j<x;j++)
			odczyt>>mapa.tab[i][j];
	}
	odczyt.close();
	return(mapa);
}
////////////////////////////
void WyswietlMap(map mapa){							// metoda s�u�aca do wy�wietlania struktury map
HANDLE text;										//zmienna u�ywana do zmiany koloru czcionki i jej t�a w systemie Windows
text = GetStdHandle( STD_OUTPUT_HANDLE );
	for(int i=y-1;i>=0;i--)							
	{												//****2****}       p�tle majace na celu wy�wietlenie mapy u�yte s� w nich r�wierz operacacje 
		cout<<i;										//wy�wietlajace pomocnicze linie i indexy
		if (i<10)
			cout<<"  | ";
		else
			cout<<" | ";
		for(int j=0;j<x;j++){
		 	switch(  mapa.tab[i][j])										//instrukcaja warunkowa switch, ustalajaca odpowiedni kolor dla wartosci z mapy
   		 	{
   				case 3:														//warto�� 3 jest przedstawiona na kolor niebieski jest to warto�c badanych p�l
        		{
					SetConsoleTextAttribute( text, BACKGROUND_BLUE );		
					cout<<mapa.tab[i][j]<<"  ";
        			break;
				}
				case 5:														//wartosc 5 jest przedstawiona kolorek czerwochym, to warto�� przeszk�d
        		{
					SetConsoleTextAttribute( text, BACKGROUND_RED );
					cout<<mapa.tab[i][j]<<"  ";
        			break;
				}
				case 9:														//warto�c 9 przedstawiona na zielono , to warto�ci wytyczajace optymalna trase
        		{
					SetConsoleTextAttribute( text, BACKGROUND_GREEN );
					cout<<mapa.tab[i][j]<<"  ";
        			break;
				}
				case 1:														//warto�ci 1,2 sa przedstawione na bia�ym tle (warto�� 1 to start, 2 to cel)
				case 2:
        		{
					SetConsoleTextAttribute( text, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
					cout<<mapa.tab[i][j]<<"  ";
        			break;
				}
				default:													//w pozosta�ycj przypadkach ustawienia wyswietlanai sa cofniete do standardowych
        		{ 
					SetConsoleTextAttribute( text, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
					cout<<mapa.tab[i][j]<<"  ";
        		break;
				}
			}
			SetConsoleTextAttribute( text, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );  //ustawienie koloru na standardowy
			}  										
			cout<<endl;
	}
	cout<<"___|_";
	for(int i=0;i<y;i++)
		cout<<"___";
	cout<<endl;
	cout<<"   | ";
	for(int i=0;i<y;i++){
		cout<<i;
	if (i<10)
		cout<<"  ";
	else
		cout<<" ";}
	cout<<endl<<endl;							//**end*2**}
}
Punkt WczytajPunkt(map m,string nazwa){								//funkcja so szczytywania z kalawiatury pozycji
	Punkt p;
	do{
		cout<<"podaj watro�c x Punktu "<<nazwa<<" : ";
		cin>>p.x;
		cout<<"podaj watro�c y Punktu "<<nazwa<<" : ";
		cin>>p.y;
		if(p.y<0||p.y>=y||p.x<0||p.x>=x)							//warunek ograniczenia warto�ci mapy
			cout<<"wartosc x lub y nie miesci sie w przedziale"<<endl;
		if (m.tab[p.y][p.x]==5)										//warunek czy nie przeszkoda
			cout<<"w tym polu wyst�puje przeszkoda"<<endl;
	}while(m.tab[p.y][p.x]==5||p.y<0||p.y>=y||p.x<0||p.x>=x);
	return p;
}
///////////////////////////////////////////////////////////////////////////
void Sprawdz(map *mapa,list<Punkt> *lista,Punkt punkt,int sx,int sy,Punkt cel){			//****3****}     funkcja sprawdzajaca czy jest mo�liwe wejscie na pole 
	map  m=*mapa;																			//je�eli tak funkcja [rze;icza warto�c pola i dodaje ja na liste 
	list<Punkt> l=*lista;																	//Otwart�, kt�ra przechowuje pola sprawdzone i kt�re nie by�y 
	if(punkt.x+sx>=0&&punkt.y+sy>=0&&punkt.x+sx<x&&punkt.y+sy<y) 							//rodzicem pozatym zmienia sprawdzone pole na mapie na 3(3 oznacza  
 	if(m.tab[punkt.y+sy][punkt.x+sx]==0||(punkt.y+sy==cel.y&& punkt.x+sx==cel.x)){ 			//pole przeliczone).
 		l.push_back(Punkt(punkt.x+sx,punkt.y+sy,cel.x,cel.y,punkt)); 	
	 	m.tab[punkt.y+sy][punkt.x+sx]=3; 			
	 } 	
	*mapa=m;
	*lista=l;																			//**end*3**}
}
///////////////////////////////////////////////////////////////////
int main(){
	system("map_generator");										//Wywo�anie aplikacji do grneracji mapy
	list<Punkt> lZamknieta;											//Stworzenie listy ZAMKNI�TEJ przechowywuj�c� pola kt�re by�y rodzicami 
	list<Punkt> lOtwarta;											//Stworzenie listy OTWARTEJ przechowywuj�c� pola kt�re NIE by�y rodzicami 
	Punkt temp;														//tymczasowa zmienna punkt 
	map mapa=WczytajMap();											//wczytanie truktury map z pliku (metoda WczytajMap();)
	WyswietlMap(mapa);												//Wy�wietlenie Mapy
	Punkt start=WczytajPunkt(mapa,"startowego");					//wczytanie z klawiatury punktu startowego
	Punkt cel=WczytajPunkt(mapa,"docelowego");						//wczytanie z klawiatury punktu docelowego
	mapa.tab[start.y][start.x]=1;									//ustawienie na mapie warto�ci 1 na punkcie startowym
	mapa.tab[cel.y][cel.x]=2;										//ustawienie na mapie warto�ci 2 na punkcie docelowym
	WyswietlMap(mapa);
	int kroki=0;													//stworzenei zmiennej krok obliczajacej liczbe krok�w od startu do celu zainicjowana zerem
	lZamknieta.push_back(Punkt(start.x,start.y,cel.x,cel.y)); 		//dodanie do listy ZAMKNIETEJ pole startowe
	do{																
		Sprawdz(&mapa,&lOtwarta,lZamknieta.back(),-1,0,cel);		//****4****}      p�tla do while wywo�uj�ca funkcje Sprawdz, kt�ra sprawdza pola wok�w 
		Sprawdz(&mapa,&lOtwarta,lZamknieta.back(),0,-1,cel);			//ostatniego pola dodanego do listy ZAMKNI�TEJ (listy rodzic�w) w 4 kierunkach
		Sprawdz(&mapa,&lOtwarta,lZamknieta.back(),1,0,cel);				//zaczynajac od lewej i kierujac si� odwrotnie do ruchu wskaz�wek zegara
		Sprawdz(&mapa,&lOtwarta,lZamknieta.back(),0,1,cel);
 		if(lOtwarta.size()==0){ 									//je�eli lista otwarta b�dzie pusta po sprawdzeniu i cel nie bedzei osiagniety stop
 			cout<<"Nie ma mozliwosci dojscia do celu"<<endl<<endl<<"Nacisbij ENTER aby zakonczyc." ;  //i wy�wietla komunikat
 			cin.sync();												//wyczyszczenie buforu wej�cie
			while (getchar() != '\n'){	}							//p�tla dopuki nie wci�ciety ENTER
 			return 0;
 		}
		lOtwarta.sort();									//Sortowanie listy otwartej od najwiekszej do najlmiejszej(opisane klasie punkt)
		lZamknieta.push_back(lOtwarta.back());				//Dodanie do listy zamknietej ostatni element przesortowanej listy otwartej (najmiejszy element)
		lOtwarta.pop_back();								//Usuni�cie ostatneigo elementu listy otwartej (kt�ry zosstal dodany do zamkni�tej listy)
		//WyswietlMap(mapa);		
	}while(lZamknieta.back().x!=cel.x||lZamknieta.back().y!=cel.y); //**end*4**}   p�tla ko�czy sie je�eli na li�cie zamkni�tej znajdzie sie pole CEL
	temp=lZamknieta.back();											//zmienna tymczasowa przyjmuje warto�� celu
 	mapa.tab[temp.y][temp.x]=9;										//mienia na mapie warto�c na 9 o pozycji celu
 	do{																				//****5****}       p�la ma na za zadanie ustalenie na podstawie listy 
 		lZamknieta.pop_back();															//zamkni�tej trase optymalna "od startu do celu"(w rzeczywisto�ci 
 		if(temp.Rx==Punkt(lZamknieta.back()).x&&temp.Ry==Punkt(lZamknieta.back()).y){	//p�tla za pomoca listy zamkni�tej wyszukuje pozycje rodzic�w p�l
 			temp=lZamknieta.back();														//zaczynajac od rodzic�w pola cel, a� natrafi na pole start
 			mapa.tab[temp.y][temp.x]=9;														
 			kroki++;																//pola trasy sa zlicane 
	 	}
 	}while(temp.x!=start.x||temp.y!=start.y);										//**end*5**}
 	WyswietlMap(mapa);
 	cout<<"Dlugosc trasy wynosi "<<kroki<<" krokow"<<endl<<endl<<"Nacisbij ENTER aby zakonczyc." ;  //wy�wietlenie d�ugo�ci trasy i stop programu
 	cin.sync();
	while (getchar() != '\n'){	}
 	return 0;
}
