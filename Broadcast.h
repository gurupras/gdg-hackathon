#ifdef __cplusplus
extern "C" {
#endif
    int bcast_setup_socket();
    int bcast_send(void *buffer, int start, int len);
    int bcast_tear_down();
#ifdef __cplusplus
}
#endif

