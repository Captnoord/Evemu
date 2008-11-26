/*  EVEmu: EVE Online Server Emulator

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY except by those people which sell it, which
  are required to give you total support for your newly bought product;
  without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
	
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef EVE_PY_REP_H
#define EVE_PY_REP_H

#include "common.h"
#include "logsys.h"

class PyVisitor;
class PyRepSubStream;

/** Static lookup table for PyRep type object type names.
*/
static const char *PyRepTypeString[] = {
	"Integer",			//0
	"Real",				//1
	"Boolean",			//2
	"Buffer",			//3
	"String",			//4
	"Tuple",			//5
	"List",				//6
	"Dict",				//7
	"None",				//8
	"SubStruct",		//9
	"SubStream",		//10
	"ChecksumedStream",	//11
	"Object",			//12
	"NewObject",		//13
	"PackedRow",		//14
	"UNKNOWN TYPE",		//15
};

/** PyRep base Python wire object
  */
class PyRep {
public:

	/** PyRep Python wire object types
	  */
	typedef enum {
		PyTypeInteger = 0,
		PyTypeReal,
		PyTypeBoolean,
		PyTypeBuffer,
		PyTypeString,
		PyTypeTuple,
		PyTypeList,
		PyTypeDict,
		PyTypeNone,
		PyTypeSubStruct,
		PyTypeSubStream,
		PyTypeChecksumedStream,
		PyTypeObject,
		PyTypeNewObject,
		PyTypePackedRow
	} Type;
	
	PyRep(Type t) : m_type(t) { }
	virtual ~PyRep() {}

	/** 
	  */
	ASCENT_INLINE bool IsInteger() {return m_type == PyTypeInteger;}
	ASCENT_INLINE bool IsReal() {return m_type == PyTypeReal;}
	ASCENT_INLINE bool IsBool() {return m_type == PyTypeBoolean;}
	ASCENT_INLINE bool IsBuffer() {return m_type == PyTypeBuffer;}
	ASCENT_INLINE bool IsString() {return m_type == PyTypeString;}
	ASCENT_INLINE bool IsTuple() {return m_type == PyTypeTuple;}
	ASCENT_INLINE bool IsList() {return m_type == PyTypeList;}
	ASCENT_INLINE bool IsDict() {return m_type == PyTypeDict;}
	ASCENT_INLINE bool IsNone() {return m_type == PyTypeNone;}
	ASCENT_INLINE bool IsSubStruct() {return m_type == PyTypeSubStruct;}
	ASCENT_INLINE bool IsSubStream() {return m_type == PyTypeSubStream;}
	ASCENT_INLINE bool IsChecksumedStream() {return m_type == PyTypeChecksumedStream;}
	ASCENT_INLINE bool IsObject() {return m_type == PyTypeObject;}
	ASCENT_INLINE bool IsNewObject() {return m_type == PyTypeNewObject;}
	ASCENT_INLINE bool IsPackedRow() {return m_type == PyTypePackedRow;}

	ASCENT_INLINE bool CheckType(Type t) const { return(m_type == t); }
	ASCENT_INLINE bool CheckType(Type t, Type t2) const { return(m_type == t || m_type == t2); }
	ASCENT_INLINE const char *TypeString() const;
	
	virtual void Dump(FILE *into, const char *pfx) const = 0;
	virtual void Dump(LogType type, const char *pfx) const = 0;
	virtual PyRep *Clone() const = 0;
	virtual void visit(PyVisitor *v) const = 0;

	//using this method is discouraged, it generally means your doing something wrong... CheckType() should cover almost all needs
	Type youreallyshouldentbeusingthis_GetType() const { return(m_type); }
protected:
	const Type m_type;
};

//storing all integers (and booleans) as uint64s is a lot of craziness right now
//but its better than a ton of virtual functions to achieve the same thing.
class PyRepInteger : public PyRep {
public:
	PyRepInteger(uint64 i) : PyRep(PyRep::PyTypeInteger), value(i) {}
	virtual ~PyRepInteger() {}
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepInteger *TypedClone() const;	
	uint64 value;
};

class PyRepReal : public PyRep {
public:
	PyRepReal(double i) : PyRep(PyRep::PyTypeReal), value(i) {}
	virtual ~PyRepReal() {}
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepReal *TypedClone() const;
	
	double value;
};

class PyRepBoolean : public PyRep {
public:
	PyRepBoolean(bool i) : PyRep(PyRep::PyTypeBoolean), value(i) {}
	virtual ~PyRepBoolean() {}
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepBoolean *TypedClone() const;
	
	bool value;
};

class PyRepNone : public PyRep {
public:
	PyRepNone() : PyRep(PyRep::PyTypeNone) {}
	virtual ~PyRepNone() {}
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepNone *TypedClone() const;
};


class PyRepBuffer : public PyRep {
public:
	PyRepBuffer(const byte *buffer, uint32 length);
	PyRepBuffer(byte **buffer, uint32 length);
	PyRepBuffer(uint32 length);

	PyRepBuffer(std::string &buffer);

	virtual ~PyRepBuffer();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return TypedClone(); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepBuffer *TypedClone() const;
	
	//treat it as a buffer:
	byte *GetBuffer() const { return m_value; }
	uint32 GetLength() const { return m_length; }

	PyRepSubStream *CreateSubStream() const;
	
protected:
	byte *const m_value;
	const uint32 m_length;
};

class EVEStringTable;
class PyRepString : public PyRep {
public:
	PyRepString(const char *str = "", bool type_1=false) : PyRep(PyRep::PyTypeString), value(str), is_type_1(type_1) {}
	PyRepString(const std::string &str, bool type_1=false) : PyRep(PyRep::PyTypeString), is_type_1(type_1) { value.assign(str.c_str(), str.length()); } //to deal with non-string buffers poorly placed in strings (CCP)
	PyRepString(const byte *data, uint32 len, bool type_1=false) : PyRep(PyRep::PyTypeString), value((const char *) data, len), is_type_1(type_1) {}
	virtual ~PyRepString() {}
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepString *TypedClone() const;

	std::string value;
	bool is_type_1;	//true if this is an Op_PyByteString instead of the default Op_PyByteString2


//string table stuff:
	static bool LoadStringFile(const char *file);
	static EVEStringTable *GetStringTable();	//always returns a valid pointer

protected:
	static EVEStringTable *s_stringTable;
};

class PyRepTuple : public PyRep {
public:
	typedef std::vector<PyRep *> storage_type;
	typedef std::vector<PyRep *>::iterator iterator;
	typedef std::vector<PyRep *>::const_iterator const_iterator;
	
	PyRepTuple(uint32 item_count) : PyRep(PyRep::PyTypeTuple), items(item_count, NULL) {}
	virtual ~PyRepTuple();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;

	void CloneFrom(const PyRepTuple *from);
	PyRepTuple *TypedClone() const;
	
	storage_type items;
	
	iterator begin() { return(items.begin()); }
	iterator end() { return(items.end()); }
	const_iterator begin() const { return(items.begin()); }
	const_iterator end() const { return(items.end()); }
	bool empty() const { return(items.empty()); }
	void clear();
};

class PyRepList : public PyRep {
public:
	typedef std::vector<PyRep *> storage_type;
	typedef std::vector<PyRep *>::iterator iterator;
	typedef std::vector<PyRep *>::const_iterator const_iterator;
	
	PyRepList() : PyRep(PyRep::PyTypeList) {}
	virtual ~PyRepList();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	void CloneFrom(const PyRepList *from);
	PyRepList *TypedClone() const;
	
	void addInt(uint64 intval);
	void addReal(double realval);
	void addStr(const char *str);
	void add(const char *str);
	void add(PyRep *i);
	
	storage_type items;
	
	iterator begin() { return(items.begin()); }
	iterator end() { return(items.end()); }
	const_iterator begin() const { return(items.begin()); }
	const_iterator end() const { return(items.end()); }
	bool empty() const { return(items.empty()); }
	void clear();
};

class PyRepDict : public PyRep {
public:
	typedef std::map<PyRep *, PyRep *> storage_type;
	typedef std::map<PyRep *, PyRep *>::iterator iterator;
	typedef std::map<PyRep *, PyRep *>::const_iterator const_iterator;
	
	PyRepDict() : PyRep(PyRep::PyTypeDict) {}
	virtual ~PyRepDict();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	void CloneFrom(const PyRepDict *from);
	PyRepDict *TypedClone() const;

	void add(const char *key, PyRep *value);
	void add(PyRep *key, PyRep *value);
	void add(const char *key, const char *value);
	
	//BE CAREFUL, this map does NOT facilitate overwritting an existing
	//key, because there is no key comparison done (ptr compare only)!
	storage_type items;
	
	iterator begin() { return(items.begin()); }
	iterator end() { return(items.end()); }
	const_iterator begin() const { return(items.begin()); }
	const_iterator end() const { return(items.end()); }
	bool empty() const { return(items.empty()); }
	void clear();
};

class PyRepObject : public PyRep {
public:
	PyRepObject(const std::string &t = "", PyRep *a = NULL) : PyRep(PyRep::PyTypeObject), type(t), arguments(a) {}
	virtual ~PyRepObject();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepObject *TypedClone() const;
	
	std::string type;
	PyRep *arguments;	//should be a tuple or a dict
};


//opcodes 0x22 and 0x23 (merged for simplicity)
class PyRepNewObject : public PyRep {
public:
	//for list data (before first PackedTerminator)
	typedef std::vector<PyRep *> list_type;
	typedef std::vector<PyRep *>::iterator list_iterator;
	typedef std::vector<PyRep *>::const_iterator const_list_iterator;

	//for dict data (after first PackedTerminator)
	typedef std::map<PyRep *, PyRep *> dict_type;
	typedef std::map<PyRep *, PyRep *>::iterator dict_iterator;
	typedef std::map<PyRep *, PyRep *>::const_iterator const_dict_iterator;

	PyRepNewObject(bool _is_type_1, PyRep *_header = NULL) : PyRep(PyRep::PyTypeNewObject), header(_header), is_type_1(_is_type_1) {}
	virtual ~PyRepNewObject();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;

	PyRepNewObject *TypedClone() const;
	void CloneFrom(const PyRepNewObject *from);

	PyRep *header;
	const bool is_type_1;	// true if opcode is 0x26 instead of 0x25

	list_type list_data;
	dict_type dict_data;

};

class PyRepPackedRow : public PyRep {
public:
	typedef std::vector<byte> buffer;
	typedef std::vector<PyRep *> rep_list;

	PyRepPackedRow(const PyRep *header, bool own_header, const byte *data = NULL, const uint32 len = 0);
	virtual ~PyRepPackedRow();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepPackedRow *TypedClone() const;
	void CloneFrom(const PyRepPackedRow *from);

	//integers
	void PushInt8(const int8 v) { Push(&v, sizeof(int8)); }
	void PushUInt8(const uint8 v) { Push(&v, sizeof(uint8)); }
	void PushInt16(const int16 v) { Push(&v, sizeof(int16)); }
	void PushUInt16(const uint16 v) { Push(&v, sizeof(uint16)); }
	void PushInt32(const int32 v) { Push(&v, sizeof(int32)); }
	void PushUInt32(const uint32 v) { Push(&v, sizeof(uint32)); }
	void PushInt64(const int64 v) { Push(&v, sizeof(int64)); }
	void PushUInt64(const uint64 v) { Push(&v, sizeof(uint64)); }

	//floating point
	void PushFloat(const float v) { Push(&v, sizeof(float)); }
	void PushDouble(const double v) { Push(&v, sizeof(double)); }

	//raw
	void Push(const void *data, uint32 len);

	//PyRep
	void PushPyRep(PyRep *const v) { m_reps.push_back(v); }

	//header access
	const PyRep *GetHeader() const { return(m_header); }
	bool OwnsHeader() const { return(m_ownsHeader); }

	//buffer access
	const byte *GetBuffer() const { return(m_buffer.empty() ? NULL : &m_buffer[0]); }
	uint32 GetBufferSize() const { return(uint32(m_buffer.size())); }

	//reps access
	rep_list::iterator begin() { return(m_reps.begin()); }
	rep_list::iterator end() { return(m_reps.end()); }
	rep_list::const_iterator begin() const { return(m_reps.begin()); }
	rep_list::const_iterator end() const { return(m_reps.end()); }

protected:
	buffer m_buffer;
	rep_list m_reps;

	const bool m_ownsHeader;
	const PyRep *const m_header;
};

class PyRepSubStruct : public PyRep {
public:
	PyRepSubStruct() : PyRep(PyRep::PyTypeSubStruct), sub(NULL) {}
	PyRepSubStruct(PyRep *t) : PyRep(PyRep::PyTypeSubStruct), sub(t) {}
	virtual ~PyRepSubStruct();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepSubStruct *TypedClone() const;
	
	PyRep *sub;
};

class PyRepSubStream : public PyRep {
public:
	PyRepSubStream() : PyRep(PyRep::PyTypeSubStream), length(0), data(NULL), decoded(NULL) {}
	PyRepSubStream(PyRep *t) : PyRep(PyRep::PyTypeSubStream), length(0), data(NULL), decoded(t) {}
	PyRepSubStream(const byte *buffer, uint32 len);
	
	virtual ~PyRepSubStream();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepSubStream *TypedClone() const;

	//call to ensure that `data` represents `decoded` IF DATA IS NULL
	void EncodeData();
	
	//call to ensure that `decoded` represents `data`
	void DecodeData() const;
	
	uint32 length;
	byte *data;

	//set this and make data NULL to make somebody else marshal it
	//if both are non-NULL, they are considered to be equivalent
	mutable PyRep *decoded;
};

class PyRepChecksumedStream : public PyRep {
public:
	PyRepChecksumedStream() : PyRep(PyRep::PyTypeChecksumedStream), checksum(0), stream(NULL) {}
	PyRepChecksumedStream(uint32 sum, PyRep *t) : PyRep(PyRep::PyTypeChecksumedStream), checksum(sum), stream(t) {}
	virtual ~PyRepChecksumedStream();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const { return(TypedClone()); }
	ASCENT_INLINE void visit(PyVisitor *v) const;
	
	PyRepChecksumedStream *TypedClone() const;

	uint32 checksum;
	PyRep *stream;
};

//this object is rather hackish, and is used to allow the
//application layer to communicate with the presentation layer
//about the usage of PyRepSaveMask/Op_PySavedStreamElement
//and is mainly used to support the layer-violating blue.DBRowDescriptor
//construct
/*class PyRepReference : public PyRep {
public:
	PyRepReference(PyRep *to);
	virtual ~PyRepReference();
	ASCENT_INLINE void Dump(FILE *into, const char *pfx) const;
	ASCENT_INLINE void Dump(LogType type, const char *pfx) const;
	ASCENT_INLINE PyRep *Clone() const;
	ASCENT_INLINE void visit(PyVisitor *v) const;

	PyRep *const to;
};*/

//this probably belongs in its own file
class EVEStringTable {
public:
	static const uint8 None = 0;	//returned for no match

	bool LoadFile(const char *file);
	
	uint8 LookupString(const std::string &str) const;
	const char *LookupIndex(uint8 index) const;

protected:
	//there are better implementations of this, but this works:
	std::vector<std::string> m_forwardLookup;
	std::map<std::string, uint8> m_reverseLookup;
};

#endif//EVE_PY_REP_H