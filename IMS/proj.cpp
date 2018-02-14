#include "simlib.h"
#include <math.h>
int i = 0;
long dojicky, poc_krav, poc_otelenych, cakaren;
Store Dojicky("Sklad dojiciek", 24);
Store Box("Mastal", 520);
Store Box_otelenia("Proces otelenia", 43);
Store Box_otelenia2("Proces otelenia2", 43);
Store Cakaren_in("VstupnaCakaren", 72);
Store Cakaren_out("VystupnaCakaren", 72);
Store Telaren("Jasle", 43);

Histogram cas_dojenia_kravy("Pocet podojenych krav za polroka", 0, 182 * 24 * 60, 4);
Histogram pocet_bykov_za_polrok("Pocet narodenych bykov za polroka", 0, 182 * 24 * 60, 4);
Histogram pocet_jalovic_za_polrok("Pocet narodenych jalovic za polroka", 0, 182 * 24 * 60, 4);

#define kravy 520
#define kap_cakaren 72
Facility Stanoviste("Miesto pre auto odvazajuce cisternu s mliekom");
Facility Stanoviste2("Miesto pre auto doplnajuce krmivo");
Facility Stanoviste3("Stanovisko nahanaca");
int cisterna = 0;
double min_time = 800*24*60;
double max_time = 0;
class Krava : public Process {
public:
	Krava(int a):Process() {
		num = a;
		otelenie = 0;
		otelit = 0;
		(new Otelenie(this))->Activate();
	};
	int num; int otelenie; int otelit;
	void Behavior () {

		while (1) {
			if (otelit == 1 && otelenie == 0 && !(Box_otelenia.Full())) {
				otelenie = 1;
				Enter(Box_otelenia, 1);
				WaitUntil(!(Box_otelenia.Full()));
				Wait(30 * 24 * 60);
				Leave(Box_otelenia, 1);
				Enter(Box_otelenia2, 1);
				WaitUntil(!(Box_otelenia2.Full()));
				Wait(30 * 24 * 60);
				Leave(Box_otelenia2, 1);

				(new Tela(num)) -> Activate();
				otelit = 0;
			}
			Enter(Box, 1);
			Wait(448);
			Leave(Box, 1);
			WaitUntil(!(Cakaren_in.Full()) && Dojicky.Empty() && Cakaren_out.Empty());
			Enter(Cakaren_in, 1);
			if (num < (poc_krav - ((poc_krav - poc_otelenych * 2) % cakaren))) {
				WaitUntil(Cakaren_in.Full());
			}
			double start_time = Time;
			Wait((Uniform(6, 8)));
			Enter(Dojicky, 1);
			//if (num == 512)
				//Print("Doji\n");
			Leave(Cakaren_in, 1);
			Wait((Uniform(4, 6)));
			Leave(Dojicky, 1);
			//cisterna = cisterna + 11;
			Enter(Cakaren_out, 1);
			if (num < (poc_krav  - ((poc_krav - poc_otelenych * 2) % cakaren))) {
				WaitUntil(Cakaren_out.Full());
			}
			else
				WaitUntil(Dojicky.Empty());
			Wait((Uniform(6,8)));
			Leave(Cakaren_out, 1);
			cas_dojenia_kravy(Time);
			if((Time - start_time) > max_time)
				max_time = (Time - start_time);
			if((Time - start_time) < min_time)
				min_time = (Time - start_time);
			
			//Enter(Box, 1);
			//Wait(Exponential(20));
			//Leave(Box, 1);
		}
	}

	class Otelenie : public Process {
	public:
		Otelenie(Krava *kdo):Process(){
			Kdo = kdo;
		};
		void Behavior() {
			   while (1) {
				//Wait (720 * 60);
				if (Kdo->otelenie == 0) {
					Kdo->otelit = 1;
					WaitUntil(Kdo->otelenie != 1);
					Wait(8760 * 60);
					Kdo->otelenie = 0;
				}
			}
		}
		Krava *Kdo;
	};

	class Tela : public Process {
	public:
		Tela(int a):Process() {
			num = a;
		};
		int num; 
		void Behavior () {
			WaitUntil(!(Telaren.Full()));
			Enter(Telaren, 1);
			if (Random() <= 0.5) {
				pocet_bykov_za_polrok(Time);
				Leave(Telaren, 1);
				this->Passivate();
			}
			else {
				pocet_jalovic_za_polrok(Time);
				Leave(Telaren, 1);
				Wait(24 * 712 * 60);
				this->Passivate();
			}	
		}
	};

};


class Auto : public Process {
	void Behavior() {
		while(1) {
			Wait(1344);
			Wait(Exponential(33));
			Seize(Stanoviste);
			//WaitUntil(cisterna < 16000);
			Wait(Exponential(30));
			//cisterna = cisterna - 16000;
			Release(Stanoviste);
			Wait(Exponential(33));
		}
	}
};		

class Stroj : public Process {
	void Behavior() {
		while (1) {
			Seize(Stanoviste2);
			Wait(5 * 60);
			Release(Stanoviste2);
			Wait(Exponential(3 * 60));
		}
	}
};

class Nahanac : public Process {
	void Behavior() {
		while(1) {
			Seize(Stanoviste3);
			WaitUntil(Cakaren_in.Full());
			Release(Stanoviste3);
			Wait(10);
			WaitUntil(Cakaren_out.Full());
			Wait(10);

		}
	}
};
int main(int argc, char *argv[]) {
		dojicky = std::atol(argv[1]);
		poc_krav = std::atol(argv[2]);
		cakaren = std::atol(argv[3]);
		double pom = round((poc_krav * 0.083));
		poc_otelenych =  (long) pom;
		Dojicky.SetCapacity(dojicky);
		Box.SetCapacity(poc_krav);
		Box_otelenia.SetCapacity(poc_otelenych);
		Box_otelenia2.SetCapacity(poc_otelenych);
		Cakaren_in.SetCapacity(cakaren);
		Cakaren_out.SetCapacity(cakaren);
		Telaren.SetCapacity(poc_otelenych);
		Init(0, 800 * 24* 60);
		SetOutput(argv[4]);


	for (int a = 0; a < poc_krav; a++) {
		(new Krava(a)) -> Activate();
	}
	(new Auto) -> Activate();
	(new Stroj) -> Activate();
	(new Nahanac) -> Activate();

 Run();

 Print("Zobrazene udaje boli ziskane za casove obdobie 800 dni\n");
 Print("Casove jednotky su uvedene v minutach\n");
 
 Print("Najvacsi cas, ktory krava stravila celym procesom dojenia je %g\n", max_time);
 Print("Najmensi cas, ktory krava stravila celym procesom dojenia je %g\n", min_time);

 Print("Pocet krav je %d\n", poc_krav);
 Print("Kapacita cakarne je %d\n", cakaren);
 Print("Pocet dojiciek je %d\n", dojicky);


 Dojicky.Output();	
 //Box.Output();
 Cakaren_out.Output();
 Cakaren_in.Output();
 Box_otelenia.Output();
 Telaren.Output();
 Stanoviste.Output();
 Stanoviste2.Output();
 Stanoviste3.Output();
 cas_dojenia_kravy.Output();
 pocet_bykov_za_polrok.Output();
 pocet_jalovic_za_polrok.Output();

}	