#include<iostream>
#include<conio.h>
#include<queue>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<windows.h>
#include<algorithm>
int MAX_CUANTO = 3;
int MAX_PROC =1;
int LIMITE = 0;
int MINIMO = 0;
using namespace std;

void delay(int number_of_seconds){
	int mili_seconds=1000*number_of_seconds;
	clock_t start_time = clock();
	while(clock()<start_time+mili_seconds);
}

class Procesos { 
	public: 
		int id; 
		int tam;
		int cuanto; 
		void setId(int id) {
			this->id = id; 
		}
		void setCuanto() {
			cuanto = rand() % MAX_CUANTO + 1; 
		}
		void setSinCuanto(){
			cuanto = 0;
		}
		void setTam(int tama){
			tam = tama;
		}
		int getId(){
			return id;
		}
		int getTam(){
			return tam;
		}
		int getCuanto(){
			return cuanto;
		}
};

class Particion {
	public:
		bool ocupado;
		int tamParticion;
		Procesos Proc;
	public:
		Particion(int tam){
			ocupado = false;
			tamParticion = tam;
			Proc.setId(0);
			Proc.setSinCuanto();
			Proc.setTam(0);
		}
		bool getOcupado(){
			return ocupado;
		}
		void setOcupado(bool aux){
			ocupado = aux;
		}
		int getTam(){
			return tamParticion;
		}
		void setProceso(Procesos proc){
			Proc.id = proc.getId();
			Proc.cuanto = proc.getCuanto();
			Proc.tam = proc.getTam();
		}
		void mostrarPro(){
			cout << "ID: " << Proc.getId() <<"	CUANTOS RESTANTES: " << Proc.getCuanto() << "	TAMANO DEL PROCESO: " << Proc.getTam() << " ";
		}
};

class Lista{
	public:
		int tamanio;
		vector<Particion> memoria;
	public:
		static int numProcesos;
		static int id;
		Lista(int tamanio) {
			this->tamanio = tamanio; 
			this-> id = 0;
			memoria.clear();
		}
		int hacerParticion(int);
		void ingresarProceso(int);
		void unirParticion();
		int restarCuantosReinicio();
		void restarCuantos();
		void mostrarMemoria(){
			HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );  
			cout << endl;
			for(auto i: memoria){
				cout << "\t";
				if(i.getOcupado()) {
					//SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),28); 
					SetConsoleTextAttribute( h, 12 );
					cout<<i.getTam()<<"\t";
					SetConsoleTextAttribute( h, 15 );
					i.mostrarPro();
				}
				else{
					SetConsoleTextAttribute( h, 10 );
					cout<<i.getTam()<<" ";
				}
				cout << endl << endl;
			}
			SetConsoleTextAttribute( h, 7 );
			//cout<<endl<<"Desea continuar? 1=Si,0=Nay: ";
			//fflush(stdin);
			//int tiempo = rand()%10+5;
			delay(2);
			system("CLS");	
		}
};

int Lista::id = 0;
int Lista::numProcesos = 0;

int Lista::hacerParticion(int tam){
	int max = 0;
	if(memoria.size()==0){
		memoria.push_back(Particion(tamanio));
	}
	for(int i=0;i<memoria.size(); i++){
			if(memoria[i].getOcupado() == true){
				continue;
			}
			if(memoria[i].getTam() <= 32){
				continue;
			}
			else{
				if(memoria[i].getTam()>=tam){
					if((memoria[i].getTam()/2)>=tam){
						int aux = (memoria[i].getTam()/2);
						memoria.emplace(memoria.begin()+i,Particion(aux));
						memoria.emplace(memoria.begin()+i,Particion(aux));
						memoria.erase(memoria.begin()+i+2);
						break;
					}
				}
			}
			
	}
	for(int j=0;j<memoria.size(); j++){
		if(memoria[j].getTam()>=max and memoria[j].getOcupado() == false)
			max = memoria[j].getTam();
	}
	//cout<<"Max: "<<max<<endl;
	return max;
}

void Lista::ingresarProceso(int tam){
	for(int i = 0; i < memoria.size();i++){
		if((tam > 31 &&(memoria[i].getTam()-tam)<=tam && (memoria[i].getTam()-tam)>=0) || 
		(tam < 32 && (memoria[i].getTam()-tam) <= memoria[i].getTam() && (memoria[i].getTam()-tam)>=0 && memoria[i].getTam() == 32)){
			if(memoria[i].getOcupado() == false){
				Lista::id++;
				Lista::numProcesos++;
				Procesos aux;
				aux.setId(Lista::id);
				aux.setTam(tam);
				aux.setCuanto();
				memoria[i].setProceso(aux);
				memoria[i].setOcupado(true);
				cout<<"Proceso " << aux.getId() <<" De tamano: "<<tam<<" en memoria ";// << endl;
				return;
			}	
		}
	}
	if(hacerParticion(tam)>=tam){
		ingresarProceso(tam);
		return;
	}
	else{
		cout<<"Proceso en espera a insertar: "<<tam<<endl;
		restarCuantos();
		unirParticion();
		ingresarProceso(tam);
	}
	return;
	
}

void Lista::unirParticion(){
	int band = 0;
	if(memoria.size()==0){//Memoria sin particiones
		return;
	}
	for(int i = 0;i<memoria.size()-1;i++){
		if(memoria[i].getOcupado() == true){//Particion ocupada, se brinca a la siguiente
			continue;
		}
		//if(memoria[i].getTam()==memoria[i+1].getTam() && memoria[i].getOcupado() == false && memoria[i+1].getOcupado() == false){
		if(memoria[i].getTam()==memoria[i+1].getTam() && memoria[i].getOcupado() == false && memoria[i+1].getOcupado() == false){
			if((i+2)==memoria.size() || i == 0){
				int aux = memoria[i].getTam()*2; //Aux es el tamaño de la nueva particion, que es la suma de ambas a borrar.
				memoria.erase(memoria.begin()+i);
				memoria.erase(memoria.begin()+i);
				memoria.emplace(memoria.begin()+i,Particion(aux)); //Hay que verificar qué posición de i es donde se va a poner la nueva particion.
				i--;
				continue;
			}
			if((memoria[i].getTam()*2)>memoria[i+2].getTam()) continue;
			//if(!(i+2>memoria.size()))continue;
			int aux = memoria[i].getTam()*2; //Aux es el tamaño de la nueva particion, que es la suma de ambas a borrar.
			memoria.erase(memoria.begin()+i);
			memoria.erase(memoria.begin()+i);
			memoria.emplace(memoria.begin()+i,Particion(aux)); //Hay que verificar qué posición de i es donde se va a poner la nueva particion.
			i--;
		}
	}
}

bool comparador(const Particion &a,const Particion &b){
	if(a.Proc.id!=b.Proc.id) return a.Proc.id > b.Proc.id;
}

void Lista::restarCuantos(){
	vector<int> aux;
	if(id==1){
		for(int i = 0; i < memoria.size();i++){
			if(memoria[i].Proc.getId() == 1){
				//cout<<"Si funciona :) "<<memoria[i].Proc.cuanto<<endl;
				mostrarMemoria();
				int resta = memoria[i].Proc.getCuanto();
				memoria[i].Proc.cuanto = resta-1;
				if(memoria[i].Proc.cuanto <= 0 ){
					memoria[i].setOcupado(false);
					memoria[i].Proc.setId(0);
					memoria[i].Proc.setTam(0);
					LIMITE = 0;
					Lista::numProcesos--;
					unirParticion();
					//ingresarProceso();
				}
				unirParticion();
				//mostrarMemoria();
				return;
			}	
		}
	}
	for(int i = 0; i < memoria.size();i++){
		if(memoria[i].getOcupado() == true)
			aux.push_back(memoria[i].Proc.getId());
	}
	sort(aux.begin(),aux.end());
	for(int i = 0; i < memoria.size();i++){
		if(memoria[i].Proc.getId() == aux[LIMITE]){
			int resta = memoria[i].Proc.getCuanto();
			memoria[i].Proc.cuanto = resta-1;
			if(memoria[i].Proc.cuanto == 0 ){
				memoria[i].setOcupado(false);
				memoria[i].Proc.setId(0);
				memoria[i].Proc.setTam(0);
				LIMITE--;
				Lista::numProcesos--;
				unirParticion();
				//ingresarProceso();
			}
			LIMITE++;
			unirParticion();
			mostrarMemoria();
			break;
		}
	}
	if(LIMITE == Lista::numProcesos){
		LIMITE = 0;
		unirParticion();
		mostrarMemoria();
	}
}
	

int main (){
	//system("color 1F");
	//SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),240); 
	srand(time(NULL));
	HANDLE h = GetStdHandle( STD_OUTPUT_HANDLE );  
	
	int tamanio,tam=100,aux=0,opc;
	
	do{
		SetConsoleTextAttribute( h, 15 );
		cout<<"Inserte el tamaño de su memoria (1,4,8)Mb: ";
		cin >> tamanio;
		tamanio*=1024;
	}while(tamanio != 1024 and tamanio != 4096 and tamanio != 8192);
	
	do{
		SetConsoleTextAttribute( h, 15 );
		cout<<"Inserte el tamaño maximo de proceso: ";
		cin >> MAX_PROC;
	}while(MAX_PROC >= tamanio or MAX_PROC < 1);
	
	Lista x(tamanio);
	
	do{
		aux = rand() % (MAX_PROC-1)+1;
		//cout << aux << endl;
		x.unirParticion();
		x.ingresarProceso(aux);
		x.restarCuantos();
		x.unirParticion();
		x.mostrarMemoria();
		/*cout << endl;
		for(auto i: x.memoria){
			cout << "\t";
			if(i.getOcupado()) {
				//SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),28); 
				SetConsoleTextAttribute( h, 12 );
				cout<<i.getTam()<<"\t";
				SetConsoleTextAttribute( h, 15 );
				i.mostrarPro();
			}
			else{
				SetConsoleTextAttribute( h, 10 );
				cout<<i.getTam()<<" ";
			}
			cout << endl << endl;
		}
		SetConsoleTextAttribute( h, 7 );
		//cout<<endl<<"Desea continuar? 1=Si,0=Nay: ";
		//fflush(stdin);
		//int tiempo = rand()%10+5;
		delay(4);
		system("CLS");
		//cin>>opc;*/
		if(GetAsyncKeyState(VK_ESCAPE))
        {
            cout <<"Fin del Programa "<<endl;
            break;
        }
	}while(true);
	cout<<"Hola que pez";
	cin>>opc;
	return 0;
}
