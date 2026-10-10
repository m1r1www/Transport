// GameForm.h — Игра «Виселица»
#include "HelpForm.h"
#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace EnterpriseProject {

public ref class GameForm : public Form
{
private:
    array<String^>^ words;
    String^ currentWord;
    String^ guessedLetters;
    int mistakes;
    int maxMistakes;

    Label^ lblWord;
    Label^ lblGallows;
    Label^ lblStatus;
    Panel^ panelButtons;
    Button^ btnNewGame;

public:
    GameForm()
    {
        maxMistakes = 6;
        InitWords();
        InitUI();
        NewGame();
    }

private:
    void InitWords()
{
    words = gcnew array<String^> {
        // Основные понятия
        L"ПРОГРАММА", L"КОД", L"АЛГОРИТМ", L"ДАННЫЕ", L"ПАМЯТЬ",
        L"ПРОЦЕССОР", L"СИСТЕМА", L"ФАЙЛ", L"КАТАЛОГ", L"ПУТЬ",
        
        // Типы данных и структуры
        L"МАССИВ", L"СПИСОК", L"СТЕК", L"ОЧЕРЕДЬ", L"ДЕРЕВО",
        L"ГРАФ", L"ТАБЛИЦА", L"СТРУКТУРА", L"КЛАСС", L"ОБЪЕКТ",
        L"ИНТЕРФЕЙС", L"МОДУЛЬ", L"ПАКЕТ", L"БИБЛИОТЕКА", L"КОЛЛЕКЦИЯ",
        
        // Переменные и константы
        L"ПЕРЕМЕННАЯ", L"КОНСТАНТА", L"ТИП", L"ЗНАЧЕНИЕ", L"ССЫЛКА",
        L"УКАЗАТЕЛЬ", L"АДРЕС", L"ИДЕНТИФИКАТОР", L"ИМЯ", L"КЛЮЧ",
        
        // Операции и выражения
        L"ОПЕРАЦИЯ", L"ВЫРАЖЕНИЕ", L"ФУНКЦИЯ", L"МЕТОД", L"ПРОЦЕДУРА",
        L"ВЫЗОВ", L"ВОЗВРАТ", L"ПАРАМЕТР", L"АРГУМЕНТ", L"РЕЗУЛЬТАТ",
        
        // Управляющие конструкции
        L"ЦИКЛ", L"УСЛОВИЕ", L"ВЕТВЛЕНИЕ", L"ВЫБОР", L"ПЕРЕКЛЮЧАТЕЛЬ",
        L"ИТЕРАЦИЯ", L"РЕКУРСИЯ", L"ПОВТОРЕНИЕ", L"ПРЕРЫВАНИЕ", L"ВОЗВРАТ",
        
        // Ввод-вывод
        L"ВВОД", L"ВЫВОД", L"ПОТОК", L"БУФЕР", L"ФОРМАТ",
        L"СТРОКА", L"СИМВОЛ", L"КОДИРОВКА", L"ЛОГИРОВАНИЕ", L"ОТЧЁТ",
        
        // Работа с памятью
        L"ВЫДЕЛЕНИЕ", L"ОСВОБОЖДЕНИЕ", L"УТЕЧКА", L"СБОРЩИК", L"МУСОР",
        L"ОБЛАСТЬ", L"ВИДИМОСТЬ", L"ЖИЗНЬ", L"ВРЕМЯ", L"СУЩЕСТВОВАНИЯ",
        
        // Ошибки и отладка
        L"ОШИБКА", L"ИСКЛЮЧЕНИЕ", L"ОТЛАДКА", L"ТЕСТИРОВАНИЕ", L"ПРОВЕРКА",
        L"ОТЛАДЧИК", L"ТОЧКА", L"ОСТАНОВКА", L"НАБЛЮДЕНИЕ", L"ТРАССИРОВКА",
        L"ЛОГ", L"СООБЩЕНИЕ", L"ПРЕДУПРЕЖДЕНИЕ", L"СБОЙ", L"ОТКАЗ",
        
        // Разработка ПО
        L"РАЗРАБОТКА", L"ПРОЕКТИРОВАНИЕ", L"АРХИТЕКТУРА", L"ДИЗАЙН", L"ШАБЛОН",
        L"ПРОГРАММИСТ", L"РАЗРАБОТЧИК", L"ИНЖЕНЕР", L"АНАЛИТИК", L"ТЕСТИРОВЩИК",
        L"СПЕЦИФИКАЦИЯ", L"ТРЕБОВАНИЕ", L"ЗАДАЧА", L"ФУНКЦИОНАЛ", L"МОДУЛЬНОСТЬ",
        
        // Алгоритмы и структуры
        L"СОРТИРОВКА", L"ПОИСК", L"ФИЛЬТРАЦИЯ", L"МАРШРУТИЗАЦИЯ", L"ШИФРОВАНИЕ",
        L"ДЕШИФРОВАНИЕ", L"СЖАТИЕ", L"КОМПРЕССИЯ", L"ХЕШИРОВАНИЕ", L"ИНДЕКС",
        
        // Базы данных
        L"БАЗА", L"ТАБЛИЦА", L"ЗАПРОС", L"ОТЧЁТ", L"ТРАНЗАКЦИЯ",
        L"ИНДЕКС", L"КЛЮЧ", L"СВЯЗЬ", L"НОРМАЛИЗАЦИЯ",
        L"СЕРВЕР", L"КЛИЕНТ", L"НОСИТЕЛЬ",
        
        // Сети и веб
        L"СЕТЬ", L"ПРОТОКОЛ", L"ПОРТ", L"АДРЕС", L"ПАКЕТ",
        L"СЕРВЕР", L"КЛИЕНТ", L"БРАУЗЕР", L"ЗАПРОС", L"ОТВЕТ",
        L"СТРАНИЦА", L"САЙТ", L"ПРИЛОЖЕНИЕ", L"ИНТЕРНЕТ",
        
        // ООП
        L"НАСЛЕДОВАНИЕ", L"ИНКАПСУЛЯЦИЯ", L"ПОЛИМОРФИЗМ",
        L"ИНТЕРФЕЙС", L"ВИРТУАЛЬНЫЙ", L"СТАТИЧЕСКИЙ",
        L"ДИНАМИЧЕСКИЙ", L"ПЕРЕГРУЗКА", L"ПЕРЕОПРЕДЕЛЕНИЕ", L"КОНСТРУКТОР", L"ДЕСТРУКТОР",
        
        // Компиляция и выполнение
        L"КОМПИЛЯТОР", L"ИНТЕРПРЕТАТОР", L"КОМПИЛЯЦИЯ", L"ТРАНСЛЯЦИЯ", L"АССЕМБЛЕР",
        L"ЗАГРУЗЧИК", L"ВЫПОЛНЕНИЕ", L"ЗАПУСК",
        L"БИНАРНЫЙ", L"ИСХОДНЫЙ", L"ПРОМЕЖУТОЧНЫЙ", L"ОПТИМИЗАЦИЯ", L"ГЕНЕРАЦИЯ",
        
        // Инструменты разработки
        L"РЕДАКТОР", L"СРЕДА", L"ИНСТРУМЕНТ", L"УТИЛИТА", L"СКРИПТ",
        L"МАКРОС", L"ПЛАГИН", L"РАСШИРЕНИЕ", L"МОДУЛЬ", L"КОМПОНЕНТ",
        L"ФРЕЙМВОРК", L"БИБЛИОТЕКА", L"ПАКЕТ", L"МЕНЕДЖЕР", L"РЕПОЗИТОРИЙ",
        
        // Версионирование
        L"ВЕРСИЯ", L"ВЕТКА", L"СЛИЯНИЕ",
        L"РЕПОЗИТОРИЙ", L"КЛОНИРОВАНИЕ", L"ОБНОВЛЕНИЕ", L"ОТКАТ",
        L"КОНФЛИКТ", L"РАЗРЕШЕНИЕ", L"ИСТОРИЯ", L"ЖУРНАЛ", L"ИЗМЕНЕНИЕ",
        
        // Тестирование
        L"ТЕСТ", L"ТЕСТИРОВАНИЕ", L"ИНТЕГРАЦИЯ", L"СИСТЕМНЫЙ",
        L"РЕГРЕССИЯ", L"НАГРУЗКА", L"ПРОИЗВОДИТЕЛЬНОСТЬ", L"БЕЗОПАСНОСТЬ", L"УЯЗВИМОСТЬ",
        L"ПРОВЕРКА",
        
        // Безопасность
        L"БЕЗОПАСНОСТЬ", L"АУТЕНТИФИКАЦИЯ", L"АВТОРИЗАЦИЯ", L"ШИФРОВАНИЕ", L"КЛЮЧ",
        L"СЕРТИФИКАТ", L"ПОДПИСЬ", L"ХЕШ", L"СОЛЬ", L"ТОКЕН",
        L"СЕССИЯ", L"КУКИ", L"БРАНДМАУЭР", L"ФИЛЬТР", L"ЗАЩИТА",
        
        // Производительность
        L"БЫСТРОДЕЙСТВИЕ", L"ПРОИЗВОДИТЕЛЬНОСТЬ", L"ОПТИМИЗАЦИЯ", L"КЭШИРОВАНИЕ", L"КЭШ",
        L"БУФЕРИЗАЦИЯ", L"ПАРАЛЛЕЛИЗМ", L"МНОГОПОТОЧНОСТЬ", L"АСИНХРОННОСТЬ", L"СИНХРОНИЗАЦИЯ",
        L"БЛОКИРОВКА", L"ОЧЕРЕДЬ", L"ПУЛ",
        
        // Архитектура
        L"АРХИТЕКТУРА", L"ПАТТЕРН", L"ШАБЛОН", L"ПРОЕКТИРОВАНИЕ", L"МОДЕЛЬ",
        L"ПРЕДСТАВЛЕНИЕ", L"КОНТРОЛЛЕР", L"СЛОЙ", L"УРОВЕНЬ", L"КОМПОНЕНТ",
        L"СЕРВИС", L"МИКРОСЕРВИС", L"МОНОЛИТ", L"РАСПРЕДЕЛЁННЫЙ", L"КЛАСТЕР",
        
        // DevOps
        L"РАЗВЁРТЫВАНИЕ", L"СБОРКА", L"БИЛД",
        L"НЕПРЕРЫВНОСТЬ", L"ИНТЕГРАЦИЯ", L"ДОСТАВКА",
        L"КОНТЕЙНЕР", L"ВИРТУАЛИЗАЦИЯ", L"ОБЛАКО",
        
        // Разное
        L"ИНТЕРФЕЙС", L"ПОЛЬЗОВАТЕЛЬ", L"ГРАФИЧЕСКИЙ", L"КОМАНДНЫЙ", L"ТЕКСТОВЫЙ",
        L"НАСТРОЙКА", L"КОНФИГУРАЦИЯ", L"ПАРАМЕТР", L"ОПЦИЯ", L"ФЛАГ",
        L"РЕЖИМ", L"СОСТОЯНИЕ", L"СТАТУС", L"РЕЗУЛЬТАТ", L"ВОЗВРАТ"
    };
}

    void InitUI()
    {
        this->Text = L"Игра - Виселица";
        this->Size = System::Drawing::Size(750, 600);
        this->StartPosition = FormStartPosition::CenterParent;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        // Заголовок
        Label^ lt = gcnew Label();
        lt->Text = L"Виселица - Угадай слово!";
        lt->AutoSize = true;
        lt->Location = System::Drawing::Point(270, 10);
        lt->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, FontStyle::Bold);
        this->Controls->Add(lt);

        // Слово с пропущенными буквами (справа от виселицы)
        lblWord = gcnew Label();
        lblWord->Text = L"";
        lblWord->AutoSize = true;
        lblWord->Location = System::Drawing::Point(270, 50);
        lblWord->Font = gcnew System::Drawing::Font(L"Courier New", 16, FontStyle::Bold);
        this->Controls->Add(lblWord);

        // Статус
        lblStatus = gcnew Label();
        lblStatus->Text = L"";
        lblStatus->AutoSize = true;
        lblStatus->Location = System::Drawing::Point(270, 90);
        this->Controls->Add(lblStatus);

        // Виселица (ASCII art) — используем Label вместо TextBox
        lblGallows = gcnew Label();
        lblGallows->Text = L"";
        lblGallows->Size = System::Drawing::Size(220, 200);
        lblGallows->Location = System::Drawing::Point(20, 40);
        lblGallows->Font = gcnew System::Drawing::Font(L"Courier New", 12);
        lblGallows->BorderStyle = BorderStyle::FixedSingle;
        lblGallows->TextAlign = ContentAlignment::TopLeft;
        this->Controls->Add(lblGallows);

        // Панель с кнопками букв — сдвинута вниз и вправо
        panelButtons = gcnew Panel();
        panelButtons->Size = System::Drawing::Size(520, 180);
        panelButtons->Location = System::Drawing::Point(20, 270);
        this->Controls->Add(panelButtons);

        // Кнопки русского алфавита (33 буквы: 3 ряда по 11)
        String^ alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
        int btnX = 0, btnY = 0;
        int btnSize = 42;
        int spacing = 47;
        int buttonsPerRow = 11;

        for (int i = 0; i < alphabet->Length; i++)
        {
            Button^ btn = gcnew Button();
            btn->Text = alphabet[i].ToString();
            btn->Size = System::Drawing::Size(btnSize, btnSize);
            btn->Location = System::Drawing::Point(btnX, btnY);
            btn->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, FontStyle::Bold);
            btn->Tag = alphabet[i].ToString();
            btn->Click += gcnew EventHandler(this, &GameForm::Letter_Click);
            panelButtons->Controls->Add(btn);

            btnX += spacing;
            if ((i + 1) % buttonsPerRow == 0)
            {
                btnX = 0;
                btnY += spacing + 5;
            }
        }

		// Кнопка новой игры
        btnNewGame = gcnew Button();
        btnNewGame->Text = L"Новая игра";
        btnNewGame->Size = System::Drawing::Size(150, 35);
        btnNewGame->Location = System::Drawing::Point(200, 460);
        btnNewGame->Click += gcnew EventHandler(this, &GameForm::NewGame_Click);
        this->Controls->Add(btnNewGame);

        // Кнопка "Как играть" (НОВАЯ)
        Button^ btnHelp = gcnew Button();
        btnHelp->Text = L"Как играть";
        btnHelp->Size = System::Drawing::Size(150, 35);
        btnHelp->Location = System::Drawing::Point(360, 460);
        btnHelp->Click += gcnew EventHandler(this, &GameForm::Help_Click);
        this->Controls->Add(btnHelp);
    }

	void Help_Click(Object^, EventArgs^)
{
    HelpForm^ help = gcnew HelpForm();
    help->ShowDialog(this);
}

    void NewGame()
    {
        Random^ rnd = gcnew Random();
        currentWord = words[rnd->Next(words->Length)];
        guessedLetters = L"";
        mistakes = 0;
        
        UpdateWordDisplay();
        UpdateGallows();
        lblStatus->Text = String::Format(L"Ошибок: {0} из {1}", mistakes, maxMistakes);
        
        for each (Control^ ctrl in panelButtons->Controls)
        {
            if (Button^ btn = dynamic_cast<Button^>(ctrl))
            {
                btn->Enabled = true;
            }
        }
    }

    void Letter_Click(Object^ sender, EventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        String^ letter = btn->Tag->ToString();
        btn->Enabled = false;

        if (currentWord->Contains(letter))
        {
            guessedLetters += letter;
            UpdateWordDisplay();
            
            if (IsWordGuessed())
            {
                lblStatus->Text = L"Поздравляем! Вы угадали слово!";
                DisableAllButtons();
            }
        }
        else
        {
            mistakes++;
            UpdateGallows();
            lblStatus->Text = String::Format(L"Ошибок: {0} из {1}", mistakes, maxMistakes);
            
            if (mistakes >= maxMistakes)
            {
                lblStatus->Text = String::Format(L"Вы проиграли! Слово было: {0}", currentWord);
                DrawFullGallows();
                DisableAllButtons();
            }
        }
    }

    bool IsWordGuessed()
    {
        for (int i = 0; i < currentWord->Length; i++)
        {
            String^ ch = currentWord[i].ToString();
            if (!guessedLetters->Contains(ch))
                return false;
        }
        return true;
    }

    void UpdateWordDisplay()
    {
        String^ display = L"";
        for (int i = 0; i < currentWord->Length; i++)
        {
            String^ ch = currentWord[i].ToString();
            if (guessedLetters->Contains(ch))
                display += ch + L" ";
            else
                display += L"_ ";
        }
        lblWord->Text = display->Trim();
    }

    void UpdateGallows()
    {
        String^ gallows = L"";
        
        switch (mistakes)
        {
        case 0:
            gallows = L"  +---+\n"
                    + L"  |   |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"=======";
            break;
        case 1:
            gallows = L"  +---+\n"
                    + L"  |   |\n"
                    + L"  O   |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"=======";
            break;
        case 2:
            gallows = L"  +---+\n"
                    + L"  |   |\n"
                    + L"  O   |\n"
                    + L"  |   |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"=======";
            break;
        case 3:
            gallows = L"  +---+\n"
                    + L"  |   |\n"
                    + L"  O   |\n"
                    + L" /|   |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"=======";
            break;
        case 4:
            gallows = L"  +---+\n"
                    + L"  |   |\n"
                    + L"  O   |\n"
                    + L" /|\\  |\n"
                    + L"      |\n"
                    + L"      |\n"
                    + L"=======";
            break;
        case 5:
            gallows = L"  +---+\n"
                    + L"  |   |\n"
                    + L"  O   |\n"
                    + L" /|\\  |\n"
                    + L" /    |\n"
                    + L"      |\n"
                    + L"=======";
            break;
        }
        
        lblGallows->Text = gallows;
    }

    void DrawFullGallows()
    {
        String^ gallows = L"  +---+\n"
                        + L"  |   |\n"
                        + L"  O   |\n"
                        + L" /|\\  |\n"
                        + L" / \\  |\n"
                        + L"      |\n"
                        + L"=======";
        lblGallows->Text = gallows;
    }

    void DisableAllButtons()
    {
        for each (Control^ ctrl in panelButtons->Controls)
        {
            if (Button^ btn = dynamic_cast<Button^>(ctrl))
                btn->Enabled = false;
        }
    }

    void NewGame_Click(Object^, EventArgs^)
    {
        NewGame();
    }
};

} // namespace