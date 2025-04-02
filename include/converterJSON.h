#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include "nlohmann/json.hpp"
#include <QString>

class ConverterJSON {

public:
    void expFile(const std::string &nameDir, const std::string &nameAudio, const std::string &progress);
};


#endif // CONVERTERJSON_H
