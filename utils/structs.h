#ifndef __STRUCTS_H__
#define __STRUCTS_H__

// Structs
struct account {
	char username[20];
	char password[20];
};

struct message {
	char sender[20];
	char receiver[20];
	char text[500];
};

// Prototypes
struct account init_account(const char* username, const char* password);
struct message init_message(const char* sender, const char* receiver, const char* text);

// Definition

struct account init_account(const char* username, const char* password) {
	struct account x;
	strcpy(x.username, username);
	strcpy(x.password, password);
	return x;
}


struct message init_message(const char* sender, const char* receiver, const char* text) {
	struct message m;
	strcpy(m.sender, sender);
	strcpy(m.receiver, receiver);
	strcpy(m.text, text);
	return m;
}

#endif
