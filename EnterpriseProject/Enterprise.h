// Enterprise.h — структура предприятия и работа с файлом
#pragma once
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Windows::Forms;

#define MAX_ENT 1000

// Ширина полей (в символах) - как в transport.cpp
#define W_NAME    23
#define W_YEAR     4
#define W_ADDR    25
#define W_GODO    13
#define W_EMP      6
#define W_SAL     11

namespace EnterpriseProject {

struct Enterprise {
    wchar_t name[64];
    int year;
    wchar_t address[64];
    double turnover;
    int employees;
    double salary;
};

static Enterprise g_data[MAX_ENT];
static int g_count = 0;

// ── CRUD ─────────────────────────────────────────────────────────────────────
inline void ENT_Add(Enterprise e) {
    if (g_count < MAX_ENT) g_data[g_count++] = e;
}

inline void ENT_Delete(int idx) {
    if (idx < 0 || idx >= g_count) return;
    for (int i = idx; i < g_count - 1; i++) g_data[i] = g_data[i + 1];
    g_count--;
}

inline void ENT_Update(int idx, Enterprise e) {
    if (idx >= 0 && idx < g_count) g_data[idx] = e;
}

// ── Сортировка (пузырёк) ─────────────────────────────────────────────────────
inline void ENT_Sort(int field, bool asc) {
    for (int i = 0; i < g_count - 1; i++)
        for (int j = 0; j < g_count - 1 - i; j++) {
            Enterprise a = g_data[j];
            Enterprise b = g_data[j + 1];
            int cmp = 0;
            switch (field) {
                case 0: cmp = wcscmp(a.name, b.name); break;
                case 1: cmp = a.year - b.year; break;
                case 2: cmp = wcscmp(a.address, b.address); break;
                case 3: cmp = (a.turnover > b.turnover) - (a.turnover < b.turnover); break;
                case 4: cmp = a.employees - b.employees; break;
                case 5: cmp = (a.salary > b.salary) - (a.salary < b.salary); break;
            }
            bool sw = asc ? (cmp > 0) : (cmp < 0);
            if (sw) { g_data[j] = b; g_data[j+1] = a; }
        }
}

// ── Кодирование/декодирование записи в формат фиксированной ширины ──────────
// Формат: [name:23]|[year:4]|[addr:25]|[godo:13]|[emp:6]|[sal:11]|
// Текст выравнивается по левому краю (PadRight)
// Числа выравниваются по правому краю (PadLeft)

String^ EncodeRecord(const Enterprise& e) {
    String^ name = gcnew String(e.name);
    String^ addr = gcnew String(e.address);
    
    return name->PadRight(W_NAME) + L"|" +
           e.year.ToString(L"D4")->PadLeft(W_YEAR) + L"|" +
           addr->PadRight(W_ADDR) + L"|" +
           e.turnover.ToString(L"F2")->PadLeft(W_GODO) + L"|" +
           e.employees.ToString()->PadLeft(W_EMP) + L"|" +
           e.salary.ToString(L"F2")->PadLeft(W_SAL) + L"|";
}

bool DecodeRecord(String^ line, Enterprise& e) {
    if (line == nullptr || line->Length < 88) return false;
    
    try {
        array<wchar_t>^ separators = { L'|' };
        array<String^>^ parts = line->Split(separators, StringSplitOptions::RemoveEmptyEntries);
        
        if (parts->Length < 6) return false;
        
        msclr::interop::marshal_context context;
        
        // Название
        String^ name = parts[0]->Trim();
        const wchar_t* wname = context.marshal_as<const wchar_t*>(name);
        wcsncpy(e.name, wname, 63);
        e.name[63] = 0;
        
        // Год
        e.year = Int32::Parse(parts[1]->Trim());
        
        // Адрес
        String^ addr = parts[2]->Trim();
        const wchar_t* waddr = context.marshal_as<const wchar_t*>(addr);
        wcsncpy(e.address, waddr, 63);
        e.address[63] = 0;
        
        // Оборот
        e.turnover = Double::Parse(parts[3]->Trim());
        
        // Сотрудники
        e.employees = Int32::Parse(parts[4]->Trim());
        
        // Зарплата
        e.salary = Double::Parse(parts[5]->Trim());
        
        return true;
    }
    catch (Exception^) {
        return false;
    }
}

// ── СОХРАНЕНИЕ В ФАЙЛ ────────────────────────────────────────────────────────
inline void ENT_SaveToFile(String^ filename) {
    try {
        StreamWriter^ writer = gcnew StreamWriter(filename, false, Encoding::UTF8);
        
        for (int i = 0; i < g_count; i++) {
            writer->WriteLine(EncodeRecord(g_data[i]));
        }
        
        writer->Close();
    }
    catch (Exception^ e) {
        MessageBox::Show(
            String::Format(L"Ошибка сохранения файла:\n{0}", e->Message),
            L"Ошибка",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error);
    }
}

// ── ЗАГРУЗКА ИЗ ФАЙЛА ────────────────────────────────────────────────────────
inline bool ENT_LoadFromFile(String^ filename) {
    try {
        if (!File::Exists(filename)) return false;
        
        StreamReader^ reader = gcnew StreamReader(filename, Encoding::UTF8);
        g_count = 0;
        
        String^ line;
        while ((line = reader->ReadLine()) != nullptr) {
            if (line->Trim()->Length == 0) continue;
            
            if (g_count >= MAX_ENT) break;
            
            Enterprise e;
            memset(&e, 0, sizeof(e));
            
            if (DecodeRecord(line, e)) {
                g_data[g_count++] = e;
            }
        }
        
        reader->Close();
        return true;
    }
    catch (Exception^ e) {
        MessageBox::Show(
            String::Format(L"Ошибка загрузки файла:\n{0}", e->Message),
            L"Ошибка",
            MessageBoxButtons::OK,
            MessageBoxIcon::Error);
        g_count = 0;
        return false;
    }
}

// ── Тестовые данные ──────────────────────────────────────────────────────────
inline void ENT_LoadSample() {
    const wchar_t* names[] = { L"ООО Кондитер", L"АО РосМаш", L"ИП Стройград", L"ЗАО ТехПром", 
                               L"ООО АлтайМолоко", L"ПАО Сибирьэнерго", L"ООО МедиаПресс", L"АО АлтайАгро" };
    const wchar_t* addrs[] = { L"Москва, Тихомирова, 2", L"Барнаул, Ленина, 45", L"Новосибирск, Мира, 12", 
                               L"Екатеринбург, Победы, 3", L"Барнаул, Советская, 78", L"Новосибирск, Горького, 1", 
                               L"Москва, Садовая, 5", L"Бийск, Зеленая, 22" };
    int years[] = { 2010, 1998, 2015, 1985, 2002, 1970, 2018, 1993 };
    double turnovers[] = { 10000, 250000, 5500, 500000, 35000, 1200000, 3200, 78000 };
    int emps[] = { 15, 320, 8, 820, 110, 2100, 12, 430 };
    double sals[] = { 50000, 68000, 42000, 75000, 45000, 88000, 60000, 38000 };
    
    for (int i = 0; i < 8; i++) {
        Enterprise e; memset(&e, 0, sizeof(e));
        wcsncpy(e.name, names[i], 63); wcsncpy(e.address, addrs[i], 63);
        e.year = years[i]; e.turnover = turnovers[i]; e.employees = emps[i]; e.salary = sals[i];
        ENT_Add(e);
    }
}

} // namespace