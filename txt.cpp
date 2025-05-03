// struct FileTransferState
// {
//     Binary_String header; // Header information for the transfer
//     std::string filePath; // Path to the file being transferred
//     std::string fileType; // Type of the file being transferred
//     std::string typeOfConnection; // Type of connection (e.g., keep-alive)
//     std::set<std::string> knownPaths; // Set of known paths for the transfer
//     int socket;        // Socket descriptor for the connection
//     int fd;           // File descriptor for the connection
//     bool isValidHeader; // Flag to indicate if the header is valid
//     time_t last_activity_time; // Last activity time for the transfer

    
//     size_t offset;       // Offset in the file for the next read
//     size_t endOffset;   // End offset for the file transfer
//     size_t fileSize;    // Size of the file being transferred
//     bool isComplete;    // Flag to indicate if the transfer is complete
//     bool isCompleteShortFile; // Flag to indicate if the transfer is complete for short files
//     int headerFlag;   // Flag to indicate if the header has been processed
//     std::ofstream* file; // Pointer to the file being transferred
//     int bytesReceived; // Number of bytes received so far
//     Binary_String buffer; // Buffer for incoming data
//     int flag;         // Flag for various purposes
//     std::map<std::string, std::string> mapOnHeader; // Map for header key-value pairs
//     Multipart multp;  // Multipart data structure

//     FileTransferState() : offset(0), fileSize(0), isComplete(false), headerFlag(false), file(NULL) {}
//     ~FileTransferState()
//     {
//         if (file)
//         {
//             file->close();
//             delete file;
//         }
//     }
// };


/*
// 
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

    FileTransferState() : offset(0), fileSize(0), isComplete(false), headerFlag(0) {}
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
    void setFileTransfers(FileTransferState state) { this->state = state; }
    void setConnection(std::string c) { connection = c; }
    void setTransferEncoding(std::string te) { transferEncoding = te; }
    void setContentLength(size_t cl) { contentLength = cl; }

public:
    FileTransferState getFileTransfers() { return state; }
    std::string getConnection() { return connection; }
    std::string getTransferEncoding() { return transferEncoding; }
    size_t getContentLength() { return contentLength; }

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
    }

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
    void buildFileTransfers() override
    {
        // Implementation for building file transfers for GET request
    }

    void buildConnection() override
    {
        // Implementation for building connection for GET request
    }

    void buildTransferEncoding() override
    {
        // Implementation for building transfer encoding for GET request
    }

    void buildContentLength() override
    {
        // Implementation for building content length for GET request
    }

    Request getRequest() const override { return request; }
};

class POST : public RequstBuilder
{
public:
    Request request;

public:
    POST(Request req) { this->request = req; }

public:
    void buildFileTransfers() override
    {
        // Implementation for building file transfers for POST request
    }

    void buildConnection() override
    {
        // Implementation for building connection for POST request
    }

    void buildTransferEncoding() override
    {
        // Implementation for building transfer encoding for POST request
    }

    void buildContentLength() override
    {
        // Implementation for building content length for POST request
    }

    Request getRequest() const override { return request; }
};

class DELETE : public RequstBuilder
{
public:
    Request request;

public:
    DELETE(Request req) { this->request = req; }

public:
    void buildFileTransfers() override
    {
        // Implementation for building file transfers for DELETE request
    }

    void buildConnection() override
    {
        // Implementation for building connection for DELETE request
    }

    void buildTransferEncoding() override
    {
        // Implementation for building transfer encoding for DELETE request
    }

    void buildContentLength() override
    {
        // Implementation for building content length for DELETE request
    }

    Request getRequest() const override { return request; }
};

class HeaderValidator
{
public:
    virtual ~HeaderValidator() {}
    virtual bool validate(RequstBuilder &builder) = 0;
    virtual HeaderValidator *getNextValidator() = 0;
};

class FileTransferValidator : public HeaderValidator
{
public:
    bool validate(RequstBuilder &builder) override
    {
        // Check if file transfers are correctly specified
        if (builder.getRequest().getFileTransfers().filePath.empty())
        {
            return false;
        }
        return true;
    }

    HeaderValidator *getNextValidator() override
    {
        return new ConnectionValidator();
    }
};

class ConnectionValidator : public HeaderValidator
{
public:
    bool validate(RequstBuilder &builder) override
    {
        // Check if connection is correctly specified
        if (builder.getRequest().getConnection().empty())
        {
            return false;
        }
        return true;
    }

    HeaderValidator *getNextValidator() override
    {
        return new TransferEncodingValidator();
    }
};

class TransferEncodingValidator : public HeaderValidator
{
public:
    bool validate(RequstBuilder &builder) override
    {
        // Check if transfer encoding is correctly specified
        if (builder.getRequest().getTransferEncoding().empty())
        {
            return false;
        }
        return true;
    }

    HeaderValidator *getNextValidator() override
    {
        return new ContentLengthValidator();
    }
};

class ContentLengthValidator : public HeaderValidator
{
public:
    bool validate(RequstBuilder &builder) override
    {
        // Check if content length is correctly specified
        if (builder.getRequest().getContentLength() <= 0)
        {
            return false;
        }
        return true;
    }
};

class Build
{
public:
    void buildRequest(Request &request)
    {
        FileTransferValidator fileTransferValidator;
        ConnectionValidator connectionValidator;
        TransferEncodingValidator transferEncodingValidator;
        ContentLengthValidator contentLengthValidator;

        std::vector<HeaderValidator *> validators;
        validators.push_back(&fileTransferValidator);
        validators.push_back(&connectionValidator);
        validators.push_back(&transferEncodingValidator);
        validators.push_back(&contentLengthValidator);

        for (std::vector<HeaderValidator *>::iterator it = validators.begin(); it != validators.end(); ++it)
        {
            if (!(*it)->validate(request))
            {
                std::cout << "Request is invalid." << std::endl;
                return;
            }
        }
        std::cout << "Request built successfully." << std::endl;
    }
};

#endif
*/