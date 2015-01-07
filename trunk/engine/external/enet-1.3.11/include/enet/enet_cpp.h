/** 
 @file  enet_cpp.h
 @brief ENet public header file for wrapper cpp-classes (for convenience)
*/
#ifndef __ENET_ENET_CPP_H__
#define __ENET_ENET_CPP_H__

#include <enet/enet.h>
#include <Object.h>
#include <vector>
#include <string>
#include <exception>
#include <Ref.h>

namespace ENet
{

class ENetException
	: public std::exception
{
public:
	ENetException( const char* const strMessage )
		: data( new char[strlen(strMessage)+1] )
	{
#ifdef _WIN32
		strcpy_s(data,strlen(strMessage),strMessage);
#else
		strcpy(data,strMessage);
#endif
	}

	virtual const char* what() const throw()
	{
		return data;
	}
private:
	char* const data;
};

class ENetHost;



/**
 * An ENet peer which data packets may be sent or received from. 
 *
 * No fields should be modified unless otherwise specified. 
 */
class ENetPeer : public yam2d::Object
{ 
public:
	ENetPeer(::ENetPeer* p)
	: Object()
	, m_p(p)
	{
	}
	
	virtual ~ENetPeer()
	{
	}
	
int send (enet_uint8 n, ENetPacket * p)
	{
		return enet_peer_send(m_p, n, (::ENetPacket*)p);
	}

	void disconnect(enet_uint32 n = 0)
	{
		enet_peer_disconnect(m_p, n);
	}

	void disconnectNow (enet_uint32 n = 0)
	{
		enet_peer_disconnect_now(m_p, n);
	}
	
	void disconnectLater (enet_uint32 n = 0)
	{
		enet_peer_disconnect_later(m_p, n);
	}

	void reset()
	{
		enet_peer_reset (m_p);
	}

private:
	::ENetPeer* m_p;
};


/** An ENet host for communicating with peers.
  *
  * No fields should be modified unless otherwise stated.

    @sa enet_host_create()
    @sa enet_host_destroy()
    @sa enet_host_connect()
    @sa enet_host_service()
    @sa enet_host_flush()
    @sa enet_host_broadcast()
    @sa enet_host_compress()
    @sa enet_host_compress_with_range_coder()
    @sa enet_host_channel_limit()
    @sa enet_host_bandwidth_limit()
    @sa enet_host_bandwidth_throttle()
  */
class ENetHost : public yam2d::Object
{
public:
	ENetHost(const std::string& hostIP, enet_uint32 hostPort, size_t maxPeerCount, size_t channelLimit = 2, enet_uint32 incomingBandwidth = 0, enet_uint32 outgoingBandwidth = 0)
	: Object()
	, m_p(0)
	, m_channelLimit(channelLimit)
	{
	
		if( hostIP.length() == 0 )
		{
			m_p = enet_host_create(ENET_HOST_ANY, maxPeerCount, m_channelLimit, incomingBandwidth, outgoingBandwidth);
		}
		else
		{
			ENetAddress address;
			address.port = hostPort;
			enet_address_set_host(&address, hostIP.c_str() );
			m_p = enet_host_create(&address, maxPeerCount, m_channelLimit, incomingBandwidth, outgoingBandwidth);
		}
		
		if( m_p == 0 )
			throw ENetException("Failed to create server host");
	};

	ENetHost( size_t channelLimit = 2, enet_uint32 incomingBandwidth = 57600/8, enet_uint32 outgoingBandwidth = 14400/8 )
	: Object()
	, m_p(0)
	, m_channelLimit(channelLimit)
	{
		m_p = enet_host_create(NULL, 1, m_channelLimit, incomingBandwidth, outgoingBandwidth);

		if( m_p == 0 )
			throw ENetException("Failed to create client host");
	};

	virtual ~ENetHost()
	{
		enet_host_destroy(m_p);
	}

	yam2d::Ref<ENetPeer> connect(const std::string& hostIP, enet_uint32 hostPort, enet_uint32 data = 0)
	{	
		ENetAddress address;
		address.port = hostPort;
		enet_address_set_host(&address, hostIP.c_str() );
		return new ENetPeer( enet_host_connect(m_p,&address,m_channelLimit,data) );
	}

	int  service( ENetEvent * event, enet_uint32 timeout)
	{
		return enet_host_service(m_p,event,timeout);
	}
	
	void flush()
	{
		enet_host_flush(m_p);
	}	
	
	void broadcast(enet_uint8 n, ENetPacket * p)
	{
		enet_host_broadcast (m_p,n,p);
	}

private:
	::ENetHost* m_p;
	size_t m_channelLimit;
};


class ENetInit
{
public:
	ENetInit() { enet_initialize(); }
	~ENetInit()  { enet_deinitialize(); }
};

#if 0

                

#endif

}


#endif /* __ENET_ENET_H__ */

