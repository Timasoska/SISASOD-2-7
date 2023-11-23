#include <iostream>
#include <vector>
#include <map>
#include <string>

struct TupleLZ77
{
    int offset;
    int len;
    char next;
};

struct TupleLZ78
{
    int index;
    char next;
};


std::vector<TupleLZ77> LZ77(const std::string& text)
{
    int len = text.size();
    int offset = 0;
    int i = 0;
    std::vector<TupleLZ77> output;

    while (i < len)
    {
        TupleLZ77 t;
        t.offset = offset;
        t.len = i - offset;
        t.next = text[i];
        output.push_back(t);

        offset = i + 1;
        ++i;
    }
    return output;
}

std::vector<TupleLZ78> LZ78(const std::string& text)
{
    int len = text.size();
    int i = 0;
    std::map<char, int> dict;
    std::vector<TupleLZ78> output;

    while (i < len)
    {
        TupleLZ78 t;
        if (dict.find(text[i]) == dict.end())
        {
            dict[text[i]] = i + 1;
        }

        t.index = dict[text[i]];
        i += 1;
        if (i == len)
        {
            t.next = '\0';
        }
        else
        {
            t.next = text[i];
        }
        output.push_back(t);
    }
    return output;
}

double calcCompressionRatio(const std::string& uncompressed, int compressedSize)
{
    int uncompressedSize = uncompressed.size() * sizeof(char);
    return (double)uncompressedSize / compressedSize;
}

// Функция для декодирования LZ77
std::string decompressLZ77(const std::vector<TupleLZ77>& compressed)
{
    std::string decompressed;
    for (const auto& t : compressed)
    {
        if (t.offset == 0)
        {
            decompressed += t.next;
        }
        else
        {
            int start = decompressed.size() - t.offset;
            for (int i = 0; i < t.len; ++i)
            {
                decompressed += decompressed[start + i];
            }
            decompressed += t.next;
        }
    }
    return decompressed;
}

// Функция для декодирования LZ78
std::string decompressLZ78(const std::vector<TupleLZ78>& compressed)
{
    std::map<int, std::string> dictionary;
    int nextCode = 1;
    std::string decompressed;
    for (const auto& t : compressed)
    {
        if (t.index == 0)
        {
            decompressed += t.next;
            dictionary[nextCode++] = std::string(1, t.next);
        }
        else
        {
            std::string phrase = dictionary[t.index] + t.next;
            decompressed += phrase;
            dictionary[nextCode++] = phrase;
        }
    }
    return decompressed;
}

int main()
{
    setlocale(LC_ALL, "RU");
    std::string textForLZ77 = "0001010010101001101";
    std::vector<TupleLZ77> compressedLZ77 = LZ77(textForLZ77);
    double compRatioLZ77 = calcCompressionRatio(textForLZ77, compressedLZ77.size() * sizeof(TupleLZ77));
    std::cout << "Коэффициент сжатия для LZ77: " << compRatioLZ77 << std::endl;

    std::string decompressedLZ77 = decompressLZ77(compressedLZ77);
    std::cout << "Сжатый текст LZ77: " << textForLZ77 << "\n";
    std::cout << "Восстановленный текст LZ77: " << decompressedLZ77 << "\n";

    std::string textForLZ78 = "кукуркукурекурекун";
    std::vector<TupleLZ78> compressedLZ78 = LZ78(textForLZ78);
    double compRatioLZ78 = calcCompressionRatio(textForLZ78, compressedLZ78.size() * sizeof(TupleLZ78));
    std::cout << "Коэффициент сжатия для LZ78: " << compRatioLZ78 << std::endl;

    std::string decompressedLZ78 = decompressLZ78(compressedLZ78);
    std::cout << "Сжатый текст LZ78: " << textForLZ78 << "\n";
    std::cout << "Восстановленный текст LZ78: " << decompressedLZ78 << "\n";

    return 0;
}