#ifndef EvilNumber_h__
#define EvilNumber_h__

// this file should have all stuff regarding damage type attribute caching..
enum EVIL_NUMBER_TYPE
{
    evil_number_nan,
    evil_number_int,
    evil_number_float,
};

#pragma pack(push,1)
class PyRep;

/**
* @brief this is a class that kinda mimics how python polymorph's numbers.
*
* @author Captnoord.
* @date Juni 2010
*/
class EvilNumber
{
public:
    /*typedef union _GenVal{
    float fVal;
    int32 iVal;
    } GenVal;*/

    typedef union _GenVal{
        double fVal;
        int64 iVal;
    } GenVal;

    EvilNumber();
    EvilNumber(float val);
    EvilNumber(int val);
    EvilNumber(double val);
    EvilNumber(int64 val);
    EvilNumber(uint64 val);

    EvilNumber operator*(const EvilNumber& val)
    {
        Multiply((EvilNumber &)val);
        return *this;
    }

    EvilNumber operator/(const EvilNumber& val)
    {
        Divide((EvilNumber &)val);
        return *this;
    }

    EvilNumber operator+(const EvilNumber& val)
    {
        Add((EvilNumber &)val);
        return *this;
    }

    EvilNumber operator+=(const EvilNumber& val)
    {
        Add((EvilNumber &)val);
        return *this;
    }

    EvilNumber operator-(const EvilNumber& val)
    {
        Sub((EvilNumber &)val);
        return *this;
    }

    bool operator==(const EvilNumber& val)
    {
        if (this->mType != val.mType)
            return false;

        // important to know is that 'if' they are both floats which would be
        // exactly the same value the raw data would be exactly the same.
        // so this check is valid for both floats and ints.
        //
        // @note I wonder... can I create 2 floats that would be equal when
        // doing normal float compare and not equal when doing integer compare.
        if (this->mValue.iVal == val.mValue.iVal)
            return true;
        else
            return false;
    }

    /* current code is wrong... it checks if type is the same and if int value is the same... but thats crap because int(1) != float(1) */
    bool operator!=(const EvilNumber& val)
    {
        if (this->mType != val.mType)
            return true;

        // see comments from '==' operator.
        if (this->mValue.iVal != val.mValue.iVal)
            return true;
        else
            return false;
    }

    bool operator!=(const uint32 val)
    {
        if (this->mType == evil_number_int)
            return val != this->mValue.iVal;
        else
            return float(val) != this->mValue.fVal;
    }

    bool operator<(const uint8 val)
    {
        if (this->mType == evil_number_int)
            return this->mValue.iVal < val;
        else
            return this->mValue.fVal < float(val);
    }

    bool operator>(const uint32 val)
    {
        if (this->mType == evil_number_int)
            return this->mValue.iVal > val;
        else
            return this->mValue.fVal > float(val);
    }

    bool operator>(const EvilNumber& val)
    {
        if (this->mType == evil_number_int && val.mType == evil_number_int)
            return this->mValue.iVal > val.mValue.iVal;
        else if (this->mType == evil_number_float && val.mType == evil_number_float)
            return this->mValue.fVal > val.mValue.fVal;
        else if (this->mType == evil_number_float)
            return this->mValue.fVal > float(val.mValue.iVal);
        else
            return float(this->mValue.iVal) > val.mValue.fVal;
    }

    bool operator<=(const EvilNumber& val)
    {
        if (this->mType == evil_number_int && val.mType == evil_number_int)
            return this->mValue.iVal <= val.mValue.iVal;
        else if (this->mType == evil_number_float && val.mType == evil_number_float)
            return this->mValue.fVal <= val.mValue.fVal;
        else if (this->mType == evil_number_float)
            return this->mValue.fVal <= float(val.mValue.iVal);
        else
            return float(this->mValue.iVal) <= val.mValue.fVal;
    }

    

    /**
    * @brief converts the EvilNumber value into a string
    *
    * @return the text representative of the value.
    */
    std::string to_str()
    {
        char buff[32]; // max uint32 will result in a 10 char string, a float will result in a ? char string.
        if (mType == evil_number_int)
            sprintf(buff, "%d", mValue.iVal);
        else if (mType == evil_number_float)
            sprintf(buff, "%f", mValue.fVal);
        else
            assert(false); // bleh crash..
        return buff;
    }

    /**
    * @brief converts the EvilNumber into a python object.
    *
    * @return the python object of the EvilNumber.
    */
    PyRep* GetPyObject();

    // old system support
    EVIL_NUMBER_TYPE get_type()
    {
        return mType;
    }

    int64 get_int()
    {
        assert(mType == evil_number_int);
        return mValue.iVal;
    }

    double get_float()
    {
        assert(mType == evil_number_float);
        return mValue.fVal;
    }
    // end of old system support

private:
    GenVal mValue;
    EVIL_NUMBER_TYPE mType;

protected:
    /**
    * @brief check if its possible a integer and do the conversion
    *
    * checking every calculation for float/int conversion can be a drain on
    * performance. But as integer math is fast then floating point math.
    */
    void CheckIntegrety();

    /**
    * @brief multiply this with @a
    *
    * Multiply this with @a.
    *
    * @param[in] val the value we are multiplying with.
    */
    void Multiply(EvilNumber & val);

    /**
    * @brief divide this with @a
    *
    * Divide this with @a.
    *
    * @param[in] val the value we are dividing by.
    * @todo handle "int(2) / int(4) = float(0.5)" in a fast enough way
    */
    void Divide(EvilNumber & val);

    // todo double check this..
    void Add(EvilNumber & val);

    // todo double check this..
    void Sub(EvilNumber & val);
};

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////
// global operators
EvilNumber operator*(const EvilNumber& val, const EvilNumber& val2);

extern const EvilNumber EvilTime_Second;
extern const EvilNumber EvilTime_Minute;
extern const EvilNumber EvilTime_Hour;
extern const EvilNumber EvilTime_Day;
extern const EvilNumber EvilTime_Month;
extern const EvilNumber EvilTime_Year;


#endif // EvilNumber_h__
