#pragma once
using namespace boost;
extern int client_end_point(std::string raw_ip_addr = "127.0.0.1", unsigned short port_num = 3333);
extern int server_end_point(unsigned short port_num = 3333);
extern int create_socket_tcp();
extern int bind_acceptor_socket();
extern int connect_to_end();
extern int accept_new_connection();
extern void use_const_buf();
extern void use_buffer_str();
extern void use_buffer_array();
extern int send_data_by_write_some();
extern int send_data_by_send();
extern int send_data_by_write();
extern int read_data_by_read_some();
extern int read_data_by_recive();
