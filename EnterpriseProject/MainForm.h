// MainForm.h — Главная форма «Справочник предприятий»
#pragma once
#include "Enterprise.h"
#include "AddEditForm.h"
#include "GameForm.h"
#include "AboutForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

namespace EnterpriseProject {

// Вспомогательная: AddEditForm -> Enterprise
inline Enterprise FromForm(AddEditForm^ f)
{
    Enterprise e;
    memset(&e, 0, sizeof(e));
    for (int i = 0; i < 63 && f->ResName[i]; i++) e.name[i]    = f->ResName[i];
    for (int i = 0; i < 63 && f->ResAddr[i]; i++) e.address[i] = f->ResAddr[i];
    e.year      = f->ResYear;
    e.turnover  = f->ResTurnover;
    e.employees = f->ResEmployees;
    e.salary    = f->ResSalary;
    return e;
}

public ref class MainForm : public Form
{
public:
    MainForm(bool isAdmin)
    {
        _admin   = isAdmin;
        _sortCol = -1;
        _sortAsc = true;
        
        // Сбрасываем счетчик перед загрузкой
        g_count = 0;
        
        // ЗАГРУЗКА ИЗ ФАЙЛА ПРИ ЗАПУСКЕ
        String^ dbFile = Application::StartupPath + "\\database.txt";
        
        // Пытаемся загрузить из файла
        bool loaded = ENT_LoadFromFile(dbFile);
        
        // Если файл не загрузился или база пустая - загружаем тестовые данные
        if (!loaded || g_count == 0)
        {
            ENT_LoadSample();
            // Сохраняем тестовые данные в файл
            ENT_SaveToFile(dbFile);
        }
        
        Build();
        RepopGrid();
    }

private:
    bool _admin;
    int  _sortCol;
    bool _sortAsc;

    DataGridView^         grid;
    StatusStrip^          statusBar;
    ToolStripStatusLabel^ lblSt;

    // Фильтры
    TextBox^ fName;
    TextBox^ fAddr;
    TextBox^ fYearMin; TextBox^ fYearMax;
    TextBox^ fEmpMin;  TextBox^ fEmpMax;
    TextBox^ fSalMin;  TextBox^ fSalMax;
    TextBox^ fObMin;   TextBox^ fObMax;

    // ── Построение UI ────────────────────────────────────────────────────────
    void Build()
    {
        this->Text        = L"Справочник предприятий";
        this->Size        = System::Drawing::Size(1120, 680);
        this->StartPosition = FormStartPosition::CenterScreen;
        this->MinimumSize = System::Drawing::Size(900, 540);

        // Подписка на событие закрытия формы для автосохранения
        this->FormClosing += gcnew FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);

        // Левая панель
        Panel^ left = gcnew Panel();
        left->Dock = DockStyle::Left; 
        left->Width = 210;

        int by = 8;
        Button^ bAdd  = gcnew Button();
        bAdd->Text = L"Добавить";
        bAdd->Location = System::Drawing::Point(8, by);
        bAdd->Size = System::Drawing::Size(192, 30);
        by += 38;

        Button^ bEdit = gcnew Button();
        bEdit->Text = L"Изменить";
        bEdit->Location = System::Drawing::Point(8, by);
        bEdit->Size = System::Drawing::Size(192, 30);
        by += 38;

        Button^ bDel  = gcnew Button();
        bDel->Text = L"Удалить";
        bDel->Location = System::Drawing::Point(8, by);
        bDel->Size = System::Drawing::Size(192, 30);
        by += 38;

        Button^ bSort = gcnew Button();
        bSort->Text = L"Сортировка";
        bSort->Location = System::Drawing::Point(8, by);
        bSort->Size = System::Drawing::Size(192, 30);
        by += 46;

        // Добавление, изменение, удаление только для админа
        bAdd->Enabled  = _admin;
        bEdit->Enabled = _admin;
        bDel->Enabled  = _admin;
        // Сортировка доступна всем
        bSort->Enabled = true;

        bAdd->Click  += gcnew EventHandler(this, &MainForm::Add_Click);
        bEdit->Click += gcnew EventHandler(this, &MainForm::Edit_Click);
        bDel->Click  += gcnew EventHandler(this, &MainForm::Del_Click);
        bSort->Click += gcnew EventHandler(this, &MainForm::Sort_Click);

        left->Controls->Add(bAdd);
        left->Controls->Add(bEdit);
        left->Controls->Add(bDel);
        left->Controls->Add(bSort);

        // Группа фильтров
        GroupBox^ gb = gcnew GroupBox();
        gb->Text = L"Поиск / Фильтр";
        gb->Location = System::Drawing::Point(5, by);
        gb->Size = System::Drawing::Size(198, 380);

        int fy = 18;
        FRow1(gb, L"Название:",     fy, fName);    fy += 42;
        FRow1(gb, L"Адрес:",        fy, fAddr);    fy += 42;
        FRow2(gb, L"Год от / до:",  fy, fYearMin, fYearMax); fy += 48;
        FRow2(gb, L"Сотрудников:",  fy, fEmpMin,  fEmpMax);  fy += 48;
        FRow2(gb, L"Зарплата:",     fy, fSalMin,  fSalMax);  fy += 48;
        FRow2(gb, L"Оборот:",       fy, fObMin,   fObMax);   fy += 48;

        Button^ bFind  = gcnew Button();
        bFind->Text = L"Найти";
        bFind->Location = System::Drawing::Point(8, fy);
        bFind->Size = System::Drawing::Size(88, 26);
        
        Button^ bReset = gcnew Button();
        bReset->Text = L"Сброс";
        bReset->Location = System::Drawing::Point(102, fy);
        bReset->Size = System::Drawing::Size(82, 26);
        
        bFind->Click  += gcnew EventHandler(this, &MainForm::Find_Click);
        bReset->Click += gcnew EventHandler(this, &MainForm::Reset_Click);
        gb->Controls->Add(bFind);
        gb->Controls->Add(bReset);
        gb->Height = fy + 36;
        left->Controls->Add(gb);

        // Кнопки внизу (Игра, О программе, Выход)
        by = gb->Bottom + 10;
        
        Button^ bGame = gcnew Button();
        bGame->Text = L"Мини-игра";
        bGame->Location = System::Drawing::Point(8, by);
        bGame->Size = System::Drawing::Size(192, 30);
        bGame->Click += gcnew EventHandler(this, &MainForm::OpenGame);
        left->Controls->Add(bGame);
        by += 38;

        Button^ bAbout = gcnew Button();
        bAbout->Text = L"О программе";
        bAbout->Location = System::Drawing::Point(8, by);
        bAbout->Size = System::Drawing::Size(192, 30);
        bAbout->Click += gcnew EventHandler(this, &MainForm::ShowAbout);
        left->Controls->Add(bAbout);
        by += 38;

        Button^ bBack = gcnew Button();
        bBack->Text = L"Выход";
        bBack->Location = System::Drawing::Point(8, by);
        bBack->Size = System::Drawing::Size(192, 30);
        bBack->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Left);
        bBack->Click += gcnew EventHandler(this, &MainForm::Back_Click);
        left->Controls->Add(bBack);

        // DataGridView
        grid = gcnew DataGridView();
        grid->Dock = DockStyle::Fill;
        grid->ReadOnly = true;
        grid->AllowUserToAddRows    = false;
        grid->AllowUserToDeleteRows = false;
        grid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        grid->MultiSelect   = false;
        grid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        grid->RowHeadersVisible = false;
        grid->CellDoubleClick +=
            gcnew DataGridViewCellEventHandler(this, &MainForm::Grid_DblClick);
        grid->ColumnHeaderMouseClick +=
            gcnew DataGridViewCellMouseEventHandler(this, &MainForm::Grid_HeaderClick);

        AddCol(L"Название",      L"c0");
        AddCol(L"Год основания", L"c1");
        AddCol(L"Адрес",         L"c2");
        AddCol(L"Оборот (тыс.)", L"c3");
        AddCol(L"Сотрудников",   L"c4");
        AddCol(L"Ср. зарплата",  L"c5");

        // Статусбар
        statusBar = gcnew StatusStrip();
        lblSt = gcnew ToolStripStatusLabel();
        statusBar->Items->Add(lblSt);

        // Компоновка
        this->Controls->Add(grid);
        this->Controls->Add(left);
        this->Controls->Add(statusBar);
    }

    // ── Вспомогательные ────────────────────────────────────────────────────

    void AddCol(String^ hdr, String^ name)
    {
        DataGridViewTextBoxColumn^ c = gcnew DataGridViewTextBoxColumn();
        c->HeaderText = hdr; 
        c->Name = name;
        c->SortMode = DataGridViewColumnSortMode::Programmatic;
        grid->Columns->Add(c);
    }

    void FRow1(GroupBox^ gb, String^ lbl, int y, TextBox^% tb)
    {
        Label^ l = gcnew Label(); 
        l->Text = lbl;
        l->Location = System::Drawing::Point(6, y); 
        l->AutoSize = true;
        gb->Controls->Add(l);
        tb = gcnew TextBox();
        tb->Location = System::Drawing::Point(6, y+15);
        tb->Size = System::Drawing::Size(182, 20);
        gb->Controls->Add(tb);
    }

    void FRow2(GroupBox^ gb, String^ lbl, int y, TextBox^% t1, TextBox^% t2)
    {
        Label^ l = gcnew Label(); 
        l->Text = lbl;
        l->Location = System::Drawing::Point(6, y); 
        l->AutoSize = true;
        gb->Controls->Add(l);
        t1 = gcnew TextBox();
        t1->Location = System::Drawing::Point(6, y+15);
        t1->Size = System::Drawing::Size(84, 20);
        t1->Text = L"";
        gb->Controls->Add(t1);
        t2 = gcnew TextBox();
        t2->Location = System::Drawing::Point(102, y+15);
        t2->Size = System::Drawing::Size(84, 20);
        t2->Text = L"";
        gb->Controls->Add(t2);
    }

    // ── Отображение ──────────────────────────────────────────────────────────

    void RepopGrid()
    {
        String^ fn = fName ? fName->Text->Trim() : L"";
        String^ fa = fAddr ? fAddr->Text->Trim() : L"";
        double yrMn = Dbl(fYearMin, 0),    yrMx = Dbl(fYearMax, 9999);
        double eMn  = Dbl(fEmpMin,  0),    eMx  = Dbl(fEmpMax,  1e9);
        double sMn  = Dbl(fSalMin,  0),    sMx  = Dbl(fSalMax,  1e9);
        double oMn  = Dbl(fObMin,   0),    oMx  = Dbl(fObMax,   1e12);

        grid->Rows->Clear();
        int shown = 0;
        for (int i = 0; i < g_count; i++)
        {
            Enterprise z = g_data[i];
            String^ sn = gcnew String(z.name);
            String^ sa = gcnew String(z.address);

            if (fn->Length > 0 &&
                sn->IndexOf(fn, StringComparison::OrdinalIgnoreCase) < 0) continue;
            if (fa->Length > 0 &&
                sa->IndexOf(fa, StringComparison::OrdinalIgnoreCase) < 0) continue;
            if (z.year      < (int)yrMn || z.year      > (int)yrMx) continue;
            if (z.employees < (int)eMn  || z.employees > (int)eMx)  continue;
            if (z.salary    < sMn       || z.salary    > sMx)       continue;
            if (z.turnover  < oMn       || z.turnover  > oMx)       continue;

            int r = grid->Rows->Add();
            grid->Rows[r]->Tag           = i;
            grid->Rows[r]->Cells[0]->Value = sn;
            grid->Rows[r]->Cells[1]->Value = z.year;
            grid->Rows[r]->Cells[2]->Value = sa;
            grid->Rows[r]->Cells[3]->Value = z.turnover.ToString(L"F2");
            grid->Rows[r]->Cells[4]->Value = z.employees;
            grid->Rows[r]->Cells[5]->Value = z.salary.ToString(L"F2");
            shown++;
        }

        lblSt->Text = String::Format(
            L"Всего записей: {0}   |   Показано: {1}   |   Режим: {2}",
            g_count, shown, _admin ? L"Администратор" : L"Пользователь");
    }

    double Dbl(TextBox^ tb, double def)
    {
        if (!tb || tb->Text->Trim()->Length == 0) return def;
        double v = 0;
        if (Double::TryParse(tb->Text->Trim()->Replace(L",", L"."),
            System::Globalization::NumberStyles::Any,
            System::Globalization::CultureInfo::InvariantCulture, v)) return v;
        return def;
    }

    int SelIdx()
    {
        if (grid->SelectedRows->Count == 0) return -1;
        Object^ tag = grid->SelectedRows[0]->Tag;
        return (tag != nullptr) ? (int)tag : -1;
    }

    void UpdateGlyphs(int col, bool asc)
    {
        for (int i = 0; i < grid->Columns->Count; i++)
            grid->Columns[i]->HeaderCell->SortGlyphDirection = SortOrder::None;
        grid->Columns[col]->HeaderCell->SortGlyphDirection =
            asc ? SortOrder::Ascending : SortOrder::Descending;
    }

    // ── Обработчики ──────────────────────────────────────────────────────────

    void Add_Click(Object^, EventArgs^)
    {
        AddEditForm^ f = gcnew AddEditForm(false, false, L"", 2000, L"", 0, 0, 0);
        if (f->ShowDialog(this) == System::Windows::Forms::DialogResult::OK && f->IsOK)
        {
            ENT_Add(FromForm(f));
            ENT_SaveToFile(Application::StartupPath + "\\database.txt");
            RepopGrid();
        }
    }

    void Edit_Click(Object^, EventArgs^)
    {
        int idx = SelIdx();
        if (idx < 0) { Info(L"Выберите запись для редактирования."); return; }
        Enterprise z = g_data[idx];
        AddEditForm^ f = gcnew AddEditForm(true, true,
            gcnew String(z.name), z.year, gcnew String(z.address),
            z.turnover, z.employees, z.salary);
        if (f->ShowDialog(this) == System::Windows::Forms::DialogResult::OK && f->IsOK)
        {
            ENT_Update(idx, FromForm(f));
            ENT_SaveToFile(Application::StartupPath + "\\database.txt");
            RepopGrid();
        }
    }

    void Del_Click(Object^, EventArgs^)
    {
        int idx = SelIdx();
        if (idx < 0) { Info(L"Выберите запись для удаления."); return; }
        String^ nm = gcnew String(g_data[idx].name);
        if (MessageBox::Show(
            String::Format(L"Удалить запись \"{0}\"?", nm),
            L"Подтверждение",
            MessageBoxButtons::YesNo, MessageBoxIcon::Question)
            == System::Windows::Forms::DialogResult::Yes)
        {
            ENT_Delete(idx);
            ENT_SaveToFile(Application::StartupPath + "\\database.txt");
            RepopGrid();
        }
    }

    void Sort_Click(Object^, EventArgs^)
    {
        Form^ fd = gcnew Form();
        fd->Text = L"Сортировка"; 
        fd->Size = System::Drawing::Size(280, 230);
        fd->StartPosition = FormStartPosition::CenterParent;
        fd->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        fd->MaximizeBox = false; 
        fd->MinimizeBox = false;

        Label^ lf = gcnew Label(); 
        lf->Text = L"Поле сортировки:";
        lf->Location = System::Drawing::Point(14, 14); 
        lf->AutoSize = true;

        ComboBox^ cb = gcnew ComboBox();
        cb->DropDownStyle = ComboBoxStyle::DropDownList;
        cb->Location = System::Drawing::Point(14, 34); 
        cb->Size = System::Drawing::Size(240, 22);
        cb->Items->Add(L"Название"); 
        cb->Items->Add(L"Год основания");
        cb->Items->Add(L"Адрес");    
        cb->Items->Add(L"Оборот");
        cb->Items->Add(L"Сотрудников"); 
        cb->Items->Add(L"Зарплата");
        cb->SelectedIndex = 0;

        RadioButton^ rA = gcnew RadioButton();
        rA->Text = L"По возрастанию"; 
        rA->Checked = true;
        rA->Location = System::Drawing::Point(14, 72); 
        rA->AutoSize = true;
        
        RadioButton^ rD = gcnew RadioButton();
        rD->Text = L"По убыванию";
        rD->Location = System::Drawing::Point(14, 98); 
        rD->AutoSize = true;

        Button^ bOK = gcnew Button();
        bOK->Text = L"Сортировать"; 
        bOK->Size = System::Drawing::Size(120, 30);
        bOK->Location = System::Drawing::Point(70, 140);
        bOK->DialogResult = System::Windows::Forms::DialogResult::OK;
        fd->AcceptButton = bOK;

        fd->Controls->Add(lf); 
        fd->Controls->Add(cb);
        fd->Controls->Add(rA); 
        fd->Controls->Add(rD); 
        fd->Controls->Add(bOK);

        if (fd->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
        {
            int col = cb->SelectedIndex;
            bool asc = rA->Checked;
            ENT_Sort(col, asc);
            _sortCol = col; 
            _sortAsc = asc;
            UpdateGlyphs(col, asc);
            RepopGrid();
        }
    }

    void Find_Click(Object^,  EventArgs^) { RepopGrid(); }

    void Reset_Click(Object^, EventArgs^)
    {
        fName->Clear(); fAddr->Clear();
        fYearMin->Clear(); fYearMax->Clear();
        fEmpMin->Clear();  fEmpMax->Clear();
        fSalMin->Clear();  fSalMax->Clear();
        fObMin->Clear();   fObMax->Clear();
        RepopGrid();
    }

    void Back_Click(Object^, EventArgs^) { this->Close(); }

    void Grid_DblClick(Object^, DataGridViewCellEventArgs^ e)
    {
        if (_admin && e->RowIndex >= 0) Edit_Click(nullptr, nullptr);
    }

    void Grid_HeaderClick(Object^, DataGridViewCellMouseEventArgs^ e)
    {
        int col = e->ColumnIndex;
        if (_sortCol == col) _sortAsc = !_sortAsc;
        else { _sortCol = col; _sortAsc = true; }
        ENT_Sort(col, _sortAsc);
        UpdateGlyphs(col, _sortAsc);
        RepopGrid();
    }

    void OpenGame(Object^, EventArgs^)
    {
        GameForm^ g = gcnew GameForm();
        g->ShowDialog(this);
    }

    void ShowAbout(Object^, EventArgs^)
{
    AboutForm^ about = gcnew AboutForm();
    about->ShowDialog(this);
}

    void Info(String^ msg)
    {
        MessageBox::Show(msg, L"Информация",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    // ── Обработчик закрытия формы (Автосохранение) ───────────────────────────
    void MainForm_FormClosing(Object^ sender, FormClosingEventArgs^ e)
    {
        // Сохраняем только если есть данные
        if (g_count > 0)
        {
            ENT_SaveToFile(Application::StartupPath + "\\database.txt");
        }
    }
};

} // namespace