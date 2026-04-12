#include "socket.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

Socket::Socket() : m_sockfd(-1) {}

Socket::~Socket() {
    if (m_sockfd != -1) {
        close(m_sockfd);
    }
}

void ServerSocket::Bind(int port) {
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_sockfd<0){
		perror("Socket creation failed.");
		return;
	}
	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	
	if (bind(m_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Socket bind failed");
		close(m_sockfd);
		m_sockfd = -1;
	}
}

void ServerSocket::Listen() {
	if(listen(m_sockfd, 10) < 0){ // 10 connections seems completely fair atm
		perror("Socket is unable to listen for new connections.");
	}
}

std::unique_ptr<Socket> ServerSocket::Accept() {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(m_sockfd, (struct sockaddr*)&client_addr, &client_len);

    if (client_fd < 0) {
        perror("Accept failed");
        return nullptr;
    }
    return std::make_unique<Socket>(client_fd);
}