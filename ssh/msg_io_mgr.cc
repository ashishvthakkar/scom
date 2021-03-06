#include "msg_io_mgr.h"

#include <config.h>

#include <cstdio>
#include <iostream>

#include "../protocol/protocol_adapter.h"
#include "ssh_initiator.h"
#include "std_io_mgr.h"

namespace scom {

template <typename T>
void MsgIoMgr<T>::ConstructProtobufMessage(
    int version,
    int request_id,
    const std::string& payload) {
  scom::HeaderWriteFields header{
      .version = version,
      .request_id = request_id,
      .payload = payload};
  scom::WriteMessage(header, buffer_);
}

template <typename T>
void MsgIoMgr<T>::Send(
    const std::string& message,
    const int request_id,
    T& io_mgr) {
  ConstructProtobufMessage(kProtocolVersion, request_id, message);
  io_mgr.Send(buffer_);
}

template <typename T>
std::string MsgIoMgr<T>::Receive(T& io_mgr, int& request_id) {
  io_mgr.Receive(buffer_);
  scom::HeaderReadFields header;
  scom::ReadMessage(buffer_, header);
  CHECK(header.version == kProtocolVersion)
      << "Unexpected version: " << header.version;
  request_id = header.request_id;
  return std::move(header.payload);
}

template class MsgIoMgr<StdIoMgr>;
template class MsgIoMgr<SshInitiator>;

}  // namespace scom