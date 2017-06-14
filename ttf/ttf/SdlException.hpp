//
//  SdlException.hpp
//  ttf
//
//  Created by Tamás Losonczi on 13/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#ifndef SdlException_hpp
#define SdlException_hpp

#include <exception>
#include <string>

class SdlException : public std::exception
{
    
private:
    std::string message;
    
public:
    SdlException(const char *message) : message(message) {}
    
    const char* what() const noexcept override;
};

#endif /* SdlException_hpp */
