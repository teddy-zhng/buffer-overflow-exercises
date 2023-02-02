#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#include "account_login.h"
#include "helper.h"

char created_uname[0x100] = {0};
char created_pass[0x100] = {0};

char currently_logged_in_uname[0x100] = {0};

bool check_auth(char* uname, char* passwd, bool* auth_success) {
	// stub
	return false;
}

void get_clientstr_login_details(char* conn_str, char** uname_out, char** passwd_out) {
	// stub
}


void set_logged_in(char* conn_str, char* uname, char* passwd) {
	// stub
}


// vuln: VERSION1 stackoverflow to overwrite auth_success
bool handle_login(int client_fd, char* client_str) {
	bool auth_success = false;
	char uname[0x100];
	char passwd[0x100];

	if (!get_str_from_client(client_fd, uname)) {
		printf("handle_login error: get_str_from_client failed\n");
		return false;
	}

	if (!get_str_from_client(client_fd, passwd)) {
		printf("handle_login error: get_str_from_client failed\n");
		return false;
	}

	if (!check_auth(uname, passwd, &auth_success)) {
		printf("check_auth error\n");
		return false;
	}

	if (auth_success) {
		log_verbose("logging in %s as %s\n", client_str, uname);
		set_logged_in(client_str, uname, passwd);
	}

	return true;
}

bool handle_logout(int client_fd, char* client_str) {
	currently_logged_in_uname = '\0';
	return true;
}

bool handle_create_user(int client_fd, char* client_str) {
	// stub
	return false;
}

bool handle_admin_run_cmd(int client_fd, char* client_str) {
	/* let admin run any commands they want*/

	if (0 !=strcmp(currently_logged_in_uname, "admin")) {
		return false;
	}

	char cmd[0x1000];
	if (!get_str_from_client(client_fd, cmd)) {
		printf("handle_admin_run_cmd error: get_str_from_client failed\n");
		return false;
	}

	system(cmd);
	return true;
}