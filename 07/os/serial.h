#ifndef _SERIAL_H_INCLUDED_
#define _SERIAL_H_INCLUDED_

int serial_init();
int serial_is_send_enable();
int serial_send_byte(char b);
int serial_is_recv_enable();
char serial_recv_byte();
//int serial_intr_is_send_enable();
//void serial_intr_send_enable();
//void serial_intr_send_disable();
//int serial_intr_is_recv_enable();
void serial_intr_recv_enable();
//void serial_intr_recv_disable();


#endif
