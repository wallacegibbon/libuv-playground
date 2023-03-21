#include <uv.h>

uv_loop_t *loop;

void on_address_resolved(
	uv_getaddrinfo_t *req, int status, struct addrinfo *res
) {
	char addr[17] = { 0 };

	if (status < 0) {
		fprintf(stderr, "** get_addrinfo: %s\n", uv_err_name(status));
		return;
	}

	uv_ip4_name((struct sockaddr_in *) res->ai_addr, addr, 16);

	printf("%s -> %s\n", (char *) req->data, addr);

	// `res` is just a copy of `req->addrinfo`
	printf("\t(%p, %p)\n", res, req->addrinfo);

	uv_freeaddrinfo(res);
}

int main(int argc, const char *argv) {
	struct addrinfo address_info;
	uv_getaddrinfo_t req;
	int r;

	/*
	address_info.ai_family = PF_INET;
	address_info.ai_socktype = SOCK_STREAM;
	address_info.ai_protocol = IPPROTO_TCP;
	address_info.ai_flags = 0;
	*/

	loop = uv_default_loop();

	/*
	r = uv_getaddrinfo(
		loop, &req, on_address_resolved, "gnu.org", NULL, &address_info
	);
	*/

	req.data = "gnu.org";
	r = uv_getaddrinfo(
		loop, &req, on_address_resolved, "gnu.org", NULL, NULL
	);

	if (r) {
		fprintf(stderr, "** uv_getaddrinfo: %s\n", uv_err_name(r));
		return 1;
	}

	return uv_run(loop, UV_RUN_DEFAULT);
}

