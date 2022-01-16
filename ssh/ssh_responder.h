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
  void ProcessInput(const std::string& input);
  int ReadNextMessageSize();
  int ReadNextMessage(std::string& buffer);
  void WriteOutputMessage(std::string& output);

private:
  std::string ConstructResponse(const std::string& request_payload);

  std::string ConstructProtobufResponse(
      int version,
      int request_id,
      const std::string& request_payload);

  std::ofstream log_file_;
};

#endif