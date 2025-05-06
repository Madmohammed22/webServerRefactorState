#include "../server.hpp"

void POST::includeBuild(std::string target, std::string &metaData, int pick)
{
    std::map<std::string, std::string>::iterator it = request.keys.find(target);
    if (it != request.keys.end())
    {
        if (Server::containsOnlyWhitespace(it->second) == false){
            if (pick == 1)
                metaData = it->first;
            else
                metaData = it->second;
        }
        else
            metaData = "empty";
        return;
    }
    metaData = "undefined";
}

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
    includeBuild("POST", request.method, 1);
}

void POST::buildConnection()
{
    includeBuild("Connection:", request.connection, 2);
}

void POST::buildTransferEncoding()
{
    includeBuild("Transfer-Encoding:", request.transferEncoding, 2);
}

void POST::buildContentLength()
{
    includeBuild("Content-Length:", request.contentLength, 2);
}

void POST::buildContentType()
{
    includeBuild("Content-Type:", request.ContentType, 2);
}

void POST::buildHost()
{
    includeBuild("Host:", request.host, 2);
}

void POST::buildAccept()
{
    includeBuild("Accept:", request.accept, 2);
}
