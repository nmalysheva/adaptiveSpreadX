#include "Segments.hpp"


std::regex const Segments::RGX{R"((\S+))"};
    

Segments::Segments(std::string_view const text)
{
    if (text.empty())
    {
        return;
    }

    m_begin = std::cregex_iterator{text.begin(), text.end(), Segments::RGX};
}

