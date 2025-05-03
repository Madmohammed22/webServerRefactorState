// #include "server.hpp"
// #include "request.hpp"
// /*
//     Build build; 

//     AmericanHouse american_house; GET get;
//     build.makeHouse(american_house); build.buildrequest(get);
//     House house = american_house.getHouse(); Requst request(state.header.to_string());
//     // GET
//     // Request request = GET.getRequest(); 

//     house.DisplayHouse(); 
//  */
// void Server::validateHeader(int fd, FileTransferState &state)
// {
//     size_t header_end = state.buffer.find("\r\n\r\n");
//     if (header_end != std::string::npos)
//     {
//         // extract header
//         state.header = state.buffer.substr(0, header_end + 4);
//         state.headerFlag = true;

//         size_t body_start = header_end + 4;
//         if (body_start < state.buffer.length())
//         {
//             Binary_String body_part = state.buffer.substr(body_start, state.buffer.length());
//             state.file->write(body_part.c_str(), body_part.length());
//             state.bytesReceived += body_part.length();
//         }
//         try
//         {
//             key_value_pair_header(fd, ft_parseRequest_T(state.fd, this, state.header.to_string()).first);
//         }
//         catch (const std::exception &e)
//         {
//             getSpecificRespond(state.fd, this, "400.html", createBadResponse);
//             return;
//         }
//         if (state.header.find("GET") != std::string::npos)
//         {
//             std::cout << "$$$$$$$$$$$$$$$$$$$\n";
//             std::cout << state.header.to_string() << std::endl;
//             std::cout << "$$$$$$$$$$$$$$$$$$$\n";
//             Request test(state.header.to_string());
//             Build build;
//             GET getMethod;
//             build.buildRequest(getMethod);
//             test = getMethod.getRequest();
//             // (void)get;
//             // build.buildRequest(get);
//             // Request request1 = get.getRequest();
//             std::cout << "**********************" << std::endl; 
//             std::cout << test.getConnecionType() << std::endl;
//             std::cout << "**********************" << std::endl; 
//             std::cout << "-------( REQUEST PARSED )-------\n\n";
//             std::cout << state.header.to_string() << std::endl;
//             std::cout << "\n-------( END OF REQUEST )-------\n\n\n";
//             //------------------
//             // here you have to parse the header
//             // and set the content length
//             // and set the file name
//             // and set the file path
//             // and set the file size
//             // and set the file type
//             // and set the file name
//             // and set the file path
//             // and set the file size
//             // and set the file type
//             //     FileTransferState state;
//             //     state.fileSize = 10;
//             //  //---------------
//             //  //     std::map<int, Request> request;
//             //  request[fd].state =  state;
//             //  request[fd].connection = "bla";
//         }
//         else if (state.header.find("POST") != std::string::npos)
//         {
//             std::cout << "-------( REQUEST PARSED )-------\n\n";
//             std::cout << state.header.to_string() << std::endl;
//             std::cout << "\n-------( END OF REQUEST )-------\n\n\n";
//         }
//         else if (state.header.find("DELETE") != std::string::npos)
//         {
//             std::cout << "-------( REQUEST PARSED )-------\n\n";
//             std::cout << state.header.to_string() << std::endl;
//             std::cout << "\n-------( END OF REQUEST )-------\n\n\n";
//         }
//         else if (state.header.find("PUT") != std::string::npos || state.header.find("PATCH") != std::string::npos || state.header.find("HEAD") != std::string::npos || state.header.find("OPTIONS") != std::string::npos)
//         {
//              // processMethodNotAllowed(state.fd, this, state.header.to_string());
//         }   
//         // here you have all the header you need to parse it
//         // you're code goes here

//         state.buffer.clear();
//     }
// }