#ifndef _PYOBJECTDUMPER_H
#define _PYOBJECTDUMPER_H

void Dump(FILE * fp, PyObject * obj, size_t deep, bool isItr = false);
void DumpObject(FILE * fd, PyObject* obj);

/** checks if a string contains printable characters
 */
inline bool IsPrintString(const char* str, const size_t len);
inline bool IsPrintString(PyString * obj);
inline void WriteSpacer(FILE * fp, size_t count);

static PyList & PyListRef_PyChameleon(PyChameleon & obj)
{
	PyObject * ret_obj = obj.getPyObject();
	if (ret_obj->gettype() != PyTypeList)
		assert(false);
	return *((PyList*)ret_obj);
}

static void ClassDumper_util_row(FILE * fp, size_t /*deep*/, PyDict* obj);
static void ClassDumper_util_rowset(FILE * fp, size_t deep, PyDict* obj);

#endif //_PYOBJECTDUMPER_H