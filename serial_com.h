
int open_serial(char *port_name, HANDLE &h);

int close_serial(HANDLE &h);

// function will block until n bytes are received
int serial_recv(char *buffer, int n, HANDLE h);

int serial_send(char *buffer, int n, HANDLE h);

int serial_recv_char(char &ch, HANDLE h);

int serial_send_char(char ch, HANDLE h);

// use this for non-blocking reveive
int serial_available(HANDLE h);

