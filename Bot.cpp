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
#include <stdlib.h>
#include <stdio.h>
#include <libconfig.h++>

using namespace std;
using namespace gloox;
using namespace libconfig;

Bot::Bot() {
	srand(time(NULL));
	connections = 0;
	parseConfigFile();
	debug(Notice, "Creating bot object");
	JID jid(username.c_str());
	j = new Client(jid, password.c_str());
	//j->registerMessageHandler(this);
	j->registerMessageSessionHandler(this, 0);
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
	debug(Debug, "Deconstructor called");
}

bool Bot::parseConfigFile() {
	Config cfg;
	int i;	
	int count;
	const char  *buffer;
	char * pch;
	char stamp[512];
	try {
		cfg.readFile("./config.txt");

		//Facts
		Setting & facts = cfg.lookup("facts");
		count = facts.getLength();
		for(i = 0; i < count; i++ ) {
			debug(Debug, "%s", facts[i].c_str());
			this->facts.push_back(facts[i]);
		}

		//admin
		Setting & admins = cfg.lookup("admins");
		count = admins.getLength();
		for(i = 0; i < count; i++) {
			struct Admin * t = new Admin;
			const Setting & admin = admins[i];
			admin.lookupValue("username", t->username);
			admin.lookupValue("password", t->password);
			debug(Debug, "Admin %d : username %s, password %s", i + 1, 
					admins[i]["username"].c_str(), admins[i]["password"].c_str());

		}

		help = cfg.lookup("help").c_str();

		Setting & brew = cfg.lookup("brew");
		brew[0].lookupValue("timestamp", buffer);
		snprintf(stamp, 512, "%s", buffer);
		brew[0].lookupValue("user", brew_user);

		pch = strtok(stamp, " ");
		brew_time.tm_wday = atoi(pch);
		pch = strtok(stamp, " ");
		brew_time.tm_year = atoi(pch);
		pch = strtok(stamp, " ");
		brew_time.tm_mon = atoi(pch);
		pch = strtok(stamp, " ");
		brew_time.tm_mday = atoi(pch);
		pch = strtok(stamp, " ");
		brew_time.tm_hour = atoi(pch);
		pch = strtok(stamp, " ");
		brew_time.tm_min = atoi(pch);
		pch = strtok(stamp, " ");
		brew_time.tm_sec = atoi(pch);

		makeTime(brew_time, stamp, 512);
		debug(Notice, "Read last brew time as %s", stamp);	

		const Setting & creds = cfg.lookup("creds");
		creds[0].lookupValue("username", username);
		creds[0].lookupValue("password", password);	

		qotd  = cfg.lookup("qotd").c_str();

	}catch (const FileIOException &t) {
		debug(Critical, "Error reading file");
		throw;
	}catch (const ParseException  &p) {
		debug(Critical, "Unable to parse config file %s at : %s ~ %s", p.getFile(), p.getLine(), p.getError() );
		throw;
	}

	debug(Notice, "Successful parse");
}



int Bot::makeTime(struct tm t, char* buffer, int size) {
	const char * weekday[] = { "Sunday", 
		"Monday",
		"Tuesday", 
		"Wednesday",
		"Thursday", 
		"Friday", 
		"Saturday"};

	const char * months[] = { "Janurary",
		"Feburary",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"};

	return snprintf(buffer, size, "Last brewed : %s, %s %d %d @ %d:%d.%d  by %s",weekday[t.tm_wday], months[t.tm_mon],
			t.tm_mday,t.tm_year, t.tm_hour, t.tm_min, t.tm_sec,  brew_user.c_str() );

}

void Bot::writeFreshTime() {

	Config cfg;
	char buffer[512];
	char line[512];
	int i;
	char c;
	int total;
	int offset;
	FILE* pfile = fopen("config.txt", "r+");
	i = 0;
	snprintf(buffer, 512, "%d %d %d %d %d %d %d %s\n",brew_time.tm_wday, brew_time.tm_mon, brew_time.tm_mday, 
			brew_time.tm_year, brew_time.tm_hour, brew_time.tm_min, brew_time.tm_sec,  brew_user.c_str());

	cfg.readFile("./config.txt");

	return;

}

void Bot::handleMessage(const Message & stanza, MessageSession * session) {


	char buffer[512];
	struct AdminSession * session_tmp;
	struct Admin *        admin_tmp;
	vector<struct AdminSession *>::iterator session_it;
	vector<struct Admin *>::iterator admin_it;
	if(stanza.subtype() == 1){  //MessageType::Chat
		debug(Debug, "CoffeBot incoming message : %s", stanza.body().c_str());
		if( (stanza.body().length() == 4) && (stanza.body().compare("help") == 0)) {
			debug(Debug, "Got help command");
			session->send(help);
			return;
		}else if( (stanza.body().length() == 4) && (stanza.body().compare("fact") == 0)) {
			debug(Debug, "Got facts command");
			session->send(facts.at(rand() % facts.size()));
			return;
		}else if( (stanza.body().length() == 4) && (stanza.body().compare("motd") == 0)) {
			debug(Debug, "Got motd command");
			session->send(motd);
			return;
		}else if( (stanza.body().length() == 4) && (stanza.body().compare("brew") == 0)) {
			debug(Debug, "Got brew command");
			makeTime(brew_time, buffer, 512);
			session->send(buffer);
			return;
		}else if( (stanza.body().length() == 5) && (stanza.body().compare("fresh") == 0)) {
			debug(Debug, "Got fresh command");
			time_t now = time(NULL);
			memcpy( &brew_time, localtime(&now), sizeof(struct tm));
			brew_user.resize(32);
			snprintf( (char *) brew_user.c_str(), 32, stanza.from().username().c_str());
			writeFreshTime();
			session->send("Wrote new time");
		}else if( (stanza.body().length() == 4) && (stanza.body().compare("qotd") == 0)) {
			debug(Debug, "Got qtod command");
			session->send(qotd);
		}else if( (stanza.body().length() == 5) && (stanza.body().compare("admin") == 0)) {
			struct AdminSession * admin = new struct AdminSession;
			admin->handle = stanza.from().username();
			debug(Notice, "User %s requesting admin status", stanza.from().username().c_str());
			session->send("Username:");
			auths.push_back(admin);
			return;
		}else if( (stanza.body().length() == 6) && (stanza.body().compare("logout") == 0)) {

		}



		for(session_it = auths.begin(); session_it != auths.end(); ++session_it) {

			session_tmp = *session_it;
			if(session_tmp->handle == stanza.from().username()) {
				break;
			}
		}

		if((session_tmp->handle == stanza.from().username()) && (session_tmp->authed == false) 
				&& (session_tmp->password.length() == 0 ) && (session_tmp->username.length() == 0)) {

			debug(Debug, "Username sent by %s", session_tmp->handle.c_str());
			session->send("Password");
			session_tmp->username = stanza.body();
			return;
		}

		if((session_tmp->handle == stanza.from().username()) && (session_tmp->authed == false) 
				&& (session_tmp->password.length() == 0 )) {

			debug(Debug, "Password sent by %s", session_tmp->handle.c_str());
			session_tmp->password = stanza.body();

			for(admin_it = admins.begin(); admin_it != admins.end(); ++admin_it) {
				admin_tmp = * admin_it;
				if( ( strcmp(admin_tmp->username.c_str(),session_tmp->username.c_str()) == 0) && 
						(strcmp(admin_tmp->password.c_str() ,session_tmp->password.c_str()) == 0) ) {
					debug(Notice, "Admin user logged in %s", admin_tmp->username.c_str());
					session_tmp->authed = true;
				}else {
					debug(Warning, "Failed log in attempt for user %s", admin_tmp->username.c_str());
					// delete session_tmp;
					// auths.erase(session_it);    
					debug(Debug, "User sent        :%s:%s:", session_tmp->username.c_str(), 
							session_tmp->password.c_str());
					debug(Debug, "Mathched against :%s:%s:", admin_tmp->username.c_str(), admin_tmp->password.c_str());
				}

			}
			return;
		}
	}
	return;
}


void Bot::onConnect(){
	debug(Notice, "Coffeebot connected to chat server" );
}

void Bot::onDisconnect(gloox::ConnectionError ce) {
	debug(Notice, "Coffebot disconnect %d",(int) ce); 
	debug(Debug, "Client base error %d", j->authError());
	if( ce == ConnNoError) {
		debug(Notice, "Not really an error. Everything went just fine. ");
	}else if (ce == ConnStreamError) {
		debug(Notice, "	A stream error occured. The stream has been closed. Use ClientBase::streamError() to find the reason. ");
	}else if (ce == ConnStreamClosed) {
		debug(Notice, "	The stream has been closed (by the server). ");
	}else if (ce == ConnProxyAuthRequired) {
		debug(Notice, "	The HTTP proxy requires authentication. ");
	}else if (ce == ConnProxyAuthFailed) {
		debug(Notice, "HTTP proxy authentication failed. ");
	}else if (ce == ConnIoError) {
		debug(Notice, "	An I/O error occured. ");
	}else if (ce == ConnParseError) {
		debug(Notice, "An XML parse error occurred. ");
	}else if (ce == ConnConnectionRefused) {
		debug(Notice, "The connection was refused by the server (on the socket level). ");
	}else if (ce == ConnDnsError) {
		debug(Notice, "	Resolving the server's hostname failed. ");
	}else if (ce == ConnOutOfMemory) {
		debug(Notice, "Out of memory. Uhoh.");
	}else if (ce == ConnNoSupportedAuth) {
		debug(Notice, "The auth mechanisms the server offers are not supported or the server offered no auth mechanisms at all. ");
	}else if (ce == ConnTlsFailed) {
		debug(Notice, "The server's certificate could not be verified or the TLS handshake did not complete successfully. ");
	}else if (ce == ConnCompressionFailed) {
		debug(Notice, "	Negotiating/initializing compression failed. ");
	}else if (ce == ConnAuthenticationFailed) {
		debug(Notice, "	Authentication failed. Username/password wrong or account does not exist. Use ClientBase::authError() to find the reason. ");
	}else if (ce == ConnUserDisconnected) {
		debug(Notice, "The user (or higher-level protocol) requested a disconnect. ");
	}else if (ce == ConnNotConnected) {
		debug(Notice, "	There is no active connection. ");
	}


}
bool Bot::onTLSConnect(const gloox::CertInfo & info) {
	time_t from( info.date_from );
	time_t to( info.date_to );

	debug(Notice,"Server requesting tls connection");
	debug(Debug, "status: %d", info.status);
	debug(Debug, "issuer: %s", info.issuer.c_str());
	debug(Debug, "peer: %s", info.server.c_str());
	debug(Debug, "protocol: %s", info.protocol.c_str());
	debug(Debug, "mac: %s",info.mac.c_str());
	debug(Debug, "cipher: %s", info.cipher.c_str());
	debug(Debug, "compression: %s", info.compression.c_str());
	debug(Debug, "from: %s",ctime(&from));
	debug(Debug, "to : %s", ctime(&to));

	return true;
}

void Bot::handlePresence(const gloox::Presence&) {
	debug(Debug, "Someting about precense");

}

void Bot::handleMessageSession(MessageSession * session) {

	debug(Debug, "MessageSession handler called tid:%s", session->threadID().c_str());
	// this example can handle only one session. so we get rid of the old session
	j->disposeMessageSession( m_session );
	m_session = session;
	m_session->registerMessageHandler( this );
	m_messageEventFilter = new MessageEventFilter( m_session );
	m_messageEventFilter->registerMessageEventHandler( this );
	m_chatStateFilter = new ChatStateFilter( m_session );
	m_chatStateFilter->registerChatStateHandler( this );

}

void Bot::handleMessageEvent( const JID& from, MessageEventType event )
{
	printf( "received event: %d from: %s\n", event, from.full().c_str() );
}

void Bot::handleChatState( const JID& from, ChatStateType state )
{
	printf( "received state: %d from: %s\n", state, from.full().c_str() );
}




