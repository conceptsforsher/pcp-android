#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseMarkdown(const std::string& json);
std::string extractHeadings(const std::string& json);
std::string extractLinks(const std::string& json);
std::string extractCodeBlocks(const std::string& json);
std::string sanitizeMarkdown(const std::string& json);
