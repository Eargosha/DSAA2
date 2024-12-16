#include <iostream>
#include <unordered_map>
#include <cctype> // Для std::tolower
#include <utility> // Для std::pair
#include <fstream> // Для работы с файлами
#include <codecvt>
#include <locale>
#include <algorithm>
#include <string>
#include <sstream>

#include "Dictionary.h"

using namespace std;

// Функция для создания SVG файла из полученного закона Ципфа, показывает только 150 рангов
void createSvg(const std::vector<std::pair<std::wstring, int>>& frequencies, const std::string& title) {
    const int width = 800;
    const int height = 600;
    const int margin = 50;
    int stopIt = 150;
    int whiii = 0;

    // Определение максимальной частоты для нормализации 
    int maxFrequency = frequencies.front().second;

    std::ofstream svgFile(title + ".svg");
    svgFile << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svgFile << "<text x=\"" << width / 2 << "\" y=\"20\" text-anchor=\"middle\" font-size=\"16\">" << title << "</text>\n";

    // Рисуем оси 
    svgFile << "<line x1=\"" << margin << "\" y1=\"" << height - margin << "\" x2=\"" << width - margin << "\" y2=\"" << height - margin << "\" stroke=\"black\" />\n"; // Ось X 
    svgFile << "<line x1=\"" << margin << "\" y1=\"" << margin << "\" x2=\"" << margin << "\" y2=\"" << height - margin << "\" stroke=\"black\" />\n"; // Ось Y 

    // Рисуем график 
    //int barWidth = (width - 2 * margin) / frequencies.size(); 
    int barWidth = 5;
    //for (size_t i = 0; i < frequencies.size(); ++i) {
        while(whiii != stopIt) {
            whiii++;
        int barHeight = static_cast<int>((static_cast<double>(frequencies[whiii].second) / maxFrequency) * (height - 2 * margin));
        svgFile << "<rect x=\"" << margin + whiii * barWidth << "\" y=\"" << height - margin - barHeight
            << "\" width=\"" << barWidth - 1 << "\" height=\"" << barHeight
            << "\" fill=\"blue\" />\n";
    }
    

    svgFile << "</svg>";
}

// Преобразователь строки в формате UTF-8 в wstring
// std::wstring — это класс, представляющий строку символов типа wchar_t. 
// Он используется для хранения текстовых данных, которые могут содержать символы, выходящие за 
// пределы стандартного ASCII (например, кириллица, иероглифы и т. д.). 
std::wstring utf8ToWstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; // Создание конвертера
    return converter.from_bytes(str); // Преобразование строки
}

// Функция для чтения текста из файла и возврата его в виде wstring
std::wstring readTextFromFile(const std::string& filename) {
    // std::wifstream — это класс для чтения из файлов, содержащих широкие символы. Он позволяет открывать файлы и
    // считывать из них данные, используя операции ввода, аналогичные тем, что используются с обычными потоками.
    std::wifstream file(filename); // Открытие файла для чтения в формате wide string
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>)); // Установка локали для поддержки UTF-8
    std::wstringstream buffer; // Создание буфера для чтения содержимого файла
    buffer << file.rdbuf(); // Чтение содержимого файла в буфер
    return buffer.str(); // Возврат прочитанного текста
}

// Функция для записи частот слов в файл
void writeFrequenciesToFile(const std::vector<std::pair<std::wstring, int>>& frequencies, const std::string& filename) {
    //std::wofstream — это класс для записи в файлы, содержащие широкие символы. 
    // Он позволяет открывать файлы и записывать в них данные, используя операции вывода.
    std::wofstream outputFile(filename); // Открытие файла для записи в формате wide string
    if (!outputFile.is_open()) { // Проверка на успешное открытие файла
        std::wcerr << L"Ошибка при открытии файла для записи: " << filename.c_str() << std::endl; // Вывод сообщения об ошибке
        return; // Завершение функции при ошибке
    }

    // Записываем заголовок в файл  
    outputFile << L"Слово;Частота;Ранг" << std::endl;

    int rank = 1; // Инициализация ранга с единицы
    for (const auto& pair : frequencies) { // Перебор всех пар (слово, частота)
        outputFile << pair.first << L";" << pair.second << L";" << rank << std::endl; // Запись слова, частоты и ранга в файл
        rank++; // Увеличение ранга для следующего слова
    }
}

/// <summary> 
/// Закон Ципфа — это эмпирическое правило, согласно которому частота слов в текстах обратно пропорциональна их рангу. 
/// Другими словами, слово, занимающее первое место по частоте, будет встречаться в n раз чаще, чем слово,  
/// занимающее n - е место по частоте. 
/// - Общая сложность будет O(n + m log m). 
/// - В худшем случае, когда все слова уникальны(m = n), сложность составит O(n log n). 
/// </summary> 
/// <param name="text">Текст, что нужно обработать</param> 
/// <returns>vector<pair<string, int>> - отсортированный по убыванию массив из пар (слово, частота)</returns> 
std::vector<std::pair<std::wstring, int>> zipfsLaw(const std::wstring& text) {
    std::unordered_map<std::wstring, int> wordCount;

    // Разбиваем текст на слова и считаем частоту слов
    // std::wistringstream — это поток, который позволяет читать из объекта std::wstring, как если бы это был входной поток. 
    // Он является частью библиотеки <sstream>. 
    // Этот класс позволяет разбивать широкие строки на слова или другие части, используя стандартные операции ввода.
    std::wistringstream iss(text);
    std::wstring word;

    while (iss >> word) {
        // Приводим все буквы слова к нижнему регистру
        std::transform(word.begin(), word.end(), word.begin(), ::towlower);

        // Проверяем, состоит ли слово только из букв
        std::wstring normalizedWord;
        for (wchar_t c : word) {
            //iswalpha() — это функция, которая проверяет, является ли переданный ей символ (типа wchar_t) буквой. Она определена в заголовочном файле <cwctype>. 
            // Функция возвращает ненулевое значение (обычно true), если символ является буквой (латинской или нелатинской), и ноль (обычно false), если это не так.
            if (iswalpha(c)) { // Проверяем, является ли символ буквой
                normalizedWord += c; // Добавляем к нормализованному слову
            }
        }

        if (!normalizedWord.empty()) { // Проверяем, что слово не пустое
            wordCount[normalizedWord]++;
        }
    }

    // Вектор для хранения пар слово-частота
    std::vector<std::pair<std::wstring, int>> frequencies;
    for (const auto& pair : wordCount) {
        frequencies.emplace_back(pair.first, pair.second);
    }

    // Сортируем вектор по частоте
    std::sort(frequencies.begin(), frequencies.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Сортировка по убыванию частоты
        });

    return frequencies; // Возвращаем отсортированный вектор
}


// можно возращать просто словарь(или pair) а не структуру для того, чтобы использовать можно было хоть кому. + подгрузку с файла
// проетестировать блоьшой текст в ципфе

int main() {

    // Установка локали для корректного вывода широких символов
    std::locale::global(std::locale("")); // Использует системную локаль

    // Чтение текста из файла
    
    // Aldous_Huxley_-_Brave_New_World

    //std::wstring wideText = readTextFromFile("input2.txt");
    //string titleOfSvg = "ZIPF of Aldous_Huxley_-_Brave_New_World";
    

    // Kalinin_Proekt-S-T-A-L-K-E-R-_1_Teni-Chernobylya_RuLit_Me
    
    std::wstring wideText = readTextFromFile("input.txt");
    string titleOfSvg = "ZIPF of Kalinin_Proekt-S-T-A-L-K-E-R-_1_Teni-Chernobylya_RuLit_Me";

    // Анализ текста с помощью закона Ципфа
    auto frequencies = zipfsLaw(wideText);

    // Запись результатов в файл
    writeFrequenciesToFile(frequencies, "zipfsOutput.csv");
    createSvg(frequencies, titleOfSvg);

    return 0;
}


