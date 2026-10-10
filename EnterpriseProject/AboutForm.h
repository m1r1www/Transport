// AboutForm.h — Форма "О программе"
#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace EnterpriseProject {

public ref class AboutForm : public Form
{
public:
    AboutForm()
    {
        Build();
    }

private:
    void Build()
    {
        this->Text = L"О программе";
        this->Size = System::Drawing::Size(600, 550);
        this->StartPosition = FormStartPosition::CenterParent;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        // Заголовок
        Label^ title = gcnew Label();
        title->Text = L"Справочник предприятий";
        title->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, FontStyle::Bold);
        title->AutoSize = true;
        title->Location = System::Drawing::Point(180, 15);
        this->Controls->Add(title);

        // Разделитель
        Label^ sep1 = gcnew Label();
        sep1->Text = L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
        sep1->AutoSize = true;
        sep1->Location = System::Drawing::Point(20, 50);
        this->Controls->Add(sep1);

        // Основная информация
        int y = 70;
        Label^ info = gcnew Label();
        info->Text = L"Версия: 1.0\n"
                   + L"Платформа: Windows Forms (WinForms)\n"
                   + L"Язык программирования: C++/CLI\n"
                   + L"Среда разработки: Microsoft Visual Studio 2010\n"
                   + L"Цель: Учебный проект по дисциплине «Программирование»";
        info->AutoSize = true;
        info->Location = System::Drawing::Point(30, y);
        this->Controls->Add(info);
        y += 90;

        Label^ sep2 = gcnew Label();
        sep2->Text = L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
        sep2->AutoSize = true;
        sep2->Location = System::Drawing::Point(20, y);
        this->Controls->Add(sep2);
        y += 25;

        // Описание функционала
        Label^ funcTitle = gcnew Label();
        funcTitle->Text = L"Функциональные возможности:";
        funcTitle->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, FontStyle::Bold);
        funcTitle->AutoSize = true;
        funcTitle->Location = System::Drawing::Point(30, y);
        this->Controls->Add(funcTitle);
        y += 25;

        Label^ func = gcnew Label();
        func->Text = L"• Авторизация пользователей (администратор/пользователь)\n"
                   + L"• Просмотр базы данных предприятий в табличном виде\n"
                   + L"• Добавление, редактирование и удаление записей (только админ)\n"
                   + L"• Фильтрация данных по всем полям (название, адрес, год,\n"
                   + L"  оборот, количество сотрудников, зарплата)\n"
                   + L"• Сортировка таблицы по любому столбцу\n"
                   + L"• Сохранение и загрузка данных из файла database.txt\n"
                   + L"• Мини-игра «Виселица» для отдыха";
        func->AutoSize = true;
        func->Location = System::Drawing::Point(40, y);
        this->Controls->Add(func);
        y += 130;

        Label^ sep3 = gcnew Label();
        sep3->Text = L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
        sep3->AutoSize = true;
        sep3->Location = System::Drawing::Point(20, y);
        this->Controls->Add(sep3);
        y += 25;

        // Технические детали
        Label^ techTitle = gcnew Label();
        techTitle->Text = L"Технические особенности:";
        techTitle->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, FontStyle::Bold);
        techTitle->AutoSize = true;
        techTitle->Location = System::Drawing::Point(30, y);
        this->Controls->Add(techTitle);
        y += 25;

        Label^ tech = gcnew Label();
        tech->Text = L"• Используется технология C++/CLI для работы с .NET Framework\n"
                   + L"• Графический интерфейс построен на Windows Forms\n"
                   + L"• Данные хранятся в текстовом файле с фиксированной шириной полей\n"
                   + L"• Формат файла: [name:23]|[year:4]|[addr:25]|[godo:13]|[emp:6]|[sal:11]\n"
                   + L"• Кодировка файла: UTF-8\n"
                   + L"• Максимальное количество записей: 1000\n"
                   + L"• Алгоритм сортировки: метод пузырька";
        tech->AutoSize = true;
        tech->Location = System::Drawing::Point(40, y);
        this->Controls->Add(tech);
        y += 120;

        Label^ sep4 = gcnew Label();
        sep4->Text = L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
        sep4->AutoSize = true;
        sep4->Location = System::Drawing::Point(20, y);
        this->Controls->Add(sep4);
        y += 25;

        // Структура проекта
        Label^ structTitle = gcnew Label();
        structTitle->Text = L"Структура проекта:";
        structTitle->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, FontStyle::Bold);
        structTitle->AutoSize = true;
        structTitle->Location = System::Drawing::Point(30, y);
        this->Controls->Add(structTitle);
        y += 25;

        Label^ structure = gcnew Label();
        structure->Text = L"• LoginForm.h — форма авторизации\n"
                        + L"• MainForm.h — главная форма с таблицей и фильтрами\n"
                        + L"• AddEditForm.h — форма добавления/редактирования записи\n"
                        + L"• GameForm.h — игра «Виселица»\n"
                        + L"• Enterprise.h — структура данных и работа с файлом\n"
                        + L"• database.txt — файл базы данных";
        structure->AutoSize = true;
        structure->Location = System::Drawing::Point(40, y);
        this->Controls->Add(structure);
        y += 100;

        // Кнопка закрытия
        Button^ btnClose = gcnew Button();
        btnClose->Text = L"Закрыть";
        btnClose->Size = System::Drawing::Size(120, 30);
        btnClose->Location = System::Drawing::Point(240, y);
        btnClose->Click += gcnew EventHandler(this, &AboutForm::Close_Click);
        this->AcceptButton = btnClose;
        this->CancelButton = btnClose;
        this->Controls->Add(btnClose);
    }

    void Close_Click(Object^, EventArgs^)
    {
        this->Close();
    }
};

} // namespace