#include "codec.h"
#include <muduo/net/Buffer.h>
#include <algorithm>

using namespace pubhub;

ParseResult parseMessage(muduo::net::Buffer* buf,
                         string* cmd,
                         string* topic,
                         string* content)
{
  ParseResult result = kError;
  const char* crlf = buf->findCRLF();
  if (crlf)
  {
    const char* space = std::find(buf->peek(), crlf, ' ');
    if (space != crlf)
    {
      cmd->assign(buf->peek(), space);
      topic->assign(space+1, crlf);
      if (*cmd == "pub")
      {
        const char* start = crlf + 2;
        crlf = buf->findCRLF(start);
        if (crlf)
        {
          content->assign(start, crlf);
          buf->retrieveUtil(crlf+2);
          result = kSuccess;
        } 
        else 
        {
          result = kContinue;
        }
      }
      else 
      {
        buf->retrieveUtil(crlf+2);
        result = kSuccess;
      } 
    }
  }
  else 
  {
    result = kContinue;
  }
  return result;
}
