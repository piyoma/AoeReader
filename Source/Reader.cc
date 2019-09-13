#include "Reader.h"
#include "AppConfig.h"
#include <ShellAPI.h>
#include <Shlwapi.h>
#include "AA_String.h"
ReaderClient::ReaderClient()
{

}
ReaderClient::~ReaderClient()
{

}
void ReaderClient::Run()
{


	//res_file_name = L"def.skin";
	//AAGuiOption gui_option;
	//gui_option.use_zip = true;
	//gui_option.resource_filename = res_file_name.c_str();
		//AAGuiInit(gui_option);


		const WORD LangID_ja_JP = MAKELANGID(LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN);
		const WORD LangID_en_US = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

		printf("LangID_ja_JP = 0x%04hx\n", LangID_ja_JP); // 0x0411
		printf("LangID_en_US = 0x%04hx\n", LangID_en_US); // 0x0409

		_ASSERTE(PRIMARYLANGID(LangID_ja_JP) == LANG_JAPANESE);
		_ASSERTE(PRIMARYLANGID(LangID_en_US) == LANG_ENGLISH);

		// なお、日本語 OS をベースとして言語パックを適用しただけでは、
		// ロケールは日本語のまま（ANSI コードページは CP932, Shift_JIS のまま）なので、
		// コマンドプロンプトではデフォルト設定で日本語文字列を表示できる。

		const WORD sysDefLangId = ::GetSystemDefaultLangID();
		const WORD usrDefLangId = ::GetUserDefaultLangID();
		const WORD sysDefUILang = ::GetSystemDefaultUILanguage();
		const WORD usrDefUILang = ::GetUserDefaultUILanguage();
		const DWORD sysDefLcid = ::GetSystemDefaultLCID();
		const DWORD usrDefLcid = ::GetUserDefaultLCID();


		//Identifier 
		//	Language

		//	0x0000 
		//	Language  Neutral   

		//	0x0400
		//	Process Default Language   

		//	0x0401
		//	Arabic   (Saudi   Arabia)
		//	阿拉伯语（沙特阿拉伯）
		//	0x0801
		//	Arabic   (Iraq)
		//	伊拉克
		//	0x0c01
		//	Arabic   (Egypt)
		//	埃及
		//	0x1001
		//	Arabic   (Libya)   
		//	利比亚
		//	0x1401
		//	Arabic   (Algeria) 
		//	阿尔及利亚
		//	0x1801
		//	Arabic   (Morocco)
		//	摩洛哥
		//	0x1c01
		//	Arabic   (Tunisia)   
		//	突尼斯
		//	0x2001
		//	Arabic   (Oman) 
		//	阿曼
		//	0x2401
		//	Arabic   (Yemen) 
		//	也门
		//	0x2801
		//	Arabic   (Syria) 
		//	叙利亚
		//	0x2c01
		//	Arabic   (Jordan) 
		//	约旦
		//	0x3001
		//	Arabic   (Lebanon)  
		//	黎巴嫩
		//	0x3401
		//	Arabic   (Kuwait)  
		//	科威特
		//	0x3801
		//	Arabic   (U.A.E.)
		//	阿拉伯联合酋长国
		//	0x3c01
		//	Arabic   (Bahrain) 
		//	巴林
		//	0x4001
		//	Arabic   (Qatar)
		//	卡塔尔
		//	0x0402
		//	Bulgarian   
		//	保加利亚语
		//	0x0403
		//	Catalan
		//	加泰罗尼亚
		//	0x0404
		//	Chinese   (Taiwan   Region) 
		//	台湾
		//	0x0804
		//	Chinese   (PRC) 
		//	中华人民共和国
		//	0x0c04
		//	Chinese   (Hong   Kong   SAR,   PRC) 
		//	香港
		//	0x1004
		//	Chinese   (Singapore) 
		//	新加坡
		//	0x0405
		//	Czech 
		//	捷克
		//	0x0406
		//	Danish
		//	丹麦
		//	0x0407
		//	German   (Standard)
		//	德语（标准）
		//	0x0807
		//	German   (Swiss) 
		//	瑞士
		//	0x0c07
		//	German   (Austrian)
		//	奥地利
		//	0x1007
		//	German   (Luxembourg)
		//	卢森堡
		//	0x1407
		//	German   (Liechtenstein)
		//	列支敦士登
		//	0x0408
		//	Greek 
		//	希腊
		//	0x0409
		//	English   (United   States)   
		//	美国
		//	0x0809
		//	English   (United   Kingdom)   
		//	英国
		//	0x0c09
		//	English   (Australian)  
		//	澳大利亚
		//	0x1009
		//	English   (Canadian) 
		//	加拿大
		//	0x1409
		//	English   (New   Zealand) 
		//	新西兰
		//	0x1809
		//	English   (Ireland) 
		//	爱尔兰
		//	0x1c09
		//	English   (South   Africa)  
		//	南非
		//	0x2009
		//	English   (Jamaica)
		//	牙买加
		//	0x2409
		//	English   (Caribbean)
		//	加勒比
		//	0x2809
		//	English   (Belize) 
		//	伯利兹
		//	0x2c09
		//	English   (Trinidad) 
		//	特立尼达
		//	0x040a
		//	Spanish   (Traditional   Sort) 
		//	西班牙语（传统风格）
		//	0x080a
		//	Spanish   (Mexican)   
		//	墨西哥
		//	0x0c0a
		//	Spanish   (Modern   Sort) 
		//	西班牙语（现代排序）
		//	0x100a
		//	Spanish   (Guatemala)  
		//	危地马拉
		//	0x140a
		//	Spanish   (Costa   Rica) 
		//	哥斯达黎加
		//	0x180a
		//	Spanish   (Panama) 
		//	巴拿马
		//	0x1c0a
		//	Spanish   (Dominican   Republic)   
		//	多米尼加共和国
		//	0x200a
		//	Spanish   (Venezuela)
		//	委内瑞拉
		//	0x240a
		//	Spanish   (Colombia)  
		//	哥伦比亚
		//	0x280a
		//	Spanish   (Peru)  
		//	秘鲁
		//	0x2c0a
		//	Spanish   (Argentina)   
		//	阿根廷
		//	0x300a
		//	Spanish   (Ecuador) 
		//	厄瓜多尔
		//	0x340a
		//	Spanish   (Chile) 
		//	智利
		//	0x380a
		//	Spanish   (Uruguay)
		//	乌拉圭
		//	0x3c0a
		//	Spanish   (Paraguay) 
		//	巴拉圭
		//	0x400a
		//	Spanish   (Bolivia)  
		//	玻利维亚
		//	0x440a
		//	Spanish   (El   Salvador)
		//	萨尔瓦多
		//	0x480a
		//	Spanish   (Honduras)
		//	洪都拉斯
		//	0x4c0a
		//	Spanish   (Nicaragua) 
		//	尼加拉瓜
		//	0x500a
		//	Spanish   (Puerto   Rico)   
		//	波多黎各
		//	0x040b
		//	Finnish  
		//	芬兰
		//	0x040c
		//	French   (Standard)
		//	法语
		//	0x080c
		//	French   (Belgian) 
		//	比利时
		//	0x0c0c
		//	French   (Canadian) 
		//	加拿大
		//	0x100c
		//	French   (Swiss)  
		//	瑞士
		//	0x140c
		//	French   (Luxembourg)
		//	卢森堡
		//	0x040d
		//	Hebrew
		//	希伯来语
		//	0x040e
		//	Hungarian
		//	匈牙利
		//	0x040f
		//	Icelandic
		//	冰岛
		//	0x0410
		//	Italian   (Standard) 
		//	意大利语（标准）
		//	0x0810
		//	Italian   (Swiss)
		//	瑞士
		//	0x0411
		//	Japanese  
		//	狗日的
		//	0x0412
		//	Korean
		//	韩国
		//	0x0812
		//	Korean   (Johab)  
		//	韩文（裘哈）
		//	0x0413
		//	Dutch   (Standard)
		//	荷兰语
		//	0x0813
		//	Dutch   (Belgian)
		//	比利时
		//	0x0414
		//	Norwegian   (Bokmal)
		//	挪威文（巴克摩）
		//	0x0814
		//	Norwegian   (Nynorsk) 
		//	耐诺斯克
		//	0x0415
		//	Polish
		//	波兰
		//	0x0416
		//	Portuguese   (Brazilian)
		//	葡萄牙语（巴西）
		//	0x0816
		//	Portuguese   (Standard)

		//	0x0418
		//	Romanian 
		//	罗马
		//	0x0419
		//	Russian
		//	俄罗斯
		//	0x041a
		//	Croatian 
		//	克罗地亚
		//	0x081a
		//	Serbian   (Latin) 
		//	塞尔维亚语（拉丁）
		//	0x0c1a
		//	Serbian   (Cyrillic) 
		//	西里尔文
		//	0x041b
		//	Slovak 
		//	斯洛伐克
		//	0x041c
		//	Albanian
		//	阿尔巴尼亚人
		//	0x041d
		//	Swedish
		//	瑞典
		//	0x081d
		//	Swedish   (Finland)  
		//	瑞典语（芬兰）
		//	0x041e
		//	Thai 
		//	泰国
		//	0x041f
		//	Turkish 
		//	土耳其
		//	0x0421
		//	Indonesian
		//	印度尼西亚
		//	0x0422
		//	Ukrainian  
		//	乌克兰
		//	0x0423
		//	Belarusian
		//	白俄罗斯
		//	0x0424
		//	Slovenian 
		//	斯洛文尼亚
		//	0x0425
		//	Estonian
		//	爱沙尼亚语
		//	0x0426
		//	Latvian  
		//	拉脱维亚
		//	0x0427
		//	Lithuanian
		//	立陶宛
		//	0x0429
		//	Farsi
		//	波斯语
		//	0x042a
		//	Vietnamese 
		//	越南
		//	0x042d
		//	Basque 
		//	巴斯克
		//	0x0436
		//	Afrikaans
		//	南非语
		//	0x0438
		//	Faeroese 

		switch (sysDefLangId)
		{
		case 0X0804:
			{
				//MessageBox(0, L"中文", 0, 0);
				int ddd = 5;
			}
			break;
		case 0x0409:
			//MessageBox(_T("English"));
			{
				//MessageBox(0, L"英语", 0, 0);
				int kkk = 3;
			}
		case  0x0411:
			{
			//	MessageBox(0, L"日语", 0, 0);
			}
			break;
		}

		LPCTSTR pszCmdLineW = GetCommandLineW();  

		std::wstring file_name = L"";


		if(pszCmdLineW != NULL)
		{
			int num_args = 0;
			wchar_t** args = NULL;
			args = ::CommandLineToArgvW(pszCmdLineW, &num_args);

			std::vector<std::wstring> new_argv;

			if(args && num_args > 1)
			{
				
				for(int i=0; i<num_args; ++i)
				{
					new_argv.push_back(args[i]);
				}

				if(new_argv.size() > 1)
				{
					file_name = new_argv[1];
				}
			}
		}

		if(!file_name.empty())
		{
			int dot_pos = file_name.rfind(L".");
			if(dot_pos != std::wstring::npos)
			{
				wstring extension = file_name.substr(dot_pos + 1, file_name.length() - dot_pos - 1);

				if(extension != L"txt")
				{
					return;
				}
			}
		}

		wchar_t wszAppPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, wszAppPath, MAX_PATH);
		std::wstring wstrAppPath = wszAppPath;
		std::wstring strTemp = wstrAppPath.substr(0, wstrAppPath.find_last_of(L'\\') + 1);

		strTemp += L"\\reader.res";


		if(!PathFileExistsW(strTemp.c_str()))
		{
			return;
		}

		//SYSTEMTIME time;
		//WIN32_FILE_ATTRIBUTE_DATA lpinf;
		//GetFileAttributesEx(strTemp.c_str(),GetFileExInfoStandard,&lpinf);//获取文件信息，path为文件路径
		//FileTimeToSystemTime(&lpinf.ftLastWriteTime,&time);//转换时间格式：FILETIME到SYSTEMTIME

		//AGuiString fileInfo;
		//fileInfo.Format(_T("%d-%d-%d"),time.wYear,time.wMonth,time.wDay);
		AppConfigure::Instance()->LoadConfig();

		AAGuiOption option;
		/*option.resource_filename = L"reader.res";
		option.use_zip = true;*/
		option.use_zip = false;
		option.disable_dpi = true;
		AAGuiInit(option);

		DataConnectorInit();


		ReaderWindow* readerWidget_ = new ReaderWindow;
		
		if(!file_name.empty())
		{
			readerWidget_->InitOpenFile(file_name);
		}
		if(readerWidget_){

			readerWidget_->Create();

			AAGuiRun();

		}

}