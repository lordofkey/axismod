#pragma once
#import <msxml4.dll>
using namespace MSXML2;
class MySetting
{
public:
	MySetting(void);
	~MySetting(void);
private:
	char *szXmlFile;
	MSXML2::IXMLDOMDocumentPtr pDoc;
	MSXML2::IXMLDOMElementPtr xmlRoot;
	MSXML2::IXMLDOMProcessingInstructionPtr pProInstruction; 
	MSXML2::IXMLDOMElementPtr pRootElement, plineElement, pcalitimeElement; 
public:
	bool initialdom(void);
	int checkline(CString linename);
	void precali(void);
	bool writesetting(CString linename);

	float calipar[6]; // l 0-roadwidth, 1-radarheight, 2-road_radar_dis  r 0-roadwidth, 1-radarheight, 2-road_radar_dis
	float rradar[6]; //0-delta_x, 1-delta_y, 2-delta_th, 3-score, 4-w1, 5-w2
	float lradar[6]; //0-delta_x, 1-delta_y, 2-delta_th, 3-score, 4-w1, 5-w2
	int ReadLine(std::vector<CString> &str_lines);
	void Loadsetting(CString linename,CString time,std::vector<CString>& parstr);
	int getsettings(CString linename, std::vector<CString>& sets);
};
