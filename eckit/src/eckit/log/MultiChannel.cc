/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/log/Log.h"
#include "eckit/log/MultiChannel.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

class MultiplexBuffer: public std::streambuf {
public:

    typedef std::map<std::string,ostream_handle> streams_t;
    
    MultiplexBuffer( std::size_t size = 1024 ) :
        std::streambuf(),
        buffer_( size + 1 ) // + 1 so we can always write the '\0'        
    {
        assert( size );        
        char *base = &buffer_.front();
        setp( base, base + buffer_.size() - 1 ); // don't consider the space for '\0'
        setg(0, 0, 0);
    }
    
    ~MultiplexBuffer() { sync(); }
    
    streams_t streams_;
    
private:

  std::vector<char> buffer_;
  
  bool dumpBuffer()
  {
      for( streams_t::iterator i = streams_.begin(); i != streams_.end(); ++i )
      {
          std::ostream& os = *(i->second);
          os.write(pbase(),pptr() - pbase());
      }
      setp(pbase(), epptr());
      return true;
  }
  
protected:
  
  virtual int_type overflow(int_type ch)
  {
      if (ch == traits_type::eof() ) { return sync(); }
      dumpBuffer();
      sputc(ch);
      return traits_type::to_int_type(ch);
  }
  
  virtual int_type sync()
  {
      if( dumpBuffer() )
      {
          for( streams_t::iterator i = streams_.begin(); i != streams_.end(); ++i )
              i->second->flush();
          return 0;
      }
      else return -1;
  }
  
};

//-----------------------------------------------------------------------------

MultiChannel::MultiChannel() : 
    Channel( new MultiplexBuffer() ),
    buff_( dynamic_cast<MultiplexBuffer*>( rdbuf() ) )
{
    assert( buff_ );
}

MultiChannel::~MultiChannel()
{
}

bool MultiChannel::remove(const string &k)
{ 
    MultiplexBuffer::streams_t& streams = buff_->streams_;
    MultiplexBuffer::streams_t::iterator i = streams.find(k);
    if( i != streams.end() )
    {
        i->second->flush();
        streams.erase(i);
        return true;
    }
    return false;
}

void MultiChannel::add(const string &k, ostream *s)
{
    if(!s) return;
    remove(k);
    buff_->streams_[k].reset(s);
}

void MultiChannel::add(const string &k, ostream &s)
{
    remove(k);
    buff_->streams_[k].reset(s);
}

void MultiChannel::clear()
{
}

//-----------------------------------------------------------------------------

} // namespace eckit
