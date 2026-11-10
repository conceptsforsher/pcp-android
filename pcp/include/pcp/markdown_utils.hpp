#pragma once
#include <string>
#include <cstdint>

std::string parseMarkdownToHtml(const std::string& json);
std::string parseHtmlToPlain(const std::string& json);
std::string stripMarkdownSyntax(const std::string& json);
std::string extractLinksFromMarkdown(const std::string& json);
