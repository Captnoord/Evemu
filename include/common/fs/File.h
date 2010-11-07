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

#ifndef __FS__FILE_H__INCL__
#define __FS__FILE_H__INCL__

#include "util/Stream.h"

#ifdef WIN32
#   include "win/File.h"
#else /* !WIN32 */
#   include "posix/File.h"
#endif /* !WIN32 */

namespace Fs
{
    /**
     * @brief A file.
     *
     * @author Bloody.Rabbit
     */
    class File
    : public Util::InputStream< void >,
      public Util::OutputStream< void >
    {
    public:
        /// Typedef for a stream element due to reference ambiguity.
        typedef Util::Stream< void >::Element Element;
        /// Typedef for a stream error due to reference ambiguity.
        typedef Util::Stream< void >::Error   Error;

        /// A file mode.
        enum Mode
        {
            /// Read access to the file.
            MODE_ACCESS_READ = 0x01,
            /// Write access to the file.
            MODE_ACCESS_WRITE = 0x02,
            /// Default access flags.
            MODE_ACCESS_DEFAULT = MODE_ACCESS_READ | MODE_ACCESS_WRITE,

            /// Creation of new file allowed?
            MODE_OPEN_NEW = 0x10,
            /// Opening an existing file allowed?
            MODE_OPEN_EXISTING = 0x20,
            /// Truncate the file on open?
            MODE_OPEN_TRUNCATE = 0x40 | MODE_OPEN_EXISTING | MODE_ACCESS_WRITE,
            /// Default open flags.
            MODE_OPEN_DEFAULT = MODE_OPEN_NEW | MODE_OPEN_EXISTING,

            /// A default mode.
            MODE_DEFAULT = MODE_ACCESS_DEFAULT | MODE_OPEN_DEFAULT
        };
        /// A seek origin.
        enum Origin
        {
            ORIGIN_BEGIN,   ///< Origin at the beginning of the file.
            ORIGIN_CURRENT, ///< Origin at the current position.
            ORIGIN_END,     ///< Origin at the end of the file.

            ORIGIN_COUNT    ///< Count of origins.
        };

#   ifndef WIN32
        /// Access mode used for all new files.
        static const mode_t MODE;
#   endif /* !WIN32 */

        /**
         * @brief Renames a file.
         *
         * @param[in] nameOld The old name.
         * @param[in] nameNew The new name.
         *
         * @retval true  Rename succeeded.
         * @retval false Rename failed.
         */
        static bool Rename( const char* nameOld, const char* nameNew );
        /**
         * @brief Removes a file.
         *
         * @param[in] name A name of the file.
         *
         * @retval true  Removal succeeded.
         * @retval false Removal failed.
         */
        static bool Remove( const char* name );

        /**
         * @brief A default constructor.
         */
        File();
        /**
         * @brief A primary constructor.
         *
         * @param[in] name A name of the file.
         * @param[in] mode A mode to open it in.
         */
        File( const char* name, Mode mode );

        /**
         * @brief Checks if the file is valid.
         *
         * @retval true  The file is valid.
         * @retval false The file is invalid.
         */
        bool isValid() const;
        /**
         * @brief Convenient conversion to boolean.
         *
         * @return A value returned by isValid.
         */
        operator bool() const { return isValid(); }

        /**
         * @brief Obtains the file size.
         *
         * @param[out] size Where to store the size.
         *
         * @retval true  Operation succeeded.
         * @retval false Operation failed.
         */
        bool GetSize( size_t& size ) const;

        /**
         * @brief Opens a file.
         *
         * @param[in] name A name of the file.
         * @param[in] mode A mode to open it in.
         *
         * @retval true  Open successfull.
         * @retval false Failed to open.
         */
        bool Open( const char* name, Mode mode = MODE_DEFAULT );
        /**
         * @brief Closes the file.
         *
         * @retval true  Closed successfully.
         * @retval false Failed to close.
         */
        bool Close();

        /**
         * @brief Seeks within the file.
         *
         * @param[in] offset The offset to set.
         * @param[in] origin The origin of the seek.
         *
         * @retval true  Seek succeeded.
         * @retval false Seek failed.
         */
        bool Seek( long int offset, Origin origin = ORIGIN_BEGIN );
        /**
         * @brief Flushes file buffers to disk.
         *
         * @retval true  Flush succeeded.
         * @retval false Flush failed.
         */
        bool Flush();

        /**
         * @brief Reads data from the file.
         *
         * @param[out] data      Where to store the data.
         * @param[out] bytesRead Where to store a number
         *                       of read bytes.
         *
         * @return An error code.
         */
        Error Read( Util::Data& data, size_t* bytesRead = NULL );
        /**
         * @brief Writes data to the file.
         *
         * @param[in]  data         The data to write.
         * @param[out] bytesWritten Where to store a number
         *                          of written bytes.
         *
         * @return An error code.
         */
        Error Write( const Util::Data& data, size_t* bytesWritten = NULL );

    protected:
#   ifdef WIN32
        /// A Windows' file handle.
        Win::File mFile;

        /// A seek origin translation table.
        static const DWORD WIN_ORIGIN[ ORIGIN_COUNT ];
#   else /* !WIN32 */
        /// A POSIX file descriptor.
        Posix::File mFile;

        /// A seek origin translation table.
        static const int POSIX_ORIGIN[ ORIGIN_COUNT ];
#   endif /* !WIN32 */
    };
}

#endif /* !__FS__FILE_H__INCL__ */
