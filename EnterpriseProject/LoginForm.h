// LoginForm.h — Форма авторизации
#pragma once
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace EnterpriseProject {

public ref class LoginForm : public Form
{
private:
    TextBox^ tbLogin;
    TextBox^ tbPass;
    Label^ lblHint;

public:
    LoginForm()
    {
        Build();
    }

private:
    void Build()
    {
        this->Text = L"Справочник предприятий — Вход";
        this->Size = System::Drawing::Size(400, 320);
        this->StartPosition = FormStartPosition::CenterScreen;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
        this->MaximizeBox = false;

        int y = 30;

        Label^ lt = gcnew Label();
        lt->Text = L"Справочник предприятий";
        lt->AutoSize = true;
        lt->Location = System::Drawing::Point(120, y);
        this->Controls->Add(lt);
        y += 40;

        Label^ ll = gcnew Label();
        ll->Text = L"Логин:";
        ll->Location = System::Drawing::Point(40, y);
        ll->AutoSize = true;
        this->Controls->Add(ll);
        y += 22;

        tbLogin = gcnew TextBox();
        tbLogin->Location = System::Drawing::Point(40, y);
        tbLogin->Size = System::Drawing::Size(310, 20);
        this->Controls->Add(tbLogin);
        y += 36;

        Label^ lp = gcnew Label();
        lp->Text = L"Пароль:";
        lp->Location = System::Drawing::Point(40, y);
        lp->AutoSize = true;
        this->Controls->Add(lp);
        y += 22;

        tbPass = gcnew TextBox();
        tbPass->Location = System::Drawing::Point(40, y);
        tbPass->Size = System::Drawing::Size(310, 20);
        tbPass->PasswordChar = '*';
        this->Controls->Add(tbPass);
        y += 36;

        lblHint = gcnew Label();
        lblHint->Text = L"";
        lblHint->AutoSize = true;
        lblHint->Location = System::Drawing::Point(40, y);
        this->Controls->Add(lblHint);
        y += 24;

        Button^ bOK = gcnew Button();
        bOK->Text = L"Войти";
        bOK->Size = System::Drawing::Size(310, 30);
        bOK->Location = System::Drawing::Point(40, y);
        bOK->Click += gcnew EventHandler(this, &LoginForm::Login_Click);
        this->AcceptButton = bOK;
        this->Controls->Add(bOK);
        y += 40;

        Label^ lhint = gcnew Label();
        lhint->Text = L"admin / 12345 - администратор\nuser / 12345 - пользователь";
        lhint->AutoSize = true;
        lhint->Location = System::Drawing::Point(40, y);
        this->Controls->Add(lhint);

        this->ClientSize = System::Drawing::Size(390, y + 40);
    }

    void Login_Click(Object^, EventArgs^)
    {
        String^ log = tbLogin->Text->Trim();
        String^ pas = tbPass->Text;
        bool admin = false;

        if (log == L"admin" && pas == L"12345")
            admin = true;
        else if (log == L"user" && pas == L"12345")
            admin = false;
        else
        {
            lblHint->Text = L"Неверный логин или пароль.";
            tbPass->Clear();
            tbPass->Focus();
            return;
        }

        lblHint->Text = L"";
        MainForm^ mf = gcnew MainForm(admin);
        mf->FormClosed += gcnew FormClosedEventHandler(this, &LoginForm::OnMainClosed);
        mf->Show();
        this->Hide();
    }

    void OnMainClosed(Object^, FormClosedEventArgs^)
    {
        this->Show();
        tbLogin->Clear();
        tbPass->Clear();
        tbLogin->Focus();
    }
};

} // namespace