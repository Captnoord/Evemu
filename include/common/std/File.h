/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Bloody.Rabbit
*/

#ifndef __STD__FILE_H__INCL__
#define __STD__FILE_H__INCL__

#include "utils/Buffer.h"

namespace Std
{
    /**
     * @brief A wrapper for standard library <code>FILE</code>.
     *
     * @author Bloody.Rabbit
     */
    class File
    {
    public:
        /**
         * @brief Renames a file.
         *
         * @param[in] oldName An old name of the file.
         * @param[in] newName A new name for the file.
         *
         * @return A value returned by <code>rename</code>.
         */
        static int Rename( const char* oldName, const char* newName );
        /**
         * @brief Removes a file.
         *
         * @param[in] name A name of the file to be removed.
         *
         * @return A value returned by <code>remove</code>.
         */
        static int Remove( const char* name );

        /// A default constructor.
        File();
        /**
         * @brief A primary constructor.
         *
         * @param[in] name A name of the file to be opened.
         * @param[in] mode A mode to open the file in.
         */
        File( const char* name, const char* mode );
        /// A destructor, closes the file.
        ~File();

        /**
         * @brief Checks if the file handle is valid.
         *
         * @retval true  The handle is valid.
         * @retval false The handle is invalid.
         */
        bool isValid() const { return NULL != mFile; }
        /**
         * @brief Convenience operator, calls isValid().
         *
         * @return A value returned by isValid().
         */
        operator bool() const { return isValid(); }

        /**
         * @brief Checks if the end of the file has been reached.
         *
         * @return A value returned by <code>feof</code>.
         */
        int eof() const;
        /**
         * @brief Checks if an error occurred.
         *
         * @return A value returned by <code>ferror</code>.
         */
        int error() const;
        /**
         * @brief Obtains current position in the file.
         *
         * @return A value returned by <code>ftell</code>.
         */
        long int tell() const;

        /**
         * @brief Opens a new file.
         *
         * Any previous handle is closed.
         *
         * @param[in] name A filename of the file.
         * @param[in] mode A mode to open the file in.
         *
         * @retval true  The file has been successfully opened.
         * @retval false Failed to open the file.
         */
        bool Open( const char* name, const char* mode );
        /**
         * @brief Closes the file.
         *
         * @return A value returned by <code>fclose</code>.
         */
        int Close();
        /**
         * @brief Closes the file if necessary.
         *
         * This method does not fail if the file is already closed.
         * In other words, call this method anytime you need to
         * assure the handle is closed/invalid.
         *
         * @return 0 if already closed; a value returned by
         *         <code>fclose</code> otherwise.
         */
        int CloseEx();

        /**
         * @brief Seeks within the file.
         *
         * @param[in] offset An offset.
         * @param[in] origin An origin of the offset.
         *
         * @return A value returned by <code>fseek</code>.
         */
        int Seek( long int offset, int origin );

        /**
         * @brief Reads data from the file.
         *
         * @param[out] into Where to store the read data.
         *
         * @return A value returned by <code>fread</code>.
         */
        size_t Read( Buffer& into );
        /**
         * @brief Writes data to the file.
         *
         * @param[in] data The data to be written.
         *
         * @return A value returned by <code>fwrite</code>.
         */
        size_t Write( const Buffer& data );
        /**
         * @brief Flushes all buffers, writing all data to disk.
         *
         * @return A value returned by <code>fflush</code>.
         */
        int Flush();

    protected:
        /// The file.
        FILE* mFile;
    };
}

#endif /* !__STD__FILE_H__INCL__ */
