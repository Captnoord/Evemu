

// some lame buffer size stuff
#define CLIENTSOCKET_SENDBUF_SIZE 131078
#define CLIENTOCKET_RECVBUF_SIZE 16384

// burst sending related stuff
enum OUTPACKET_RESULT
{
	OUTPACKET_RESULT_SUCCESS				= 1,
	OUTPACKET_RESULT_NO_ROOM_IN_BUFFER		= 2,
	OUTPACKET_RESULT_NOT_CONNECTED			= 3,
	OUTPACKET_RESULT_SOCKET_ERROR			= 4,
};

//authorize state machine
enum EVE_AUTH_STATE
{
	EVE_AUTH_STATE_HANDSHAKE,			// version handshake
	EVE_AUTH_STATE_QUEUE_COMMAND,
	EVE_AUTH_STATE_NO_CRYPTO,			//
	EVE_AUTH_STATE_CRYPTO_CHALLENGE,
	EVE_AUTH_STATE_HANDSHAKE_SEND,
	EVE_AUTH_STATE_DONE,				// finished
};

class EveClientSocket : public Socket
{
public:

	typedef void (EveClientSocket::*stateProc)(PyRep&);

	EveClientSocket(SOCKET fd);
	~EveClientSocket();

	// wrapper functions make the code readable
	ASCENT_INLINE void _sendHandShake();
	ASCENT_INLINE void _sendQueuePos( int queuePos );
	ASCENT_INLINE void _sendAccept();

	ASCENT_INLINE void _sendRequirePasswordType(int passwordType);


	void HandleHandShake();

	// simple packet send function
	ASCENT_INLINE OUTPACKET_RESULT _outPacket(EVENetPacket * packet);

	ASCENT_INLINE void OutPacket(PyRep * packet );

	void OnRead();
	void OnConnect();
	void OnDisconnect();

	bool Authed;

	//void (EveClientSocket::*currentStateMachine)(PyRep&);

	stateProc currentStateMachine;

	// the authorization state machine
	void _authStateHandshake(PyRep& packet);
	void _authStateQueueCommand(PyRep& packet);
	void _authStateNoCrypto(PyRep& packet);
	void _authStateCryptoChallenge(PyRep& packet);
	void _authStateHandshakeSend(PyRep& packet);
	void _authStateDone(PyRep& packet);

protected:

private:

	uint32 mRemaining;
	CryptoChallengePacket *mRequest;

	/*uint32 mOpcode;
	
	uint32 mSize;
	uint32 mSeed;
	uint32 mClientSeed;
	uint32 mClientBuild;
	uint32 mRequestID;*/

	//WorldSession *mSession;
	//WorldPacket * pAuthenticationPacket;
	//FastQueue<WorldPacket*, DummyLock> _queue;
	Mutex queueLock;

//	uint32 _latency;
	bool m_nagleEanbled;
//	string * m_fullAccountName;
};