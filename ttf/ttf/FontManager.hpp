//
//  FontManager.hpp
//  ttf
//
//  Created by Tamás Losonczi on 13/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#ifndef FontManager_hpp
#define FontManager_hpp

#include <vector>
#include "Font.hpp"

class FontManager {
    
public:
    static void destroy();
    static void init();
    static Font *getFontBySize(int size);
private:
    static std::string defaultFontPath;
    static std::vector<Font*> fonts;
    FontManager() {}
};
#endif /* FontManager_hpp */
