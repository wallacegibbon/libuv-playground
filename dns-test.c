#include <uv.h>
#include <string.h>

uv_loop_t *loop;

void on_address_resolved(
	uv_getaddrinfo_t *req, int status, struct addrinfo *res
) {
	char addr[16];

	if (status < 0) {
		fprintf(stderr, "** get_addrinfo: %s\n", uv_err_name(status));
		return;
	}

	/// uv_ip4_name will put '\0' on tail.
	uv_ip4_name((struct sockaddr_in *) res->ai_addr, addr, 16);
	printf("address length: %ld\n", strlen(addr));

	printf("%s -> %s\n", (char *) req->data, addr);

	/// `res` is just a copy of `req->addrinfo`
	printf("\t(%p, %p)\n", res, req->addrinfo);

	uv_freeaddrinfo(res);
}

int main(int argc, const char *argv) {
	struct addrinfo address_info;
	uv_getaddrinfo_t req;
	int r;
	char *host_to_query;

	/*
	address_info.ai_family = PF_INET;
	address_info.ai_socktype = SOCK_STREAM;
	address_info.ai_protocol = IPPROTO_TCP;
	address_info.ai_flags = 0;
	*/

	loop = uv_default_loop();

	host_to_query = "gnu.org";
	//host_to_query = "docker-builder";
	req.data = host_to_query;

	/*
	r = uv_getaddrinfo(
		loop, &req, on_address_resolved, host_to_query, NULL, &address_info
	);
	*/
	r = uv_getaddrinfo(
		loop, &req, on_address_resolved, host_to_query, NULL, NULL
	);

	if (r) {
		fprintf(stderr, "** uv_getaddrinfo: %s\n", uv_err_name(r));
		return 1;
	}

	return uv_run(loop, UV_RUN_DEFAULT);
}

