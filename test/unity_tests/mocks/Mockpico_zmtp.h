/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKPICO_ZMTP_H
#define _MOCKPICO_ZMTP_H

#include "pico_zmtp.h"

void Mockpico_zmtp_Init(void);
void Mockpico_zmtp_Destroy(void);
void Mockpico_zmtp_Verify(void);


typedef void(*cmock_pico_zmtp_func_ptr1)(uint16_t ev, struct zmtp_socket* s);


#define zmtp_socket_open_ExpectAndReturn(net, proto, type, wakeup, cmock_retval) zmtp_socket_open_CMockExpectAndReturn(__LINE__, net, proto, type, wakeup, cmock_retval)
void zmtp_socket_open_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint16_t net, uint16_t proto, uint8_t type, cmock_pico_zmtp_func_ptr1 wakeup, struct zmtp_socket* cmock_to_return);
typedef struct zmtp_socket* (* CMOCK_zmtp_socket_open_CALLBACK)(uint16_t net, uint16_t proto, uint8_t type, cmock_pico_zmtp_func_ptr1 wakeup, int cmock_num_calls);
void zmtp_socket_open_StubWithCallback(CMOCK_zmtp_socket_open_CALLBACK Callback);
#define zmtp_socket_bind_ExpectAndReturn(s, local_addr, port, cmock_retval) zmtp_socket_bind_CMockExpectAndReturn(__LINE__, s, local_addr, port, cmock_retval)
void zmtp_socket_bind_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, struct zmtp_socket* s, void* local_addr, uint16_t* port, int8_t cmock_to_return);
typedef int8_t (* CMOCK_zmtp_socket_bind_CALLBACK)(struct zmtp_socket* s, void* local_addr, uint16_t* port, int cmock_num_calls);
void zmtp_socket_bind_StubWithCallback(CMOCK_zmtp_socket_bind_CALLBACK Callback);
#define zmtp_socket_connect_ExpectAndReturn(s, srv_addr, remote_port, cmock_retval) zmtp_socket_connect_CMockExpectAndReturn(__LINE__, s, srv_addr, remote_port, cmock_retval)
void zmtp_socket_connect_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, struct zmtp_socket* s, void* srv_addr, uint16_t remote_port, int8_t cmock_to_return);
typedef int8_t (* CMOCK_zmtp_socket_connect_CALLBACK)(struct zmtp_socket* s, void* srv_addr, uint16_t remote_port, int cmock_num_calls);
void zmtp_socket_connect_StubWithCallback(CMOCK_zmtp_socket_connect_CALLBACK Callback);
#define zmtp_socket_send_ExpectAndReturn(s, msg, len, cmock_retval) zmtp_socket_send_CMockExpectAndReturn(__LINE__, s, msg, len, cmock_retval)
void zmtp_socket_send_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, struct zmtp_socket* s, struct zmq_msg** msg, uint16_t len, int8_t cmock_to_return);
typedef int8_t (* CMOCK_zmtp_socket_send_CALLBACK)(struct zmtp_socket* s, struct zmq_msg** msg, uint16_t len, int cmock_num_calls);
void zmtp_socket_send_StubWithCallback(CMOCK_zmtp_socket_send_CALLBACK Callback);
#define zmtp_socket_close_ExpectAndReturn(s, cmock_retval) zmtp_socket_close_CMockExpectAndReturn(__LINE__, s, cmock_retval)
void zmtp_socket_close_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, struct zmtp_socket* s, int8_t cmock_to_return);
typedef int8_t (* CMOCK_zmtp_socket_close_CALLBACK)(struct zmtp_socket* s, int cmock_num_calls);
void zmtp_socket_close_StubWithCallback(CMOCK_zmtp_socket_close_CALLBACK Callback);

#endif
