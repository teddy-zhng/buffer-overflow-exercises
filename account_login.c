#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "account_login.h"
#include "helper.h"

#define ADMIN_PERMISSION (1)
#define NO_PERMISSION (0)

#define STACK_COOKIE_VALUE (0x01234567)

char created_uname[0x100] = {0};
char created_pass[0x100] = {0};
int default_persmissions = NO_PERMISSION;

bool enable_created_user_login = false;
int current_logged_in_permissions = 0;
char currently_logged_in_uname[0x100] = {0};

char secret_admin_password[0x10] = {0};

// create random password with digits 0-9
void set_random_admin_password() {
	srand(time(NULL));

	for (int i = 0; i < sizeof(secret_admin_password); ++i)
	{
		secret_admin_password[i] = rand()%10 + '0';
	}
}

bool check_user_auth(char* uname, char* passwd, bool* auth_success) {
	if ((enable_created_user_login)
		&&
		(0 == strncmp(uname, created_uname, sizeof(created_uname))) 
		&& 
		(0 == strncmp(passwd, created_pass, sizeof(created_pass)))) {
		*auth_success = true;
	}
	return true;
}

void set_login_details(char* conn_str, char* uname, char* passwd, int permissions) {
	current_logged_in_permissions = permissions;
	memcpy(currently_logged_in_uname, uname, sizeof(currently_logged_in_uname));
}

bool handle_get_currently_logged_in_uname(int client_fd, char* client_str) {
	respond_str_to_client(client_fd, currently_logged_in_uname);
	return true;
}

bool handle_login(int client_fd, char* client_str) {
	bool auth_success = false;
	int stack_cookie_1;
	char uname[0x100];
	char passwd[0x100];
	char *status_str = "login attempted\n";

	stack_cookie_1 = STACK_COOKIE_VALUE;

	if (!get_str_from_client(client_fd, uname)) {
		printf("handle_login error: get_str_from_client failed\n");
		return false;
	}

	if (!get_str_from_client(client_fd, passwd)) {
		printf("handle_login error: get_str_from_client failed\n");
		return false;
	}

	if (!check_user_auth(uname, passwd, &auth_success)) {
		printf("check_user_auth error\n");
		status_str = "bad login!!!\n";
		return false;
	}

	if (auth_success) {
		log_verbose("no perm logging in %s as %s\n", client_str, uname);
		set_login_details(client_str, uname, passwd, default_persmissions);
		status_str = "login succes!!!\n";
	}

	if (STACK_COOKIE_VALUE != stack_cookie_1) {
		printf("hacker tried to buffer overflow! :O exploding!\n");
		handle_logout(client_fd, client_str);
		status_str = "hacker tried to buffer overflow! :O exploding!\n";
		return false;
	}

	
	printf("stack_cookie_1: %x\n", stack_cookie_1);
	printf("%s", status_str);
	respond_str_to_client(client_fd, status_str);

	return true;
}

bool handle_login_admin(int client_fd, char* client_str) {
	char passwd[0x100];

	if (!get_buffer_from_client(client_fd, passwd, sizeof(passwd))) {
		printf("handle_login_admin error: get_buffer_from_client failed\n");
		return false;
	}

	if (0 != strcmp(passwd, secret_admin_password)) {
		log_verbose("admin login attempted and failed! hacker or typo?!\n");
		return false;
	}

	memcpy(currently_logged_in_uname, "admin", 6);
	current_logged_in_permissions = ADMIN_PERMISSION;
	char *success_str = "admin login success!\n";
	printf("%s", success_str);
	respond_str_to_client(client_fd, success_str);
	return true;
}


bool handle_logout(int client_fd, char* client_str) {
	currently_logged_in_uname[0] = '\0';
	current_logged_in_permissions = 0;
	char *success_str = "logout success!\n";
	printf("%s", success_str);
	respond_str_to_client(client_fd, success_str);
	return true;
}

bool handle_create_user(int client_fd, char* client_str) {
	if (!get_buffer_from_client(client_fd, created_uname, sizeof(created_uname))) {
		printf("handle_create_user error: get_buffer_from_client failed\n");
		return false;
	}

	if (!get_buffer_from_client(client_fd, created_pass, sizeof(created_pass))) {
		printf("handle_create_user error: get_buffer_from_client failed\n");
		return false;
	}

	char *success_str = "create user success!\n";
	printf("%s", success_str);
	respond_str_to_client(client_fd, success_str);
	return true;
}

bool handle_admin_run_cmd(int client_fd, char* client_str) {
	/* let admin run any commands they want*/

	if (0 !=strcmp(currently_logged_in_uname, "admin")) {
		return false;
	}

	if (current_logged_in_permissions != ADMIN_PERMISSION) {
		printf("hacker detected! and hacker averted ;)\n");
		return false;
	}

	char cmd[0x1000];
	if (!get_str_from_client(client_fd, cmd)) {
		printf("handle_admin_run_cmd error: get_str_from_client failed\n");
		return false;
	}

	printf("will run cmd '%s'\n", cmd);
	system(cmd);
	char *success_str = "admin cmd run success!\n";
	printf("%s", success_str);
	respond_str_to_client(client_fd, success_str);
	return true;
}