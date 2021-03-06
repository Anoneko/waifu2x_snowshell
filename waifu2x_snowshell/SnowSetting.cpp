﻿#include "Main.h"

SnowSetting *SnowSetting::Singletone;
wstring SnowSetting::NewPath;
wstring SnowSetting::CurrPath;
wstring SnowSetting::LangPath;
wstring SnowSetting::INIPath;
wstring SnowSetting::CONVERTER_x64_EXE;
int SnowSetting::CoreNum;

const int SnowSetting::LangNum = 3;
wstring SnowSetting::LangName[3] = { L"한국어", L"English", L"日本語" };
wstring SnowSetting::LangFile[3] = { L"Korean.ini", L"English.ini", L"Japanese.ini" };

SnowSetting::SnowSetting()
{
	WCHAR path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	CurrPath = path;
	NewPath = L"\\output";
	INIPath = CurrPath + L"\\config.ini";
	LangPath = CurrPath + L"\\Lang";
	CONVERTER_x64_EXE = CurrPath + L"\\waifu2x-converter\\waifu2x-converter-cpp.exe";
	CoreNum = thread::hardware_concurrency();

	Noise = 1;
	Scale = 2;
	CPU = 0;
	Export = 0;
	Confirm = 0;
	Lang = 1;
}

SnowSetting::~SnowSetting()
{
	saveSetting();
}

SnowSetting *SnowSetting::Init()
{
	if (Singletone == nullptr) {
		Singletone = new SnowSetting();
		loadSetting();
	}
	return Singletone;
}

void SnowSetting::loadLocale()
{
	if (Singletone == nullptr)
		Init();

	WCHAR buf[200];
	wstring Key, Value, Section;
	wstring LangFileName = LangPath + L"\\" + LangFile[getLang()];

	//MessageBox(NULL, LangFileName.c_str(), L"", MB_OK);

	if (!FileExists(LangFileName.c_str()) && LangFileName.find(L"English") == std::string::npos) {
		CreateDirectory(L"Lang", NULL);
		HRSRC hSrc=NULL;
		if (LangFileName.find(L"Korean") != std::string::npos) hSrc=FindResource(g_hInst, MAKEINTRESOURCE(IDR_LANG1), L"LANG");
		else if (LangFileName.find(L"Japanese") != std::string::npos) hSrc=FindResource(g_hInst, MAKEINTRESOURCE(IDR_LANG2), L"LANG");
		else{
			MessageBox(NULL, L"No Lang File", L"Error", MB_ICONWARNING | MB_OK);
			setLang(1);
		}
		if (hSrc != NULL) {
			HGLOBAL hRes = LoadResource(g_hInst, hSrc);
			LPVOID memRes = LockResource(hRes);
			DWORD sizeRes = SizeofResource(g_hInst, hSrc);
			HANDLE hFile = CreateFile(LangFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD dwWritten = 0;

			WriteFile(hFile, memRes, sizeRes, &dwWritten, NULL);
			if(hFile!=NULL)
				CloseHandle(hFile);
		}
	}

	Section = L"Menu";

	Key = L"STRING_MENU_FILE";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"File", buf, 200, LangFileName.c_str());
	STRING_MENU_FILE = buf;

	Key = L"STRING_MENU_NOISE";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Noise Reduce", buf, 200, LangFileName.c_str());
	STRING_MENU_NOISE = buf;

	Key = L"STRING_MENU_SCALE";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Scale", buf, 200, LangFileName.c_str());
	STRING_MENU_SCALE = buf;

	Key = L"STRING_MENU_CPU";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"CPU", buf, 200, LangFileName.c_str());
	STRING_MENU_CPU = buf;

	Key = L"STRING_MENU_EXPORT";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Export", buf, 200, LangFileName.c_str());
	STRING_MENU_EXPORT = buf;

	Key = L"STRING_MENU_CONFIRM";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Warning", buf, 200, LangFileName.c_str());
	STRING_MENU_CONFIRM = buf;


	Section = L"File";

	Key = L"STRING_MENU_FILE_IMAGE_SEL";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Select image And execute", buf, 200, LangFileName.c_str());
	STRING_MENU_FILE_IMAGE_SEL = buf;

	Key = L"STRING_MENU_FILE_CREDIT";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Credit", buf, 200, LangFileName.c_str());
	STRING_MENU_FILE_CREDIT = buf;

	Key = L"STRING_MENU_FILE_QUIT";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Quit", buf, 200, LangFileName.c_str());
	STRING_MENU_FILE_QUIT = buf;


	Section = L"Noise";

	Key = L"STRING_MENU_NOISE_NONE";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"None", buf, 200, LangFileName.c_str());
	STRING_MENU_NOISE_NONE = buf;

	Key = L"STRING_MENU_NOISE_LOW";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Low", buf, 200, LangFileName.c_str());
	STRING_MENU_NOISE_LOW = buf;

	Key = L"STRING_MENU_NOISE_HIGH";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"High", buf, 200, LangFileName.c_str());
	STRING_MENU_NOISE_HIGH = buf;

	Key = L"STRING_MENU_NOISE_MAX";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Max", buf, 200, LangFileName.c_str());
	STRING_MENU_NOISE_MAX = buf;


	Section = L"Scale";

	Key = L"STRING_MENU_SCALE_x1_0";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"x1.0", buf, 200, LangFileName.c_str());
	STRING_MENU_SCALE_x1_0 = buf;

	Key = L"STRING_MENU_SCALE_x1_5";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"x1.5", buf, 200, LangFileName.c_str());
	STRING_MENU_SCALE_x1_5 = buf;

	Key = L"STRING_MENU_SCALE_x1_6";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"x1.6", buf, 200, LangFileName.c_str());
	STRING_MENU_SCALE_x1_6 = buf;

	Key = L"STRING_MENU_SCALE_x2_0";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"x2.0", buf, 200, LangFileName.c_str());
	STRING_MENU_SCALE_x2_0 = buf;


	Section = L"CPU";

	Key = L"STRING_MENU_CPU_MID";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Half", buf, 200, LangFileName.c_str());
	STRING_MENU_CPU_MID = buf;

	Key = L"STRING_MENU_CPU_HIGH";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Almost All", buf, 200, LangFileName.c_str());
	STRING_MENU_CPU_HIGH = buf;

	Key = L"STRING_MENU_CPU_FULL";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Full Power!!!!", buf, 200, LangFileName.c_str());
	STRING_MENU_CPU_FULL = buf;


	Section = L"Export";

	Key = L"STRING_MENU_EXPORT_SAME";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Same folder of input file", buf, 200, LangFileName.c_str());
	STRING_MENU_EXPORT_SAME = buf;

	Key = L"STRING_MENU_EXPORT_NEW";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"\"output\" folder", buf, 200, LangFileName.c_str());
	STRING_MENU_EXPORT_NEW = buf;


	Section = L"Confirm";

	Key = L"STRING_MENU_CONFIRM_SHOW";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Show warning", buf, 200, LangFileName.c_str());
	STRING_MENU_CONFIRM_SHOW = buf;

	Key = L"STRING_MENU_CONFIRM_SKIP";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Hide warning", buf, 200, LangFileName.c_str());
	STRING_MENU_CONFIRM_SKIP = buf;


	Section = L"Text";

	Key = L"STRING_TEXT_NOISE_NONE";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Anti Noise    None", buf, 200, LangFileName.c_str());
	STRING_TEXT_NOISE_NONE = buf;

	Key = L"STRING_TEXT_NOISE_LOW";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Anti Noise    Low", buf, 200, LangFileName.c_str());
	STRING_TEXT_NOISE_LOW = buf;

	Key = L"STRING_TEXT_NOISE_HIGH";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Anti Noise    High", buf, 200, LangFileName.c_str());
	STRING_TEXT_NOISE_HIGH = buf;

	Key = L"STRING_TEXT_NOISE_MAX";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Anti Noise    Max", buf, 200, LangFileName.c_str());
	STRING_TEXT_NOISE_MAX = buf;


	Key = L"STRING_TEXT_SCALE_x1_0";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Zoom Scale   x1.0", buf, 200, LangFileName.c_str());
	STRING_TEXT_SCALE_x1_0 = buf;

	Key = L"STRING_TEXT_SCALE_x1_5";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Zoom Scale   x1.5", buf, 200, LangFileName.c_str());
	STRING_TEXT_SCALE_x1_5 = buf;

	Key = L"STRING_TEXT_SCALE_x1_6";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Zoom Scale   x1.6", buf, 200, LangFileName.c_str());
	STRING_TEXT_SCALE_x1_6 = buf;

	Key = L"STRING_TEXT_SCALE_x2_0";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Zoom Scale   x2.0", buf, 200, LangFileName.c_str());
	STRING_TEXT_SCALE_x2_0 = buf;


	Key = L"STRING_TEXT_CPU_MID";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Thread Num  Half", buf, 200, LangFileName.c_str());
	STRING_TEXT_CPU_MID = buf;

	Key = L"STRING_TEXT_CPU_HIGH";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Thread Num  Almost all", buf, 200, LangFileName.c_str());
	STRING_TEXT_CPU_HIGH = buf;

	Key = L"STRING_TEXT_CPU_FULL";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Thread Num  All", buf, 200, LangFileName.c_str());
	STRING_TEXT_CPU_FULL = buf;


	Key = L"STRING_TEXT_EXPORT_SAME";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Export dir     Same folder", buf, 200, LangFileName.c_str());
	STRING_TEXT_EXPORT_SAME = buf;

	Key = L"STRING_TEXT_EXPORT_NEW";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Export dir     \"output\" folder", buf, 200, LangFileName.c_str());
	STRING_TEXT_EXPORT_NEW = buf;


	Key = L"STRING_TEXT_CONFIRM_SHOW";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Warning       Show", buf, 200, LangFileName.c_str());
	STRING_TEXT_CONFIRM_SHOW = buf;

	Key = L"STRING_TEXT_CONFIRM_SKIP";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Warning       Hide", buf, 200, LangFileName.c_str());
	STRING_TEXT_CONFIRM_SKIP = buf;

	Key = L"STRING_TEXT_CONFIRM_TITLE";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Warning", buf, 200, LangFileName.c_str());
	STRING_TEXT_CONFIRM_TITLE = buf;

	Key = L"STRING_TEXT_CONFIRM_MESSAGE";
	GetPrivateProfileStringW(Section.c_str(), Key.c_str(), L"Large image will require high spec.\nDo you want to continue?", buf, 200, LangFileName.c_str());
	STRING_TEXT_CONFIRM_MESSAGE = buf;
	int nl = STRING_TEXT_CONFIRM_MESSAGE.find(L"\\n");
	while (nl != wstring::npos) {
		STRING_TEXT_CONFIRM_MESSAGE.replace(nl, 2, L"\n");
		nl = STRING_TEXT_CONFIRM_MESSAGE.find(L"\\n");
	}

}

bool SnowSetting::loadSetting()
{
	if (Singletone == nullptr)
		Init();
	
	wstring Section = L"SnowShell";
	wstring Key, Value;

	Key = L"Noise";
	setNoise(GetPrivateProfileInt(Section.c_str(), Key.c_str(), 1, INIPath.c_str()));

	Key = L"Scale";
	setScale(GetPrivateProfileInt(Section.c_str(), Key.c_str(), 2, INIPath.c_str()));

	Key = L"CPU";
	setCPU(GetPrivateProfileInt(Section.c_str(), Key.c_str(), 0, INIPath.c_str()));

	Key = L"Export";
	setExport(GetPrivateProfileInt(Section.c_str(), Key.c_str(), 0, INIPath.c_str()));

	Key = L"Confirm";
	setConfirm(GetPrivateProfileInt(Section.c_str(), Key.c_str(), 0, INIPath.c_str()));

	Key = L"Lang";

	int langsel = GetPrivateProfileInt(Section.c_str(), Key.c_str(), -1, INIPath.c_str());

	if (langsel == -1) {
		WCHAR buf[40];
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, buf, 40);
		for(int i=0; i<LangNum; i++)
			if (LangFile[i].find(buf) >= 0)
			{
				langsel = i;
				break;
			}
		if (langsel == -1)
			langsel = 1;
	}
	setLang(langsel);
	loadLocale();

	return true;
}

bool SnowSetting::saveSetting()
{
	if (Singletone == nullptr)
		Init();

	wstring Section = L"SnowShell";
	wstring Key, Value;

	Key = L"Noise";
	WritePrivateProfileString(Section.c_str(), Key.c_str(), itos(getNoise()).c_str(), INIPath.c_str());

	Key = L"Scale";
	WritePrivateProfileString(Section.c_str(), Key.c_str(), itos(getScale()).c_str(), INIPath.c_str());

	Key = L"CPU";
	WritePrivateProfileString(Section.c_str(), Key.c_str(), itos(getCPU()).c_str(), INIPath.c_str());

	Key = L"Export";
	WritePrivateProfileString(Section.c_str(), Key.c_str(), itos(getExport()).c_str(), INIPath.c_str());

	Key = L"Confirm";
	WritePrivateProfileString(Section.c_str(), Key.c_str(), itos(getConfirm()).c_str(), INIPath.c_str());

	Key = L"Lang";
	WritePrivateProfileString(Section.c_str(), Key.c_str(), itos(getLang()).c_str(), INIPath.c_str());

	return true;
}

void SnowSetting::loadMenuString(HMENU hMenu)
{
	ModifyMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, 0, STRING_MENU_FILE.c_str());
	ModifyMenu(hMenu, 1, MF_BYPOSITION | MF_STRING, 1, STRING_MENU_NOISE.c_str());
	ModifyMenu(hMenu, 2, MF_BYPOSITION | MF_STRING, 2, STRING_MENU_SCALE.c_str());
	ModifyMenu(hMenu, 3, MF_BYPOSITION | MF_STRING, 3, STRING_MENU_CPU.c_str());
	ModifyMenu(hMenu, 4, MF_BYPOSITION | MF_STRING, 4, STRING_MENU_EXPORT.c_str());
	ModifyMenu(hMenu, 5, MF_BYPOSITION | MF_STRING, 5, STRING_MENU_CONFIRM.c_str());

	ModifyMenu(hMenu, MENU_FILE_IMAGE_SEL, MF_BYCOMMAND | MF_STRING, MENU_FILE_IMAGE_SEL, STRING_MENU_FILE_IMAGE_SEL.c_str());
	ModifyMenu(hMenu, MENU_FILE_CREDIT, MF_BYCOMMAND | MF_STRING, MENU_FILE_CREDIT, STRING_MENU_FILE_CREDIT.c_str());
	ModifyMenu(hMenu, MENU_FILE_QUIT, MF_BYCOMMAND | MF_STRING, MENU_FILE_QUIT, STRING_MENU_FILE_QUIT.c_str());

	ModifyMenu(hMenu, MENU_NOISE_NONE, MF_BYCOMMAND | MF_STRING, MENU_NOISE_NONE, STRING_MENU_NOISE_NONE.c_str());
	ModifyMenu(hMenu, MENU_NOISE_LOW, MF_BYCOMMAND | MF_STRING, MENU_NOISE_LOW, STRING_MENU_NOISE_LOW.c_str());
	ModifyMenu(hMenu, MENU_NOISE_HIGH, MF_BYCOMMAND | MF_STRING, MENU_NOISE_HIGH, STRING_MENU_NOISE_HIGH.c_str());
	ModifyMenu(hMenu, MENU_NOISE_MAX, MF_BYCOMMAND | MF_STRING, MENU_NOISE_MAX, STRING_MENU_NOISE_MAX.c_str());

	ModifyMenu(hMenu, MENU_SCALE_x1_0, MF_BYCOMMAND | MF_STRING, MENU_SCALE_x1_0, STRING_MENU_SCALE_x1_0.c_str());
	ModifyMenu(hMenu, MENU_SCALE_x1_5, MF_BYCOMMAND | MF_STRING, MENU_SCALE_x1_5, STRING_MENU_SCALE_x1_5.c_str());
	ModifyMenu(hMenu, MENU_SCALE_x1_6, MF_BYCOMMAND | MF_STRING, MENU_SCALE_x1_6, STRING_MENU_SCALE_x1_6.c_str());
	ModifyMenu(hMenu, MENU_SCALE_x2_0, MF_BYCOMMAND | MF_STRING, MENU_SCALE_x2_0, STRING_MENU_SCALE_x2_0.c_str());

	ModifyMenu(hMenu, MENU_CPU_MID, MF_BYCOMMAND | MF_STRING, MENU_CPU_MID, STRING_MENU_CPU_MID.c_str());
	ModifyMenu(hMenu, MENU_CPU_HIGH, MF_BYCOMMAND | MF_STRING, MENU_CPU_HIGH, STRING_MENU_CPU_HIGH.c_str());
	ModifyMenu(hMenu, MENU_CPU_FULL, MF_BYCOMMAND | MF_STRING, MENU_CPU_FULL, STRING_MENU_CPU_FULL.c_str());

	ModifyMenu(hMenu, MENU_EXPORT_SAME, MF_BYCOMMAND | MF_STRING, MENU_EXPORT_SAME, STRING_MENU_EXPORT_SAME.c_str());
	ModifyMenu(hMenu, MENU_EXPORT_NEW, MF_BYCOMMAND | MF_STRING, MENU_EXPORT_NEW, STRING_MENU_EXPORT_NEW.c_str());

	ModifyMenu(hMenu, MENU_CONFIRM_SHOW, MF_BYCOMMAND | MF_STRING, MENU_CONFIRM_SHOW, STRING_MENU_CONFIRM_SHOW.c_str());
	ModifyMenu(hMenu, MENU_CONFIRM_SKIP, MF_BYCOMMAND | MF_STRING, MENU_CONFIRM_SKIP, STRING_MENU_CONFIRM_SKIP.c_str());
}

wstring SnowSetting::BuildParam(LPCWSTR inputFile)
{
	if (Singletone == nullptr)
		Init();

	wstringstream ss;

	wstring ExpName;

	ExpName = inputFile;

	int dotPos = ExpName.find_last_of(L".");

	wstring ext = L".png";	// waifu2x's result is always png file.

	ExpName=ExpName.substr(0, dotPos) + L"_waifu2x";

	ss << L"-i \"" << inputFile << L"\" ";

	if (getNoise() == NOISE_NONE)
		ss << "-m scale ";
	else if (getScale() == SCALE_x1_0)
		ss << L"-m noise ";
	else
		ss << L"-m noise_scale ";

	switch (getCPU()) {
	case CPU_MID:
		ss << L"-j " << (int) CoreNum/2 << L" ";
		break;
	case CPU_FULL:
		ss << L"-j " << CoreNum << L" ";
		break;
	case CPU_HIGH:
	default:
		ss << L"-j " << CoreNum-1 << L" ";
		break;
	}

	switch (getScale()) {
	case SCALE_x1_0:
		if (getNoise() == NOISE_NONE)
			ss << L"--scale_ratio 1.0 ";
		break;
	case SCALE_x1_5:
		ss << L"--scale_ratio 1.5 ";
		ExpName += L"_scale_x1_5";
		break;
	case SCALE_x1_6:
		ss << L"--scale_ratio 1.6 ";
		ExpName += L"_scale_x1_6";
		break;
	case SCALE_x2_0:
		ss << L"--scale_ratio 2.0 ";
		ExpName += L"_scale_x2_0";
		break;
	}

	switch (getNoise()) {
	case NOISE_LOW:
		ss << L"--noise_level 1 ";
		ExpName += L"_noise1";
		break;
	case NOISE_HIGH:
		ss << L"--noise_level 2 ";
		ExpName += L"_noise2";
		break;
	case NOISE_MAX:
		ss << L"--noise_level 3 ";
		ExpName += L"_noise3";
		break;
	default:
		break;
	}

	ExpName += ext;

	if (getExport() == EXPORT_SAME)
	{
		ss << L"-o \"" << ExpName << L"\"";
	}
	else {
		int last=ExpName.find_last_of(L'\\');
		CreateDirectory((ExpName.substr(0, last) + NewPath).c_str(), NULL);
		ExpName = ExpName.substr(0, last) + NewPath + ExpName.substr(last);
		ss << L"-o \"" << ExpName << L"\"";
	}

	return ss.str();
}

int SnowSetting::getNoise()
{
	if (Singletone == nullptr)
		Init();

	return Singletone->Noise;
}

int SnowSetting::getScale()
{
	if (Singletone == nullptr)
		Init();

	return Singletone->Scale;
}

int SnowSetting::getCPU()
{
	if (Singletone == nullptr)
		Init();

	return Singletone->CPU;
}

int SnowSetting::getExport()
{
	if (Singletone == nullptr)
		Init();

	return Singletone->Export;
}

BOOL SnowSetting::getConfirm()
{
	if (Singletone == nullptr)
		Init();

	return Singletone->Confirm;
}

int SnowSetting::getLang()
{
	if (Singletone == nullptr)
		Init();

	return Singletone->Lang;
}

wstring SnowSetting::getLangName()
{
	return LangFile[getLang()];
}

void SnowSetting::setNoise(int Noise)
{
	if (Singletone == nullptr)
		Init();

	if (Noise > NOISE_MAX || Noise < 0)
		Noise = 0;

	Singletone->Noise = Noise;
}

void SnowSetting::setScale(int Scale)
{
	if (Singletone == nullptr)
		Init();

	if (Scale > SCALE_MAX || Scale < 0)
		Scale = 0;

	Singletone->Scale = Scale;
}

void SnowSetting::setCPU(int CPU)
{
	if (Singletone == nullptr)
		Init();

	if (CPU > CPU_MAX || CPU < 0)
		CPU = 0;

	Singletone->CPU = CPU;
}

void SnowSetting::setExport(int Export)
{
	if (Singletone == nullptr)
		Init();

	if (Export > EXPORT_MAX || Export < 0)
		Export = 0;

	Singletone->Export = Export;
}

void SnowSetting::setConfirm(BOOL Confirm)
{
	if (Singletone == nullptr)
		Init();

	Singletone->Confirm = Confirm;
}

void SnowSetting::setLang(int Lang)
{
	if (Singletone == nullptr)
		Init();

	if (Lang >= LangNum || Lang < 0)
		Lang = 0;

	Singletone->Lang = Lang;
	loadLocale();
}

void SnowSetting::checkMenuAll (HMENU hMenu)
{
	checkNoise(hMenu);
	checkScale(hMenu);
	checkCPU(hMenu);
	checkExport(hMenu);
	checkConfirm(hMenu);
	checkLang(hMenu);
}

void SnowSetting::checkNoise(HMENU hMenu, int sel)
{
	HMENU hSubMenu = GetSubMenu(hMenu, 1);

	if (sel != -1)
		setNoise(sel);

	for (int i = 0; i<=NOISE_MAX; i++)
		CheckMenuItem(hSubMenu, i, MF_BYPOSITION | MF_UNCHECKED);
	CheckMenuItem(hSubMenu, getNoise(), MF_BYPOSITION | MF_CHECKED);
}

void SnowSetting::checkScale(HMENU hMenu, int sel)
{
	HMENU hSubMenu = GetSubMenu(hMenu, 2);

	if (sel != -1)
		setScale(sel);

	for (int i = 0; i<=SCALE_MAX; i++)
		CheckMenuItem(hSubMenu, i, MF_BYPOSITION | MF_UNCHECKED);
	CheckMenuItem(hSubMenu, getScale(), MF_BYPOSITION | MF_CHECKED);
}

void SnowSetting::checkCPU(HMENU hMenu, int sel)
{
	HMENU hSubMenu = GetSubMenu(hMenu, 3);

	if (sel != -1)
		setCPU(sel);

	for (int i = 0; i<=CPU_MAX; i++)
		CheckMenuItem(hSubMenu, i, MF_BYPOSITION | MF_UNCHECKED);
	CheckMenuItem(hSubMenu, getCPU(), MF_BYPOSITION | MF_CHECKED);
}

void SnowSetting::checkExport(HMENU hMenu, int sel)
{
	HMENU hSubMenu = GetSubMenu(hMenu, 4);

	if (sel != -1)
		setExport(sel);

	for (int i = 0; i<=EXPORT_MAX; i++)
		CheckMenuItem(hSubMenu, i, MF_BYPOSITION | MF_UNCHECKED);
	CheckMenuItem(hSubMenu, getExport(), MF_BYPOSITION | MF_CHECKED);
}

void SnowSetting::checkConfirm(HMENU hMenu, int sel)
{
	HMENU hSubMenu = GetSubMenu(hMenu, 5);

	if (sel != -1)
		setConfirm(sel);

	for (int i = 0; i<=CONFIRM_MAX; i++)
		CheckMenuItem(hSubMenu, i, MF_BYPOSITION | MF_UNCHECKED);
	CheckMenuItem(hSubMenu, getConfirm(), MF_BYPOSITION | MF_CHECKED);
}

void SnowSetting::checkLang(HMENU hMenu, int sel)
{
	HMENU hSubMenu = GetSubMenu(hMenu, 6);

	if (sel != -1)
		setLang(sel);

	for (int i = 0; i< LangNum; i++)
		CheckMenuItem(hSubMenu, i, MF_BYPOSITION | MF_UNCHECKED);
	CheckMenuItem(hSubMenu, getLang(), MF_BYPOSITION | MF_CHECKED);
}

void SnowSetting::getTexts(wstring*(*UIText)[5]) {
	(*UIText)[0] = getNoiseText();
	(*UIText)[1] = getScaleText();
	(*UIText)[2] = getCPUText();
	(*UIText)[3] = getExportText();
	(*UIText)[4] = getConfirmText();
}

wstring * SnowSetting::getNoiseText()
{
	switch (getNoise()) {
	case NOISE_NONE:
		return &STRING_TEXT_NOISE_NONE;
	case NOISE_LOW:
		return &STRING_TEXT_NOISE_LOW;
	case NOISE_HIGH:
		return &STRING_TEXT_NOISE_HIGH;
	case NOISE_MAX:
		return &STRING_TEXT_NOISE_MAX;
	}
	return nullptr;
}

wstring * SnowSetting::getScaleText()
{
	switch (getScale()) {
	case SCALE_x1_0:
		return &STRING_TEXT_SCALE_x1_0;
	case SCALE_x1_5:
		return &STRING_TEXT_SCALE_x1_5;
	case SCALE_x1_6:
		return &STRING_TEXT_SCALE_x1_6;
	case SCALE_x2_0:
		return &STRING_TEXT_SCALE_x2_0;
	}
	return nullptr;
}

wstring * SnowSetting::getCPUText()
{
	switch (getCPU()) {
	case CPU_MID:
		return &STRING_TEXT_CPU_MID;
	case CPU_HIGH:
		return &STRING_TEXT_CPU_HIGH;
	case CPU_FULL:
		return &STRING_TEXT_CPU_FULL;
	}
	return nullptr;
}

wstring * SnowSetting::getExportText()
{
	switch (getExport()) {
	case EXPORT_SAME:
		return &STRING_TEXT_EXPORT_SAME;
	case EXPORT_NEW:
		return &STRING_TEXT_EXPORT_NEW;
	}
	return nullptr;
}

wstring * SnowSetting::getConfirmText()
{
	if (getConfirm() == CONFIRM_SHOW)
		return &STRING_TEXT_CONFIRM_SHOW;
	else
		return &STRING_TEXT_CONFIRM_SKIP;
}

wstring itos(int n) {
	wstringstream ss;

	ss << n;

	return ss.str();
}

BOOL FileExists(LPCWSTR file) {
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(file, &FindFileData);
	BOOL found = handle != INVALID_HANDLE_VALUE;
	if (found) {
		FindClose(handle);
	}
	return found;
}

int contain(wstring str, wstring find) {
	int index=0, count=0;
	while ((index = str.find(find, index+1))!= std::string::npos) {
		count++;
	}
	return count;
}