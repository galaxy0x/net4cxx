//
// Created by yuwenyong on 17-9-18.
//

#include "net4cxx/common/configuration/options.h"
#include <boost/functional/factory.hpp>


NS_BEGIN


void OptionParser::parseCommandLine(int argc, const char * const argv[], bool final) {
    auto opts = composeOptions();
    po::store(po::parse_command_line(argc, argv, opts), _vm);
    if (has("help")) {
        helpCallback();
    }
    if (has("version")) {
        versionCallback();
    }
    if (final) {
        po::notify(_vm);
        runParseCallbacks();
    }
}

void OptionParser::parseConfigFile(const char *path, bool final) {
    auto opts = composeOptions();
    po::store(po::parse_config_file<char>(path, opts, true), _vm);
    if (has("help")) {
        helpCallback();
    }
    if (has("version")) {
        versionCallback();
    }
    if (final) {
        po::notify(_vm);
        runParseCallbacks();
    }
}

void OptionParser::parseEnvironment(const boost::function1<std::string, std::string> &nameMapper, bool final) {
    auto opts = composeOptions();
    if (!nameMapper.empty()) {
        po::store(po::parse_environment(opts, nameMapper), _vm);
    } else {
        po::store(po::parse_environment(opts, [](std::string name){
            return "";
        }), _vm);
    }
    if (has("help")) {
        helpCallback();
    }
    if (has("version")) {
        versionCallback();
    }
    if (final) {
        po::notify(_vm);
        runParseCallbacks();
    }
}

OptionParser* OptionParser::instance() {
    static OptionParser instance("Allowed options");
    return &instance;
}

po::options_description* OptionParser::getGroup(std::string group) {
    auto iter = _groups.find(group);
    if (iter != _groups.end()) {
        return iter->second;
    } else {
        po::options_description *options = boost::factory<po::options_description *>()(group);
        _groups.insert(group, options);
        return options;
    }
}

po::options_description OptionParser::composeOptions() const {
    po::options_description opts(_opts);
    for (const auto &group: _groups) {
        opts.add(*group->second);
    }
    return opts;
}

NS_END