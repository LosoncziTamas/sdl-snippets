//
//  SdlException.cpp
//  ttf
//
//  Created by Tamás Losonczi on 13/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#include "SdlException.hpp"

const char* SdlException::what() const noexcept {
    return message.c_str();
}
