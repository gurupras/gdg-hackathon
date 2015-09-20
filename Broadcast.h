#define BCAST_PORT 1234
#define BCAST_GROUP "225.0.0.37"

#ifdef __cplusplus
extern "C" {
#endif
	int bcast_setup_tx_socket();
	int bcast_setup_rx_socket();
	int bcast_tx(void *buffer, int start, int len);
	int bcast_rx(void *buffer, int len);
	int bcast_tear_down_tx();
	int bcast_tear_down_rx();
#ifdef __cplusplus
}
#endif

