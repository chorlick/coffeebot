#include <gloox/client.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/presencehandler.h>
#include <pthread.h>
#include <string>
#include <vector>
using namespace gloox;
using namespace std;

class Bot : public MessageHandler, ConnectionListener, PresenceHandler{

    public  :
        enum LogLevels {
            Debug = ( 1 << 0),
            Warning = (1 << 1),
            Critical = (1 << 2),
            Notice = (1 << 3)
        };

        Bot();
        ~Bot();
        Bot(const Bot &);

        virtual void handleMessage(const Message & stanz, MessageSession* session =0);

        virtual void onConnect();
        virtual void onDisconnect(gloox::ConnectionError);
        virtual bool onTLSConnect(const gloox::CertInfo&);
        virtual void handlePresence(const gloox::Presence&);

        bool parseConfigFile();
        void debug(int debug, const char * fmt, ...);
        
        void * doRandomFactMessage(void * arg);
        void * doHelpMessage(void * arg);

    private : 
        Client * j;
        pthread_t thread;
        int connections ;
        string help;
        string motd;
        vector<string> admins;
        vector<string> facts;
};
