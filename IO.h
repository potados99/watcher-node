//
//  io.hpp
//  watcher-node
//
//  Created by Potados on 2021/01/20.
//

#ifndef io_hpp
#define io_hpp

class IO {
public:
    static void setup();
    static void printf(const char *format, ...);
};

#endif /* io_hpp */
