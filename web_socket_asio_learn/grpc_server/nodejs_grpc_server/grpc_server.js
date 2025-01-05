const grpc = require('@grpc/grpc-js')
const hello_proto = require('./proto')

let cnt = 1

function sayHello(call, callback) {
    callback(null, { message: `[${cnt++}] echo: ` + call.request.message })
}

function main() {
    var server = new grpc.Server()
    server.addService(hello_proto.Greeter.service, { sayHello: sayHello })
    server.bindAsync('0.0.0.0:50051', grpc.ServerCredentials.createInsecure(), function(err, port) {
        if (err != null) {
          return console.error(err);
        }
        console.log(`gRPC listening on ${port}`)
      });
}

main()