#ifndef PYSTRING_H
#define PYSTRING_H

#pragma pack(push,1)

class PyString : public PyObject
{
public:
	
	uint32 hash();

public:
	explicit PyString();
	explicit PyString(const char* str);
	explicit PyString(const char* str, size_t len);
	explicit PyString(std::string& str);
	~PyString();
	bool set(const char* str, size_t len);
	const char* content();
	const size_t length();

	/**
	 * \brief appends a string to the end of the PyString
	 *
	 * @param[in] str the std string that should be appended to the PyString.
	 * @return this function should return the "this" object.
	 */
	PyString& append ( const std::string& str );

	/**
	 * \brief appends a string to the end of the PyString
	 *
	 * @param[in] str the std string that should be appended to the PyString.
	 * @param[in] pos where the string should be appended.
	 * @param[in] n how much of the string should be appended.
	 * @return this function should return the "this" object.
	 */
	PyString& append ( const std::string& str, size_t pos, size_t n );

	bool resize(size_t size);

	bool operator==(const char *str1);
	PyString &operator=(const char *str);
	char &operator[](const int index);
private:

	char* mStr;
	size_t mStrLen;
	uint32 mHashValue;
};

#pragma pack(pop)

/**
 * wrapper function to create a PyString from a buffer
 *
 * @param[in] str the const char buffer that is fed into the constructor of the object.
 * @param[in] len the length of the buffer.
 * @return this function should return the created PyString and NULL if it failed.
 */
PyString* PyString_FromStringAndSize(const char* str, size_t len);

#endif // PYSTRING_H