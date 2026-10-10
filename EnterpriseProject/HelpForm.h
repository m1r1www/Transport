// HelpForm.h — Форма "Как играть" для игры Виселица
#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace EnterpriseProject {

public ref class HelpForm : public Form
{
public:
    HelpForm()
    {
        Build();
    }

private:
    void Build()
    {
        this->Text = L"Как играть - Виселица";
        this->Size = System::Drawing::Size(500, 450);
        this->StartPosition = FormStartPosition::CenterParent;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        // Заголовок
        Label^ title = gcnew Label();
        title->Text = L"Правила игры «Виселица»";
        title->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, FontStyle::Bold);
        title->AutoSize = true;
        title->Location = System::Drawing::Point(130, 15);
        this->Controls->Add(title);

        // Разделитель
        Label^ sep1 = gcnew Label();
        sep1->Text = L"━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
        sep1->AutoSize = true;
        sep1->Location = System::Drawing::Point(20, 45);
        this->Controls->Add(sep1);

        // Правила
        int y = 65;
        Label^ rules = gcnew Label();
        rules->Text = L"Цель игры: угадать загаданное слово по буквам.\n\n"
                    + L"Правила:\n"
                    + L"1. Компьютер загадывает слово из списка\n"
                    + L"2. Вы видите количество букв в слове (прочерки)\n"
                    + L"3. Нажимайте на кнопки с буквами русского алфавита\n"
                    + L"4. Если буква есть в слове — она откроется\n"
                    + L"5. Если буквы нет — добавляется часть виселицы\n"
                    + L"6. Максимальное количество ошибок: 6\n\n"
                    + L"Этапы рисования виселицы:\n"
                    + L"• Ошибка 1: голова\n"
                    + L"• Ошибка 2: тело\n"
                    + L"• Ошибка 3: одна рука\n"
                    + L"• Ошибка 4: вторая рука\n"
                    + L"• Ошибка 5: одна нога\n"
                    + L"• Ошибка 6: вторая нога (проигрыш)\n\n"
                    + L"Условия победы:\n"
                    + L"• Угадать все буквы слова до 6 ошибок\n\n"
                    + L"Условия проигрыша:\n"
                    + L"• Сделать 6 ошибок — виселица построена полностью";
        rules->AutoSize = true;
        rules->Location = System::Drawing::Point(30, y);
        this->Controls->Add(rules);

        // Кнопка закрытия
        Button^ btnClose = gcnew Button();
        btnClose->Text = L"Понятно!";
        btnClose->Size = System::Drawing::Size(120, 30);
        btnClose->Location = System::Drawing::Point(190, 380);
        btnClose->Click += gcnew EventHandler(this, &HelpForm::Close_Click);
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