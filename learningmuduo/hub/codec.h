#ifndef __LEARNINGMUDUO_HUB_CODEC_H__
#define __LEARNINGMUDUO_HUB_CODEC_H__

#include <muduo/base/Types.h>
#include <muduo/net/Buffer.h>

namespace pubsub
{
using muduo::string;

enum ParseResult
{
  kError,
  kSuccess,
  kContinue
};

ParseResult parseMessage(muduo::net::Buffer* buf,
                         string* cmd,
                         string* topic,
                         string* content);
}
#endif
