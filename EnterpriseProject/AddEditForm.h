// AddEditForm.h — Форма добавления / редактирования предприятия
#pragma once
#include "Enterprise.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

namespace EnterpriseProject {

public ref class AddEditForm : public Form
{
public:
    bool IsOK;
    array<wchar_t>^ ResName;
    int ResYear;
    array<wchar_t>^ ResAddr;
    double ResTurnover;
    int ResEmployees;
    double ResSalary;

    AddEditForm(bool editMode, bool hasSource, String^ srcName, int srcYear, 
                String^ srcAddr, double srcTurnover, int srcEmployees, double srcSalary)
    {
        IsOK = false;
        ResName = gcnew array<wchar_t>(64);
        ResAddr = gcnew array<wchar_t>(64);
        Build(editMode, hasSource, srcName, srcYear, srcAddr, srcTurnover, srcEmployees, srcSalary);
    }

private:
    TextBox^ tbName;
    TextBox^ tbYear;
    TextBox^ tbAddr;
    TextBox^ tbTurn;
    TextBox^ tbEmp;
    TextBox^ tbSal;

    void Build(bool editMode, bool hasSource, String^ srcName, int srcYear, 
               String^ srcAddr, double srcTurnover, int srcEmployees, double srcSalary)
    {
        this->Text = editMode ? L"Редактирование предприятия" : L"Добавление предприятия";
        this->Size = System::Drawing::Size(440, 400);  // Увеличил высоту с 360 до 400
        this->StartPosition = FormStartPosition::CenterParent;
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;

        int y = 20;
        
        Label^ l1 = gcnew Label();
        l1->Text = L"Название предприятия:";
        l1->Location = System::Drawing::Point(20, y);
        l1->AutoSize = true;
        this->Controls->Add(l1);
        
        tbName = gcnew TextBox();
        tbName->Location = System::Drawing::Point(20, y + 20);
        tbName->Size = System::Drawing::Size(380, 20);
        tbName->MaxLength = 63;
        this->Controls->Add(tbName);
        y += 50;

        Label^ l2 = gcnew Label();
        l2->Text = L"Год основания (1800-2026):";
        l2->Location = System::Drawing::Point(20, y);
        l2->AutoSize = true;
        this->Controls->Add(l2);
        
        tbYear = gcnew TextBox();
        tbYear->Location = System::Drawing::Point(20, y + 20);
        tbYear->Size = System::Drawing::Size(100, 20);
        this->Controls->Add(tbYear);
        y += 50;

        Label^ l3 = gcnew Label();
        l3->Text = L"Адрес:";
        l3->Location = System::Drawing::Point(20, y);
        l3->AutoSize = true;
        this->Controls->Add(l3);
        
        tbAddr = gcnew TextBox();
        tbAddr->Location = System::Drawing::Point(20, y + 20);
        tbAddr->Size = System::Drawing::Size(380, 20);
        tbAddr->MaxLength = 63;
        this->Controls->Add(tbAddr);
        y += 50;

        Label^ l4 = gcnew Label();
        l4->Text = L"Годовой оборот (тыс. руб.):";
        l4->Location = System::Drawing::Point(20, y);
        l4->AutoSize = true;
        this->Controls->Add(l4);
        
        tbTurn = gcnew TextBox();
        tbTurn->Location = System::Drawing::Point(20, y + 20);
        tbTurn->Size = System::Drawing::Size(150, 20);
        this->Controls->Add(tbTurn);
        y += 50;

        Label^ l5 = gcnew Label();
        l5->Text = L"Число сотрудников:";
        l5->Location = System::Drawing::Point(20, y);
        l5->AutoSize = true;
        this->Controls->Add(l5);
        
        tbEmp = gcnew TextBox();
        tbEmp->Location = System::Drawing::Point(20, y + 20);
        tbEmp->Size = System::Drawing::Size(100, 20);
        this->Controls->Add(tbEmp);
        y += 50;

        Label^ l6 = gcnew Label();
        l6->Text = L"Средняя зарплата (руб.):";
        l6->Location = System::Drawing::Point(20, y);
        l6->AutoSize = true;
        this->Controls->Add(l6);
        
        tbSal = gcnew TextBox();
        tbSal->Location = System::Drawing::Point(20, y + 20);
        tbSal->Size = System::Drawing::Size(150, 20);
        this->Controls->Add(tbSal);

        if (hasSource)
        {
            tbName->Text = srcName;
            tbYear->Text = srcYear.ToString();
            tbAddr->Text = srcAddr;
            tbTurn->Text = srcTurnover.ToString("F2");
            tbEmp->Text = srcEmployees.ToString();
            tbSal->Text = srcSalary.ToString("F2");
        }

        Button^ bOK = gcnew Button();
        bOK->Text = L"Сохранить";
        bOK->Size = System::Drawing::Size(110, 30);
        bOK->Location = System::Drawing::Point(120, 340);  // Сместил с 300 на 340
        bOK->Click += gcnew EventHandler(this, &AddEditForm::OK_Click);
        this->AcceptButton = bOK;

        Button^ bCan = gcnew Button();
        bCan->Text = L"Отмена";
        bCan->Size = System::Drawing::Size(100, 30);
        bCan->Location = System::Drawing::Point(240, 340);  // Сместил с 300 на 340
        bCan->Click += gcnew EventHandler(this, &AddEditForm::Can_Click);
        this->CancelButton = bCan;

        this->Controls->Add(bOK);
        this->Controls->Add(bCan);
    }

    bool TryDbl(String^ s, double% v)
    {
        return Double::TryParse(
            s->Trim()->Replace(L",", L"."),
            System::Globalization::NumberStyles::Any,
            System::Globalization::CultureInfo::InvariantCulture, v);
    }

    void Err(String^ msg, Control^ ctrl)
    {
        MessageBox::Show(msg, L"Ошибка ввода", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        ctrl->Focus();
    }

    void OK_Click(Object^, EventArgs^)
    {
        String^ nm = tbName->Text->Trim();
        if (nm->Length == 0 || nm->Length > 63)
        {
            Err(L"Название: от 1 до 63 символов.", tbName);
            return;
        }

        int yr = 0;
        if (!Int32::TryParse(tbYear->Text->Trim(), yr) || yr < 1800 || yr > 2026)
        {
            Err(L"Год: целое число от 1800 до 2026.", tbYear);
            return;
        }

        String^ ad = tbAddr->Text->Trim();
        if (ad->Length == 0 || ad->Length > 63)
        {
            Err(L"Адрес: от 1 до 63 символов.", tbAddr);
            return;
        }

        double tv = 0;
        if (!TryDbl(tbTurn->Text, tv) || tv < 0 || tv > 1e9)
        {
            Err(L"Оборот: число >= 0.", tbTurn);
            return;
        }

        int em = 0;
        if (!Int32::TryParse(tbEmp->Text->Trim(), em) || em < 0 || em > 1000000)
        {
            Err(L"Сотрудников: 0 - 1000000.", tbEmp);
            return;
        }

        double sl = 0;
        if (!TryDbl(tbSal->Text, sl) || sl < 0 || sl > 1e7)
        {
            Err(L"Зарплата: число >= 0.", tbSal);
            return;
        }

        IntPtr pn = Marshal::StringToHGlobalUni(nm);
        IntPtr pa = Marshal::StringToHGlobalUni(ad);
        const wchar_t* cn = static_cast<const wchar_t*>(pn.ToPointer());
        const wchar_t* ca = static_cast<const wchar_t*>(pa.ToPointer());

        for (int i = 0; i < 63 && cn[i]; i++) ResName[i] = cn[i];
        ResName[63] = 0;
        for (int i = 0; i < 63 && ca[i]; i++) ResAddr[i] = ca[i];
        ResAddr[63] = 0;

        Marshal::FreeHGlobal(pn);
        Marshal::FreeHGlobal(pa);

        ResYear = yr;
        ResTurnover = tv;
        ResEmployees = em;
        ResSalary = sl;
        IsOK = true;
        this->DialogResult = System::Windows::Forms::DialogResult::OK;
        this->Close();
    }

    void Can_Click(Object^, EventArgs^)
    {
        this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
        this->Close();
    }
};

} // namespace