// Program.cpp — точка входа приложения
#include "stdafx.h"
#include "LoginForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    EnterpriseProject::LoginForm form;
    Application::Run(%form);
    return 0;
}
