#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

#define DB_FILE   "zavod.txt"
#define TMP_FILE  "zavod.tmp"

#define W_NUM      3
#define W_NAME    23
#define W_YEAR     4
#define W_ADDR    25
#define W_GODO    13
#define W_EMP      6
#define W_SAL     11


#define RECORD_SIZE  91   // байт на одну запись включая \r\n 

 // структура одной записи (только в памяти) 
typedef struct {
    char   name[W_NAME + 1];
    int    year;
    char   address[W_ADDR + 1];
    double godoborot;
    int    employees;
    double avg_salary;
} Zavod;


 // Вспомогательные функции ввода / строковые:
 // Безопасный ввод целого числа в диапазоне [min, max] 
int safe_int(const char* prompt, int min, int max)
{
    int val, ok;
    do {
        printf("%s", prompt);
        ok = scanf("%d", &val);
        { int c; while ((c = getchar()) != '\n' && c != EOF); }
        if (ok != 1)
            printf("  Ошибка: введите целое число.\n");
        else if (val < min || val > max) {
            printf("  Ошибка: число от %d до %d.\n", min, max);
            ok = 0;
        }
    } while (ok != 1);
    return val;
}

// Безопасный ввод вещественного числа в диапазоне [min, max] 
double safe_double(const char* prompt, double min, double max)
{
    double val; int ok;
    do {
        printf("%s", prompt);
        ok = scanf("%lf", &val);
        { int c; while ((c = getchar()) != '\n' && c != EOF); }
        if (ok != 1)
            printf("  Ошибка: введите число.\n");
        else if (val < min || val > max) {
            printf("  Ошибка: значение от %.2f до %.2f.\n", min, max);
            ok = 0;
        }
    } while (ok != 1);
    return val;
}

// Удаляет пробелы в начале и конце строки 
void trim(char* s)
{
    int st = 0;
    while (s[st] == ' ') st++;
    int en = (int)strlen(s) - 1;
    while (en >= st && s[en] == ' ') en--;
    int j = 0;
    for (int i = st; i <= en; i++) s[j++] = s[i];
    s[j] = '\0';
}

// Возвращает 1, если строка содержит хотя бы одну букву 
int has_letter(const char* s)
{
    for (int i = 0; s[i]; i++) {
        unsigned char c = (unsigned char)s[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 0xC0) return 1;
    }
    return 0;
}

// Проверяет допустимость адреса 
int valid_address(const char* s)
{
    int letter = 0;
    for (int i = 0; s[i]; i++) {
        unsigned char c = (unsigned char)s[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
            c == ' ' || c == '.' || c == ',' || c == '-' || c == '/' || c == 0xB9/*№*/) {
        }
        else if (c >= 0xC0) {}
        else return 0;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 0xC0) letter = 1;
    }
    return letter;
}

// Перевод строки в нижний регистр cp1251 
void str_lower(const char* src, char* dst, int maxlen)
{
    int i;
    for (i = 0; src[i] && i < maxlen - 1; i++) {
        unsigned char c = (unsigned char)src[i];
        if (c >= 'A' && c <= 'Z')         dst[i] = c + 32;
        else if (c >= 0xC0 && c <= 0xDF)       dst[i] = c + 32;
        else if (c == 0xA8)                dst[i] = 0xB8;
        else                             dst[i] = c;
    }
    dst[i] = '\0';
}

void cls(void) { system("cls"); }

/* ============================================================
 *  Кодирование / декодирование строки файла
 *
 *  Формат одной строки (91 байт = 89 печатных + \r\n):
 *  [name:23]|[year:4]|[addr:25]|[godo:13]|[emp:6]|[sal:11]|\r\n
 * ============================================================ */

  // Записывает структуру Zavod в буфер buf[RECORD_SIZE].
  // Все поля выравниваются до фиксированной ширины пробелами.
void encode_record(const Zavod* z, char buf[RECORD_SIZE])
{
    // Формируем строку без \r\n сначала в tmp (89 символов + \0) 
    char tmp[RECORD_SIZE];
    snprintf(tmp, sizeof(tmp),
        "%-*s|%04d|%-*s|%13.2f|%6d|%11.2f|",
        W_NAME, z->name,
        z->year,
        W_ADDR, z->address,
        z->godoborot,
        z->employees,
        z->avg_salary);
    // tmp должен быть ровно RECORD_SIZE-2 символов 
    memcpy(buf, tmp, RECORD_SIZE - 2);
    buf[RECORD_SIZE - 2] = '\r';
    buf[RECORD_SIZE - 1] = '\n';
}

 // Разбирает буфер buf[RECORD_SIZE] и заполняет структуру Zavod.
 // Возвращает 1 при успехе, 0 при ошибке формата.
int decode_record(const char buf[RECORD_SIZE], Zavod* z)
{
    // Поля разделены '|', ширины фиксированы 
    char tmp[RECORD_SIZE];
    memcpy(tmp, buf, RECORD_SIZE);
    tmp[RECORD_SIZE - 2] = '\0'; // убираем \r\n 

    // name: позиция 0, длина W_NAME 
    memset(z->name, 0, sizeof(z->name));
    memcpy(z->name, tmp, W_NAME);
    trim(z->name);

    // year: позиция W_NAME+1, длина W_YEAR 
    char yr[W_YEAR + 1] = { 0 };
    memcpy(yr, tmp + W_NAME + 1, W_YEAR);
    z->year = atoi(yr);

    // addr: позиция W_NAME+1+W_YEAR+1 
    int p = W_NAME + 1 + W_YEAR + 1;
    memset(z->address, 0, sizeof(z->address));
    memcpy(z->address, tmp + p, W_ADDR);
    trim(z->address);

    // godoborot 
    p += W_ADDR + 1;
    char gb[W_GODO + 1] = { 0 };
    memcpy(gb, tmp + p, W_GODO);
    z->godoborot = atof(gb);

    // employees 
    p += W_GODO + 1;
    char em[W_EMP + 1] = { 0 };
    memcpy(em, tmp + p, W_EMP);
    z->employees = atoi(em);

    // avg_salary 
    p += W_EMP + 1;
    char sl[W_SAL + 1] = { 0 };
    memcpy(sl, tmp + p, W_SAL);
    z->avg_salary = atof(sl);

    return 1;
}

 // -- Функции работы с файлом --

  // Подсчитывает количество записей в файле
  // (последовательный доступ).
int file_count(void)
{
    FILE* f = fopen(DB_FILE, "rb"); 
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fclose(f);
    return (int)(sz / RECORD_SIZE);
}

 // Читает одну запись по индексу (0-based) — ПРЯМОЙ ДОСТУП.
 // Вычисляет смещение как index * RECORD_SIZE и читает ровно RECORD_SIZE байт.
 // Возвращает 1 при успехе, 0 при ошибке.
int read_record(int index, Zavod* z)
{
    FILE* f = fopen(DB_FILE, "rb");
    if (!f) return 0;
    char buf[RECORD_SIZE];
    int ok = (fseek(f, (long)index * RECORD_SIZE, SEEK_SET) == 0) &&
        (fread(buf, 1, RECORD_SIZE, f) == RECORD_SIZE);
    fclose(f);
    if (ok) decode_record(buf, z);
    return ok;
}

 // Перезаписывает одну запись по индексу — ПРЯМОЙ ДОСТУП.
 // Возвращает 1 при успехе, 0 при ошибке.
int write_record(int index, const Zavod* z)
{
    FILE* f = fopen(DB_FILE, "r+b");
    if (!f) return 0;
    char buf[RECORD_SIZE];
    encode_record(z, buf);
    int ok = (fseek(f, (long)index * RECORD_SIZE, SEEK_SET) == 0) &&
        (fwrite(buf, 1, RECORD_SIZE, f) == RECORD_SIZE);
    fclose(f);
    return ok;
}

 // Добавляет запись в конец файла.
 // Возвращает 1 при успехе, 0 при ошибке.
int append_record(const Zavod* z)
{
    FILE* f = fopen(DB_FILE, "ab");
    if (!f) { printf("  Ошибка: не удалось открыть файл.\n"); return 0; }
    char buf[RECORD_SIZE];
    encode_record(z, buf);
    int ok = (fwrite(buf, 1, RECORD_SIZE, f) == RECORD_SIZE);
    fclose(f);
    return ok;
}

 // Удаляет запись с индексом del_idx — ПОСЛЕДОВАТЕЛЬНЫЙ ДОСТУП
 // через временный файл: читаем все записи подряд, пропускаем
 // нужную, остальные пишем во временный файл, затем заменяем.
 // Возвращает 1 при успехе, 0 при ошибке.
int delete_record(int del_idx)
{
    FILE* fin = fopen(DB_FILE, "rb");
    FILE* fout = fopen(TMP_FILE, "wb");
    if (!fin || !fout) {
        if (fin)  fclose(fin);
        if (fout) fclose(fout);
        printf("  Ошибка при открытии файла.\n");
        return 0;
    }
    char buf[RECORD_SIZE];
    int i = 0, ok = 1;
    while (fread(buf, 1, RECORD_SIZE, fin) == RECORD_SIZE) {
        if (i != del_idx) {
            if (fwrite(buf, 1, RECORD_SIZE, fout) != RECORD_SIZE) {
                ok = 0; break;
            }
        }
        i++;
    }
    fclose(fin);
    fclose(fout);
    if (!ok) { remove(TMP_FILE); return 0; }
    remove(DB_FILE);
    rename(TMP_FILE, DB_FILE);
    return 1;
}

 //  Вывод таблицы

void print_header(void)
{
    printf("\n%-3s %-23s %-6s %-25s %-13s %-6s %-11s\n",
        "N", "Предприятие", "Год", "Адрес",
        "Оборот(тыс)", "Сотруд", "Ср.зарплата");
    printf("%-3s %-23s %-6s %-25s %-13s %-6s %-11s\n",
        "---", "-----------------------", "------",
        "-------------------------",
        "-------------", "------", "-----------");
}

void print_zavod_row(int n1, const Zavod* z)
{
    printf("%-3d %-23s %-6d %-25s %-13.2f %-6d %-11.2f\n",
        n1, z->name, z->year, z->address,
        z->godoborot, z->employees, z->avg_salary);
}

 // Выводит весь список — ПОСЛЕДОВАТЕЛЬНЫЙ ДОСТУП.
 // Возвращает количество выведенных записей.
int print_list(void)
{
    FILE* f = fopen(DB_FILE, "rb");

    if (!f) {
        printf("\nФайл базы данных не найден.\n");
        return 0;
    }

    char buf[RECORD_SIZE];
    Zavod z;
    int n = 0;

    printf("\n=== Список предприятий ===\n");

    print_header();

    while (fread(buf, 1, RECORD_SIZE, f) == RECORD_SIZE) {
        decode_record(buf, &z);
        print_zavod_row(++n, &z);
    }

    fclose(f);

    if (n == 0)
        printf("\nСписок предприятий пуст.\n");
    else
        printf("\nВсего предприятий: %d\n", n);

    return n;
}

 //  Ввод полей одной записи с клавиатуры

void input_zavod(Zavod* z)
{
    // Название 
    char tmp[256];
    do {
        printf("  Название предприятия (до %d симв.): ", W_NAME);
        fgets(tmp, sizeof(tmp), stdin);
        tmp[strcspn(tmp, "\n")] = '\0';
        trim(tmp);
        if (!strlen(tmp))
            printf("  Ошибка: название не может быть пустым.\n");
        else if ((int)strlen(tmp) > W_NAME)
            printf("  Ошибка: не более %d символов (введено %d).\n",
                W_NAME, (int)strlen(tmp));
        else if (!has_letter(tmp))
            printf("  Ошибка: название должно содержать буквы.\n");
    } while (!strlen(tmp) || (int)strlen(tmp) > W_NAME || !has_letter(tmp));
    memset(z->name, 0, sizeof(z->name));
    strncpy(z->name, tmp, W_NAME);

    // Год 
    z->year = safe_int("  Год основания (1800-2026): ", 1800, 2026);

    // Адрес 
    do {
        printf("  Адрес (до %d симв.): ", W_ADDR);
        fgets(tmp, sizeof(tmp), stdin);
        tmp[strcspn(tmp, "\n")] = '\0';
        trim(tmp);
        if (!strlen(tmp))
            printf("  Ошибка: адрес не может быть пустым.\n");
        else if ((int)strlen(tmp) > W_ADDR)
            printf("  Ошибка: не более %d символов.\n", W_ADDR);
        else if (!valid_address(tmp))
            printf("  Ошибка: недопустимые символы или нет букв.\n");
    } while (!strlen(tmp) || (int)strlen(tmp) > W_ADDR ||
        !valid_address(tmp));
    memset(z->address, 0, sizeof(z->address));
    strncpy(z->address, tmp, W_ADDR);

    z->godoborot = safe_double(
        "  Годовой оборот (тыс. руб., 0-100000000): ", 0.0, 100000000.0);
    z->employees = safe_int(
        "  Число сотрудников (0-1000000): ", 0, 1000000);
    z->avg_salary = safe_double(
        "  Средняя зарплата (руб., 0-10000000): ", 0.0, 10000000.0);
}

 //  CRUD-операции

 // Добавление новой записи 
void add_zavod(void)
{
    cls();

    int cnt = file_count();

    printf("\n--- Добавление нового предприятия ---\n");

    Zavod z;
    memset(&z, 0, sizeof(z));

    input_zavod(&z);

    if (append_record(&z))
        printf("  Предприятие добавлено (запись N%d).\n", cnt + 1);
    else
        printf("  Ошибка записи в файл!\n");

    printf("\nНажмите Enter...");
    getchar();
}

// Удаление записи через временный файл (последовательный доступ) 
void delete_zavod(void)
{
    cls();
    int cnt = print_list();
    if (cnt == 0) return;
    int num = safe_int(
        "\nВведите номер записи для удаления (0 - отмена): ", 0, cnt);
    if (num == 0) { printf("  Отмена удаления.\n"); return; }
    Zavod z;
    read_record(num - 1, &z);   // прямой доступ для отображения имени 
    printf("  Будет удалено: %s\n", z.name);
    int c = safe_int("  Вы уверены? (1-да, 0-нет): ", 0, 1);
    if (c == 0) { printf("  Удаление отменено.\n"); return; }
    if (delete_record(num - 1))
        printf("  Запись удалена. Осталось: %d\n", cnt - 1);
    else
        printf("  Ошибка при удалении!\n");
    printf("\nНажмите Enter...");
    getchar();
}

// Редактирование полей записи (прямой доступ для чтения и записи) 
void edit_zavod(void)
{
    cls();
    int cnt = print_list();
    if (cnt == 0) return;
    int num = safe_int(
        "\nВведите номер записи для редактирования (0 - отмена): ", 0, cnt);
    if (num == 0) { printf("  Отмена.\n"); return; }

    Zavod z;
    if (!read_record(num - 1, &z)) {     // прямой доступ 
        printf("  Ошибка чтения из файла.\n"); return;
    }

    char tmp[256];
    int choice = -1;
    while (choice != 0) {
        cls();
        printf("\n--- Редактирование: %s ---\n", z.name);
        printf("  1. Название       : %s\n", z.name);
        printf("  2. Год основания  : %d\n", z.year);
        printf("  3. Адрес          : %s\n", z.address);
        printf("  4. Оборот (тыс.)  : %.2f\n", z.godoborot);
        printf("  5. Число сотруд.  : %d\n", z.employees);
        printf("  6. Средняя зарпл. : %.2f\n", z.avg_salary);
        printf("  0. Сохранить и выйти\n");
        choice = safe_int("Выберите поле: ", 0, 6);

        switch (choice) {
        case 1:
            do {
                printf("  Новое название (до %d симв.): ", W_NAME);
                fgets(tmp, sizeof(tmp), stdin);
                tmp[strcspn(tmp, "\n")] = '\0'; trim(tmp);
                if (!strlen(tmp) || (int)strlen(tmp) > W_NAME ||
                    !has_letter(tmp))
                    printf("  Ошибка: недопустимое название.\n");
            } while (!strlen(tmp) || (int)strlen(tmp) > W_NAME ||
                !has_letter(tmp));
            memset(z.name, 0, sizeof(z.name));
            strncpy(z.name, tmp, W_NAME);
            break;
        case 2:
            z.year = safe_int("  Новый год (1800-2026): ", 1800, 2026);
            break;
        case 3:
            do {
                printf("  Новый адрес (до %d симв.): ", W_ADDR);
                fgets(tmp, sizeof(tmp), stdin);
                tmp[strcspn(tmp, "\n")] = '\0'; trim(tmp);
                if (!strlen(tmp) || (int)strlen(tmp) > W_ADDR ||
                    !valid_address(tmp))
                    printf("  Ошибка: недопустимый адрес.\n");
            } while (!strlen(tmp) || (int)strlen(tmp) > W_ADDR ||
                !valid_address(tmp));
            memset(z.address, 0, sizeof(z.address));
            strncpy(z.address, tmp, W_ADDR);
            break;
        case 4:
            z.godoborot = safe_double(
                "  Новый оборот (0-100000000): ", 0.0, 100000000.0);
            break;
        case 5:
            z.employees = safe_int(
                "  Новое число сотрудников: ", 0, 1000000);
            break;
        case 6:
            z.avg_salary = safe_double(
                "  Новая зарплата (0-10000000): ", 0.0, 10000000.0);
            break;
        case 0:
            // сохраняем изменённую запись через прямой доступ 
            if (!write_record(num - 1, &z))
                printf("  Ошибка записи в файл!\n");
            else
                printf("  Изменения сохранены.\n");
            printf("\nНажмите Enter...");
            getchar();
            break;
        }
    }
}

 // Числовые фильтры для запроса

  // mode: 0=нет, 1=нижняя граница (>=), 2=верхняя граница (<=),
  //       3=диапазон
void print_num_filter(const char* label, int mode,
    double vmin, double vmax, int is_int)
{
    printf("  %s", label);
    if (mode == 0) printf("(не задан)\n");
    else if (mode == 1) {
        if (is_int) printf(">= %.0f\n", vmin);
        else        printf(">= %.2f\n", vmin);
    }
    else if (mode == 2) {
        if (is_int) printf("<= %.0f\n", vmax);
        else        printf("<= %.2f\n", vmax);
    }
    else {
        if (is_int) printf("от %.0f до %.0f\n", vmin, vmax);
        else        printf("от %.2f до %.2f\n", vmin, vmax);
    }
}

int setup_num_filter(const char* fname, int cur_mode,
    double* vmin, double* vmax,
    double abs_min, double abs_max, int is_int)
{
    printf("\n  Фильтр \"%s\":\n", fname);
    printf("    1. Нижняя граница (>= X)\n");
    printf("    2. Верхняя граница (<= X)\n");
    printf("    3. Диапазон (от X до Y)\n");
    printf("    0. Снять фильтр\n");
    int t = safe_int("  Тип: ", 0, 3);
    if (t == 0) { printf("  Фильтр снят.\n"); return 0; }
    if (t == 1 || t == 3) {
        if (is_int)
            *vmin = (double)safe_int("  Минимум: ",
                (int)abs_min, (int)abs_max);
        else
            *vmin = safe_double("  Минимум: ", abs_min, abs_max);
    }
    if (t == 2 || t == 3) {
        double lo = (t == 3) ? *vmin : abs_min;
        if (is_int)
            *vmax = (double)safe_int("  Максимум: ",
                (int)lo, (int)abs_max);
        else
            *vmax = safe_double("  Максимум: ", lo, abs_max);
    }
    return t;
}

/* ============================================================
   Строковый фильтр для поля «Адрес»
 
   mode: 0 = не задан
         1 = содержит подстроку
         2 = начинается с
         3 = заканчивается на
         4 = точное совпадение
  ============================================================ */

  // Выводит текущее состояние фильтра по адресу в строку меню.
void print_addr_filter(int mode, const char* val)
{
    printf("  3. Адрес          : ");
    switch (mode) {
    case 0: printf("(не задан)\n");                        break;
    case 1: printf("содержит \"%s\"\n", val);        break;
    case 2: printf("начинается с \"%s\"\n", val);        break;
    case 3: printf("заканчивается на \"%s\"\n", val);      break;
    case 4: printf("точно равен \"%s\"\n", val);        break;
    }
}

 // Меню выбора типа строкового фильтра для адреса.
 // Возвращает новый mode; val обновляется при mode != 0.
int setup_addr_filter(int cur_mode, char* val, int maxlen)
{
    printf("\n  Фильтр по полю \"Адрес\":\n");
    printf("    1. Содержит подстроку\n");
    printf("    2. Начинается с\n");
    printf("    3. Заканчивается на\n");
    printf("    4. Точное совпадение\n");
    printf("    0. Снять фильтр\n");
    int t = safe_int("  Тип фильтра: ", 0, 4);
    if (t == 0) { printf("  Фильтр по адресу снят.\n"); return 0; }

    char tmp[256];
    do {
        printf("  Введите значение для сравнения: ");
        fgets(tmp, sizeof(tmp), stdin);
        tmp[strcspn(tmp, "\n")] = '\0';
        trim(tmp);
        if (strlen(tmp) == 0)
            printf("  Ошибка: значение не может быть пустым.\n");
        else if ((int)strlen(tmp) > maxlen - 1)
            printf("  Ошибка: не более %d символов.\n", maxlen - 1);
    } while (strlen(tmp) == 0 || (int)strlen(tmp) > maxlen - 1);

    strncpy(val, tmp, maxlen - 1);
    val[maxlen - 1] = '\0';
    return t;
}

 // Проверяет, удовлетворяет ли строка addr фильтру (mode, val).
 // Сравнение без учёта регистра (cp1251).
 // Возвращает 1 если подходит, 0 если нет.
int match_addr(const char* addr, int mode, const char* val)
{
    if (mode == 0) return 1;   // фильтр не задан — всё подходит 

    char a_lo[W_ADDR + 1], v_lo[W_ADDR + 1];
    str_lower(addr, a_lo, W_ADDR + 1);
    str_lower(val, v_lo, W_ADDR + 1);
    int alen = (int)strlen(a_lo);
    int vlen = (int)strlen(v_lo);

    switch (mode) {
    case 1: // содержит 
        return strstr(a_lo, v_lo) != NULL;
    case 2: // начинается с 
        return strncmp(a_lo, v_lo, vlen) == 0;
    case 3: // заканчивается на 
        if (vlen > alen) return 0;
        return strcmp(a_lo + alen - vlen, v_lo) == 0;
    case 4: // точное совпадение 
        return strcmp(a_lo, v_lo) == 0;
    }
    return 0;
}

 //  Обработка запроса — ПОСЛЕДОВАТЕЛЬНЫЙ ДОСТУП по всем записям

void query_menu(void)
{
    if (file_count() == 0) {
        printf("\n  Ошибка: база данных пуста.\n"); return;
    }

    int    use_name = 0, mode_addr = 0;
    int    mode_year = 0, mode_g = 0, mode_e = 0, mode_s = 0;
    char   q_name[W_NAME + 1] = "";
    char   q_addr[W_ADDR + 1] = "";
    int    q_yr1 = 1800, q_yr2 = 2026;
    double q_gmin = 0, q_gmax = 100000000;
    double q_emin = 0, q_emax = 1000000;
    double q_smin = 0, q_smax = 10000000;

    int choice = -1;
    while (choice != 0) {
        cls();
        print_list();   // таблица вверху экрана 

        printf("\n--- Настройка запроса ---\n");
        printf("  1. Название       : %s\n",
            use_name ? q_name : "(не задан)");
        print_num_filter("2. Год            : ", mode_year,
            (double)q_yr1, (double)q_yr2, 1);
        print_addr_filter(mode_addr, q_addr);
        print_num_filter("4. Оборот (тыс.)  : ", mode_g,
            q_gmin, q_gmax, 0);
        print_num_filter("5. Сотрудников    : ", mode_e,
            q_emin, q_emax, 1);
        print_num_filter("6. Ср.зарплата    : ", mode_s,
            q_smin, q_smax, 0);
        printf("  7. Выполнить запрос\n");
        printf("  0. Отмена\n");
        choice = safe_int("Действие: ", 0, 7);

        switch (choice) {
        case 1:
            printf("  Часть названия: ");
            fgets(q_name, sizeof(q_name), stdin);
            q_name[strcspn(q_name, "\n")] = '\0';
            use_name = (strlen(q_name) > 0);
            break;
        case 2: {
            // год — те же 4 варианта что у числовых полей 
            double yd1 = q_yr1, yd2 = q_yr2;
            mode_year = setup_num_filter("Год основания", mode_year,
                &yd1, &yd2, 1800.0, 2026.0, 1);
            q_yr1 = (int)yd1;
            q_yr2 = (int)yd2;
            break;
        }
        case 3:
            mode_addr = setup_addr_filter(mode_addr,
                q_addr, W_ADDR + 1);
            break;
        case 4:
            mode_g = setup_num_filter("Оборот (тыс.)", mode_g,
                &q_gmin, &q_gmax, 0.0, 100000000.0, 0);
            break;
        case 5:
            mode_e = setup_num_filter("Число сотрудников", mode_e,
                &q_emin, &q_emax, 0.0, 1000000.0, 1);
            break;
        case 6:
            mode_s = setup_num_filter("Средняя зарплата", mode_s,
                &q_smin, &q_smax, 0.0, 10000000.0, 0);
            break;
        case 7: {
            if (!use_name && !mode_year && !mode_addr &&
                !mode_g && !mode_e && !mode_s) {
                printf("  Ошибка: не задан ни один критерий.\n");
                break;
            }
            // последовательный проход по файлу 
            FILE* f = fopen(DB_FILE, "rb");
            if (!f) { printf("  Ошибка открытия файла.\n"); break; }
            cls();
            printf("\n=== Результаты запроса ===\n");
            print_header();
            char buf[RECORD_SIZE];
            char bn[W_NAME + 1];
            char bqn[W_NAME + 1];
            str_lower(q_name, bqn, W_NAME + 1);
            Zavod z;
            int found = 0;
            while (fread(buf, 1, RECORD_SIZE, f) == RECORD_SIZE) {
                decode_record(buf, &z);
                int ok = 1;
                if (use_name) {
                    str_lower(z.name, bn, W_NAME + 1);
                    if (!strstr(bn, bqn)) ok = 0;
                }
                // фильтр по адресу — строковый с выбором типа 
                if (mode_addr && !match_addr(z.address, mode_addr, q_addr))
                    ok = 0;
                // фильтр по году — те же режимы что у числовых 
                if (mode_year == 1 && z.year < q_yr1) ok = 0;
                if (mode_year == 2 && z.year > q_yr2) ok = 0;
                if (mode_year == 3 &&
                    (z.year < q_yr1 || z.year > q_yr2)) ok = 0;
                if (mode_g == 1 && z.godoborot < q_gmin) ok = 0;
                if (mode_g == 2 && z.godoborot > q_gmax) ok = 0;
                if (mode_g == 3 &&
                    (z.godoborot<q_gmin || z.godoborot>q_gmax)) ok = 0;
                if (mode_e == 1 && z.employees < (int)q_emin) ok = 0;
                if (mode_e == 2 && z.employees > (int)q_emax) ok = 0;
                if (mode_e == 3 &&
                    (z.employees<(int)q_emin ||
                        z.employees>(int)q_emax)) ok = 0;
                if (mode_s == 1 && z.avg_salary < q_smin) ok = 0;
                if (mode_s == 2 && z.avg_salary > q_smax) ok = 0;
                if (mode_s == 3 &&
                    (z.avg_salary<q_smin ||
                        z.avg_salary>q_smax)) ok = 0;
                if (ok) print_zavod_row(++found, &z);
            }
            fclose(f);
            if (found == 0)
                printf("  Предприятий по критериям не найдено.\n");
            else
                printf("\nНайдено предприятий: %d\n", found);
            printf("\nНажмите Enter для возврата...");
            getchar();
            choice = -1;
            break;
        }
        case 0:
            printf("  Запрос отменён.\n");
            break;
        }
    }
}

/* ============================================================
   Сортировка — пузырёк поверх файла.
   При каждом обмене двух соседних записей используется
   прямой доступ write_record для их перезаписи.
   Сами сравниваемые записи читаются через read_record (fseek).
  ============================================================ */

void sort_menu(void)
{
    int cnt = file_count();
    if (cnt == 0) {
        printf("\n  Ошибка: список пуст.\n");
        printf("\nНажмите Enter...");
        getchar();
        return;
    }

    cls();
    print_list();

    printf("\n--- Сортировка ---\n");
    printf("  1. По названию\n");
    printf("  2. По году основания\n");
    printf("  3. По адресу\n");
    printf("  4. По обороту\n");
    printf("  5. По числу сотрудников\n");
    printf("  6. По средней зарплате\n");
    printf("  0. Отмена\n");

    int field = safe_int("Ваш выбор: ", 0, 6);
    if (field == 0) return;

    printf("  1. По возрастанию\n");
    printf("  2. По убыванию\n");

    int order = safe_int("Ваш выбор: ", 1, 2);

    printf("  Сортировка...\n");

    for (int i = 0; i < cnt - 1; i++) {
        for (int j = 0; j < cnt - 1 - i; j++) {

            Zavod a, b;
            read_record(j, &a);
            read_record(j + 1, &b);

            int cmp = 0;

            switch (field) {
            case 1: cmp = strcmp(a.name, b.name); break;
            case 2: cmp = a.year - b.year; break;
            case 3: cmp = strcmp(a.address, b.address); break;
            case 4:
                cmp = (a.godoborot > b.godoborot) -
                    (a.godoborot < b.godoborot);
                break;
            case 5: cmp = a.employees - b.employees; break;
            case 6:
                cmp = (a.avg_salary > b.avg_salary) -
                    (a.avg_salary < b.avg_salary);
                break;
            }

            int need_swap = 0;

            if (order == 1 && cmp > 0) need_swap = 1;
            if (order == 2 && cmp < 0) need_swap = 1;

            if (need_swap) {
                write_record(j, &b);
                write_record(j + 1, &a);
            }
        }
    }

    cls();
    printf("  Список отсортирован.\n");
    print_list();

    printf("\nНажмите Enter...");
    getchar();
}


 //  Подменю «Ввод и корректировка»

void menu_input_edit(void)
{
    int choice = -1;
    while (choice != 0) {
        cls();
        printf("\n=== Ввод и корректировка ===\n");
        printf("  1. Добавить предприятие\n");
        printf("  2. Удалить предприятие\n");
        printf("  3. Редактировать поля предприятия\n");
        printf("  0. Назад\n");
        choice = safe_int("Ваш выбор: ", 0, 3);
        switch (choice) {
        case 1: add_zavod();    break;
        case 2: delete_zavod(); break;
        case 3: edit_zavod();   break;
        case 0: break;
        }
    }
}

 //  Главное меню

void main_menu(void)
{
    int choice = -1;
    while (choice != 0) {
        cls();
        printf("\n========================================\n");
        printf("   СПРАВОЧНИК ПРЕДПРИЯТИЙ\n");
        printf("   Файл: %-28s\n", DB_FILE);
        printf("   Записей: %d\n", file_count());
        printf("========================================\n");
        printf("  1. Ввод и корректировка списка\n");
        printf("  2. Вывод списка\n");
        printf("  3. Обработка запроса\n");
        printf("  4. Сортировка списка\n");
        printf("  0. Выход\n");
        printf("========================================\n");
        choice = safe_int("Ваш выбор: ", 0, 4);
        switch (choice) {
        case 1: menu_input_edit(); break;
        case 2:
            cls(); print_list();
            printf("\nНажмите Enter для возврата...");
            getchar();
            break;
        case 3: query_menu();  break;
        case 4: sort_menu();   break;
        case 0: {
            int c = safe_int(
                "  Данные в файле. Выйти? (1-да, 0-нет): ", 0, 1);
            if (c == 1)
                printf("\nВыход. Файл %s сохранён. До свидания!\n",
                    DB_FILE);
            else {
                printf("  Выход отменён.\n");
                choice = -1;
            }
            break;
        }
        }
    }
}

 //  main

int main(void)
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");

    printf("Справочник предприятий\n");
    printf("Файл данных: %s\n\n", DB_FILE);

    /*
     * Если файла нет — создаём пустой.
     * Открываем в бинарном режиме ("r+b" для обновления требует
     * существующего файла, поэтому при отсутствии создаём "wb").
     */
    FILE* f = fopen(DB_FILE, "rb");
    if (!f) {
        f = fopen(DB_FILE, "wb");
        if (!f) {
            printf("ОШИБКА: не удалось создать файл %s\n", DB_FILE);
            return 1;
        }
        printf("Файл базы данных создан.\n");
    }
    fclose(f);

    printf("Нажмите Enter для продолжения...");
    getchar();

    main_menu();
}