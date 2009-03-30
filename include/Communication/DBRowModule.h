

/** class that simulates the internals of the blue.DBRow python instance
 *
 * The main idea is ( design stage can and will change ) is that you have
 * 2 parts, 1 is static and is the factory and 1 is the row itself which
 * is a wrapper around the python objects. How this is done exactly I dono
 * yet, the future will tell if I got it correct.
 *
 * ruw: de objecten die na de ruwe data in die sequence zitten zijn waarschijnlijk de objecten die in de ruwe data missen.
 * hoe dit allemaal in elkaar zit heb ik geen idee maar het is logisch want op die manier kan je veel data batchen en toch
 * een vorm van dynamica in je database packet inbouwen.
 *
 */

typedef enum {
	DBTYPE_I1			= 0x10,
	DBTYPE_UI1			= 0x11,
	DBTYPE_BOOL			= 0x0B,
	DBTYPE_I2			= 0x02,
	DBTYPE_UI2			= 0x12,
	DBTYPE_I4			= 0x03,
	DBTYPE_UI4			= 0x13,
	DBTYPE_R4			= 0x04,
	DBTYPE_I8			= 0x14,
	DBTYPE_R8			= 0x05,
	DBTYPE_UI8			= 0x15,
	DBTYPE_CY			= 0x06, //money
	DBTYPE_FILETIME		= 0x40, //64 bits
	DBTYPE_BYTES		= 0x80,
	DBTYPE_STR			= 0x81,
	DBTYPE_WSTR			= 0x82,
	DBTYPE_VIRTUAL		= 0x00, // virtual db field..
} DBTYPE;

#define MARSHALSTREAM_RETURN(p) {PyObject * x = ((PyObject*)p); assert(x != NULL); return x;}

static size_t mReadIndex;
static uint8* mReadBuffer;
static size_t mReadBufferSize;
static size_t mVirtualFieldCount;

class DBRowModule
{
public:
	DBRowModule();
	~DBRowModule();


	/* strings and buffers seem to be stored in a itr sequence after the raw part, these are simply skipped
	  
	*/

	static PyObject* ReadRawDbField(MarshalStream& stream, PyInt &type, size_t &virtualFieldCount);

	static PyObject* parseraw(MarshalStream& stream, PyObject* header, uint8 * data, size_t len, size_t & virtualFieldCount, size_t & fieldCount);

	static size_t GetRawFieldSizeFromHeader(PyObject* tuple);

	static size_t GetFieldSizeFromType(uint8 type);

	// keeps track of the read offset in the raw buffer.
	/*static size_t mReadIndex;
	static uint8* mReadBuffer;
	static size_t mReadBufferSize;*/

private:

	// LOL DIT IS HELEMAAL VERKEERT..... database header zooi enzo...
	class DBRowField
	{
	public:
		DBRowField(PyObject* object);
		~DBRowField();
	private:
		PyObject* mObject;
	};

	int mParamCount; // guessed, something like this

	typedef std::vector<DBRowField*>	RowContainer;
	typedef RowContainer::iterator		RowContainerItr;

	RowContainer mDBRow;
};

#undef MARSHALSTREAM_RETURN