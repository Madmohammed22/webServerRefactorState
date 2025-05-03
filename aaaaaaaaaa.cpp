#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "server.hpp"
#include "globalInclude.hpp"

struct FileTransferState
{

    std::string filePath;
    size_t offset;
    size_t fileSize;
    bool isComplete;
    size_t endOffset;

    Binary_String header;
    time_t last_activity_time;
    std::string fileType;
    int fd;
    bool isValidHeader;
    int headerFlag;
    std::ofstream *file;
    bool isCompleteShortFile;

    int bytesReceived;
    Binary_String buffer;
    int flag;
    std::map<std::string, std::string> mapOnHeader;
    struct Multipart multp;
    std::set<std::string> knownPaths;
    FileTransferState() : offset(0), fileSize(0), isComplete(false), headerFlag(false) {}
    ~FileTransferState() {}
};

class Request
{

public:
    FileTransferState state;
    std::string header;
    std::map<std::string, std::string> keys;

public:
    Request() {}
    Request(std::string h, std::map<std::string, std::string> k)
    {
        this->header = h;
        this->keys = k;
    }

public:
    std::string connection;
    std::string transferEncoding;
    size_t contentLength;

public:
    void setFileTransfers(FileTransferState state) { state = state; }
    void setConnection(std::string c) { connection = c; };
    void setTransferEncoding(std::string transferEncoding) { transferEncoding = transferEncoding; };
    void setContentLength(size_t h) { contentLength = h; };

public:
    FileTransferState getFileTransfers() { return state; }
    std::string getConnection() { return connection; };
    std::string getTransferEncoding() { return transferEncoding; };
    size_t getContentLength() { return contentLength; };

public:
    void Data()
    {
        std::cout << "-------(Start : Meta data) --------" << std::endl;
        std::cout << connection << std::endl;
        std::cout << transferEncoding << std::endl;
        std::cout << contentLength << std::endl;
        std::cout << state.filePath << std::endl;
        std::cout << state.offset << std::endl;
        std::cout << state.isComplete << std::endl;
        std::cout << "-------(End : Meta data) --------" << std::endl;
    };

public:
    ~Request() {}
};

class RequstBuilder
{

public:
    virtual void buildFileTransfers() = 0;
    virtual void buildConnection() = 0;
    virtual void buildTransferEncoding() = 0;
    virtual void buildContentLength() = 0;

public:
    virtual Request getRequest() const = 0;
};

class GET : public RequstBuilder
{
public:
    Request request;

public:
    GET(Request req) { this->request = req; }

public:
    void buildFileTransfers();
    void buildConnection();
    void buildTransferEncoding();
    void buildContentLength();
    Request getRequest() const { return request; };
};

class POST : public RequstBuilder
{
public:
    Request request;

public:
    POST(Request req) { this->request = req; }

public:
    void buildFileTransfers();
    void buildConnection();
    void buildTransferEncoding();
    void buildContentLength();
    Request getRequest() const { return request; };
};

class DELETE : public RequstBuilder
{
public:
    Request request;

public:
    DELETE(Request req) { this->request = req; }

public:
    void buildFileTransfers();
    void buildConnection();
    void buildTransferEncoding();
    void buildContentLength();
    Request getRequest() const { return request; };
};

class Build
{
public:
    void buildRequest(RequstBuilder &requstBuilder)
    {
        requstBuilder.buildFileTransfers();
        requstBuilder.buildConnection();
        requstBuilder.buildTransferEncoding();
        requstBuilder.buildContentLength();
        std::cout << "Request built successfully." << std::endl;
    };
};



#endif