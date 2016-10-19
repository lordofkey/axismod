//Date:2016-07-2
//Author:杨浩
//Description:此文件用于配置文件读写，遵从xml协议
//            如有需求请按照说明进行修改


#include "StdAfx.h"
#include "MySetting.h"


#define f2str(target) sprintf_s(buffer,100,"%.2f",target);
#define i2str(target) sprintf_s(buffer,100,"%d",target);

MySetting::MySetting(void)
{

	CString  strPathName;
	GetModuleFileName(NULL,strPathName.GetBuffer(256),256);
	strPathName.ReleaseBuffer(256);
	int nPos  = strPathName.ReverseFind('\\');
	strPathName = strPathName.Left(nPos + 1);
	strPathName += "setting.xml";
	szXmlFile = strPathName;

	version = "1.0.0.160801";
//	szXmlFile  = "setting.xml";
	memset(&pLeft,0,sizeof(pLeft));
	memset(&pRight,0,sizeof(pRight));
	memset(&cpar,0,sizeof(cpar));
	pDoc = NULL;
	xmlRoot = NULL;
	pProInstruction = NULL; 
	pRootElement = NULL;
	plineElement = NULL;
	pcalitimeElement = NULL;

}
MySetting::~MySetting(void)
{
	pDoc = NULL;
	xmlRoot = NULL;
	pProInstruction = NULL; 
	pRootElement = NULL;
	plineElement = NULL;
	pcalitimeElement = NULL;
}
//输入 文件名
//输入 是否初始化成功
bool MySetting::initialdom(void)
{
	HRESULT hr = pDoc.CreateInstance(_uuidof(MSXML2::DOMDocument40));
	if (FAILED(hr))
	{
		::MessageBox(NULL,"无法创建DOM文件对象，检查是否安装MS XML Parser运行库！","警告",MB_OK);
		return false;
	}

	VARIANT_BOOL bXmlLoad = pDoc->load((_variant_t)szXmlFile);
	if (!bXmlLoad) // 加载失败
	{
		if(IDOK == ::MessageBox(NULL,"当前不存在标定结果文件，请确认是否创建结果文件?","警告",MB_OKCANCEL))
		{
			pProInstruction = pDoc->createProcessingInstruction((_bstr_t)(char*)"xml", (_bstr_t)(char*)"version=\"1.0\" encoding=\"utf-8\"");
			pDoc->appendChild((MSXML2::IXMLDOMNode*)pProInstruction);
			pRootElement =  pDoc->createElement((_bstr_t)(char*)"lines");   
			pDoc->PutRefdocumentElement(pRootElement); // pXMLDomDoc->documentElement = pRootElement;
			pDoc->save((_variant_t)szXmlFile);
		}
		else
		{
			return false;
		}
	}
	else
	{
		pRootElement = pDoc->GetdocumentElement();
	}

	return true;
}
//输入：线路名称
//输出：线路节点
int MySetting::checkline(CString linename)
{
	long length = 0;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	plineElement = NULL;
	pNodeList = pDoc->getElementsByTagName((_bstr_t)"Line");
	pNodeList->get_length(&length);
	for(int i = 0;i < length; i++)
	{
		if(strcmp(pNodeList->Getitem(i)->Getattributes()->Getitem(0)->Gettext(),linename) == 0)
		{
			printf("发现重名线路:%s\n",pNodeList->Getitem(i)->Getattributes()->Getitem(0)->Gettext(),linename);
			plineElement = pNodeList->Getitem(i);
			break;
		}
	}
	if(plineElement == NULL)
	{
		plineElement = pDoc->createElement((_bstr_t)"Line");
		plineElement->setAttribute((_bstr_t)"name",linename.GetBuffer());
		pRootElement->appendChild(plineElement);
	}
	return 1;
}
//<lines><line><caltime>
void MySetting::checkcal(void)
{
	pcalitimeElement = pDoc ->createElement("caltime");
	plineElement->appendChild(pcalitimeElement);
}
bool MySetting::checkcal(CString caltime)
{
	long length = 0;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	pcalitimeElement = NULL;
	pNodeList = plineElement->getElementsByTagName((_bstr_t)"caltime");
	pNodeList->get_length(&length);
	for(int i = 0;i < length; i++)
	{
		pcalitimeElement = pNodeList->Getitem(i);
		if(0 == strcmp((CString)(pcalitimeElement->getAttribute((_bstr_t)"time")),caltime))
			break;
	}
	if(pcalitimeElement == NULL)
		return false;
	return true;
}
bool MySetting::writesetting(CString linename)
{

	if(!initialdom())
		return false;
	checkline(linename);
	checkcal();
	update(true);
	return true;
}
int MySetting::ReadLine(std::vector<CString> &str_lines)
{
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	initialdom();
	long length = 0;
	pNodeList = pDoc->getElementsByTagName((_bstr_t)"Line");
	pNodeList->get_length(&length);
	for(int i = 0;i < length; i++)
	{
		str_lines.push_back((char*)(pNodeList->Getitem(i)->Getattributes()->Getitem(0)->Gettext()));
	}
	return 0;
}
bool MySetting::Loadsetting(CString linename,CString time)
{
	if(linename.IsEmpty()||time.IsEmpty())
		return false;
	initialdom();
	checkline(linename);
	checkcal(time);
	update(false);
	return true;
}
int MySetting::getsettings(CString linename, std::vector<CString>& sets)
{
	long length;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;

	this->initialdom();
	this->checkline(linename);
	pNodeList = plineElement->getElementsByTagName((_bstr_t)"caltime");
	pNodeList->get_length(&length);
	for(int i = 0;i < length; i++)
	{
		CString tem = ((MSXML2::IXMLDOMElementPtr)pNodeList->Getitem(i))->getAttribute((_bstr_t)"time");
		sets.push_back(tem);
	}
	return 0;
}
bool MySetting::update(bool isout)//isout == true 时，保存到文件。 isout = false时，从文件读出
{
	MSXML2::IXMLDOMElementPtr pPar = NULL, prradar = NULL, plradar = NULL,pcut = NULL,pcar = NULL;
#pragma region 保存
	if(isout)
	{
		CTime now = CTime::GetCurrentTime();
		pcalitimeElement->setAttribute((_bstr_t)"time",now.Format("%Y-%m-%d %H:%M:%S: ").GetBuffer());
		pcalitimeElement->setAttribute((_bstr_t)"version",version.GetBuffer());
		pcalitimeElement->setAttribute((_bstr_t)"source",fSource.GetBuffer());

		char buffer[100];
		pPar = pDoc->createElement((_bstr_t)"cali_par");
		f2str(pRight.roadwidth)
			pPar->setAttribute((_bstr_t)"roadwidth",buffer);
		f2str(pRight.radarheight)
			pPar->setAttribute((_bstr_t)"radarheight",buffer);
		f2str(pRight.road_radar_dis)
			pPar->setAttribute((_bstr_t)"road_radar_dis",buffer);
		prradar = pDoc->createElement("right_radar");
		f2str(pRight.delta_x)
			prradar->setAttribute((_bstr_t)"delta_x",buffer);
		f2str(pRight.delta_y)
			prradar->setAttribute((_bstr_t)"delta_y",buffer);
		f2str(pRight.delta_th*57.2958) //    180/pi
			prradar->setAttribute((_bstr_t)"delta_th",buffer);
		f2str(pRight.score)
			prradar->setAttribute((_bstr_t)"score",buffer);
		f2str(pRight.w1)
			prradar->setAttribute((_bstr_t)"w1",buffer);
		f2str(pRight.w2)
			prradar->setAttribute((_bstr_t)"w2",buffer);
		prradar->appendChild(pPar);


		pPar = pDoc->createElement((_bstr_t)"cali_par");
		f2str(pLeft.roadwidth)
			pPar->setAttribute((_bstr_t)"roadwidth",buffer);
		f2str(pLeft.radarheight)
			pPar->setAttribute((_bstr_t)"radarheight",buffer);
		f2str(pLeft.road_radar_dis)
			pPar->setAttribute((_bstr_t)"road_radar_dis",buffer);

		plradar = pDoc->createElement("left_radar");
		f2str(pLeft.delta_x)
			plradar->setAttribute((_bstr_t)"delta_x",buffer);
		f2str(pLeft.delta_y)
			plradar->setAttribute((_bstr_t)"delta_y",buffer);
		f2str(pLeft.delta_th*57.2958)
			plradar->setAttribute((_bstr_t)"delta_th",buffer);
		f2str(pLeft.score)
			plradar->setAttribute((_bstr_t)"score",buffer);
		f2str(pLeft.w1)
			plradar->setAttribute((_bstr_t)"w1",buffer);
		f2str(pLeft.w2)
			plradar->setAttribute((_bstr_t)"w2",buffer);
		plradar->appendChild(pPar);

		pcut = pDoc->createElement("cut");
		i2str(cpar.num);
		pcut->setAttribute((_bstr_t)"number",buffer);

		pcalitimeElement->appendChild(prradar);
		pcalitimeElement->appendChild(plradar);
		pcalitimeElement->appendChild(pcut);
		for(int i = 0;i<cpar.num;i++)
		{
			pcar = pDoc->createElement("car");
			i2str(cpar.stpoint[i])
			pcar->setAttribute((_bstr_t)"stpoint",buffer);
			i2str(cpar.length[i])
				pcar->setAttribute((_bstr_t)"length",buffer);
			pcut->appendChild(pcar);
		}
		pDoc->save((_variant_t)szXmlFile);
		return true;
	}
#pragma endregion 保存
	else
	{
		prradar = pcalitimeElement->getElementsByTagName((_bstr_t)"right_radar")->Getitem(0);
		plradar = pcalitimeElement->getElementsByTagName((_bstr_t)"left_radar")->Getitem(0);
		CString tem;
		if(prradar == NULL||plradar == NULL)
			return false;
		tem = prradar->getAttribute((_bstr_t)"delta_x");
		pRight.delta_x = (float)atof(tem);
		tem = prradar->getAttribute((_bstr_t)"delta_y");
		pRight.delta_y = (float)atof(tem);
		tem = prradar->getAttribute((_bstr_t)"delta_th");
		pRight.delta_th = (float)atof(tem)/57.3f;

		tem = plradar->getAttribute((_bstr_t)"delta_x");
		pLeft.delta_x = (float)atof(tem);
		tem = plradar->getAttribute((_bstr_t)"delta_y");
		pLeft.delta_y = (float)atof(tem);
		tem = plradar->getAttribute((_bstr_t)"delta_th");
		pLeft.delta_th = (float)atof(tem)/57.3f;
		return true;
	}
}
