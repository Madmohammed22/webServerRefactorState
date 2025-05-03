/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmad <mmad@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:08:56 by mmad              #+#    #+#             */
/*   Updated: 2025/04/21 16:55:45 by mmad             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../request.hpp"

void GET::buildFileTransfers()
{
    FileTransferState &state = request.state;
    state.filePath = Server::parseSpecificRequest(request.header);
    state.offset = 0;
    state.fileSize = Server::getFileSize(PATHC + state.filePath);
    state.isComplete = false;
    state.mime = Server::getContentType(state.filePath);
    state.uriLength = state.filePath.length();
    state.test = 0;
    state.last_activity_time = time(NULL);
    // state.stopConnection = 0;
}

void GET::buildMethod()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("GET");
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false)
            request.method = it->first;
        else
            request.method = "empty";
        return;
    }
    request.method = "undefined";
}

void GET::buildConnection()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("Connection:");
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false)
            request.connection = it->second;
        else
            request.connection = "empty";
        return;
    }
    request.connection = "keep-alive";
}

void GET::buildTransferEncoding()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("Transfer-Encoding:");
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false)
            request.transferEncoding = it->second;
        else
            request.transferEncoding = "empty";
        return;
    }
    request.transferEncoding = "undefined";
}

void GET::buildContentLength()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("Content-Length:");
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false)
            request.contentLength = it->second;
        else
            request.contentLength = "empty";
        return;
    }
    request.contentLength = "undefined";
}

void GET::buildContentType()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("Content-Type:");
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false)
            request.ContentType = it->second;
        else
            request.ContentType = "empty";
        return;
    }
    request.ContentType = "undefined";
}

void GET::buildHost()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("Host:");
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false)
            request.host = it->second;
        else
            request.host = "empty";
        return;
    }
    request.host = "undefined";
}

void GET::buildAccept()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("Accept:");
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false)
            request.accept = it->second;
        else
            request.accept = "empty";
        return;
    }
    request.accept = "undefined";
}

std::ifstream::pos_type Server::getFileSize(const std::string &path)
{
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file.is_open())
        return 0;

    return file.tellg();
}

bool readFileChunk(const std::string &path, char *buffer, size_t offset, size_t chunkSize, size_t &bytesRead)
{
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    file.seekg(offset);
    file.read(buffer, chunkSize);
    bytesRead = file.gcount();

    return true;
}

bool Server::canBeOpen(std::string &filePath)
{

    std::string new_path;
    if (filePath.at(0) != '/' && getFileType("/" + filePath) == 2)
        new_path = "/" + filePath;
    else if (filePath.at(0) != '/' && getFileType("/" + filePath) == 1)
        return true;
    else
    {
        // new_path = STATIC + filePath;
        new_path = TEST + filePath;
        // new_path = PATHC + filePath;
    }
    std::ifstream file(new_path.c_str());
    if (!file.is_open())
        return std::cerr << "" << std::ends, false;
    filePath = new_path;
    return true;
}

bool sendChunk(int fd, const char *data, size_t size)
{
    std::ostringstream chunkHeader;
    chunkHeader << std::hex << size << "\r\n";
    std::string header = chunkHeader.str();

    if (send(fd, header.c_str(), header.length(), MSG_NOSIGNAL) == -1)
        return false;
    if (send(fd, data, size, MSG_NOSIGNAL) == -1)
        return false;
    if (send(fd, "\r\n", 2, MSG_NOSIGNAL) == -1)
        return false;
    return true;
}

bool sendFinalChunk(int fd)
{
    return sendChunk(fd, "", 0) &&
           send(fd, "\r\n", 2, MSG_NOSIGNAL) != -1;
}

int Server::continueFileTransfer(int fd)
{

    FileTransferState &state = request[fd].state;
    char buffer[CHUNK_SIZE];
    size_t remainingBytes = state.fileSize - state.offset;
    size_t bytesToRead;
    if (remainingBytes > CHUNK_SIZE)
        bytesToRead = CHUNK_SIZE;
    else
        bytesToRead = remainingBytes;
    size_t bytesRead = 0;
    if (!readFileChunk(PATHC + state.filePath, buffer, state.offset, bytesToRead, bytesRead))
    {
        if (request[fd].getConnection() == "close")
            return -1;
        return 0;
    }

    if (!sendChunk(fd, buffer, bytesRead))
    {
        if (request[fd].getConnection() == "close")
            return -1;
        return 0;
    }
    state.offset += bytesRead;

    if (state.offset >= state.fileSize)
    {
        if (!sendFinalChunk(fd))
        {
            if (request[fd].getConnection() == "close")
                return -1;
            return 0;
        }
        if (request[fd].getConnection() == "close")
            return close(fd), request.erase(fd), 0;
        state.test = 0;
        // request[fd].state.stopConnection = 1;
        // request[fd].state.last_activity_time = time(NULL);
    }
    return 0;
}

int Server::handleFileRequest(int fd, const std::string &filePath, std::string Connection)
{

    std::string contentType = request[fd].state.mime;
    size_t fileSize = getFileSize(filePath);

    const size_t LARGE_FILE_THRESHOLD = 1024 * 1024;

    if (request[fd].getTransferEncoding() == "undefined" && !(fileSize > LARGE_FILE_THRESHOLD))
    {
        std::string httpRespons = httpResponse(contentType, fileSize);
        if (send(fd, httpRespons.c_str(), httpRespons.length(), MSG_NOSIGNAL) == -1)
            return std::cerr << "Failed to send HTTP header." << std::endl, -1;
        char buffer[fileSize];
        size_t bytesRead = 0;
        if (!readFileChunk(filePath, buffer, 0, fileSize, bytesRead))
            return std::cerr << "Failed to read file: " << filePath << std::endl, 0;

        if (send(fd, buffer, bytesRead, MSG_NOSIGNAL) == -1)
        {
            return close(fd), request.erase(fd), 0;
        }

        if (Connection == "close"){
            return close(fd), request.erase(fd), 0;
        }
        else
        {
            close(fd), request.erase(fd);
        }
        return 0;
    }
    else if ((fileSize > LARGE_FILE_THRESHOLD) || request[fd].getTransferEncoding() == "chunked")
    {
        request[fd].state.test = 1;
        std::string httpRespons = createChunkedHttpResponse(contentType);
        if (send(fd, httpRespons.c_str(), httpRespons.length(), MSG_NOSIGNAL) == -1)
        {
            return std::cerr << "Failed to send chunked HTTP header." << std::endl, -1;
        }
        return continueFileTransfer(fd);
    }
    return 0;
}

std::string Server::readFile(const std::string &path)
{
    std::ifstream infile(path.c_str(), std::ios::binary);
    if (!infile)
        return std::cerr << "Failed to open file: " << path << std::endl, "";

    std::ostringstream oss;
    oss << infile.rdbuf();
    return oss.str();
}

int Server::serve_file_request(int fd)
{
    if (request[fd].state.test == 1)
    {
        if (continueFileTransfer(fd) == -1)
            return std::cerr << "Failed to continue file transfer" << std::endl, request.erase(fd) , close(fd), 0;
        return 0;
    }
    std::string Connection = request[fd].connection;
    std::string filePath = request[fd].state.filePath;
    if (canBeOpen(filePath) && getFileType(filePath) == 2)
    {
        return handleFileRequest(fd, filePath, Connection);
    }
    else
    {
        return getSpecificRespond(fd, this, "404.html", createNotFoundResponse);
    }
    return 0;
}

bool Server::closeConnection(int fd)
{
    if (request[fd].state.isComplete == true)
    {
        time_t current_time = time(NULL);
        std::cout << "[" << current_time << "]" << std::endl;
        if (current_time - request[fd].state.last_activity_time > TIMEOUT)
        {
            std::cerr << "Client " << fd << " timed out." << "[getSpecificRespond]" << std::ends;
            return true;
        }
    }
    return false;
}