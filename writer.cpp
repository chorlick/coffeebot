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
    string blah;
	try {
    
		cfg.readFile("./config.txt");
        cfg.setTabWidth(4);
 
        if(strstr(argv[1], "qotd")){

    		Setting & qotd = cfg.lookup("qotd");
            cout << argv[2] << endl;
            qotd = argv[2];
        }else if(strstr(argv[1], "wotd")) {

    		Setting & qotd = cfg.lookup("wotd");
            cout << argv[2] << endl;
            qotd = argv[2];
        }
   
        cfg.writeFile("./config.txt");
	}catch (const FileIOException &t) {
		throw;
	}catch (const ParseException  &p) {
		throw;
	}catch (const SettingTypeException  &w) {
        cout << "Bad set" << endl;
        throw;
    }



}
