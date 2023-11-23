#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <string>
#include <queue>



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

// Структура для представления узла в префиксном дереве Хаффмана
struct HuffmanNode {
	char character;
	int frequency;
	HuffmanNode* left;
	HuffmanNode* right;

	HuffmanNode(char ch, int freq, HuffmanNode* l, HuffmanNode* r) {
		character = ch;
		frequency = freq;
		left = l;
		right = r;
	}
};

// Сравнение для очереди с приоритетом
struct CompareNodes {
	bool operator()(HuffmanNode* a, HuffmanNode* b) {
		return a->frequency > b->frequency;
	}
};

// Функция для построения кодов символов в префиксном дереве Хаффмана
void buildHuffmanCodes(HuffmanNode* root, std::string code, std::map<char, std::string>& huffmanCodes) {
	if (root == nullptr)
		return;

	if (root->character)
		huffmanCodes[root->character] = code;

	buildHuffmanCodes(root->left, code + "0", huffmanCodes);
	buildHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Функция для кодирования строки с помощью префиксного кода Хаффмана
std::string encodeString(std::string input, std::map<char, std::string> & huffmanCodes) {
	std::string encodedString = "";
	for (char ch : input)
		encodedString += huffmanCodes[ch];

	return encodedString;
}

// Функция для расчета коэффициента сжатия
float compressionRatio(std::string input, std::string encodedString) {
	float inputSize = input.size() * 8;
	float encodedSize = encodedString.size();
	return inputSize / encodedSize;
}

// Тестирование на входном файле
void testEncoding(const std::string & inputFile) {
	std::ifstream file(inputFile);
	std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	// Подсчет частоты вхождений символов
	std::map<char, int> characterFrequency;
	for (char ch : input)
		characterFrequency[ch]++;

	// Построение префиксного дерева Хаффмана
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;
	for (const auto& pair : characterFrequency)
		pq.push(new HuffmanNode(pair.first, pair.second, nullptr, nullptr));

	while (pq.size() > 1) {
		HuffmanNode* left = pq.top();
		pq.pop();
		HuffmanNode* right = pq.top();
		pq.pop();

		int combinedFreq = left->frequency + right->frequency;
		pq.push(new HuffmanNode('\0', combinedFreq, left, right));
	}

	HuffmanNode* root = pq.top();

	// Построение кодов символов в префиксном дереве Хаффмана
	std::map<char, std::string> huffmanCodes;
	buildHuffmanCodes(root, "", huffmanCodes);

	// Вывод кодов символов
	std::cout << "Huffman Codes:" << std::endl;
	for (const auto& pair : huffmanCodes)
		std::cout << pair.first << " : " << pair.second << std::endl;

	// Кодирование строки
	std::string encodedString = encodeString(input, huffmanCodes);

	// Вывод закодированной строки
	std::cout << "Закадированная строка: " << encodedString << std::endl;

	// Расчет и вывод коэффициента сжатия
	float ratio = compressionRatio(input, encodedString);
	std::cout << "Коэффицент сжатия: " << ratio << std::endl;
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

	std::string inputFile = "input.txt";
	testEncoding(inputFile);

	return 0;
}