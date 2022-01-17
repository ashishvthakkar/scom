#ifndef SCOM_SSH_RESPONDER_H_
#define SCOM_SSH_RESPONDER_H_

#include <config.h>
#include <glog/logging.h>

#include <fstream>
#include <iostream>

#include "../protocol/protocol_adapter.h"

class SshResponder {
public:
  explicit SshResponder(const std::string& log_file_name);
  int GetNextMessageSize();
  void HandleNextMessage(int message_size);

private:
  int Read(void* buffer, int buffer_size);
  void ProcessMessage();
  std::string ConstructResponse(const std::string& request_payload);
  void ConstructProtobufResponse(
      int version,
      int request_id,
      const std::string& request_payload);
  void Send(std::string& output);

  std::string buffer_;
  std::ofstream log_file_;
};

#endif