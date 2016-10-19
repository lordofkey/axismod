#pragma once
#import <msxml4.dll>
using namespace MSXML2;

struct TrainPar //标定参数
{
	float delta_x;
	float delta_y;
	float delta_th;
	float w1;
	float w2;
	float score;
	float road_radar_dis;
	float radarheight;
	float roadwidth;
};
struct CutPar //切车参数
{
	int num;
	int stpoint[1000];
	int length[1000];
};

class MySetting
{
public:
	MySetting(void);
	~MySetting(void);
private:
	CString szXmlFile;
	CString version;
public:
	MSXML2::IXMLDOMDocumentPtr pDoc;
	MSXML2::IXMLDOMElementPtr xmlRoot;
	MSXML2::IXMLDOMProcessingInstructionPtr pProInstruction; 
	MSXML2::IXMLDOMElementPtr pRootElement, plineElement, pcalitimeElement; 
	TrainPar pLeft,pRight;
	CString fSource;
	CutPar cpar;

	bool initialdom(void);
	int checkline(CString linename);
	void checkcal(void);
	bool checkcal(CString calname);
	bool writesetting(CString linename);
	int ReadLine(std::vector<CString> &str_lines);
	bool Loadsetting(CString linename,CString time);
	int getsettings(CString linename, std::vector<CString>& sets);
	bool update(bool isout);
};
