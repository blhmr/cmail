#include <string.h>
#include <czmq.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils/structs.h"
#include "utils/database.h"

int main(void) {
	zsock_t* socket = zsock_new(ZMQ_REP);
	int r = zsock_bind(socket, "tcp://*:8080");

	if (r != 8080) {
		fprintf(stderr, "Can't bind to port 8080.\n");
		return -1;
	}

	printf("Server is running...\n");
	while (true) {
		char* msg = zstr_recv(socket);
		printf("CLIENT SENT: %s\n", msg);

		// Conditions of the msg
		char d[] = "^_";
		char* cmd = strtok(msg, d);

		// Register (register^_username^_password)
		if (!strcmp(cmd, "register")) {
			char* username = strtok(NULL, d);
			char* password = strtok(NULL, d);

			if (account_exist(username)) {
				zstr_send(socket, "Unavailable username.");
			}
			else {
				add_account(username, password);
				zstr_send(socket, "Successfully added account to database!");
			}
		}

		// Send message (send^_sender^_password^_receiver^_text)
		if (!strcmp(cmd, "send")) {
			char* sender = strtok(NULL, d);
			char* password = strtok(NULL, d);

			if (!account_exist(sender)) {
				zstr_send(socket, "Sender's account does not exist. (Consider registering)");
			}
			else {
				if (!check_username_password(sender, password)) {
					zstr_send(socket, "Incorrect username or password.");
				}
				else {
					char* receiver = strtok(NULL, d);
					char* text = strtok(NULL, d);
					printf("%s -> %s [%s]\n", sender, receiver, text);

					add_message(sender, receiver, text);
					zstr_send(socket, "Successfully sent message!");
				}
			}
		}

		// See inbox (inbox^_username^_password)
		if (!strcmp(cmd, "inbox")) {
			char* username = strtok(NULL, d);
			char* password = strtok(NULL, d);
			char file_path[50];
			sprintf(file_path, "database/messages/%s.bin", username);

			FILE* file = fopen(file_path, "rb+");

			if (file == NULL) {
				FILE_ERR("inbox (server.c)");
				break;
			}
			else printf("Successully opened %s!!\n", file_path);

			char buffer[5000];
			struct message m;

			while (fread(&m, sizeof(struct message), 1, file)) {
				char info[600];
				printf("SENDER: %s, TO: %s, MSG: %s\n", m.sender, m.receiver, m.text);
				sprintf(info, "%s: %s\n", m.sender, m.text);
				strcat(buffer, info);
			}

			zstr_send(socket, buffer);
		}

		else {
			zstr_send(socket, "Unknown command.");
		}

		free(msg);
	}

	zsock_destroy(&socket);
}
