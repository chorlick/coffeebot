#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h++>


using namespace std;
using namespace libconfig;



int main(int argc, char * argv[]) {

	Config cfg;
	int i;
	int count;
	const char  *buffer;
	char * pch;
	char stamp[512];
	try {
		cfg.readFile("./config.txt");
		Setting & qotd = cfg.lookup("qotd");
		qotd = 123123123;
				
	}catch (const FileIOException &t) {
		throw;
	}catch (const ParseException  &p) {
		throw;
	}



}
