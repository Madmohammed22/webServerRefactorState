#include "../server.hpp"

void POST::buildFileTransfers()
{
    FileTransferState &state = request.state;
    state.filePath = Server::parseSpecificRequest(request.header);
    state.offset = 0;
    state.fileSize = Server::getFileSize(PATHC + state.filePath);
    state.isComplete = false;
    state.mime = Server::getContentType(state.filePath);
    state.uriLength = state.filePath.length();
    state.test = 0;
    state.headerFlag = true;
    // state.stopConnection = 0;
}

void POST::buildMethod()
{
    std::map<std::string, std::string>::iterator it = request.keys.find("POST");
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

void POST::buildConnection()
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

void POST::buildTransferEncoding()
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

void POST::buildContentLength()
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

void POST::buildContentType()
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

void POST::buildHost()
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

void POST::buildAccept()
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