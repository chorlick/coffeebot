#include "Bot.h"
#include <iostream>
#include <string>
#include <signal.h>

using namespace std;

void sig_handler(int i);


int main(int argc, char * argv[]) {
	
	signal(SIGKILL, sig_handler);
	signal(SIGINT, sig_handler);
	cout << "[main.cpp][notice] Starting coffeebot .1 " << endl;
	Bot  b;
	return 0;

}


void sig_handler(int sig) {

	cout << "[main.cpp][notice] Got signal " << sig<< endl;
	switch (sig) {
		case SIGKILL :
		break;
			
		case SIGINT :
		break;
	}
}
