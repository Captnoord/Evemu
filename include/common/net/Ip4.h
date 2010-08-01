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

#ifndef __NET__IP4_H__INCL__
#define __NET__IP4_H__INCL__

/**
 * @brief Contains information about IPv4.
 *
 * @author Bloody.Rabbit
 */
struct Ip4
{
    /// Typedef for socket address.
    typedef sockaddr_in SocketAddress;
    /// Typedef for address.
    typedef in_addr Address;
    /// Typedef for port.
    typedef uint16 Port;

    /// Address family.
    static const int ADDRESS_FAMILY;

    /// Socket pseudo-address "any".
    static const SocketAddress SOCKET_ADDRESS_ANY;

    /// Pseudo-address "any".
    static const Address ADDRESS_ANY;
    /// Pseudo-address "broadcast".
    static const Address ADDRESS_BROADCAST;

    /**
     * @brief Obtains a socket address.
     *
     * @param[in] address The address to use.
     * @param[in] port    The port to use.
     *
     * @return The socket address.
     */
    static SocketAddress GetSocketAddress( const Address& address, const Port& port );

    /**
     * @brief Obtains an address.
     *
     * @param[in] address The address.
     *
     * @return The address.
     */
    static Address GetAddress( uint32 address );
    /**
     * @brief Obtains an address by IP.
     *
     * @param[in] ip An IP address in dotted form.
     *
     * @return The address.
     */
    static Address GetAddressByIP( const char* ip );
    /**
     * @brief Obtains an address by hostname.
     *
     * @param[in] hostname A hostname.
     *
     * @return The address.
     */
    static Address GetAddressByHostname( const char* hostname );
    /**
     * @brief Obtains an address by socket address.
     *
     * @param[in] socketAddress The socket address.
     *
     * @return The address.
     */
    static Address GetAddressBySocketAddress( const SocketAddress& socketAddress );

    /**
     * @brief Obtains a port.
     *
     * @param[in] port A port number.
     *
     * @return The port.
     */
    static Port GetPort( uint16 port );
    /**
     * @brief Obtains a port by socket address.
     *
     * @param[in] socketAddress The socket address.
     *
     * @return The port.
     */
    static Port GetPortBySocketAddress( const SocketAddress& socketAddress );

    /**
     * @brief Prints a socket address.
     *
     * @param[in] socketAddress The socket address.
     *
     * @return The resulting string.
     */
    static std::string PrintSocketAddress( const SocketAddress& socketAddress );
    /**
     * @brief Prints an address.
     *
     * @param[in] address The address.
     *
     * @return The resulting string.
     */
    static std::string PrintAddress( const Address& address );
    /**
     * @brief Prints a port.
     *
     * @param[in] port The port.
     *
     * @return The resulting string.
     */
    static std::string PrintPort( const Port& port );
};

#endif /* !__NET__IP4_H__INCL__ */
