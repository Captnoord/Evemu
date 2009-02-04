/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#if !defined(FIELD_H)
#define FIELD_H

class Field
{
public:
	ASCENT_INLINE void SetValue(char* value) { mValue = value; }

	ASCENT_INLINE const char *GetString() { return mValue; }
	ASCENT_INLINE double GetDouble() { return mValue ? static_cast<double>(atof(mValue)) : 0; }
	ASCENT_INLINE float GetFloat() { return mValue ? static_cast<float>(atof(mValue)) : 0; }
	ASCENT_INLINE bool GetBool() { return mValue ? atoi(mValue) > 0 : false; }
	ASCENT_INLINE uint8 GetUInt8() { return mValue ? static_cast<uint8>(atol(mValue)) : 0; }
	ASCENT_INLINE int8 GetInt8() { return mValue ? static_cast<int8>(atol(mValue)) : 0; }
	ASCENT_INLINE uint16 GetUInt16() { return mValue ? static_cast<uint16>(atol(mValue)) : 0; }
	ASCENT_INLINE int16 GetInt16() { return mValue ? static_cast<int16>(atol(mValue)) : 0; }
	ASCENT_INLINE uint32 GetUInt32() { return mValue ? static_cast<uint32>(atol(mValue)) : 0; }
	ASCENT_INLINE int32 GetInt32() { return mValue ? static_cast<int32>(atol(mValue)) : 0; }
	uint64 GetUInt64()
	{
		if(mValue == NULL)
			return 0;
		
		uint64 value;
		#ifndef WIN32	// Make GCC happy.
		  sscanf(mValue, I64FMTD, (long long unsigned int*)&value);
		#else
		  sscanf(mValue, I64FMTD, &value);
		#endif
		return value;
	}

	int64 GetInt64() 
	{
		if(mValue == NULL)
			return 0;

		int64 value;
#ifndef WIN32	// Make GCC happy.
		sscanf(mValue, SI64FMTD, (long long int*)&value);
#else
		sscanf(mValue, SI64FMTD, &value);
#endif
		return value;
	}

private:
	char *mValue;
};

/**
 * \class FieldExtendedInfo
 *
 * @brief the FieldExtendedInfo class is a utility class to store additional info about a field.
 *
 * This class is intended as a utility class for the database core, it should he used to store
 * info about fetched fields.
 *
 * @author Captnoord
 * @date January 2009
 */
class FieldExtendedInfo
{
public:
	/**
	 * @brief SetExtendedInfo is used to store the data in class.
	 *
	 * 
	 *
	 * @param[in] fieldName the field name as its fetched from the database query result.
	 * @param[in] fieldType the field type as its fetched from the database query result.
	 */
	void SetExtendedInfo(char* fieldName, const int fieldType, uint32 fieldFlags) { mFieldName = fieldName; mFieldType = fieldType; mFieldFlags = fieldFlags; }

	/**
	 * @brief GetName does what says it gets the fields name.
	 *
	 * 
	 *
	 * @return a const char pointer of the string.
	 * @note this info is lost when the query result is deleted.
	 */
	const char *GetName() const { return mFieldName; }

	/**
	 * @brief GetType does what the name tells you it gets the type of the field.
	 *
	 * 
	 *
	 * @return a int that represents the field type
	 */
	const int	GetType() const { return mFieldType; }

	uint32 GetFlags() const { return mFieldFlags; }

private:
	char*	mFieldName;
	int		mFieldType;
	uint32	mFieldFlags; // generic variable to store back end spec flags.
};

#endif//FIELD_H
