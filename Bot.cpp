#include "Bot.h"
#include <string>
#include <gloox/messagehandler.h>
#include <gloox/client.h>
#include <gloox/message.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>

using namespace std;
using namespace gloox;


Bot::Bot() {

    debug(Warning, "[Bot.cpp][notice] Creating bot object");
    JID jid("chris.horlick@fis-cal.com");
    j = new Client(jid, "XXXXXXXX");
    j->registerMessageHandler(this);
    j->registerConnectionListener(this);
    j->setServer("talk.google.com");
    j->setPresence(Presence::Available, 1, "Im a coffee bot!");    
    j->connect(true);
    debug("[Bot.cpp][notice] Event loop delatched");
    j->disconnect();
    debug("[Bot.cpp][notice] Coffeebot disconnected");
}

void Bot::debug(int debug, const char * fmt, ...) {

    if(debug == Warning) {
        printf("[warning] ");
    }else{
        printf("[notice] ");
    }

    va_list ap;
    int ret;
    va_start(ap, fmt);
    ret = vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}




Bot::~Bot() {

}

bool Bot::parseConfigFile() {

    FILE * pfile = fopen("./config.txt", "r");
    if(pfile) {
        fclose(pfile);
        return true;
    }else{
        return false;
    }

}

void Bot::handleMessage(const Message & stanza, MessageSession * sesion) {

    debug("[Bot.cpp][notice] CoffeBot incoming message : %s", stanza.body());
    std::string resp = "hello world";

    if(stanza.subtype() == 1){  //MessageType::Chat
        Message msg(Message::Chat, stanza.from(), "hello world", "subject", "thread", "");
        j->send(msg);   
    }
}


void Bot::onConnect(){
    debug( "[Bot.cpp][notice] Coffeebot connected to chat server" );
}

void Bot::onDisconnect(gloox::ConnectionError ce) {
    debug("[Bot.cpp][notice] Coffebot disconnect %d",(int) ce); 
    if( ce == 9) {
        cout << "[Bot.cpp][critical] Coffeebot connection Refused by server" << endl;
    } 

}
bool Bot::onTLSConnect(const gloox::CertInfo&) {
    cout << "[Bot.cpp][warning] Coffeebot requestiong tls connection " << endl;
    return true;
}

void Bot::handlePresence(const gloox::Presence&) {
        cout << "[Bot.cpp][debug] someting about precense" << endl;
    
}

