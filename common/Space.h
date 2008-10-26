

// keep track of the space info stuff related to this server blaa blaa
// for the moment keep only track of the amount of authorized connections
class SERVER_DECL Space : public Singleton<Space>
{
public:
	Space();
	size_t mAcceptedConnections;
	size_t mAuthorizedConnections;
};

#define sSpace Space::getSingleton()