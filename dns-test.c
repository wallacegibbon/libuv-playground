#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <uv.h>

uv_loop_t *loop;

void print_addrinfo(struct addrinfo *addr, char *request_url) {
	char buffer[16];

	/// uv_ip4_name will put '\0' on tail.
	uv_ip4_name((struct sockaddr_in *)addr->ai_addr, buffer, 16);

	printf("\t%s -> %s (len: %ld)\n", request_url, buffer, strlen(buffer));
}

void on_address_resolved(uv_getaddrinfo_t *req, int status, struct addrinfo *resolved_addr) {
	struct addrinfo *cursor;

	if (status < 0) {
		fprintf(stderr, "** get_addrinfo: %s\n", uv_err_name(status));
		return;
	}

	/// `resolved_addr` is just a copy of `req->addrinfo`
	printf("resolved_addr: %p, req->addrinfo: %p\n", resolved_addr, req->addrinfo);

	cursor = resolved_addr;
	while (cursor) {
		print_addrinfo(cursor, (char *)req->data);
		cursor = cursor->ai_next;
	}

	uv_freeaddrinfo(resolved_addr);
}

int main(int argc, const char *argv) {
	struct addrinfo address_info;
	uv_getaddrinfo_t req;
	char *host_to_query;
	int r;

	/// without address_info, you will get many resolved addresses

	/*
	address_info.ai_family = PF_INET;
	address_info.ai_socktype = SOCK_STREAM;
	address_info.ai_protocol = IPPROTO_TCP;
	address_info.ai_flags = 0;
	*/

	/// According to the official document, the default loop can (and should) be closed with uv_loop_close()
	/// so the resources associated with it are freed.
	loop = uv_default_loop();
	if (loop == NULL)
		exit_info(-10, "failed getting default loop");

	host_to_query = "gnu.org";
	// host_to_query = "docker-builder";
	req.data = host_to_query;

	// r = uv_getaddrinfo(loop, &req, on_address_resolved, host_to_query, NULL, &address_info);
	r = uv_getaddrinfo(loop, &req, on_address_resolved, host_to_query, NULL, NULL);

	if (r)
		exit_info(-11, "** uv_getaddrinfo: %s\n", uv_err_name(r));

	uv_run(loop, UV_RUN_DEFAULT);
	uv_loop_close(loop);

	return 0;
}
