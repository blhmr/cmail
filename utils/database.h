#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

#define FILE_ERR(func) fprintf(stderr, "Error opening file. (%s, %d, %s)\n", __FILE__, __LINE__, func);

// Prototypes
void add_account(const char* username, const char* password);
void add_message(const char* sender, const char* receiver, const char* text);
bool account_exist(const char* username);
bool check_username_password(const char* username, const char* password);

// Definitions
void add_account(const char* username, const char* password) {
	FILE* file = fopen("database/accounts.bin", "ab+");

	if (file == NULL) {
		FILE_ERR("add_account()");
	}

	else {
		struct account x = init_account(username, password);
		fwrite(&x, sizeof(struct account), 1, file);
		fclose(file);
	}
}

void add_message(const char* sender, const char* receiver, const char* text) {
	char file_path[50];
	FILE* file;
	struct message m;

	sprintf(file_path, "database/messages/%s.bin", receiver);
	file = fopen(file_path, "ab+");

	if (file == NULL) {
		FILE_ERR("add_message()");
	}
	else {
		m = init_message(sender, receiver, text);
		fwrite(&m, sizeof(struct message), 1, file);
		fclose(file);
	}
}

bool account_exist(const char* username) {
	FILE* file;
	struct account acc;

	file = fopen("database/accounts.bin", "rb+");

	if (file == NULL) {
		FILE_ERR("accounts_exist()");
	}
	else {
		while (fread(&acc, sizeof(struct account), 1, file)) {
			if (!strcmp(acc.username, username)) {
				fclose(file);
				return true;
				break;
			}
		}
		return false;
	}
}

bool check_username_password(const char* username, const char* password) {
	FILE* file;
	struct account acc;

	file = fopen("database/accounts.bin", "rb+");

	while (fread(&acc, sizeof(struct account), 1, file)) {
		if (!strcmp(acc.username, username) && !strcmp(acc.password, password)) {
			fclose(file);
			return true;
			break;
		}
	}
	return false;
}

#endif
