﻿#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "demo.grpc.pb.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using hello::HelloRequest;
using hello::HelloReply;
using hello::Greeter;
// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
    Status SayHello(ServerContext* context, const HelloRequest* request,
        HelloReply* reply) override {
        std::string prefix("wgt grpc server has received :  ");
        reply->set_message(prefix + request->message());
        return Status::OK;
    }
};
void RunServer() {
    std::string server_address("127.0.0.1:50051");
    GreeterServiceImpl service;
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    // 监听给定的地址
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}
int main(int argc, char** argv) {
    RunServer();
    return 0;
}