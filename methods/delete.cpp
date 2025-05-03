#include "../server.hpp"

void DELETE::buildFileTransfers()
{
    FileTransferState &state = request.state;
    state.filePath = Server::parseSpecificRequest(request.header);
    state.offset = 0;
    state.fileSize = Server::getFileSize(PATHC + state.filePath);
    state.isComplete = false;    
    state.isValidHeader = false;
    // state.stopConnection = 0;
}

void DELETE::buildMethod()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("DELETE");
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

void DELETE::buildConnection()
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
    request.connection = "undefined";
}

void DELETE::buildTransferEncoding()
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

void DELETE::buildContentLength()
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

void DELETE::buildContentType()
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

void DELETE::buildHost()
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

void DELETE::buildAccept()
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

void deleteDirectoryContents(const std::string &dir)
{
    DIR *dp = opendir(dir.c_str());
    if (dp == NULL)
    {
        std::cerr << "Error: Unable to open directory " << dir << std::endl;
        return;
    }

    try
    {
        struct dirent *entry;
        while ((entry = readdir(dp)) != NULL)
        {
            if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
            {
                continue;
            }

            struct stat entryStat;
            std::string entryPath = dir + "/" + entry->d_name;
            if (stat(entryPath.c_str(), &entryStat) == -1)
            {
                std::cerr << "Error: Unable to stat " << entryPath << std::endl;
                continue;
            }

            if (S_ISDIR(entryStat.st_mode))
            {
                if (rmdir(entryPath.c_str()) == -1)
                {
                    std::cerr << "Error: Unable to remove directory " << entryPath << std::endl;
                }
            }
            else
            {
                if (unlink(entryPath.c_str()) == -1)
                {
                    std::cerr << "Error: Unable to remove file " << entryPath << std::endl;
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    closedir(dp);
    rmdir(dir.c_str());
}

int DELETE(std::string request)
{
    const char *filename = request.c_str();
    if (unlink(filename) == -1)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int Server::handle_delete_request(int fd, Server *server, std::string rqst)
{
    // std::cout << "-------( REQUEST PARSED )-------\n\n";
    // server->key_value_pair_header(fd, server, ft_parseRequest_T(fd, server, request).first);
    // std::cout << "-------( END OF REQUEST )-------\n\n\n";

    std::pair<std::string, std::string> pair_request = ft_parseRequest_T(fd, server, rqst);
    FileTransferState state;
    server->request[fd].connection = server->request[fd].state.mapOnHeader.find("Connection:")->second;
    server->request[fd].state = state;
    std::string filePath = parseSpecificRequest(rqst);
    if (server->canBeOpen(filePath))
    {
        if (filePath.at(0) != '/')
            filePath = "/" + filePath;
        if (server->getFileType(filePath) == 1)
        {
            deleteDirectoryContents(filePath.c_str());
        }

        if (DELETE(filePath) == -1)
        {
            return server->request.erase(fd), close(fd), std::cerr << "Failed to delete file or directory: " << filePath << std::endl, 0;
        }
        std::string httpResponse = server->deleteHttpResponse(server);
        if (send(fd, httpResponse.c_str(), httpResponse.length(), MSG_NOSIGNAL) == -1)
        {
            std::cerr << "Failed to send HTTP header." << std::endl;
            return server->request.erase(fd), close(fd), 0;
        }
        return server->request.erase(fd), close(fd), 0;
    }
    return getSpecificRespond(fd, server, "404.html", server->createNotFoundResponse);
}