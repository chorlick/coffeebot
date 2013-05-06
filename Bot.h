#include <gloox/client.h>
#include <gloox/messagehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/presencehandler.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatefilter.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/messageeventhandler.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatehandler.h>
#include <gloox/chatstatefilter.h>
#include <gloox/connectionlistener.h>
#include <libconfig.h++>

#include <pthread.h>
#include <string>
#include <vector>
using namespace gloox;
using namespace std;
using namespace libconfig;

struct Admin {
    string username;
    string password;

};

struct AdminSession{ 
    string handle;
    bool authed;
    string username;
    string password;
    
};


class Bot : public MessageHandler, ConnectionListener, PresenceHandler, MessageSessionHandler, MessageEventHandler, ChatStateHandler {

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
        virtual void handleMessageSession(MessageSession * session);
        virtual void handleMessageEvent( const JID& from, MessageEventType event );
        virtual void handleChatState( const JID& from, ChatStateType state );
        int makeTime(struct tm, char* buffer, int size) ;

        bool parseConfigFile();
        void debug(int debug, const char * fmt, ...);
        void writeFreshTime();

    private : 
        Client * j;
        pthread_t thread;
        int connections ;
        string help;
        string motd;
        string username;
        string password;
        struct tm brew_time;
        vector<struct Admin * > admins;
        vector<string> facts;
        MessageSession  * m_session;
        MessageEventFilter * m_messageEventFilter;
        ChatStateFilter * m_chatStateFilter;
        vector<AdminSession *> auths;
        char brew_user[32];
};
