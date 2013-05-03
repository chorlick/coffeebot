#include "Bot.h"
#include <string>
#include <gloox/messagehandler.h>
#include <gloox/messagesession.h>
#include <gloox/client.h>
#include <gloox/message.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h> 

using namespace std;
using namespace gloox;


Bot::Bot() {

    connections = 0;
    parseConfigFile();
    debug(Notice, "Creating bot object");
    JID jid("chris.horlick@fis-cal.com");
    j = new Client(jid, "chris9562");
    j->registerMessageHandler(this);
    j->registerConnectionListener(this);
    j->setServer("talk.google.com");
    j->setPresence(Presence::Available, 1, "Im a coffee bot!");    
    j->connect(true);
    debug(Notice, "Event loop delatched");
    j->disconnect();
    debug(Notice, "Coffeebot disconnected");
}


Bot::Bot(const Bot & b) {
    j = b.j;



}

void Bot::debug(int debug, const char * fmt, ...) {
    char buffer[256];


    if(debug == Warning) {
        snprintf(buffer, 256, "[Bot.cpp][warning] - ");
    }else if(debug == Notice){
        snprintf(buffer, 256,   "[Bot.cpp][notice] -  ");
    }else if(debug == Critical) {
        snprintf(buffer, 256,   "[Bot.cpp][critcal] - ");
    }else if(debug == Debug) {
        snprintf(buffer, 256,   "[Bot.cpp][debug] -   ");
    }
    va_list ap;
    int ret;
    va_start(ap, fmt);
    printf(buffer);
    ret = vprintf( fmt, ap);
    va_end(ap);
    printf("\n");
}




Bot::~Bot() {

}

bool Bot::parseConfigFile() {
    int i;
    char c;
    char context[64];
    char buffer[512];
    
    debug(Notice, "Starting to parse config file");
    i = 0;
    memset(buffer, 0, 512);
    memset(context,0, 64);
    FILE * pfile = fopen("./config.txt", "r");
    if(pfile) {
        while(!feof(pfile)){
            c = fgetc(pfile);
            if(c == '\n') {

                
                if(buffer[0] == '[' && buffer[i - 1] == ']') {
                    snprintf(context, strlen(buffer) - 1, "%s", buffer +1 );
                    debug(Debug, "Found a context %s", context);
                }else if(strcmp(context, "facts") == 0){
                    facts.push_back(buffer);   
                }else if(strcmp(context, "motd") == 0) {
                    buffer[i] = c;
                    motd += buffer;
                }else if(strcmp(context, "help" ) == 0) {
                    buffer[i] = c;
                    help += buffer;
                }else if(strcmp(context, "admin") == 0 ) {
                    
                    admins.push_back(buffer);
                }
                i = 0;
                memset(buffer, 0, 512);
                continue;
            }else if(c == '[') {
                debug(Debug, "Parsing context header");
                memset(context, 0, 64);
            }
        
            buffer[i++] = c;
            
        }
        debug(Notice, "Successful parse file");
        fclose(pfile);
        return true;
    }else{
        debug(Warning, "No config file found");
        return false;
    }

}

void Bot::handleMessage(const Message & stanza, MessageSession * session) {

    debug(Debug, "CoffeBot incoming message : %s", stanza.body().c_str());
    std::string resp = "hello world";

    if(stanza.subtype() == 1){  //MessageType::Chat
        connections++;
        if( (stanza.body().length() == 4) && (stanza.body().compare("help") == 0)) {
            debug(Debug, "Got help message");
            Message msg(Message::Chat, stanza.from(), help,  "subject", "thread", "");
            j->send(msg);
        }
        connections--;
        return;
    }
}


void Bot::onConnect(){
    debug(Notice, "Coffeebot connected to chat server" );
}

void Bot::onDisconnect(gloox::ConnectionError ce) {
    debug(Notice, "Coffebot disconnect %d",(int) ce); 
    if( ce == 9) {
        debug(Critical, "Coffeebot connection Refused by server");
    }

}
bool Bot::onTLSConnect(const gloox::CertInfo&) {
    debug(Warning,"Coffeebot requestiong tls connection");
    return true;
}

void Bot::handlePresence(const gloox::Presence&) {
    debug(Debug, "Someting about precense");

}

void * Bot::doHelpMessage(void * arg) {
    Bot * b = static_cast<Bot*>(arg);

    b->debug(Debug, "Doing a help message");
    
    return 0;

}

